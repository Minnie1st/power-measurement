#include "PowerManagement.h"

PowerManagement::PowerManagement(float voltRange, int rmsWindow,
                                 float sensorGain, float ampsPerVolt,
                                 float voltageSensitivity,
                                 unsigned long samplePeriodMicros)
    : voltRange(voltRange), rmsWindow(rmsWindow),
      sensorGain(sensorGain), ampsPerVolt(ampsPerVolt),
      voltageSensitivity(voltageSensitivity),
      samplePeriod(samplePeriodMicros)
{
}

void PowerManagement::begin()
{
    analogReadResolution(12);
}

float PowerManagement::getCurrentRMSManual(uint8_t pin)
{
    constexpr float ADC_SCALE = 4096.0f;
    constexpr float FREQUENCY = 50.0f;
    const uint32_t period = 1000000 / FREQUENCY;

    // Step 1: Get zero point
    uint32_t Vsum = 0;
    uint32_t count = 0;
    uint32_t t_start = micros();
    while (micros() - t_start < period)
    {
        Vsum += analogRead(pin);
        count++;
    }
    int zeroPoint = Vsum / count;

    // Step 2: RMS calculation
    t_start = micros();
    uint32_t sumSq = 0;
    count = 0;
    while (micros() - t_start < period)
    {
        int val = analogRead(pin) - zeroPoint;
        sumSq += val * val;
        count++;
    }

    float rmsADC = sqrt((float)sumSq / count);
    float currentRMS = (rmsADC / ADC_SCALE) * voltRange * ampsPerVolt * 1000.0f; // Convert to mA

    return currentRMS;
}

float PowerManagement::getVoltageRMSManual(uint8_t pin)
{
    constexpr float ADC_SCALE = 4095.0f;
    constexpr float VREF = 3.3f;
    constexpr float FREQUENCY = 50.0f;
    const uint32_t period = 1000000 / FREQUENCY;

    // Step 1: Get zero point
    uint32_t Vsum = 0;
    uint32_t count = 0;
    uint32_t t_start = micros();
    while (micros() - t_start < period)
    {
        Vsum += analogRead(pin);
        count++;
    }
    int zeroPoint = Vsum / count;

    // Step 2: RMS calculation
    t_start = micros();
    uint32_t sumSq = 0;
    count = 0;
    while (micros() - t_start < period)
    {
        int val = analogRead(pin) - zeroPoint;
        sumSq += val * val;
        count++;
    }

    float rmsADC = sqrt((float)sumSq / count);
    float voltageRMS = (rmsADC / 4096) * voltRange * voltageSensitivity;

    return voltageRMS;
}