// === Your full Arduino code starts here ===

#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <mbedtls/md.h>

const char* ssid = "God's Plan";       // Change to your WiFi SSID
const char* password = "ghost345";     // Change to your WiFi password

#define TRIG_PIN 5
#define ECHO_PIN 18

WebServer server(80);
String latestSensorData = "No data";
String latestHash = "";
String latestTimestamp = "";
void setCORSHeaders() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
}
String computeHash(String data) {
  unsigned char hash[32];
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char*)data.c_str(), data.length());
  mbedtls_md_finish(&ctx, hash);
  mbedtls_md_free(&ctx);

  char hashString[65];
  for (int i = 0; i < 32; i++) {
    sprintf(hashString + (i * 2), "%02x", hash[i]);
  }
  hashString[64] = 0;
  return String(hashString);
}

void readSensor() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;
  latestTimestamp = String(millis());
  latestSensorData = String(distance) + " cm";
  latestHash = computeHash(latestSensorData + latestTimestamp);
}

void handleRoot() {
  setCORSHeaders();
  server.send(200, "text/html","CIAOCIAO");
}

void handleSensor() {
  setCORSHeaders();
  readSensor();
  StaticJsonDocument<200> doc;
  doc["sensor_data"] = latestSensorData;
  doc["timestamp"] = latestTimestamp;
  doc["hash"] = latestHash;
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected!");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/sensor", handleSensor);
  server.begin();
}

void loop() {
  server.handleClient();
}
