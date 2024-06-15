# Raycasting 3D Engine C++

This is a simple 3D Raycasting engine implemented from scratch in C++, utilizing the SFML library for graphics rendering. The engine provides both a 3D perspective view and a 2D top-down map generated using a random path-fusion algorithm.

## Features

- **3D Raycasting Perspective :**
  The engine generates a 3D perspective view using the classic Raycasting technique.

- **2D Top-Down Map :**
  A 2D map is displayed alongside the 3D view, offering a top-down perspective of the randomly generated environment.

- **Random Map Generation :**
  The project incorporates an algorithm for random map generation using path-fusion techniques.

## Project Structure

- **/src :**
  - `2d.cpp`: Implementation of the 2D view.
  - `3d.cpp`: Implementation of the 3D Raycasting engine.
  - `Raycast.cpp`: Main file handling window creation and event loops.

- **/include :**
  - `2d.h`: Header file for the 2D view.
  - `3d.h`: Header file for the 3D Raycasting engine.

## Build

1. **Clone the Repository :**

```bash
git clone https://github.com/yashbbb009/tinyraytracer/
```

2. **Navigate to the Project Directory :**

```bash
cd tinyraytracer
```

3. **Create a Build Directory :**

```bash
mkdir build
cd build
```

4. **Run CMake :**

```bash
cmake ..
```

5. **Build the Project :**

```bash
cmake --build .
```

6. **Copy dlls :**

```bash
cp _deps/sfml-build/lib/sfml-window-d-2.dll Debug/
cp _deps/sfml-build/lib/sfml-graphics-d-2.dll Debug/
cp _deps/sfml-build/lib/sfml-system-d-2.dll Debug/
```

7. **Run the Executable :**

```bash
./Debug/tinyraytracer
```

## Usage

- Movement
- Use `W` - forward.
- Use `A` - left.
- Use `S` -  backward.
- Use `D` -  right.
- mouse to move 


### 3D Perspective View
<p align="center">
	<img src="resources/img/3d.png" width="400">
</p>

### 2D Top-Down Map
<p align="center">
	<img src="resources/img/2d.png" width="400">
</p>


