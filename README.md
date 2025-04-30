# Omni Snake NEAT

https://github.com/HenrySchiff/Omni-Snake-NEAT

## Compilation Instructions (Windows)

### Prerequisites

- A C++ Toolchain (e.g. [MSVC](https://visualstudio.microsoft.com/))
- CMake version 3.28 or later ([Download CMake](https://cmake.org/download/))

### Steps

1. Clone or download this repository.

2. In the project root, create a build directory:
   ```bash
   mkdir build
   cd build
   ```

3. Configure the project with CMake:
   ```bash
   cmake ..
   ```

   This will set up the build system and automatically fetch SFML and Cereal as dependencies.

4. Open `build/Omni-Snake-NEAT.sln` in Visual Studio.

5. Build and run the project from Visual Studio.

## Usage

Open `main.cpp` and modify the `MODE` variable to control how the program runs:

- `PLAY`: Play the game yourself manually. The controls are `A`/`D` to turn left/right and `W` to sprint.
- `TRAIN`: Train a new neural network model. Modify the `GENERATAIONS` variable to train the model for longer.
- `LOAD`: Load and run a previously trained model.

If using `LOAD`, update the `FILE_PATH` variable to point to a `.json` file in the `data/` directory. When training, new models are automatically saved to `data/recent.json`.