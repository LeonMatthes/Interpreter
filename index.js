const path = require('path');

// https://github.com/webpack/webpack/issues/4175#issuecomment-342931035
const requireFunction = typeof __webpack_require__ === 'function' ? __non_webpack_require__ : require;

module.exports = requireFunction(path.resolve(__dirname, 'build', 'Release', 'InterpreterNan.node'));