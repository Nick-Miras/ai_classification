#include <ArduinoJson.h>
#include <esp32cam.h>
#include <WifiEspNow.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <StreamString.h> // For StreamBuffer functionality

#define CHANNEL 1

const char* ssid = "Miras Family 2.5"; //TODO: Change
const char* password = "dingyenajjanjan18763"; //TODO: Change
const char* apiUrl = "http://192.168.254.103:8000/upload"; // LOCAL ADDRESS FOR TESTING
// const char* apiUrl = "http://172.83.14.130:8000/upload"; // REMOTE SERVER ADDRESS
const uint8_t PEER[]{0x4A, 0x55, 0x19, 0xC8, 0x6B, 0x21};

static bool isWasteDetected;

esp32cam::Resolution initialResolution = esp32cam::Resolution::find(255, 255);

void setupCamera() {
    using namespace esp32cam;

    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(initialResolution);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    if (!ok) {
        Serial.println("camera initialize failure");
        delay(5000);
        ESP.restart();
    }
    Serial.println("camera initialize success");
}

// returns -1 if there are errors, 0 if the waste is not biodegradable, 1 if the waste is biodegradable
short sendImageToAPI(uint8_t * imageData, size_t imageSize) {
    short response;
    HTTPClient client;
    if (client.begin(apiUrl)) {
        client.addHeader("Content-Type", "application/octet-stream");
        int httpResponseCode = client.POST(imageData, imageSize);
        if (httpResponseCode > 0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            String responseBody = client.getString();
            Serial.println(responseBody);

            JsonDocument doc;
            deserializeJson(doc, responseBody);
            bool whatIsWaste = doc["message"]["isWasteBiodegradable"];
            Serial.print("Is Waste Biodegradable: ");
            Serial.println(whatIsWaste);

            response = whatIsWaste;
        } else {
            Serial.println("Error sending data");
            response = -1;
        }
    client.end();
    } else {
        Serial.println("Connection to server failed");
        response = -1;
    }
    return response;
}

auto captureImage() -> std::unique_ptr<esp32cam::Frame> {
    auto frame = esp32cam::capture();
    if (frame == nullptr) {
            Serial.println("capture() failure");
    } else {
        Serial.printf("capture() success: %dx%d %zub\n", frame->getWidth(), frame->getHeight(), frame->size());
    }
    return frame;
}

void onWasteDetectedESPNow(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t* buf, size_t count, void* arg) { 
    Serial.println();
    Serial.print("Message Received From: ");
    Serial.printf("{0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X};\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    Serial.println();
    if (buf[0] != 0) {
            // if there is waste detected (i.e: :param:buf == true)
            isWasteDetected = true;
    }
}

void setupESPNOW() {
    bool ok = WifiEspNow.begin();
    if (!ok) {
        Serial.println("WifiEspNow.begin() failed");
        ESP.restart();
    }

    WifiEspNow.onReceive(onWasteDetectedESPNow, nullptr);

    ok = WifiEspNow.addPeer(PEER);
    if (!ok) {
        Serial.println("WifiEspNow.addPeer() failed");
        ESP.restart();
    }
}

void testCaptureImageAndSendToAPI() {
    auto frame = captureImage();
    if (frame != nullptr) {
        Serial.println("Sending Image To API");
        short response = sendImageToAPI(frame->data(), frame->size());
        Serial.print("Response: ");
        Serial.println(response);
    }
}

void testJsonResponse() {
    HTTPClient client;
    const char* url = "http://192.168.254.103:8000";
    if (client.begin(url)) {
        int httpResponseCode = client.GET();
        if (httpResponseCode > 0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            String responseBody = client.getString();
            Serial.println(responseBody);

            JsonDocument doc;
            deserializeJson(doc, responseBody);
            bool whatIsWaste = doc["message"]["isWasteBiodegradable"];
            Serial.print("Is Waste Biodegradable: ");
            Serial.println(whatIsWaste);
        } else {
            Serial.println("Error sending data");
        }
    client.end();
    } else {
        Serial.println("Connection to server failed");
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

    delay(2000);

    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    WiFi.softAP("ESPNOW", nullptr, 3);
    WiFi.softAPdisconnect(false);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    printMacAddress();

    setupCamera();
    setupESPNOW();
}

void onWasteDetected() {
    Serial.println("Waste Detected");
    auto frame = captureImage();
    if (frame != nullptr) {
        short isWasteBiodegradable = sendImageToAPI(frame->data(), frame->size());
        if (isWasteBiodegradable != -1) {
            byte data[1];
            data[0] = isWasteBiodegradable;
            if (WifiEspNow.send(PEER, data, sizeof(data)) == true) {
                Serial.println("Successfully Informed PEER");
            } else {
                Serial.println("Failed To Inform Peer");
            }
        } else {
            Serial.println("API ERROR");
        }
    }
}

void loop() {
    if (isWasteDetected == true) {
        onWasteDetected();
        isWasteDetected = false;
    }
}