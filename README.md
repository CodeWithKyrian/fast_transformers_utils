# Fast Transformers Utils C Library

## Overview

Fast Transformers Utils is a C library providing a collection of fast operations to complement the PHP package
TransformersPHP. These operations are designed to handle computationally intensive tasks that would be inefficient if
implemented directly in PHP. The library is designed to be used via PHP's Foreign Function Interface (FFI) and must be
compiled into a shared library.

## Features

- High Performance: Optimized operations to significantly reduce computation time for specific tasks.
- PHP Integration: Seamless integration with PHP using PHP FFI.
- Core Functions:
    - `spectrogram`: Computes the spectrogram of a waveform.
    - `pad_reflect`: Reflectively pads an array.

## Requirements

- CMake 3.10 or higher
- C compiler (e.g., GCC)

## Building the Library

1. Clone the repository:

```bash
git clone https://github.com/Codewithkyrian/fast-transformers-utils.git
cd fast-transformers-utils
```

2. Generate the build files using CMake:

```bash
cmake -S . -B build
```

3. Build the shared library:

```bash
cmake --build build
```

4. Package the shared library:

```bash
(cd build; cpack -C Release)
```

The shared library will be available in the `build` directory.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request to contribute.

## License

Fast Transformers Utils is open-source software licensed under the [MIT license](https://opensource.org/licenses/MIT).
See the [LICENSE](LICENSE) file for more information.
