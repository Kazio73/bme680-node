# bme680-node

> Native Node.js addon for Bosch BME680 environmental sensor (temperature, humidity, pressure, gas resistance) using N-API and official Bosch C driver.

[![npm version](https://img.shields.io/npm/v/bme680-node.svg)](https://www.npmjs.com/package/bme680-node)
[![license: MIT](https://img.shields.io/badge/license-BSD--3--Clause-blue.svg)](LICENSE)

## Features

- Native performance using N-API
- Uses official Bosch `bme68x` C driver
- I²C communication via `/dev/i2c-1`
- Access to temperature, humidity, pressure, gas resistance
- Full configuration API including heater settings
- Designed for Raspberry Pi / Linux SBCs

## Installation

```bash
npm install bme680-node
