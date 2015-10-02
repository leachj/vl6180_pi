## Synopsis

This library provides convenient access to distance measurements from the VL6180 time of flight distance sensor on the raspberry pi. The sensor board i have been using is from SparkFun https://www.sparkfun.com/products/12785

Although it is designed for the Pi it is likely to work with most linux systems.

## Code Example

The library is very simple to use, here is the basic code for getting distance measurements

```c
#include <vl6180_pi/vl6180_pi.h>

...

vl6180 handle = vl6180_initialise(1);

int distance = get_distance(handle);
```

## Motivation

Whilst there as already an existing Arduino library for the sensor there was nothing to allow it to work with the Pi. This project solves that.

## Installation

to install the library download the code and then run 

```
make
sudo make install
```

## Tests

to test the basic functionality download the code and then run

```
make check
```

## Contributors

Contributions are welcome. This only implements a very simple usage of the VL6180, it has many other functions which could be added to this library.

## License

This software is made available under the MIT license
