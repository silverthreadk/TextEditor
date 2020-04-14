## TextEditor
VI-like text editor

<img src="resource/images/screenshot.png">

## Getting Started

### Prerequisites
* cmake
* a C++11-standard-compliant compiler
* python
* pip
* conan
```sh
pip install conan
pip install pyopenssl
# Add python script path to environment variables
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
```

### Installation
#### For windows
Clone and run the build:
```sh
git clone https://github.com/silverthreadk/TextEditor.git
mkdir build && cd build

#Debug
conan install .. -s build_type=Debug -s compiler.runtime=MTd
#Release
conan install .. -s build_type=Release -s compiler.runtime=MT

cmake -DCMAKE_GENERATOR_PLATFORM=x64 ../
cmake --build ./ --
```
