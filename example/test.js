const bme = require('./build/Release/bme680.node');

bme.init(0x76);
bme.setConfig(8, 4, 2, 0);
bme.setHeater(300, 100);
bme.setMode(1); // FORCED_MODE
const data = bme.readData();
console.log(data);
bme.close();