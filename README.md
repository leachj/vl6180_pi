## Synopsis

This library provides convenient access to distance measurements from the VL6180 i2c time of flight distance sensor on the raspberry pi. The sensor board I have been using is from SparkFun https://www.sparkfun.com/products/12785

Although it is designed for the Pi it is likely to work with most linux systems.

## Code Example

The library is very simple to use, here is the basic code for getting distance measurements

```c
#include <vl6180_pi/vl6180_pi.h>

...

vl6180 handle = vl6180_initialise(1);

int distance = get_distance(handle);
```

The integer passed into initialise is the i2c device number /dev/i2c-n

The distance returned will be in mm

## Motivation

Whilst there as already an existing Arduino library for the sensor there was nothing to allow it to work with the Pi. This project solves that.

## Installation

To install the library download the code and then run 

```
make
sudo make install
```

If you do not have the i2c device on your pi you need to enable i2c. This can be done by following the instructions here: https://learn.adafruit.com/adafruits-raspberry-pi-lesson-4-gpio-setup/configuring-i2c

You may also need to install i2c-tools libi2c-dev

```
sudo apt-get install i2c-tools libi2c-dev
```

## Tests

To test the basic functionality download the code and then run

```
make check
```

## Contributors

Contributions are welcome. This only implements a very simple usage of the VL6180, it has many other functions which could be added to this library.

## License

This software is made available under the MIT license
