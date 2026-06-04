# Snakes and Ladders - SFML

A desktop **Snakes and Ladders** game made in **C++** using the **SFML** graphics/audio library.

The project includes a menu screen, singleplayer mode, local multiplayer mode, board graphics, player sprites, background music, and sound effects.

---

## Features

- Singleplayer mode
- Two-player local multiplayer mode
- Dice rolling using the spacebar
- Snake and ladder movement logic
- Menu screen for choosing game mode
- Background music and sound effects
- Visual board, player sprites, and side panel

---

## Controls

| Key / Action | Use |
| --- | --- |
| Mouse click | Select Singleplayer or Multiplayer from the menu |
| Spacebar | Roll the dice |
| Escape | Return to the menu / close from menu |
| Enter | Restart after game over |

---

## Project Structure

```text
SnakesAndLadders-SFML/
├── src/
│   └── main.cpp
├── assets/
│   ├── images/
│   ├── audio/
│   └── fonts/
├── docs/
│   └── FlowChart.pdf
├── Makefile
├── .gitignore
└── README.md
```

---

## Required Assets

The game expects these files to exist:

```text
assets/images/Player1.png
assets/images/Player2.png
assets/images/GameBoard.png
assets/images/texture.png
assets/images/background.png

assets/audio/background_music.ogg
assets/audio/WinSound.ogg
assets/audio/moveSound.ogg

assets/fonts/arial.ttf
assets/fonts/SuperFunky.ttf
```

---

# How to Run

Make sure you run the game from the **main project folder**, not from inside `src/`.

Correct folder:

```text
SnakesAndLadders-SFML/
```

Wrong folder:

```text
SnakesAndLadders-SFML/src/
```

This matters because the code loads assets using paths like:

```text
assets/images/GameBoard.png
assets/audio/background_music.ogg
assets/fonts/arial.ttf
```

If you run the game from the wrong folder, the code may compile, but images/audio/fonts may not load.

---

## Running on Linux

These instructions work for most Debian/Ubuntu-based Linux systems.

### 1. Install dependencies

```bash
sudo apt update
sudo apt install g++ make libsfml-dev
```

For Fedora:

```bash
sudo dnf install gcc-c++ make SFML-devel
```

### 2. Go to the project folder

Example:

```bash
cd ~/Downloads/SnakesAndLadders-SFML
```

Check that you are in the right place:

```bash
ls
```

You should see something like:

```text
src  assets  docs  Makefile  CMakeLists.txt  README.md
```

### 3. Compile and run

```bash
make run
```

This will compile the game into the `build/` folder and then run it.

### 4. Manual compile option

If `make run` does not work, you can compile manually:

```bash
g++ src/main.cpp -o snakes-and-ladders \
-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

Then run:

```bash
./snakes-and-ladders
```

---

## Running on Windows using WSL

This is the easiest Windows method if you already have WSL installed.

### 1. Open WSL

Open your Ubuntu/WSL terminal.

If your project is inside your Windows Downloads folder, go to it like this:

```bash
cd /mnt/c/Users/YOUR_WINDOWS_USERNAME/Downloads/SnakesAndLadders-SFML
```

Example:

```bash
cd /mnt/c/Users/Harris/Downloads/SnakesAndLadders-SFML
```

### 2. Install dependencies

```bash
sudo apt update
sudo apt install g++ make libsfml-dev
```

### 3. Check that assets exist

```bash
ls assets
ls assets/images
ls assets/audio
ls assets/fonts
```

The fonts folder should contain:

```text
arial.ttf
SuperFunky.ttf
```

### 4. Run the game

```bash
make run
```

If everything is installed correctly, the game window should open.

### Important WSL note

Because this is a graphics game, WSL needs GUI support.

- On **Windows 11**, this usually works automatically through WSLg.
- On **Windows 10**, you may need an X server such as VcXsrv before SFML windows can open.

If you get a display/window error, the issue is probably WSL graphics support, not the C++ code.

---

## Running on Windows using MSYS2

Use this if you want to build and run it directly on Windows without WSL.

### 1. Install MSYS2

Install MSYS2, then open:

```text
MSYS2 MINGW64
```

Do not use the plain `MSYS2 MSYS` terminal for this project.

### 2. Update MSYS2

```bash
pacman -Syu
```

If it asks you to close the terminal, close it, reopen **MSYS2 MINGW64**, and continue.

### 3. Install compiler, Make, and SFML

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-sfml make
```

### 4. Go to the project folder

Example if the project is in Downloads:

```bash
cd /c/Users/YOUR_WINDOWS_USERNAME/Downloads/SnakesAndLadders-SFML
```

Example:

```bash
cd /c/Users/Harris/Downloads/SnakesAndLadders-SFML
```

### 5. Run the game

```bash
make run
```

If `make run` fails, try compiling manually:

```bash
g++ src/main.cpp -o snakes-and-ladders.exe \
-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

Then run:

```bash
./snakes-and-ladders.exe
```

---

## Cleaning Build Files

To remove compiled files made by the Makefile:

```bash
make clean
```

This deletes the `build/` folder.

---

## About

This project was created as a C++/SFML implementation of the classic Snakes and Ladders board game.
