# Interpreter [![Appveyor Status](https://ci.appveyor.com/api/projects/status/github/MrModder/Interpreter?branch=master&svg=true)](https://ci.appveyor.com/project/MrModder/interpreter)[![Build Status](https://travis-ci.org/MrModder/Interpreter.svg?branch=master)](https://travis-ci.org/MrModder/Interpreter)[![Coverage Status](https://coveralls.io/repos/github/MrModder/Interpreter/badge.svg?branch=master)](https://coveralls.io/github/MrModder/Interpreter?branch=master)


## Prerequisites
* CMake (v3.1.3 or higher)
* a C++11 compatible compiler
    * Note: gcc 4.8.1 is also supported, even though it does not support the \<regex\> C++11 feature
    
## Installing the Interpreter as a node module
Please first install all dependencies as listed above.
Install the Interpreter using ```npm install MrModder/Interpreter```.
You can now use ```require('InterpreterNan')``` to use the interpreter.

If you are using electron, you need to add additional configurations to your package.json file:
``` json
"cmake-js": {
    "runtime": "electron",
    "runtimeVersion": "electron-runtime-version-here",
    "arch": "whatever-setting-is-appropriate-for-your-application's-windows-build"
  }
```
This is neccessary because electron might use a different node version than the one you have installed.
For more information take a look at [cmake-js](https://github.com/cmake-js/cmake-js)
