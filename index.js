const sensor = require('bindings')('bme680');

module.exports = {
    readData: sensor.readData,
    // np. init(), close() itd.
};