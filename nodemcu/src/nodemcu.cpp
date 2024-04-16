#include "pins.h"
#include <WifiEspNow.h>
#include "ESP8266_ISR_Servo.h"
#include <ESP8266WiFi.h>

const char* ssid = "Miras Family 2.5";
const char* password = "dingyenajjanjan18763";
const uint8_t PEER[]{0x30, 0xC6, 0xF7, 0x30, 0xBB, 0x39}; // MAC Address Of ESP32

static short servoIndex = -1;
static bool isWasteBiodegradable;
static bool isMessageReceived = false;

bool getInfraredSensorStatus() {
    int sensorStatus = digitalRead(INFRARED_SENSOR_PIN);
    if (sensorStatus == 1) {
        return false;
    } else {
        return true;
    }
}

bool isWasteInFrontOfCamera() {
    // use infrared sensor
    if (getInfraredSensorStatus() == true) {
        return true;
    } else {
        return false;
    }
}

void throwWasteIntoBiodegradableBin() {
    ISR_Servo.setPosition(servoIndex, 20);
}

void throwWasteIntoNonBiodegradableBin() {
    ISR_Servo.setPosition(servoIndex, 160);
}

void setServoToNeutralPosition() {
    ISR_Servo.setPosition(servoIndex, 90);
}

void setupServoPins() {
    servoIndex = ISR_Servo.setupServo(SEGRAGATOR_SERVO_PIN); //TODO: Verify MIN-MAX Pulse Width
    if (servoIndex == -1) {
        Serial.println("Servo Setup Error!");
    } else {
        setServoToNeutralPosition();
    }
}

void onMessageReceived(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t* buf, size_t count, void* arg)
{   
    Serial.println();
    Serial.print("Message Received From: ");
    Serial.printf("{0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X}\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    Serial.println();
    isMessageReceived = true;
    if (buf[0] != 0) {
        // waste is biodegradable
        isWasteBiodegradable = true;
    } else {
        isWasteBiodegradable = false;
    }
}

void setupESPNOW() {
    bool ok = WifiEspNow.begin();
    if (!ok) {
        Serial.println("WifiEspNow.begin() failed");
        ESP.restart();
    }

    WifiEspNow.onReceive(onMessageReceived, nullptr);

    ok = WifiEspNow.addPeer(PEER);
    if (!ok) {
        Serial.println("WifiEspNow.addPeer() failed");
        ESP.restart();
    }
}

void printMacAddress() {
    Serial.print("MAC address of this node is ");
    Serial.println(WiFi.softAPmacAddress());

    uint8_t mac[6];
    WiFi.softAPmacAddress(mac);
    Serial.println();
    Serial.println("You can paste the following into the program for the other device:");
    Serial.printf("static uint8_t PEER[]{0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X};\n", mac[0],
                    mac[1], mac[2], mac[3], mac[4], mac[5]);
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    Serial.println();

    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    WiFi.softAP("ESPNOW", nullptr, 3);
    WiFi.softAPdisconnect(false);

    printMacAddress();

    setupESPNOW();

    setupServoPins();
    pinMode(INFRARED_SENSOR_PIN, INPUT);
}   

void checkForWastePresenceAndInformESP32() {
    if (isWasteInFrontOfCamera() == true) {
        delay(1000); // CHANGE DELAY
        Serial.println("Waste In Front Of Camera: True");
        byte data[1];
        data[0] = true;
        if (WifiEspNow.send(PEER, data, sizeof(data)) == true) {
            Serial.println("Successfully Informed PEER");
        } else {
            Serial.println("Failed To Inform Peer");
        }

    } else {
        Serial.println("Waste In Front Of Camera: False");
    }
}

void segregateWaste() {
    if (isMessageReceived == true) {
        if (isWasteBiodegradable == true) {
            Serial.println("Waste Is Biodegradable");
            throwWasteIntoBiodegradableBin();
        } else {
            Serial.println("Waste Is Non-Biodegradable");
            throwWasteIntoNonBiodegradableBin();
        }
        isMessageReceived = false;
        delay(5000);
        setServoToNeutralPosition();
    }
}

void loop() {
    checkForWastePresenceAndInformESP32();
    segregateWaste();
    delay(1000);
}
