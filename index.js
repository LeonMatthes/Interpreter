const path = require('path');
const fs = require('fs');

// https://github.com/webpack/webpack/issues/4175#issuecomment-342931035
const requireFunction = typeof __webpack_require__ === 'function' ? __non_webpack_require__ : require;

const pathsToTry = [
    path.resolve(__dirname, 'build', 'Debug', 'InterpreterNan.node'),
    path.resolve(__dirname, 'build', 'Release', 'InterpreterNan.node')
];

const modulePath = pathsToTry.find((path)=>fs.existsSync(path))

if(!modulePath){
    throw 'The path to the Interpereter node module could not be found!';
}

module.exports = requireFunction(modulePath);