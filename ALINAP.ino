#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid     = "ESP1_Alina"; 
const char* password = "12345678";

ESP8266WebServer server(80);

const int leds[] = {D5, D3, D4}; 
const int numLeds = 3;
const int mainBtnPin = D8;    
const int extraBtnPin = D6;   

int currentLed = 0;  
bool reverseOrder = false;
bool isPaused = false;
uint32_t pauseStartTime = 0;
const uint32_t pauseDuration = 15000;

unsigned long lastUpdate = 0;
const int interval = 300;
unsigned long lastBtnTimeMain = 0;
unsigned long lastBtnTimeExtra = 0;
const int debounceDelay = 250; 

bool lastMainState = LOW;
bool lastExtraState = HIGH;

const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8"><title>Laboratory Dashboard</title>
  <style>
    body { font-family: 'Segoe UI', sans-serif; background: #f0f2f5; text-align: center; padding: 20px; color: #333; }
    .container { display: flex; flex-wrap: wrap; justify-content: center; gap: 20px; margin-top: 20px; }
    .panel { background: white; padding: 25px; border-radius: 25px; box-shadow: 0 8px 25px rgba(0,0,0,0.1); width: 340px; }
    .led-box { background: #2d2d2d; padding: 20px; border-radius: 20px; display: flex; justify-content: center; gap: 15px; margin: 15px 0; }
    .led { width: 45px; height: 45px; border-radius: 50%; opacity: 0.15; transition: 0.1s; }
    .pink { background-color: #ff2d55; }
    .yellow { background-color: #ffcc00; }
    .active { opacity: 1; box-shadow: 0 0 20px currentColor; }
    .btn { width: 100%; padding: 14px; border: none; border-radius: 12px; font-weight: bold; cursor: pointer; margin-bottom: 10px; color: white; transition: 0.2s; font-size: 14px; }
    .btn-pink { background: #ff2d55; }
    .btn-yellow { background: #ffcc00; color: #333; }
    .btn-dark { background: #333; }
    .btn:active { transform: scale(0.97); }
    h2 { font-size: 18px; margin-bottom: 5px; color: #666; }
  </style>
</head>
<body>
  <h1>Laboratory Control Panel</h1>
  <div class="container">
    <div class="panel">
      <h2>LOCAL DEVICE (ESP №2)</h2>
      <div id="statusL">Running</div>
      <div id="dirL" style="font-weight: bold; margin-bottom: 10px;">Direction: →</div>
      <div class="led-box">
        <div id="l0" class="led pink"></div><div id="l1" class="led pink"></div><div id="l2" class="led pink"></div>
      </div>
      <button class="btn btn-pink" onclick="fetch('/rev_local')">REVERSE LOCAL (ESP2)</button>
      <button class="btn btn-dark" onclick="fetch('/pause_both')">PAUSE BOTH DEVICES</button>
    </div>

    <div class="panel">
      <h2>PARTNER DEVICE (ESP №1)</h2>
      <div id="statusP">Partner Status</div>
      <div id="dirP" style="font-weight: bold; margin-bottom: 10px;">Partner Direction: →</div>
      <div class="led-box">
        <div id="p0" class="led yellow"></div><div id="p1" class="led yellow"></div><div id="p2" class="led yellow"></div>
      </div>
      <button class="btn btn-yellow" onclick="fetch('http://192.168.4.1/p_stop_only')">PAUSE PARTNER (ESP1)</button>
      <button class="btn btn-dark" onclick="fetch('/pause_both')">PAUSE BOTH DEVICES</button>
    </div>
  </div>
  <script>
    let partnerLedIndex = 0;
    setInterval(() => {
      fetch('/status').then(r => r.json()).then(d => {
        // Оновлення локальної плати
        document.getElementById('statusL').innerText = d.paused ? "PAUSED ("+d.timeLeft+"s)" : "Running";
        document.getElementById('dirL').innerText = d.rev ? "Direction: ←" : "Direction: →";
        for(let i=0; i<3; i++) document.getElementById('l'+i).classList.toggle('active', d.currentLed == i);
        
        // Оновлення партнера з даних, які проксіює ESP2
        let p = d.partner;
        document.getElementById('statusP').innerText = p.paused ? "PAUSED ("+p.timeLeft+"s)" : "Running";
        document.getElementById('dirP').innerText = "Partner Direction: " + (d.rev ? "←" : "→");

        if(!p.paused) {
          if(!d.rev) partnerLedIndex = (partnerLedIndex + 1) % 3;
          else partnerLedIndex = (partnerLedIndex - 1 + 3) % 3;
        }
        for(let i=0; i<3; i++) document.getElementById('p'+i).classList.toggle('active', partnerLedIndex == i);

      }).catch(e => {
        document.getElementById('statusP').innerText = "Connection Error";
      });
    }, 400);
  </script>
</body>
</html>
)=====";

void setup() {
  Serial.begin(115200); 
  Serial.println(""); 

  for (int i = 0; i < numLeds; i++) { 
    pinMode(leds[i], OUTPUT); 
    digitalWrite(leds[i], LOW); 
  }
  digitalWrite(leds[currentLed], HIGH);
  pinMode(mainBtnPin, INPUT);
  pinMode(extraBtnPin, INPUT_PULLUP);

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() { server.send(200, "text/html", MAIN_page); });
  
  server.on("/rev_local", []() { 
    reverseOrder = !reverseOrder; 
    Serial.print('R'); 
    server.send(200, "text/plain", "OK"); 
  });

  server.on("/pause_both", []() { 
    isPaused = !isPaused; 
    if(isPaused) pauseStartTime = millis(); else lastUpdate = millis();
    Serial.print('S'); 
    server.send(200, "text/plain", "OK"); 
  });

  server.on("/p_stop_only", []() { 
    Serial.print('S'); 
    server.send(200, "text/plain", "OK"); 
  });

  server.on("/status", []() {
    WiFiClient client;
    HTTPClient http;
    String partnerJson = "{\"paused\":false,\"timeLeft\":0}";
    
    if (http.begin(client, "http://192.168.4.1/status")) {
      int httpCode = http.GET();
      if (httpCode > 0) partnerJson = http.getString();
      http.end();
    }

    int timeLeft = isPaused ? (15000 - (millis() - pauseStartTime)) / 1000 : 0;
    if (timeLeft < 0) timeLeft = 0;

    String json = "{";
    json += "\"rev\":" + String(reverseOrder ? "true" : "false") + ",";
    json += "\"currentLed\":" + String(currentLed) + ",";
    json += "\"paused\":" + String(isPaused ? "true" : "false") + ",";
    json += "\"timeLeft\":" + String(timeLeft) + ",";
    json += "\"partner\":" + partnerJson;
    json += "}";
    
    server.send(200, "application/json", json);
  });
  
  server.begin();
}

void loop() {
  server.handleClient();
  unsigned long now = millis();

  if (Serial.available() > 0) {
    char incoming = Serial.read();
    if (incoming == 'R') reverseOrder = !reverseOrder;
    if (incoming == 'S') { 
      isPaused = !isPaused; 
      if(isPaused) pauseStartTime = now; else lastUpdate = now;
    }
  }

  bool extraR = digitalRead(extraBtnPin);
  if (extraR == LOW && lastExtraState == HIGH) {
    if (now - lastBtnTimeExtra > debounceDelay) { 
      isPaused = !isPaused; 
      if(isPaused) pauseStartTime = now; else lastUpdate = now;
      Serial.print('S'); 
      lastBtnTimeExtra = now; 
    }
  }
  lastExtraState = extraR;

  bool mainR = digitalRead(mainBtnPin);
  if (mainR == HIGH && lastMainState == LOW) {
    if (now - lastBtnTimeMain > debounceDelay) { 
      reverseOrder = !reverseOrder; 
      Serial.print('R'); 
      lastBtnTimeMain = now; 
    }
  }
  lastMainState = mainR;

  if (isPaused) {
    if (now - pauseStartTime >= pauseDuration) { isPaused = false; lastUpdate = now; }
    for (int i = 0; i < numLeds; i++) digitalWrite(leds[i], (i == currentLed) ? HIGH : LOW);
  } else {
    if (now - lastUpdate >= interval) {
      lastUpdate = now;
      for (int i = 0; i < numLeds; i++) digitalWrite(leds[i], LOW);
      if (!reverseOrder) currentLed = (currentLed + 1) % numLeds;
      else currentLed = (currentLed - 1 + numLeds) % numLeds;
      digitalWrite(leds[currentLed], HIGH);
    }
  }
  yield();
}