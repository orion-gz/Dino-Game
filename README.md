# Dino Game

A Chrome-dino-style runner game for Arduino with a TFT LCD touch screen. Jump over or slide under obstacles, survive as long as you can, and compete for the top score.

## Features

- **Playable characters:** turtle, panda and elephant, each drawn with LCD primitives
- **Difficulty levels:** Easy / Soso / Hard (obstacle speed 4 / 5 / 6)
- **Custom map:** design your own obstacle sequence, saved to EEPROM
- **Ranking:** persistent leaderboard (EEPROM) with an on-screen keyboard for entering names, and touch scrolling
- **Controls:** touch-only (jump, slide, pause, restart)

## Hardware
- Arduino Uno board
- LCDWIKI KBV-compatible TFT LCD shield with resistive touch screen
- Touch pins: `XP=8`, `YM=9`, `XM=A2`, `YP=A3` (see [`_define.h`](_define.h))

## Project Structure

| File | Description |
|------|-------------|
| `main.ino` | `setup()` / `loop()` entry point |
| `_define.h` | Includes, pin and touch calibration, color constants |
| `_mainscreen.ino` | Main menu UI and touch handling |
| `game.ino` | Game loop, obstacles, collision, score, pause, game over |
| `character.ino` | Character drawing and selection |
| `map.ino` | Custom obstacle map editor and EEPROM storage |
| `ranking.ino` | Leaderboard display and EEPROM storage |
| `user.ino` | Character selection screen |
| `library/` | Bundled `LCDWIKI_GUI`, `LCDWIKI_KBV`, `TouchScreen` libraries |

## Code Details

### Game Logic (`game.ino`)

`startGame()` calls `initGame()` (sets obstacle speed, score, character position, obstacle state), then repeatedly calls `game()` until the game ends. Each frame of `game()`:

1. `updateScore()` – every second adds score by difficulty (+1 / +2 / +3) and refreshes the HUD, including the current 1st-place score.
2. `movingCustomObs()` – scrolls obstacles and checks collisions.
3. `movingUser()` – handles jump and slide.
4. `gameControl()` – reads touch input.

**Controls** (`gameControl()`): touch the left half of the screen to jump, the right half to slide (held while pressed), and the top-right corner (the "eyes" on the sun) to pause.

**Character movement**
- Jump: `y` moves from 150 up to ~85–95 and back down. Jumping is blocked while sliding.
- Slide: lowers the character briefly (~100 ms) and returns. Sliding is blocked while jumping.
- Characters are redrawn each step by erasing and redrawing at the new position (no framebuffer).

**Obstacles**
- Three obstacles (`obsX1..3`) are on screen at a time, spaced 80 px apart, each moving left by `obsSpeed` every 20 ms.
- When one leaves the screen (`x <= -20`), it respawns at `x = 320` and advances 3 entries in `mapSet[]`, so the three obstacles cycle through the map interleaved. At the end of the map, the sequence wraps to the start.
- Obstacle types: `1` bottom (ground), `2` top (hanging), `3` middle (mid-air).
- Collision (`checkGameOver()`) uses axis-aligned hitboxes: a body box (`x..x+25`) and a head box (`x+25..x+37`) against each obstacle's box. Jump over types 1, slide under type 2 (and 3 depending on position).

**Menus**: `gameOver()` offers Restart / Ranking / Menu; `pause()` offers Resume / Restart / Menu. `rankingAdd()` shows an on-screen keyboard to enter a name for the leaderboard.

### Custom Map (`map.ino`)

- The map is stored in `mapSet[100]` (`int8_t`, `0` none / `1` bottom / `2` top / `3` middle). Each entry is one obstacle slot.
- **Map size** is `maxMapSize`, cycling through 15 / 30 / 45 / 99 slots (stored as 14 / 29 / 44 / 98).
- **Editor UI** (`startMap()` → `map()` / `showObs()` / `selectObs()`): shows a window of 5 slots at a time. Use `<<` / `>>` to scroll (`nowMap` = first visible slot), pick an obstacle type from the palette (tap again to deselect), then tap one of the 5 slot columns to place it. Choosing no type clears the slot (`0`).
- **Persistence**: `saveMapToEEPROM()` / `loadMapFromEEPROM()` write and read `mapSet[]` and `maxMapSize` as bytes starting at EEPROM address 0. `initMap()` clears the map.
- The "Custom Game" shown on the main menu plays this map.

### Ranking (`ranking.ino`)

- Keeps up to 20 entries (`maxRankingSize`) of `RankingData` (name + score) in `rankingData[]`, sorted by score.
- Saved/loaded via `saveRankingToEEPROM()` / `loadRankingFromEEPROM()` (`setup()` loads on boot).
- The ranking screen scrolls by touching the top or bottom half; the main menu shows the top 3.

### Characters (`character.ino`, `user.ino`)

- `turtle()`, `panda()`, `elephant()` draw each character from LCD primitives; `choiceUser()` picks the one matching `types`, with jump/slide pose variants.
- `user.ino` is the selection screen; the choice is kept in `types`.


## Setup

1. Copy the three folders in `library/` into your Arduino `libraries` directory. Because of the Arduino's limited program/data memory, the bundled libraries have been modified: unneeded LCD drivers are disabled, so only the driver for the target display is kept.
2. Open the project in the Arduino IDE (sketch: `project2.ino`).
3. Adjust `TS_MINX/MAXX/MINY/MAXY` in `_define.h` if touch is misaligned.
4. Select your board and port, then upload.

To reset the leaderboard, uncomment `initRanking();` in `setup()` for one upload.

## Menu

- **Start** – play the game
- **User** – choose a character
- **Map** – build a custom obstacle map
- **Rank** – view the leaderboard
- Tap the difficulty label to change difficulty

## Credits

Bundled libraries are from [LCDWIKI](http://www.lcdwiki.com) and Adafruit (TouchScreen); see their respective license files.

## Contributors
- [Seonghyeon Kim](https://github.com/krSeonghyeon) – Ranking system and overall UI
- [DongWoo Lee](https://github.com/woodong-222) – Character and map customization systems
- [JungWoo Lee](https://github.com/orion-gz) – Game logic (character and obstacle movement)