# AcnhPatternTools

Animal Crossing: New Horizons save/pattern editor.

## Requirements

- mbedtls
- wxWidgets (tested on 3.1.3)

### Debian/Ubuntu/etc

```sh
sudo apt install libmbedtls-dev

# Install wxWidgets with GTK (default)
https://wiki.wxwidgets.org/Compiling_and_getting_started

# Or install wxWidget with QT
https://docs.wxwidgets.org/3.1.3/plat_qt_install.html
```

### Windows and macOS

Set wxWidgets_ROOT_DIR and wxWidgets_LIB_DIR, [example](https://cmake.org/cmake/help/latest/module/FindwxWidgets.html).

#### Windows only

See https://tls.mbed.org/kb/compiling-and-building/using-mbedtls-in-microsoft-visual-studio-2015 and glhf.  
I hardcoded the path inside `horizonlib/CMakeLists.txt` so you'll need to change that.

You also need to copy the wxWidgets DLLs from wxWidgets_LIB_DIR to the directory containing the executable.

#### macOS only

```sh
brew install mbedtls libpng
```

## How to compile

```sh
mkdir build
cd build
cmake ..

# Now use make/msvc/whatever to build
make
```

## Credits

This project is based on or uses parts of the following software.

- [Cuyler36/MyHorizons](https://github.com/Cuyler36/MyHorizons)
- [3096/effective-guacamole](https://github.com/3096/effective-guacamole)
- [aappleby/smhasheraappleby/smhasher](https://github.com/aappleby/smhasher)
- [fmtlib/fmt](https://github.com/fmtlib/fmt)
- [stb_image and stb_image_write](https://github.com/nothings/stb)
