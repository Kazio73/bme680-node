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
#ifndef BME680_WRAPPER_H
#define BME680_WRAPPER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float temperature;
    float humidity;
    float pressure;
    float gas_resistance;
} bme680_data_t;

// Main sensor API
int bme680_init(uint8_t i2c_addr);
int bme680_read_data(bme680_data_t *data);
void bme680_close(void);

// Additional configuration
int bme680_set_config(uint8_t os_temp, uint8_t os_pres, uint8_t os_hum, uint8_t filter);
int bme680_set_heater(uint16_t temp, uint16_t duration_ms);
int bme680_set_mode(uint8_t op_mode);
int bme680_get_mode(uint8_t *mode);
int bme680_selftest(void);

#ifdef __cplusplus
}
#endif

#endif // BME680_WRAPPER_H

