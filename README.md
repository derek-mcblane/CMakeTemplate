## CMake Buildsystem
### CMake Workflow
This will configure, build, test, and install to a local directory: `install`. The build directory will be `./build/<preset-name>`, so `build/release` in this case.
```bash
cmake --workflow --preset=release
```
#### Available presets
- "release"
- "debug"
### CMake Presets
The stees of the above workflow can be run individually with "release and "debug" presets. This does the same as the above workflow.
```bash
cmake --preset=release
cmake --build --preset=release
ctest --preset=release
cmake --install build/release
```
### Configure, Build, Test, Install with CMake
Standard cmake commands work fine. This also does the same as the above workflow.
```bash
cmake -S . -B build/release
cmake --build build/release
ctest --test-dir build/release
cmake --install build/release --prefix=install
```
