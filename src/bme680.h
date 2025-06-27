int bme680_set_config(uint8_t os_temp, uint8_t os_pres, uint8_t os_hum, uint8_t filter);
int bme680_set_heater(uint16_t temp, uint16_t duration_ms);
int bme680_set_mode(uint8_t op_mode);
int bme680_get_mode(uint8_t *mode);
int bme680_selftest(void);
