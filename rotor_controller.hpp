/* Abstraction layer for controlling helicopter motors via PWM.
* @author Tanishq Aggarwal
* @date July 4, 2018
* @license MIT
*/
#include <stdint.h>

#ifndef MOTOR_CONTROLLER_HPP_
#define MOTOR_CONTROLLER_HPP_

class RotorController {
private:
	// Output pin values for motors.
	uint8_t rotorA, rotorB, tailRotor;
	// Current speed values of rotors. At construction, defaults to zero.
	uint8_t Aspeed, Bspeed, tailSpeed;

public:
	/* Initialization code for motors.
	* @param a The output pin that rotor A's PWM controller is attached to.
	* @param b The output pin that rotor B's PWM controller is attached to.
	* @param tail The output pin that the tail rotor's PWM controller is attached to.
	*/
	RotorController(uint8_t a, uint8_t b, uint8_t tail);

	/* Sets the speed of the rotors using PWM.
	* @param speedA The speed of rotor A, an int from 0 to 255.
	* @param speedB The speed of rotor B, an int from 0 to 255.
	* @param speedTail The speed of the tail rotor, an int from 0 to 255.
	*/
	void setSpeeds(uint8_t speedA, uint8_t speedB, uint8_t speedTail);

	/* Set tail speed. Does not change speed of main rotors.
	* @param s A speed value from 0 to 255.
	*/
	void setTailRotorSpeed(uint8_t s);

	/* Rotates the main rotors at specified speeds. Does not change tail rotor speed.
	* @param speedA A speed value for rotor A from 0 to 255.
	* @param speedB A speed value for rotor B from 0 to 255.
	*/
	void setMainRotorsSpeed(uint8_t speedA, uint8_t speedB);

	/* Rotates the main rotors at the same specified speed. Does not change tail rotor speed.
	* @param s A speed value from 0 to 255.
	*/
	void setMainRotorSpeed(uint8_t s);

	/* Allows clockwise rotation of the copter (as seen from above, in the copter frame) 
	* by setting A to a desired speed and changing B's speed appropriately.
	* @param s Speed of rotor A.
	* @param rrate Rotation rate of body, measured in degrees/second.
	*/
	void rotateClockwiseALeading(uint8_t s, uint8_t rrate);
};

#endif