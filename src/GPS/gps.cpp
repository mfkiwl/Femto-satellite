/**
 * @brief Simple GPS Library
 */
 
/*Author: Matias Vidal*/

#include "gps.h"

#define DEBUG_LEVEL 1

GPS::GPS(void) {
    gps_baudrate = 9600;
}

GPS::GPS(int baudrate) {
    gps_baudrate = baudrate;
}

void GPS::init() {
	if (I2Cgps.begin() == false)
	    SerialUSB.println("GPS failed to respond. Please check wiring.");	
    //Serial2.begin(gps_baudrate);
}

void GPS::updateData() {
    while (I2Cgps.available()) {
        if (gps.encode(I2Cgps.read())) {
            checkValidity();

            gpsData.hour = gps.time.hour();
            gpsData.minute = gps.time.minute();
            gpsData.second = gps.time.second();
            gpsData.satellites = gps.satellites.value();
            gpsData.latitude = gps.location.lat();
            gpsData.longitude = gps.location.lng();
            gpsData.altitude = gps.altitude.meters();
        }
    }
    if (millis() > 5000 && gps.charsProcessed() < 10) {
        SerialUSB.println(F("No GPS detected: check wiring."));
    }
}

uint8_t GPS::checkValidity(void) {
    uint8_t validity_ = 0;
    validity_ = gps.location.isValid()*(B1000000);
    validity_ += gps.altitude.isValid()*(B0100000);
    validity_ += gps.time.isValid()*(B00100000);
    validity_ += gps.satellites.isValid()*(B00010000);
    return validity_;
}
