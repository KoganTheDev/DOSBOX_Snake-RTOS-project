# 🐍 Snake Game – DOS Edition

![C](https://img.shields.io/badge/Built%20with-C-blue) ![Turbo%20C](https://img.shields.io/badge/IDE-Turbo%20C-green) ![DOS](https://img.shields.io/badge/Platform-DOS-black) ![Retro](https://img.shields.io/badge/Style-Retro-red)

The **Snake Game – DOS Edition** is a retro-inspired implementation of the classic Snake game, designed for the DOS environment using **Turbo C**.  
It features keyboard interrupts, direct cursor manipulation, custom ASCII graphics, and even sound effects for a nostalgic arcade feel 🎮.

---

## 🧠 Overview
This project brings the **Snake Game** to life in a DOS environment.  
It leverages **low-level hardware interrupts**, **cursor control**, and **direct screen drawing** to simulate an interactive game experience.  

The game includes:
- Snake movement with arrow keys
- Food spawning with ASCII art 🍎
- Walls as obstacles
- Music and scoring
- Cursor control following the snake’s head

---

## ✨ Features

- 🎮 **Real-time Snake Movement** with keyboard interrupt handler  
- 🍏 **ASCII Art Food** (a small apple drawn with characters)  
- 🧱 **Walls & Obstacles** dynamically spawned  
- 🎶 **Retro Music Support**  
- 📊 **Score Tracking**  
- ⚡ **Fast Rendering** with display buffer (`display_draft`)  
- ⌨️ **Low-level Keyboard Handler**  

---

## 📚 Table of Contents

- [🐍 Snake Game – DOS Edition](#-snake-game--dos-edition)
  - [🧠 Overview](#-overview)
  - [✨ Features](#-features)
  - [📚 Table of Contents](#-table-of-contents)
  - [🚀 How to Play](#-how-to-play)
  - [🛠️ Tech Stack](#️-tech-stack)
  - [🚧 Getting Started](#-getting-started)
  - [🖼️ Screenshots](#-screenshots)
  - [📂 Module Documentation](#-module-documentation)
  - [👨‍💻 Authors](#-authors)

---

## 🚀 How to Play

1. Use **Arrow Keys** to move the snake.  
2. Eat the **apple** to grow and increase your score.  
3. Avoid crashing into the **walls** or yourself.  
4. Enjoy the retro **sound effects** and **cursor tracking**.  

---

## 🛠️ Tech Stack

| Category         | Technologies                |
|------------------|-----------------------------|
| **Language**     | C (Turbo C, Borland C)      |
| **Platform**     | DOS                         |
| **Graphics**     | ASCII Characters + Drafting |
| **Input**        | Keyboard Interrupt Handler  |
| **Sound**        | Turbo C `<dos.h>` + speaker |
| **IDE**          | Turbo C / DOSBox-X          |

---

## 🚧 Getting Started

```bash
### 1. Clone the project
git clone https://github.com/KoganTheDev/DOSBOX_Snake-RTOS-project.git

### 2. Open in Turbo C (or DOSBox-X with TCC) and run the batch file from src which runs the game
````

---

## 🖼️ Screenshots

<table width="100%">
  <tr>
    <td align="center" width="50%">
      <img src="https://github.com/user-attachments/assets/1" width="90%"/>
      <br/>
      <b>Welcome Screen</b>
    </td>
    <td align="center" width="50%">
      <img src="https://github.com/user-attachments/assets/2" width="90%"/>
      <br/>
      <b>Snake Eating Food</b>
    </td>
  </tr>
  <tr>
    <td align="center" width="50%">
      <img src="https://github.com/user-attachments/assets/3" width="90%"/>
      <br/>
      <b>Game Over Screen</b>
    </td>
    <td align="center" width="50%">
      <img src="https://github.com/user-attachments/assets/4" width="90%"/>
      <br/>
      <b>Walls & Obstacles</b>
    </td>
  </tr>
</table>

---

## 📂 Module Documentation

| Module           | Summary                                                               |
| --------------------- | --------------------------------------------------------------------- |
| **dosbox_snake.c**    | Main loop                                                             |
| **clock.c**           | Controls the game`s timer and ticking speed using interrupts          |
| **cursor.c**          | Controls the hardware text cursor (block mode + follow snake’s head). |
| **food.c**            | Spawns, draws, and checks collisions with food (ASCII apple).         |
| **snake.c**           | Manages snake movement, growth, and self-collision detection.         |
| **wall.c**            | Spawns walls as obstacles in the play area.                           |
| **music.c**           | Handles retro sound effects and background music.                     |
| **score.c**           | Tracks and updates player score.                                      |
| **screens.c**         | Displays welcome, game, and game-over screens.                        |
| **keyboard.c**        | Interrupt-driven keyboard input handling.                             |
| **game_management.c** | Orchestrates updates and rendering.                                   |
| **colors**            | Handles the colors for the game screen                                |

---

## 👨‍💻 Authors

<table width="100%">
  <tr>
    <td><b>Yuval Kogan</b></td>
    <td>
      <a href="https://www.linkedin.com/in/yuval-kogan/">
        <img src="https://img.shields.io/badge/LinkedIn-Profile-blue?style=flat&logo=linkedin" alt="LinkedIn" height="24"/>
      </a>
    </td>
    <td>
      <a href="https://github.com/KoganTheDev">
        <img src="https://img.shields.io/badge/GitHub-Profile-black?style=flat&logo=github" alt="GitHub" height="24"/>
      </a>
    </td>
  </tr>
  <tr>
    <td><b>Almog Raz</b></td>
    <td>
      <a href="https://www.linkedin.com/in/almog-raz/">
        <img src="https://img.shields.io/badge/LinkedIn-Profile-blue?style=flat&logo=linkedin" alt="LinkedIn" height="24"/>
      </a>
    </td>
    <td>
      <a href="https://github.com/almograz1">
        <img src="https://img.shields.io/badge/GitHub-Profile-black?style=flat&logo=github" alt="GitHub" height="24"/>
      </a>
    </td>
  </tr>
  <tr>
    <td><b>Ron Salama</b></td>
    <td>
      <a href="https://www.linkedin.com/in/ron-salama-3860a0107/">
        <img src="https://img.shields.io/badge/LinkedIn-Profile-blue?style=flat&logo=linkedin" alt="LinkedIn" height="24"/>
      </a>
    </td>
    <td>
      <a href="https://github.com/RS-OG">
        <img src="https://img.shields.io/badge/GitHub-Profile-black?style=flat&logo=github" alt="GitHub" height="24"/>
      </a>
    </td>
  </tr>
</table>
