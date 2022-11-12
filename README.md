## Twenty-Forty Eight

A 2048 clone, written in modern C++

![Preview of playing the game](./preview.gif)

### Building

Build with cmake to automatically pull in all dependencies:

```
cmake -S . -B build
cmake --build build -j 4
./build/src/TFE
```

### Controls

- **WASD** or **arrow keys** to slide
- **N** to start a new game
- **ESC** to exit

### Acknowledgements

Graphics have been done with the help of the [SFML](https://github.com/SFML/SFML) library

This game is heavily inspired by [2048 by Gabriele Cirulli](https://github.com/gabrielecirulli/2048)
