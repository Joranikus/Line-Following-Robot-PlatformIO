# Linefollowing Robot

### Making a linefollowing robot using arduino
This project blah blah lorem ipsum

#### TODO
* Tune pid-controller
* Implementer enkoder (Ørjan)
* Motorsignal (sende signal til motorer)
* Motorkontroller (koble sammen enkoder, pid, motorsignal)

Motorsignal class is responsible for sending signal to right pin, and making sure it is a valid pwm value.

Enkoder class is resposible for getting the position of the motor, and deriving to get the speed.

Pid class is responsible for adjusting the output value.

The direction class is responisible for converting the sensor inputs to outeput direction.

The motorcontroller class is responsible for combining the direction, pid, and enkoder.

#### Notes for testing
* Make sure left motor in real life is left motor in code, easy to switch up left and right (remember to check pins too!)
* 