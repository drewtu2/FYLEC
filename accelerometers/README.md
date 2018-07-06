# Working with the ADXL337
In this project we explored potential use cases for the ADXL337 accelerometer 
from SparkFun. We did this by building a color coded velocity indicator to give
the user an idea of the speed they're traveling. The velocity is initialized to 
zero with the LED set to red. As the users velocity increases, the LED gradually
transitions from red, through white, to green. The target points for each color
are set at 0 m/s, 1 m/s and 2 m/s. 

## Hookup Guide
TODO

## Reading from the Accelerometer
The acceleration measurements from the ADXL337 can be read from the analog ports. 
Each (X, Y, and Z) can be read from its own port. To keep the code better organized,
an accelerometer struct is created to hold the accelerometer data (line 3). 
Line 63 shows an example of how to read the data from the analog ports. 

## Using the Accelerometer Data
Accelerometer data can be used in a number of ways. In this section, we discuss
scaling, using the acceleration to find velocity and jerk, and discuss some of the 
limitations and nuances with the accelerometer.

### Scaling Data
The raw values from the accelerometer are given on a scale of 0-1023; however,
in practice this range of values is not always seen (nor needed). Using the `mapf` 
function, (a custom map function for floats), the values can be rescaled onto a 
desired range. In this example, we used a bounding range of (350, 650) to encapsulate
the raw range we then mapped to (-1, 1). Raw values below 350 or above 650 would
will saturate to -1 and 1 respectively. See function `scaleAccel` defined on line
95. 

### Gravity and Acceleration
One important aspect to note when using the acclerometer data is that
the accelerometer will experience a magnitude of ~10m/s^2 of acceleration in 
one direction due to gravity. When the accelerometer is resting flat on a surface, 
this acceleration will be experienced along the Z axis. If the accelerometer is
at an angle, the acceleration due to gravity will be split across a subset of the
X, Y, and Z axes. 

The total magnitude of acceleration can be found by using the following equation:

`|A| = sqrt(accel_x^2 + accel_y^2 + accel_z^2)`

Since the acceleration due to gravity is a known constant, the acceleration 
without gravity can be found by subtracting gravity out in the magnitude 
equation as follows:

`|A| = sqrt(accel_x^2 + accel_y^2 + accel_z^2 - 10^2)`

### Velocity

In this project, we take the integral of the accelerometer data over time to 
find the velocity. We do this by summing the accelerations in each direction for
a period of time (`numReadings * deltaT`). The resulting velocity is then given 
by multiplying the accumulated acceleration by to total time spent accumulating 
acceleration.

`V = sum(accel(i), 0, n) * (n *dT)`

As mentioned in the previous sections, the acceleration due to gravity may 
skew the resulting velocity values if the accelerometer is not level. 
As mentioned before, the magnitude of the acceleration can be found, (integrating
with this will give the magnitude of the velocity) but the resulting value will
not include a direciton.

Unfortunatley, I am not aware of a way to find individual component velocities 
corrected for the gravitational skew without additional sensors. 

### Detecting Jerk
Another use case the accelerometers may be used for is detecting movement, or jerk. 
Jerk is the derivative of acceleration, or "how fast" something is accelerating. 





