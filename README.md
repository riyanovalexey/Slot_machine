# Slot Machine

Test Task: A slot machine game with keyboard and mouse controls.

[Русская версия](README_RU.md)

## Controls
- **Mouse**: Click on interface buttons
- **Keyboard**:
  - **Space** - Start spinning
  - **Enter** - Stop spinning
  - **↑/↓** - Increase/decrease bet
  - **Escape** - Exit

## Requirements
- C++17
- SFML 3.0.x
- CMake 3.10+

## Building

### Linux
```bash
chmod +x setup.sh
./setup.sh
```

### Windows
```batch
setup.bat
```

The script will automatically detect SFML and offer to install it via vcpkg if needed.

### Manual SFML Installation

Install into `external/SFML` directory:

```
external/SFML/
├── include/
├── lib/
└── ...

```

Note that the file placement is crucial for successful project building.

## Project Structure
- `src/` - Source files
- `include/` - Header files
- `resources/` - Resources (fonts)
- `external/` - External libraries 