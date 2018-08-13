/* Main controller logic for RC helicopter.
* @author Tanishq Aggarwal
* @date July 4, 2018
* @license MIT
*/

#include "rotor_controller.hpp"
#include <SparkFunMAX17043.h>
#include <SparkFunTMP102.h>
#include <MPU9150.h>
#include <NMEAGPS.h>
#include <RF24.h>
#include <COBS.h>

#define ROTOR_A_PIN 9
#define ROTOR_B_PIN 10
#define TAIL_ROTOR_PIN 11

#define TMP102_ADDR 0x48

#define RADIO_CE_PIN 1
#define RADIO_CS_PIN 2

RotorController rotors(ROTOR_A_PIN, ROTOR_B_PIN, TAIL_ROTOR_PIN);

RF24 radio(RADIO_CE_PIN, RADIO_CS_PIN);

MPU9150 imu(MPU9150_ADDRESS_AD0_LOW);
NMEAGPS gps;

MAX17043 lipo;
TMP102 tempSensor(TMP102_ADDR);

void setup() {
    radio.begin();
    imu.initialize();
    lipo.begin();
    tempSensor.begin();
}

void loop() {

}