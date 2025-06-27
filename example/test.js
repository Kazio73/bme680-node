/*
 * MIT License
 *
 * Copyright (c) 2025 Kazik Karczewski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

const bme = require('../');
const bme680 = require('../build/Release/bme680.node');


console.log('✅ Exports:', Object.keys(bme));

bme680.init(0x77);
bme680.setConfig(8, 4, 2, 0);
bme680.setHeater(300, 100);

try {
    const data = bme.readData();
    const iaq = bme.calculateIAQ(data.gas, 6000000);

    console.log('Temperature:', data.temperature.toFixed(2), '°C');
    console.log('Humidity:   ', data.humidity.toFixed(2), '%');
    console.log('Pressure:   ', data.pressure.toFixed(2), 'hPa');
    console.log('Gas:        ', data.gas.toFixed(2), 'Ω');
    console.log('Pseudo IAQ:', iaq);
} catch (err) {
    console.error('❌ Read error:', err.message);
}

bme.close();
