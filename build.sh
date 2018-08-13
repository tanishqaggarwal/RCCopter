ARDUINO_HOME=/Users/tanishqaggarwal/Documents/Arduino
ARDUINO_APP_HOME=/Applications/Arduino.app/Contents/Java
PROJECT_HOME=/Users/tanishqaggarwal/RCCopter

ARDUINO_LIB=$ARDUINO_HOME/libraries

# Copy libraries into Arduino's library folder
cd external
for lib in */; do
    cp -r $PWD/$lib $ARDUINO_LIB/$lib
done
cd ..

# Compile code
arduino-builder -hardware $ARDUINO_APP_HOME/hardware -tools $ARDUINO_APP_HOME/hardware/tools -tools $ARDUINO_APP_HOME/tools-builder  -fqbn arduino:avr:nano:cpu=atmega328 -libraries $ARDUINO_LIB RCCopter.ino

# Remove libraries from Arduino's library folder
cd external
for lib in */; do
    lib_stripped="$(basename $lib)"
    rm -r $ARDUINO_LIB/$lib_stripped
done
cd ..