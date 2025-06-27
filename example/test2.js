const bme680 = require('../build/Release/bme680.node');

console.log('🔧 Initializing BME680...');
const result = bme680.init(0x77); // or 0x77
if (result !== 0) {
    console.error('❌ Initialization failed:', result);
    process.exit(1);
}

bme680.setConfig(8, 4, 2, 0);    // Temp x8, Pressure x4, Humidity x2, no filter
bme680.setHeater(300, 100);     // Heater: 300°C for 100ms

console.log('🌡 Reading sensor data...');
try {
    const data = bme680.readData();
    console.log('Temperature:', data.temperature.toFixed(2), '°C');
    console.log('Humidity:   ', data.humidity.toFixed(2), '%');
    console.log('Pressure:   ', data.pressure.toFixed(2), 'hPa');
    console.log('Gas:        ', data.gas.toFixed(2), 'Ω');
} catch (err) {
    console.error('❌ Read error:', err.message);
}

bme680.close();
console.log('✅ Done.');