#include <Servo.h>
#include "pins.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
// #include <HCSR04.h>


const char* ssid = "AITrashCan";
const char* password = "12345876";


// HCSR04 gUltrasonicSensor(ULTRASONIC_SENSOR_TRIGGER_PIN, ULTRASONIC_SENSOR_ECHO_PIN);
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
ESP8266WebServer server(80);

Servo gSegregatorServo;


bool getInfraredSensorStatus() {
    int sensorStatus = digitalRead(INFRARED_SENSOR_PIN);
    if (sensorStatus == 1) {
        return false;
    } else {
        return true;
    }
}

// bool getUltrasonicSensorStatus() {
//     float distance = gUltrasonicSensor.dist();
//     Serial.println(distance);
//     if (distance < 5 ) { // TODO: Adjust Distance
//         return true;
//     } else {
//         return false;
//     }
// }   

// TODO: Double Which Sensors Will Be Used For Which
// bool isSomeoneTryingToUseBin() {
//     // checks whether someone is trying to throw trash
//     // use ultrasonic sensor
//     if (getUltrasonicSensorStatus() == true) {
//         return true;
//     } else {
//         return false;
//     }
// }

bool isWasteInFrontOfCamera() {
    // use infrared sensor
    if (getInfraredSensorStatus()== true) {
        return true;
    } else {
        return false;
    }
}

// bool isWasteBiodegradable() {
//     // TODO Asks Server for Information From AI Camera
// }

void throwWasteIntoBiodegradableBin() {
    gSegregatorServo.write(250);
}

void throwWasteIntoNonBiodegradableBin() {
    gSegregatorServo.write(20);
}

void setupServoPins() {
    gSegregatorServo.attach(SEGRAGATOR_SERVO_PIN);
    gSegregatorServo.write(0);
}

// void setupServer() {
//     server.on("/", handleOnConnect) 
// }

// void handleOnConnect () {
// }

void setup() {
    Serial.begin(115200);
    pinMode(INFRARED_SENSOR_PIN, INPUT);
    setupServoPins();

    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ssid, password);
    // setupServer();
    server.begin();
    Serial.println("Server started at:");
    Serial.println(WiFi.softAPIP());
}

// void loop() {
//     if (isWasteInFrontOfCamera() == true) {
//         if (isWasteBiodegradable() == true) {
//             throwWasteIntoBiodegradableBin();
//         } else {
//             throwWasteIntoNonBiodegradableBin();
//         }
//     }
// }

void loop() {
    delay(2000);
    throwWasteIntoBiodegradableBin();
    delay(2000);
    throwWasteIntoNonBiodegradableBin();
    if (isWasteInFrontOfCamera() == true) {
        Serial.println("Waste In Front Of Camera: True");
    } else {
        Serial.println("Waste In Front Of Camera: False");
    }
}
