#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "PowerManagement.h"

const char *ssid = "firstAID22";
const char *password = "iotempire";
const char *mqtt_server = "192.168.14.1";

WiFiClient espClient;
PubSubClient client(espClient);

PowerManagement power;

int channel = 0; // Current channel for the multiplexer

const int muxControlPins[] = {32, 33, 13, 16};
const int voltageSensorPins[] = {36, 39, 34};

const int muxOutputPin = 35;

void selectMuxChannel(uint8_t channel)
{
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(muxControlPins[i], (channel >> i) & 0x01);
    }
}

void setup_wifi()
{
    delay(10);
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("WiFi connected: ");
    Serial.println(WiFi.localIP());
}

void reconnect()
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP32Client"))
        {
            Serial.println("connected");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void setup()
{
    Serial.begin(115200);

    for (int i = 0; i < 4; i++)
    {
        pinMode(muxControlPins[i], OUTPUT);
    }

    pinMode(muxOutputPin, INPUT);

    setup_wifi();
    client.setServer(mqtt_server, 1883);

    power.begin();

    selectMuxChannel(channel);
}

void loop()
{
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();

    for (int channel = 0; channel < 16; channel++)
    {
        selectMuxChannel(channel);
        float current = power.getCurrentRMSManual(muxOutputPin, channel);
        Serial.print("> Current ");
        Serial.print(channel);
        Serial.print(" :");
        Serial.println(current);

        char currentTopic[32];
        char payload[10];
        snprintf(currentTopic, sizeof(currentTopic), "esp32/mux/channel%d", channel);
        snprintf(payload, sizeof(payload), "%.2f", current);
        client.publish(currentTopic, payload);
    }

    for (int i = 0; i < 3; i++)
    {
        float voltage = power.getVoltageRMSManual(voltageSensorPins[i]);

        Serial.print("> Voltage ");
        Serial.print(voltageSensorPins[i]);
        Serial.print(" :");
        Serial.println(voltage);

        char voltageTopic[32];
        char payload[10];
        snprintf(voltageTopic, sizeof(voltageTopic), "esp32/voltage/channel%d", i);
        snprintf(payload, sizeof(payload), "%.2f", voltage);
        client.publish(voltageTopic, payload);
    }
}