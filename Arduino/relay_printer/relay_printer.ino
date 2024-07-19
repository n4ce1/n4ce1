#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "kp";        // Replace with your WiFi SSID
const char* password = "relay-printer"; // Replace with your WiFi password

const int relay1 = 5;                  // Define Pin of the Relay

WebServer server(80);

void handle_Root() {
  String html = "<html>\
                  <head>\
                    <title>ESP32 Relay Control</title>\
                    <style>\
                      body { font-family: Arial; display: flex; justify-content: center; align-items: center; height: 100vh; background-color: #f0f0f0; }\
                      .container { text-align: center; }\
                      button { padding: 20px; font-size: 24px; margin: 10px; cursor: pointer; }\
                      .on { background-color: green; color: white; }\
                      .off { background-color: red; color: white; }\
                    </style>\
                    <script>\
                      function sendRequest(action) {\
                        var xhr = new XMLHttpRequest();\
                        xhr.open('GET', '/' + action, true);\
                        xhr.onreadystatechange = function() {\
                          if (xhr.readyState == 4 && xhr.status == 200) {\
                            document.getElementById('status').innerHTML = xhr.responseText;\
                          }\
                        };\
                        xhr.send();\
                      }\
                    </script>\
                  </head>\
                  <body>\
                    <div class='container'>\
                      <h1>ESP32 Relay Control</h1>\
                      <p><button class='on' onclick=\"sendRequest('on')\">Włącz drukarkę</button></p>\
                      <p><button class='off' onclick=\"sendRequest('off')\">Wyłącz drukarkę</button></p>\
                      <p id='status'></p>\
                    </div>\
                  </body>\
                </html>";
  server.send(200, "text/html", html);
}

void handle_OnConnect() {
  digitalWrite(relay1, LOW);   // Turn the relay on
  server.send(200, "text/plain", "Drukarka Włączona");
}

void handle_OffConnect() {
  digitalWrite(relay1, HIGH);  // Turn the relay off
  server.send(200, "text/plain", "Drukarka Wyłączona");
}

void setup() {
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, HIGH);  // Ensure the relay is off

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handle_Root);
  server.on("/on", HTTP_GET, handle_OnConnect);
  server.on("/off", HTTP_GET, handle_OffConnect);

  server.begin();
}

void loop() {
  server.handleClient();
}
