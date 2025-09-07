# OpenSR

OpenSR is an open-source project featuring a modern remake of the classic 2004 game *Space Rangers 2: Dominators*, along with a comprehensive toolkit for managing the original game's resources. Built on the Qt framework, the project leverages QML for creating a fluid and modern game interface.

**Current Status**: The engine core is now functional, and a small technical demo is available for testing. This early version showcases the foundational systems while development continues toward full gameplay implementation.

## Features

- **Engine Foundation**: Core systems implemented and operational
- **Game Demo**: Early showcase of engine capabilities
- **Resource Management Tools**: Comprehensive utilities for handling original game assets
- **Comprehensive Testing**: GUI testing with Spix and unit tests with Qt Test
- **Code Coverage**: Integrated gcovr support for test coverage analysis

## Requirements

### Build Dependencies
- **Qt 6** or newer (versions below 6 are not supported)
- **CMake** 3.10 or above
- **OpenAL** (Open Audio Library)

### Optional Dependencies (for testing)
- **[Spix](https://github.com/faaxm/spix)** - For GUI testing functionality
- **GTest framework** - Works in combination with Spix
- **gcovr** - For code coverage reporting

## Platform Support

### Current Build Status
- **Linux**: Fully supported, primary development platform
- **Windows**: Builds are possible but functionality is not thoroughly tested

### Windows Build Considerations
While Windows builds are possible through CMake and Qt6, please note:
- **No Automatic Setup**: Currently there is no script for automatic resources preparation and symlink creation on Windows, so manual setup is required
- **Build Limitations**: Build functionality has not been extensively tested on Windows
- **Platform Issues**: Some platform-specific issues may occur
- **GUI Testing**: GUI testing may require additional setup and configuration

For detailed instructions on manual resource setup, please refer to the [Detailed Manual Setup](#detailed-manual-setup) section. Windows users will need to use appropriate Windows utilities to perform the equivalent operations (such as using `mklink` for symlinks instead of `ln -s`).

## Building from Source

```bash
# Clone the repository
git clone https://github.com/OpenSRQt/OpenSR.git
cd OpenSR

# Create build directory
mkdir build && cd build

# Configure with CMake (build all tools)
cmake -DBUILD_ALL_TOOLS=ON -DCMAKE_BUILD_TYPE=Debug ../

# Build the project
cmake --build .
```

### CMake Build Options

- `BUILD_RESOURCE_VIEWER`: Build Resource Viewer tool (default: OFF)
- `BUILD_PLANET_VIEWER`: Build Planet Viewer tool (default: OFF)
- `BUILD_QUEST_PLAYER`: Build Quest Player tool (default: OFF)
- `BUILD_DAT_TOOLS`: Build tools for DAT files (default: OFF)
- `BUILD_ALL_TOOLS`: Build all tools (overrides individual options, default: OFF)
- `BUILD_TESTS`: Build GUI and regular unit tests (default: OFF)
- `ENABLE_COVERAGE`: Enable code coverage (automatically enables BUILD_TESTS, default: OFF)

## Usage

### Game Demo

The demo executable is located at: `OpenSR/build/Engine/opensr`. You have to run either without resources in test mode or prepare original game resource first. See next sections.

**Important**: Currently, the executable must be launched from the OpenSR source directory:
```bash
./build/Engine/opensr
```

#### Running with Test Mode
OpenSR supports running without original game resources using test mode.

Before running in test mode, set up required library symlinks:
```bash
./opensr_setup.sh -t demo
```

Run the game demo:
```bash
./build/Engine/opensr --test-mode
```

#### Preparing Original Game Resources

**Important**: OpenSR has only been tested and verified with resources from **Space Rangers 2: Reboot** (Космические Рейнджеры 2: Доминаторы. Перезагрузка). Other versions of the game (including the original Space Rangers 2: Dominators release) may not work correctly or may cause unexpected issues. We strongly recommend using the "Reboot" version to ensure compatibility and proper functionality.


To use original Space Rangers 2 resources:

1. Build with tools enabled: `cmake -DBUILD_ALL_TOOLS=ON ...`
2. Unpack your game image
3. Choose the appropriate setup method

##### Quick Setup (Linux only)

###### Step 1: Run the Setup Script

Run setup script: 
```bash
./opensr_setup.sh -a /path/to/your/iso/SR2/
```

For more information on setup script features run:
```bash
./opensr_setup.sh -h
```

To correctly run the demo following PKG files are expected in your DATA directory:
```
2captain.pkg
2gov.pkg
2Items.pkg
1main.pkg
2main.pkg
common.pkg
ShipFull.pkg
ShipSmall.pkg
Sound.pkg
Star.pkg
```

**Default assets location**: `OpenSR/../OpenSRData`

###### Step 2: Launch OpenSR
Run the game demo executable:

```bash
./build/Engine/opensr
```

##### Detailed Manual Setup

###### Step 1: Copy Resource Files
Copy the following .pkg files from your game installation to the `data/` folder:

```bash
# Essential resource files
SR2/DATA/2captain.pkg
SR2/DATA/2gov.pkg
SR2/DATA/2Items.pkg
SR2/DATA/1main.pkg
SR2/DATA/2main.pkg
SR2/DATA/common.pkg
SR2/DATA/ShipFull.pkg
SR2/DATA/ShipSmall.pkg
SR2/DATA/Sound.pkg
SR2/DATA/Star.pkg

# Music files
SR2/Music/
```

###### Step 2: Decode DAT Files
Use the DATTools to decode essential configuration files:

```bash
# Decode CacheData
./build/tools/DATTools/opensr-dat-convert hd <iso>/SR2/CFG/CacheData.dat data/CacheData.dat

# Decode Main configuration
./build/tools/DATTools/opensr-dat-convert d <iso>/SR2/CFG/Main.dat data/Main.dat

# Decode language file (for Russian version)
./build/tools/DATTools/opensr-dat-convert d <iso>/SR2/CFG/Rus.dat data/Rus.dat
# OR for English:
./build/tools/DATTools/opensr-dat-convert d <iso>/SR2/CFG/Eng.dat data/Eng.dat
```

###### Step 3: Set Up Library Symlinks
Create required symlinks for libraries:

```bash
# World library
ln -s build/World/libworld.so

# Image plugin
mkdir -p imageformats
ln -s ../build/ImagePlugin/libQtOpenSRImagePlugin.so imageformats/libQtOpenSRImagePlugin.so
```

###### Step 4: Launch OpenSR
Run the game executable:

```bash
./build/Engine/opensr
```

##### File Structure After Setup
Your project directory should contain:

```
OpenSR/
├── data/
│   ├── *.pkg (game resource files)
│   ├── CacheData.dat (decoded)
│   ├── Main.dat (decoded)
│   ├── Rus.dat or Eng.dat (decoded)
│   └── Music/ (music files)
├── libworld.so -> build/World/libworld.so
└── imageformats/
    └── libQtOpenSRImagePlugin.so -> ../build/ImagePlugin/libQtOpenSRImagePlugin.so
```

### Toolkit

#### ResourceViewer
GUI application for viewing and extracting resources from .pkg files. Supports game-specific formats (.gai, .gi, .hai, etc.).

**Note**: If built separately, in project directory run: 
```bash
./opensr_setup.sh -t tools
```

#### DATTools
Command-line utilities for managing DAT game files:

**opensr-dat-convert** - Encrypt/decrypt and compress/decompress DAT files:
```bash
./build/tools/DATTools/opensr-dat-convert hd ../OpenSRData/CacheData.dat data/CacheData.dat
```

**opensr-dat-json** - Convert between DAT and JSON formats:
```bash
./build/tools/DATTools/opensr-dat-json d2j data/main.dat data/main.json
```

Run tools with `-h` option for detailed usage information.

#### PlanetViewer
GUI application for interactive planet visualization with dynamic lighting. Must be launched from the project directory.

#### QuestPlayer
GUI application for viewing and testing .qm files containing text adventure content.

## Testing

### Testing Framework
OpenSR employs a comprehensive testing strategy to ensure code quality and reliability:
- **GUI Testing**: Automated interface testing using the Spix framework
- **Unit Testing**: Core functionality testing with Qt Test framework

### Building Tests

#### GUI Tests Setup

To enable GUI testing functionality, you need to install the Spix framework:

**Important**: Before installing Spix, ensure you have installed its required dependency - AnyRPC. Please refer to the [Spix documentation](https://github.com/faaxm/spix) for detailed AnyRPC installation instructions.

1. **Clone Spix repository** in your project directory:
```bash
git clone https://github.com/faaxm/spix
```

2. **Build and install Spix**:
```bash
cd spix
mkdir build && cd build
cmake -DSPIX_QT_MAJOR=6 -DSPIX_BUILD_EXAMPLES=OFF ..
cmake --build .
sudo cmake --install .
```

**Note**: GUI tests require Spix source code to be available in the `spix/` directory within the project. If Spix sources are not found, GUI test compilation will be automatically disabled.

To enable testing capabilities, build with the `BUILD_TESTS` option:
```bash
# In build directory
cmake -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug ../
cmake --build .
```

### Running Tests
Execute all tests using ctest with verbose output:
```bash
cd build
ctest -VV
```

**Note**: GUI tests automatically run in `--test-mode` when executed via ctest. For standalone GUI testing, run the test executable directly from the project directory:
```bash
./build/tests/qml/opensr_ui_test
```

### Code Coverage Analysis
OpenSR integrates gcovr for detailed code coverage reporting:

1. Build with coverage support enabled:
```bash
# In build directory
cmake -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ../
cmake --build .
```

2. Generate comprehensive coverage report:
```bash
# In project directory
cmake --build ./build --target global_coverage
```

The `global_coverage` target automatically verifies test availability and executes them via ctest before generating coverage data.

Coverage results in JSON format are available at:
`build/coverage/coverage.json`

## License

This project is licensed under the GNU General Public License - see the [LICENSE](LICENSE) file for details.

---

*This project is a fan-made recreation and is not affiliated with or endorsed by the original developers of Space Rangers 2.*