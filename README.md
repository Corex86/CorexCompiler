# CodeIDE - Multi-Language Compiler

A modern and user-friendly IDE/Compiler application.

## Features

- ✅ Supports C++, C, Python, and JavaScript  
- ✅ Syntax highlighting  
- ✅ Line numbers  
- ✅ Real-time compilation and execution  
- ✅ Error display  
- ✅ File management (open/save)  
- ✅ Code editor (undo/redo, copy/paste)  
- ✅ Zoom in/out  
- ✅ Dark theme  
- ✅ Status bar  
- ✅ Keyboard shortcuts  

## Build

### Using Qt Creator:
1. Open `CodeIDE.pro` in Qt Creator  
2. Click **Build > Run**

### Using qmake:
```bash
qmake
make
```

### Using CMake:
```bash
mkdir build
cd build
cmake ..
make
```

### Manual compilation:
```bash
# Linux/Mac
g++ -std=c++17 main.cpp compileride.cpp codeeditor.cpp syntaxhighlighter.cpp     -o CodeIDE `pkg-config --cflags --libs Qt5Widgets` -fPIC

# Windows (MinGW)
g++ -std=c++17 main.cpp compileride.cpp codeeditor.cpp syntaxhighlighter.cpp     -o CodeIDE.exe -I%QTDIR%\include -L%QTDIR%\lib -lQt5Widgets -lQt5Gui -lQt5Core
```

## Requirements

- Qt 5.12 or higher  
- C++17 compatible compiler  
- g++/gcc (for running C/C++ code)  
- python3 (for running Python code)  
- node.js (for running JavaScript code)  

## Usage

1. Select a language (C++, C, Python, JavaScript)  
2. Write your code  
3. Press **F5** or click the **Run** button  
4. View the output in the bottom panel  

## Keyboard Shortcuts

- **F5**: Run code  
- **Ctrl+N**: New file  
- **Ctrl+O**: Open file  
- **Ctrl+S**: Save  
- **Ctrl+Shift+S**: Save As  
- **Ctrl+Z**: Undo  
- **Ctrl+Y**: Redo  
- **Ctrl+X**: Cut  
- **Ctrl+C**: Copy  
- **Ctrl+V**: Paste  
- **Ctrl+A**: Select all  
- **Ctrl++**: Zoom in  
- **Ctrl+-**: Zoom out  

## File Structure

```
CodeIDE/
├── main.cpp                 # Main program
├── compileride.h            # Main window header
├── compileride.cpp          # Main window implementation
├── codeeditor.h             # Code editor header
├── codeeditor.cpp           # Code editor implementation
├── syntaxhighlighter.h      # Syntax highlighter header
├── syntaxhighlighter.cpp    # Syntax highlighter implementation
├── CodeIDE.pro              # Qt project file
├── CMakeLists.txt           # CMake build file
└── README.md                # This file
```

## License

MIT License  

## Contributing

Pull requests are welcome. For major changes, please open an issue first.  

## Author

Corex86 Team - 2025  
