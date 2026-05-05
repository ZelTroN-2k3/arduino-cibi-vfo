# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Current version
- add step increment mode. Increment can be set in config menu
- add scan feature in step increment mode.

## 2.1.0 - 2019-08-17
- enable FI in Rx CW
- change DDS's PLL frequency
- add clarifier offset feature

## 2.0.0 - 2019-07-29
- add RX clarifier feature
- add VFO offset feature
- add S-meter feature
- heavy refactorisation to introduce application modes config/cibi/transverter
- roll back to Adafruit si5351, modified to enable/disable output one by one

## 1.1.0 - 2019-05-14 (etherkit si5351)
- change si5351 library for one supporting: si5351B + allowing to select output one by one:
https://github.com/etherkit/Si5351Arduino

## 1.0.0 - 2019-05-02 (first stable version)
- initial creation
