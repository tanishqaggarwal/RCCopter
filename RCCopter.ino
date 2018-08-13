/* Main controller logic for RC helicopter.
* @author Tanishq Aggarwal
* @date July 4, 2018
* @license MIT
*/

#include <math.h>

#include "rotor_controller.hpp"
#include <SparkFunMAX17043.h>
#include <SparkFunTMP102.h>
#include <MPU9150.h>
#include <NMEAGPS.h>
#include <RF24.h>
#include <COBS.h>
#include <FastCRC.h>

#define ROTOR_A_PIN 9
#define ROTOR_B_PIN 10
#define TAIL_ROTOR_PIN 11

#define TMP102_ADDR 0x48

#define RADIO_CE_PIN 1
#define RADIO_CS_PIN 2

RotorController rotors(ROTOR_A_PIN, ROTOR_B_PIN, TAIL_ROTOR_PIN);

RF24 radio(RADIO_CE_PIN, RADIO_CS_PIN);
FastCRC16 CRC16;
FastCRC8 CRC8;
COBS cobs;

MPU9150 imu(MPU9150_ADDRESS_AD0_LOW);
NMEAGPS gps;
gps_fix gps_fix;

MAX17043 lipo;
TMP102 tempSensor(TMP102_ADDR);

/** \brief Creates COBS-encoded packet of sensor data 
 * for sending to control server. 
 * \param packet Array into which the packed data will be dumped.
 * \param encoded_packet Array into which the COBS-encoded packed data will be dumped.
 * \param timestamp Microsecond timestamp at which the packet data capture began.
 * \param imu_values 16-bit accelerometer, gyroscope, and magnetometer values reported by IMU.
 * \param charge 8-bit value indicating charge remaining on battery.
 * \param temp 16-bit value indicating ambient temperature of helicopter.
 * \param gps_data 32-bit latitude, longitude, and altitude values reported by IMU.
 * */
void packet_create(uint8_t *packet,
                   uint8_t *encoded_packet,
                   uint32_t timestamp,
                   int16_t *imu_values,
                   uint8_t charge,                   
                   uint16_t temp,
                   int32_t *gps_data) {
    /** Pack up data **/
    // Pack up timestamp
    packet[0] = timestamp;
    packet[1] = timestamp >> 8;
    packet[2] = timestamp >> 16;
    packet[3] = timestamp >> 24;
    // Pack up IMU data
    for (int i = 0; i < 9; i++)
    {
        packet[4 + 2 * i] = imu_values[i];
        packet[4 + 2 * i + 1] = imu_values[i] >> 8;
    }
    // LiPO charge and temperature
    packet[22] = charge;
    packet[23] = temp;
    packet[24] = temp >> 8;
    // GPS information
    for (int i = 0; i < 3; i++)
    {
        packet[25 + 4 * i] = gps_data[i];
        packet[25 + 4 * i + 1] = gps_data[i] >> 8;
        packet[25 + 4 * i + 2] = gps_data[i] >> 16;
        packet[25 + 4 * i + 3] = gps_data[i] >> 24;
    }

    // CRC16 Checksum
    uint16_t checksum = CRC16.ccitt(packet, 37);
    packet[37] = checksum;
    packet[38] = checksum >> 8;

    // COBS Encoding
    cobs.encode(packet, 39, encoded_packet);
    encoded_packet[40] = (uint8_t)0x00;
}

/** Decomposes the packet sent by the control server into motor 
 * and LED control instructions. 
 * \param control_packet A 9-byte packet sent by control server. 
 * \param instructions Motor and LED control instructions, structured like this:
 * Bytes 0,1 Rotor A speed
 * Bytes 2,3 Rotor B speed
 * Bytes 4,5 Tail Rotor speed
 * Bytes 6 LED control
 * The array sent in needs to have a length of at least 8 bytes, in order to hold the CRC
 * checksum as well. This is to prevent this function from having to create its own array,
 * which is inefficient.
 * \returns True if CRC8 checksum passed, false otherwise.
 **/
bool packet_decompose(uint8_t *control_packet, uint8_t *instructions ) {
    cobs.decode(control_packet, 8, instructions);
    if (CRC8.smbus(instructions, 7) != instructions[7]) {
        return false;
    }
    return true;
}

void setup()
{
    radio.begin();
    imu.initialize();
    lipo.begin();
    tempSensor.begin();
}

void loop() {
    uint8_t packet[39];
    uint8_t encoded_packet[41];

    uint32_t timestamp = micros();

    int16_t imu_values[9];
    imu.getMotion9(imu_values[0], imu_values[1], imu_values[2], imu_values[3], 
        imu_values[4], imu_values[5], imu_values[6], imu_values[7], imu_values[8]);
    uint8_t charge = (uint8_t) lipo.getSOC();
    uint16_t temp = (uint16_t) tempSensor.readTempF();

    int32_t gps_data[3];
    if (gps.available(Serial)) {
        gps_fix = gps.read();
        gps_data[0] = gps_fix.latitudeL();
        gps_data[1] = gps_fix.longitudeL();
        gps_data[2] = gps_fix.altitude_cm();
    }

    packet_create(packet, encoded_packet, timestamp, imu_values, charge, temp, gps_data);
}