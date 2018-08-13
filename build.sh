# Used to trick Arduino compiler into thinking the local "external" directory is also the libraries directory
ln -s external/ /Users/tanishqaggarwal/Documents/Arduino/libraries
# Compile code
arduino-builder -hardware /Applications/Arduino.app/Contents/Java/hardware -tools /Applications/Arduino.app/Contents/Java/hardware/tools -tools /Applications/Arduino.app/Contents/Java/tools-builder  -fqbn arduino:avr:nano:cpu=atmega328 -libraries /Users/tanishqaggarwal/Documents/Arduino/libraries RCCopter.ino