/* Abstraction layer for controlling helicopter motors via PWM.
* @author Tanishq Aggarwal
* @date Aug 1, 2018
* @license MIT
*/
#include <Arduino.h>
#include "rotor_controller.hpp"

RotorController::RotorController(uint8_t a, uint8_t b, uint8_t tail) {
    rotorA = a;
    rotorB = b;
    tailRotor = tail;
    pinMode(rotorA, OUTPUT);
    pinMode(rotorB, OUTPUT);
    pinMode(tailRotor, OUTPUT);
    Aspeed = 0;
    Bspeed = 0;
    tailSpeed = 0;
}

void RotorController::setSpeeds(uint8_t speedA, uint8_t speedB, uint8_t speedTail) {
    Aspeed = speedA;
    Bspeed = speedB;
    tailSpeed = speedTail;
    analogWrite(rotorA, convertSpeed(speedA));
    analogWrite(rotorB, convertSpeed(speedB));
    analogWrite(tailRotor, convertSpeed(speedTail));
}

void RotorController::setTailRotorSpeed(uint8_t s) {
    setSpeeds(Aspeed, Bspeed, s);
}

void RotorController::setMainRotorsSpeed(uint8_t speedA, uint8_t speedB) {
    setSpeeds(speedA, speedB, tailSpeed);
}

void RotorController::setMainRotorSpeed(uint8_t s) {
    setSpeeds(s, s, tailSpeed);
}

void RotorController::rotateClockwiseALeading(uint8_t s, uint8_t rrate) {
    setSpeeds(s, s - rrate, 0);
}

uint8_t RotorController::convertSpeed(uint8_t s) {
    return (s * 255 / 100);
}
