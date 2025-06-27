#include "bme680.h"
#include "lib/bme68x.h"

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <string.h>

// Static sensor instance
static struct bme68x_dev dev;
static int i2c_fd = -1;
static uint8_t current_i2c_addr = 0x76;

/**
 * Custom I2C read function required by Bosch API.
 */
static int8_t user_i2c_read(uint8_t reg, uint8_t *data, uint32_t len, void *intf_ptr)
{
    if (write(i2c_fd, &reg, 1) != 1)
        return -1;
    if (read(i2c_fd, data, len) != (int)len)
        return -2;
    return 0;
}

/**
 * Custom I2C write function required by Bosch API.
 */
static int8_t user_i2c_write(uint8_t reg, const uint8_t *data, uint32_t len, void *intf_ptr)
{
    uint8_t buffer[len + 1];
    buffer[0] = reg;
    memcpy(&buffer[1], data, len);

    if (write(i2c_fd, buffer, len + 1) != (int)(len + 1))
        return -1;

    return 0;
}

/**
 * Custom delay function required by Bosch API (in microseconds).
 */
static void user_delay_us(uint32_t period, void *intf_ptr)
{
    usleep(period);
}

/**
 * Initialize BME680 sensor via I2C.
 */
int bme680_init(uint8_t i2c_addr)
{
    const char *i2c_path = "/dev/i2c-1";
    i2c_fd = open(i2c_path, O_RDWR);
    if (i2c_fd < 0)
        return -1;

    current_i2c_addr = i2c_addr;

    if (ioctl(i2c_fd, I2C_SLAVE, current_i2c_addr) < 0)
        return -2;

    dev.intf = BME68X_I2C_INTF;
    dev.intf_ptr = &current_i2c_addr;
    dev.read = user_i2c_read;
    dev.write = user_i2c_write;
    dev.delay_us = user_delay_us;

    if (bme68x_init(&dev) != BME68X_OK)
        return -3;

    return 0;
}

/**
 * Configure oversampling and filter settings.
 */
int bme680_set_config(uint8_t os_temp, uint8_t os_pres, uint8_t os_hum, uint8_t filter)
{
    struct bme68x_conf conf;
    conf.filter = filter;
    conf.odr = BME68X_ODR_NONE;
    conf.os_hum = os_hum;
    conf.os_temp = os_temp;
    conf.os_pres = os_pres;

    return bme68x_set_conf(&conf, &dev);
}

/**
 * Set heater configuration for gas measurements.
 */
int bme680_set_heater(uint16_t temp, uint16_t duration_ms)
{
    struct bme68x_heatr_conf heatr;
    heatr.enable = BME68X_ENABLE;
    heatr.heatr_temp = temp;
    heatr.heatr_dur = duration_ms;

    return bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr, &dev);
}

/**
 * Set sensor operation mode (e.g., FORCED_MODE).
 */
int bme680_set_mode(uint8_t op_mode)
{
    return bme68x_set_op_mode(op_mode, &dev);
}

/**
 * Get current operation mode.
 */
int bme680_get_mode(uint8_t *mode)
{
    return bme68x_get_op_mode(mode, &dev);
}

/**
 * Read compensated sensor data.
 */
int bme680_read_data(bme680_data_t *data)
{
    if (!data)
        return -1;

    if (bme680_set_mode(BME68X_FORCED_MODE) != BME68X_OK)
        return -2;

    dev.delay_us(150000, dev.intf_ptr); // Wait for measurement

    struct bme68x_data sensor_data;
    uint8_t n_data = 0;
    if (bme68x_get_data(BME68X_FORCED_MODE, &sensor_data, &n_data, &dev) != BME68X_OK)
        return -3;

    if (n_data == 0)
        return -4;

    data->temperature = sensor_data.temperature;
    data->humidity = sensor_data.humidity;
    data->pressure = sensor_data.pressure;
    data->gas_resistance = sensor_data.gas_resistance;

    return 0;
}

/**
 * Run self-test on the sensor (for low-gas variants).
 */
int bme680_selftest(void)
{
    return bme68x_selftest_check(&dev);
}

/**
 * Close I2C connection and cleanup.
 */
void bme680_close(void)
{
    if (i2c_fd >= 0)
        close(i2c_fd);
    i2c_fd = -1;
}
