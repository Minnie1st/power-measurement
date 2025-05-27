#ifndef POWER_MANAGEMENT_H
#define POWER_MANAGEMENT_H

#include <Arduino.h>
#include <TrueRMS.h>
#include <map>

class PowerManagement
{
public:
    PowerManagement(float voltRange = 3.3, int rmsWindow = 50,
                    float sensorGain = 1.0, float ampsPerVolt = 15.0,
                    float voltageSensitivity = 507.25,
                    unsigned long samplePeriodMicros = 1000);

    void begin();
    float getCurrentRMSManual(uint8_t pin);
    float getVoltageRMSManual(uint8_t pin); // Manual RMS method for AC voltage

private:
    float voltRange;
    int rmsWindow;
    float sensorGain;
    float ampsPerVolt;
    float voltageSensitivity;
    unsigned long samplePeriod;
};

#endif