# Altimeter Simulation (OpenGL ES 2.0)

This repository contains an interactive altimeter program implemented using OpenGL. The program displays a circular altimeter with needles indicating altitude levels. Users can simulate altitude changes using the keyboard arrow keys.

https://github.com/yesillmustafa/Altimeter-Simulation/assets/74787246/03e5db85-a73d-4bff-a5f5-b15e0eca4faa

## Features

- Display of a circular altimeter with multiple needles indicating different altitude levels.
- Displaying the visibility flag (it lights white when the altitude value is below 10,000ft, and turns off when it goes above 10,000ft)
- Interactive altitude adjustments using the keyboard arrow keys.
- Visualization of altitude using graphical indicators.
- Integration of textures and shaders for a visually appealing interface.

## Requirements

- OpenGL ES 2.0
- [GLFW 3.3](https://www.glfw.org/)
- [stb_image library](https://github.com/nothings/stb) (included in the project)

## Installation and Usage

1. Clone the repository:
   ```bash
   git clone https://github.com/yesillmustafa/altimeter-simulation.git
   ```
2. Navigate to the repository folder:
    ```bash
   cd altimeter-simulation
    ```
3. Build the program using a C++ compiler (e.g., g++):
    ```bash
   g++ -o altimeter altimeter.cpp -lglfw -lGLESv2
    ```
4. Run the program:
    ```bash
   ./altimeter
     ```
5. Use the arrow keys (up and down) to simulate changes in altitude.
