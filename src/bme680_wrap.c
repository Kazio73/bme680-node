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

#include <node_api.h>
#include <stdio.h>
#include <stdint.h>
#include "bme680.h"

#define DECLARE_NAPI_METHOD(name, func)                \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

/**
 * init(address)
 */
napi_value InitSensor(napi_env env, napi_callback_info info) {
    napi_status status;
    size_t argc = 1;
    napi_value args[1];

    uint32_t address = 0x76;
    status = napi_get_cb_info(env, info, &argc, args, NULL, NULL);
    if (status != napi_ok || argc < 1)
        napi_throw_error(env, NULL, "Expected I2C address as argument");

    napi_get_value_uint32(env, args[0], &address);

    int result = bme680_init((uint8_t)address);

    napi_value ret;
    napi_create_int32(env, result, &ret);
    return ret;
}

/**
 * setConfig(osTemp, osPres, osHum, filter)
 */
napi_value SetConfig(napi_env env, napi_callback_info info) {
    napi_value args[4];
    size_t argc = 4;
    napi_get_cb_info(env, info, &argc, args, NULL, NULL);

    uint32_t osTemp, osPres, osHum, filter;
    napi_get_value_uint32(env, args[0], &osTemp);
    napi_get_value_uint32(env, args[1], &osPres);
    napi_get_value_uint32(env, args[2], &osHum);
    napi_get_value_uint32(env, args[3], &filter);

    int result = bme680_set_config(osTemp, osPres, osHum, filter);

    napi_value ret;
    napi_create_int32(env, result, &ret);
    return ret;
}

/**
 * setHeater(temp, duration)
 */
napi_value SetHeater(napi_env env, napi_callback_info info) {
    napi_value args[2];
    size_t argc = 2;
    napi_get_cb_info(env, info, &argc, args, NULL, NULL);

    uint32_t temp, duration;
    napi_get_value_uint32(env, args[0], &temp);
    napi_get_value_uint32(env, args[1], &duration);

    int result = bme680_set_heater(temp, duration);

    napi_value ret;
    napi_create_int32(env, result, &ret);
    return ret;
}

/**
 * setMode(mode)
 */
napi_value SetMode(napi_env env, napi_callback_info info) {
    napi_value args[1];
    size_t argc = 1;
    napi_get_cb_info(env, info, &argc, args, NULL, NULL);

    uint32_t mode;
    napi_get_value_uint32(env, args[0], &mode);

    int result = bme680_set_mode((uint8_t)mode);

    napi_value ret;
    napi_create_int32(env, result, &ret);
    return ret;
}

/**
 * getMode()
 */
napi_value GetMode(napi_env env, napi_callback_info info) {
    uint8_t mode = 0;
    int result = bme680_get_mode(&mode);

    napi_value obj;
    napi_create_object(env, &obj);

    napi_value resVal, modeVal;
    napi_create_int32(env, result, &resVal);
    napi_create_uint32(env, mode, &modeVal);

    napi_set_named_property(env, obj, "result", resVal);
    napi_set_named_property(env, obj, "mode", modeVal);
    return obj;
}

/**
 * readData()
 */
napi_value ReadData(napi_env env, napi_callback_info info) {
    bme680_data_t data;
    int result = bme680_read_data(&data);

    if (result != 0) {
        napi_throw_error(env, NULL, "Sensor read failed");
        return NULL;
    }

    napi_value obj;
    napi_create_object(env, &obj);

    napi_value val;
    napi_create_double(env, data.temperature, &val);
    napi_set_named_property(env, obj, "temperature", val);

    napi_create_double(env, data.humidity, &val);
    napi_set_named_property(env, obj, "humidity", val);

    napi_create_double(env, data.pressure, &val);
    napi_set_named_property(env, obj, "pressure", val);

    napi_create_double(env, data.gas_resistance, &val);
    napi_set_named_property(env, obj, "gas", val);

    return obj;
}

/**
 * selftest()
 */
napi_value Selftest(napi_env env, napi_callback_info info) {
    int result = bme680_selftest();

    napi_value ret;
    napi_create_int32(env, result, &ret);
    return ret;
}

/**
 * close()
 */
napi_value Close(napi_env env, napi_callback_info info) {
    bme680_close();
    return NULL;
}

/**
 * Module init
 */
napi_value Init(napi_env env, napi_value exports) {
    napi_property_descriptor descriptors[] = {
        DECLARE_NAPI_METHOD("init", InitSensor),
        DECLARE_NAPI_METHOD("setConfig", SetConfig),
        DECLARE_NAPI_METHOD("setHeater", SetHeater),
        DECLARE_NAPI_METHOD("setMode", SetMode),
        DECLARE_NAPI_METHOD("getMode", GetMode),
        DECLARE_NAPI_METHOD("readData", ReadData),
        DECLARE_NAPI_METHOD("selftest", Selftest),
        DECLARE_NAPI_METHOD("close", Close)
    };
    napi_define_properties(env, exports, sizeof(descriptors) / sizeof(*descriptors), descriptors);
    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
