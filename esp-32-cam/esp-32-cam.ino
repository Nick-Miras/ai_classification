#include <esp32cam.h>
#include <WiFi.h>
#include <HTTPClient.h>
// #include <ArduinoHttpClient.h>

#define CHANNEL 1

const char* ssid = "Miras Family 2.5";
const char* password = "dingyenajjanjan18763";
const char* apiUrl = "http://192.168.254.103/receive_framebuffer";
const short port = 5000
static uint8_t PEER[]{0x4A, 0x55, 0x19, 0xC8, 0x6B, 0x21};

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
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

// Init ESP Now with fallback
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}

bool captureImageAndSendToAPI() {
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("capture() failure");
    return;
  }
  Serial.printf("capture() success: %dx%d %zub\n", frame->getWidth(), frame->getHeight(),
                frame->size());

  // HTTPClient client;
  // if (client.begin(apiUrl)) {
  //   client.addHeader("Content-Type", "application/octet-stream");
  //   int httpResponseCode = client.POST(frame->data(), frame->size());
  //   if (httpResponseCode>0) {
  //     Serial.print("HTTP Response code: ");
  //     Serial.println(httpResponseCode);
  //   } else {
  //     Serial.println("Error sending data");
  //   }
  //   client.end();
  // } else {
  //   Serial.println("Connection to server failed");
  // }

  return sendImageToApi(frame->data(), frame->size());
}


bool sendImageToAPI(uint8_t * imageData, size_t imageSize) {
  HTTPClient client;
  if (client.begin(apiUrl)) {
    client.addHeader("Content-Type", "application/octet-stream");
    int httpResponseCode = client.POST(imageData&, imageSize&);
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.println("Error sending data");
    }
    client.end();
  } else {
    Serial.println("Connection to server failed");
  }

  delay(5000);
}

void setupESPNOW() {
  bool ok = WifiEspNow.begin();
  if (!ok) {
    Serial.println("WifiEspNow.begin() failed");
    ESP.restart();
  }

  WifiEspNow.onReceive(printReceivedMessage, nullptr);

  ok = WifiEspNow.addPeer(PEER);
  if (!ok) {
    Serial.println("WifiEspNow.addPeer() failed");
    ESP.restart();
  }
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

  Serial.print("IP Address: http://");
  Serial.println(WiFi.localIP());

  setupCamera();
  setupESPNOW();

  WifiEspNow.onReceive(onWasteDetected, nullptr);
}

void onWasteDetected(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t* buf, size_t count,
                     void* arg)
{ 
  if (buf[0] != 0) {
    // if there is waste detected (i.e: :param:buf == true)
    int isWasteBiodegradable
    if (captureImageAndSendToAPI() == true) {
      isWasteBiodegradable = true;
    } else {
      isWasteBiodegradable = false;
    }
    byte data[1];
    data[0] = isWasteBiodegradable;
    WifiEspNow.send(PEER, data, sizeof(data));
    delay(1000);
  }
}

void loop() {

}