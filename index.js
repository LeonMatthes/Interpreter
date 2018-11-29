const path = require('path');
const requireFunction = typeof __webpack_require__ === 'function' ? __non_webpack_require__ : require;
console.log(path.resolve(__dirname, 'build', 'Release', 'InterpreterNan.node'));
module.exports = requireFunction(path.resolve(__dirname) + "/build/Release/InterpreterNan.node");