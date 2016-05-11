#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <FS.h>
#include <ArduinoJson.h>
#include "DHT.h"

#include "PAGE_Index.h"
#include "PAGE_Signin.h"
#include "PAGE_Style.h"
#include "PAGE_PubGpio.h"
#include "PAGE_SubGpio.h"

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

String initFileData;
String userFileData;
String mqttFileData;
String pubGpioFileData;
String subGpioFileData;
String stateFileData;

char message_buff[50];
long lastMsg = 0;
char testStr[50];
char tempStr[50];
int value = 0;
float oldT;
int i = 0;

//int subGpio[] = {4,5,12,13,14,15};

ESP8266WebServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);

void readRoot();
bool connectSTA();
void connectAP();
bool isAuthentified();
void mqttSubscribe();
void mqttPublish();
void connectMqtt();
void writeFile(String fileName, String state);
void readFile(int gpio);
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();

void readRoot() {
  Dir root = SPIFFS.openDir("/");
  Serial.println("Reading Root");
  while(root.next()) {
    String fileName = root.fileName();
    File f = root.openFile("r");
    Serial.printf("%s: %d\r\n", fileName.c_str(), f.size());
  }
}

void handleNotFound() {
  server.send(404, "text/plain", "Not Found Function");
}

bool isAuthentified() {
  if(server.hasHeader("Cookie")) {

    if(SPIFFS.exists("/user.json")) {
      Serial.println("Opening File");
      File file = SPIFFS.open("/user.json", "r");
      if(file) {
        Serial.println("File open successfully");
        while(file.available()) {
          userFileData = file.readStringUntil('~');
        }
        file.close();
        
        StaticJsonBuffer<200> jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(userFileData);
  
        const char* cookieName = root["cookieName"];
        const char* cookieValue = root["cookieValue"];
  
        Serial.println("cookieName: " + String(cookieName) + "\ncookieValue: " + String(cookieValue));

        String myCookie = String(cookieName) + "=" + String(cookieValue);
        String checkCookie = server.header("Cookie");
        Serial.println(checkCookie);
        Serial.println(myCookie);
        if(checkCookie == myCookie) {
          return true;
        } else {
          return false;
        }
   
      } else {
        Serial.println("Error opening file");
      }
      
    } else {
      Serial.println("File doesn't exist");
    }
  } else {
      return false;
  }  
}

bool connectSTA() {
  if(SPIFFS.exists("/init.json")) {
    Serial.println("Opening File");
    File file = SPIFFS.open("/init.json", "r");
    if(file) {
      Serial.println("File open successfully");
      while(file.available()) {
        initFileData = file.readStringUntil('~');
      }
      file.close();
      
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(initFileData);

      const char* ssidSTA = root["ssidSTA"];
      const char* passwordSTA = root["passwordSTA"];

      Serial.println("ssidSTA: " + String(ssidSTA) + "\npasswordSTA: " + String(passwordSTA));
  
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssidSTA, passwordSTA);
      int wifiStart = millis();
      while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println(millis());
        if((millis() - wifiStart) > 10000) {
          Serial.println("Breaking STA Loop");
          break;
        }
      }
      if(WiFi.status() == WL_CONNECTED) {
        return true;
      } else {
        return false;
      }
      
    } else {
      Serial.println("Error opening file");
    }
  } else {
    Serial.println("File doesn't exist");
  }
}

void connectAP() {
  if(SPIFFS.exists("/init.json")) {
    Serial.println("Opening File");
    File file = SPIFFS.open("/init.json", "r");
    if(file) {
      Serial.println("File open successfully");
      while(file.available()) {
        initFileData = file.readStringUntil('~');
      }
      file.close();
      
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(initFileData);

      const char* ssidAP = root["ssidAP"];
      const char* passwordAP = root["passwordAP"];

      Serial.println("ssidAP: " + String(ssidAP) + "\npasswordAP: " + String(passwordAP));

      WiFi.mode(WIFI_AP);
      WiFi.softAP(ssidAP, passwordAP);
      Serial.print("AP IP Address: ");
      Serial.println(WiFi.softAPIP());
      
    } else {
      Serial.println("Error opening file");
    }
  } else {
    Serial.println("File doesn't exist");
  }
}

void connectMqtt() {
  if(SPIFFS.exists("/mqtt.json")) {
    Serial.println("Opening File");
    File file = SPIFFS.open("/mqtt.json", "r");
    if(file) {
      Serial.println("File open successfully");
      while(file.available()) {
        mqttFileData = file.readStringUntil('~');
      }
      file.close();
      
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(mqttFileData);

      const char* mqttServer = root["mqttServer"];
     
      Serial.println("mqttServer: " + String(mqttServer));
      
      client.setServer(mqttServer, 1883);
      client.setCallback(callback);
      
    } else {
      Serial.println("Error opening file");
    }
  } else {
    Serial.println("File doesn't exist");
  }
}

void writeFile(String fileName, String state) {
  Serial.println("Writing GPIO State File");
  if(SPIFFS.exists(fileName)) {
    File uf = SPIFFS.open(fileName, "w");
    if(uf) {
      uf.print("{");
      uf.println("\t\"state\": \"" + state + "\"");
      uf.println("}");
      uf.print("~");
    }
    uf.close();
  }
}

void readFile(int gpio) {
  Serial.println("Reading Gpio State File");
  String fileName = "/" + String(gpio) + ".json";
  Serial.println(fileName);
  if(SPIFFS.exists(fileName)) {
    Serial.println("Opening File");
    File file = SPIFFS.open(fileName, "r");
    if(file) {
      Serial.println("File open successfully");
      while(file.available()) {
        stateFileData = file.readStringUntil('~');
      }
      file.close();
      
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(stateFileData);

      const char* state = root["state"];

      Serial.println("state: " + String(state));

      if(String(state) == "ON") {
        digitalWrite(gpio, HIGH);
      } else {
        digitalWrite(gpio, LOW);
      }
      
    } else {
      Serial.println("Error opening file");
    }
  } else {
    Serial.println("File doesn't exist");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  int i = 0;  
  
  // create character buffer with ending null terminator (string)
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  
  StaticJsonBuffer<400> jsonBuffer;        
  JsonObject& subGpio = jsonBuffer.parseObject(subGpioFileData);

  const char* gpio4 = subGpio["gpio4"];
  const char* gpio5 = subGpio["gpio5"];
  const char* gpio12 = subGpio["gpio12"];
  const char* gpio13 = subGpio["gpio13"];
  const char* gpio14 = subGpio["gpio14"];
  const char* gpio15 = subGpio["gpio15"];
       
  String msgString = String(message_buff);
  
  Serial.println("Topic: " + String(topic) + "\n" + "Payload: " + msgString);
  if(String(topic) == String(gpio4)){
    if (msgString == "ON") {
      digitalWrite(4, HIGH);
    } else {
      digitalWrite(4, LOW);
    }
    writeFile("/4.json", msgString);
  }

  if(String(topic) == String(gpio5)){
    if (msgString == "ON") {
      digitalWrite(5, HIGH);
    } else {
      digitalWrite(5, LOW);
    }
    writeFile("/5.json", msgString);
  }

  if(String(topic) == String(gpio12)){
    if (msgString == "ON") {
      digitalWrite(12, HIGH);
    } else {
      digitalWrite(12, LOW);
    }
    writeFile("/12.json", msgString);
  }

  if(String(topic) == String(gpio13)){
    if (msgString == "ON") {
      digitalWrite(13, HIGH);
    } else {
      digitalWrite(13, LOW);
    }
    writeFile("/13.json", msgString);
  }

  if(String(topic) == String(gpio14)){
    if (msgString == "ON") {
      digitalWrite(14, HIGH);
    } else {
      digitalWrite(14, LOW);
    }
    writeFile("/14.json", msgString);
  }

  if(String(topic) == String(gpio15)){
    if (msgString == "ON") {
      digitalWrite(15, HIGH);
    } else {
      digitalWrite(15, LOW);
    }
    writeFile("/15.json", msgString);
  }
  
}

void reconnect() {
  // Loop until we're reconnected
  int mqttStart = millis();
  
  while (!client.connected()) {
    if((millis() - mqttStart) > 2000) {
      Serial.println("Breaking MQTT Loop");
      break;
    }
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("wViOkOiD14zeO0TL484J")) {
      Serial.println("connected");

      mqttSubscribe();
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  if(client.connected()) {
    i = 0;
  } else {
    i++;
  }
}

void mqttSubscribe() {
  if(client.connected()) {
    if(SPIFFS.exists("/subgpio.json")) {
      Serial.println("Opening File");
      File file = SPIFFS.open("/subgpio.json", "r");
      if(file) {
        Serial.println("File open successfully");
        while(file.available()) {
          subGpioFileData = file.readStringUntil('~');
        }
        file.close();
  
  
        StaticJsonBuffer<400> jsonBuffer;        
        JsonObject& subGpio = jsonBuffer.parseObject(subGpioFileData);
  
        const char* gpio4 = subGpio["gpio4"];
        const char* gpio5 = subGpio["gpio5"];
        const char* gpio12 = subGpio["gpio12"];
        const char* gpio13 = subGpio["gpio13"];
        const char* gpio14 = subGpio["gpio14"];
        const char* gpio15 = subGpio["gpio15"];
       
        Serial.println("gpio4: " + String(gpio4) + "\ngpio5: " + String(gpio5) + "\ngpio12: " + String(gpio12) + "\ngpio13: " + String(gpio13) + "\ngpio14: " + String(gpio14) + "\ngpio15: " + String(gpio15));         
        client.subscribe(gpio4);
        client.subscribe(gpio5);
        client.subscribe(gpio12);
        client.subscribe(gpio13);
        client.subscribe(gpio14);
        client.subscribe(gpio15);
        
      } else {
        Serial.println("Error opening file");
      }
    } else {
      Serial.println("File doesn't exist");
    }
  }
}

void mqttPublish() {
  if(client.connected()) {
    if(SPIFFS.exists("/pubgpio.json")) {
      File file = SPIFFS.open("/pubgpio.json", "r");
      if(file) {
        while(file.available()) {
          pubGpioFileData = file.readStringUntil('~');
        }
        file.close();
  
  
        StaticJsonBuffer<200> jsonBuffer;        
        JsonObject& pubGpio = jsonBuffer.parseObject(pubGpioFileData);
  
        const char* gpio2 = pubGpio["gpio2"];

        long now = millis();
        if (now - lastMsg > 2000) {
          lastMsg = now;
          ++value;
          snprintf (testStr, 75, "hello world # %ld", value);
          client.publish("madhanani/outTopic", testStr);
        }

          float newT = dht.readTemperature();

          if (!isnan(newT)) {
            if (oldT != newT) {
              snprintf (tempStr, 75, "%d°C", (int)newT);
              Serial.print("Temperature: ");
              Serial.println(tempStr);
              client.publish(gpio2, tempStr);
              oldT = newT;
            }
          }

      } else {
        Serial.println("Error opening file");
      }
    } else {
      Serial.println("File doesn't exist");
    }
  }
}

void setup() { 
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);

  Serial.begin(115200);
  
  Serial.println("Begining SPIFFS");
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS Init Failed");
  }
  
  Serial.println("Initializing");
  readFile(4);
  readFile(5);
  readFile(12);
  readFile(13);
  readFile(14);
  readFile(15);
  
  readRoot();
  
  if(connectSTA()) {
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    connectAP();
  }
  
  server.on("/signin", HTTP_POST, [](){  
    if(server.hasArg("username") && server.hasArg("password")) {

      if(SPIFFS.exists("/user.json")) {
        Serial.println("Opening File");
        File file = SPIFFS.open("/user.json", "r");
        if(file) {
          Serial.println("File open successfully");
          while(file.available()) {
            userFileData = file.readStringUntil('~');
          }
          file.close();
          
          StaticJsonBuffer<200> jsonBuffer;
          JsonObject& root = jsonBuffer.parseObject(userFileData);

          const char* username = root["username"];
          const char* password = root["password"];
          const char* cookieName = root["cookieName"];
          const char* cookieValue = root["cookieValue"];
    
          Serial.println("username: " + String(username) + "password: " + String(password) + "cookieName: " + String(cookieName) + "\ncookieValue: " + String(cookieValue));
      
          if(server.arg("username") == username &&  server.arg("password") == password ){
            server.send(200, "text/json", "{\"success\":true, \"message\":\"You are now Logged In!\", \"cookie\": {\"name\":\"" + String(cookieName) + "\", \"value\":\"" + String(cookieValue) + "\"}}");
            return;
          } else {
            server.send(200, "text/json", "{ \"success\":false, \"message\":\"Incorrect Username or Password!\" }");
          }
          
        } else {
          Serial.println("Error opening file");
        }
      } else {
        Serial.println("File doesn't exist");
      }
    }
  });
    
  server.on("/", []() { 
    if(isAuthentified()){
      Serial.println("index.html"); 
      server.send (200, "text/html", PAGE_Index);  
    } else {
      Serial.println("Signin.html"); 
      server.send (200, "text/html", PAGE_Signin);
    }     
  }); 

  server.on("/pubgpio", []() { 
    if(isAuthentified()){
      server.send (200, "text/html", PAGE_PubGpio);  
    } else {
      server.send (200, "text/html", "{ \"success\":false, \"message\":\"you are not authorized!\" }");
    }     
  }); 

  server.on("/getpubgpiodata", HTTP_GET, []() { 
    if(isAuthentified()){
      server.send(200, "text/json", pubGpioFileData);
    } else {
      server.send (200, "text/json", "{ \"success\":false, \"message\":\"you are not authorized!\" }");
    }
  });

  server.on("/subgpio", []() { 
    if(isAuthentified()){
      server.send (200, "text/html", PAGE_SubGpio);  
    } else {
      server.send (200, "text/html", "{ \"success\":false, \"message\":\"you are not authorized!\" }");
    }     
  }); 

  server.on("/getsubgpiodata", HTTP_GET, []() { 
    if(isAuthentified()){
      server.send(200, "text/json", subGpioFileData);
    } else {
      server.send (200, "text/json", "{ \"success\":false, \"message\":\"you are not authorized!\" }");
    }
  });

  server.on("/getuserdata", HTTP_GET, []() { 
    if(isAuthentified()){
      server.send(200, "text/json", userFileData);
    } else {
      server.send (200, "text/json", "{ \"success\":false, \"message\":\"you are not authorized!\" }");
    }
  });

  server.on("/getinitdata", HTTP_GET, []() { 
    if(isAuthentified()){
      server.send(200, "text/json", initFileData);
    } else {
      server.send (200, "text/json", "{ \"success\":false, \"message\":\"you are not authorized!\" }");
    }
  });

  server.on("/getmqttdata", HTTP_GET, []() { 
    if(isAuthentified()){
      server.send(200, "text/json", mqttFileData);
    } else {
      server.send (200, "text/json", "{ \"success\":false, \"message\":\"you are not authorized!\" }");
    }
  });

  server.on("/config", HTTP_POST, [](){
    if(isAuthentified()){  
      
      if(SPIFFS.exists("/user.json")) {
        File uf = SPIFFS.open("/user.json", "w");
        if(uf) {
          uf.print("{");
          uf.println("\t\"username\": \"" + server.arg("username") + "\",");
          uf.println("\t\"password\": \"" + server.arg("password") + "\",");
          uf.println("\t\"cookieName\": \"" + server.arg("cookieName") + "\",");
          uf.println("\t\"cookieValue\": \"" + server.arg("cookieValue") + "\"");
          uf.println("}");
          uf.print("~");
        }
        uf.close();
      }

      if(SPIFFS.exists("/init.json")) {
        File inf = SPIFFS.open("/init.json", "w");
        if(inf) {
          inf.print("{");
          inf.println("\t\"ssidSTA\": \"" + server.arg("ssidSTA") + "\",");
          inf.println("\t\"passwordSTA\": \"" + server.arg("passwordSTA") + "\",");
          inf.println("\t\"ssidAP\": \"" + server.arg("ssidAP") + "\",");
          inf.println("\t\"passwordAP\": \"" + server.arg("passwordAP") + "\"");
          inf.println("}");
          inf.print("~");
        }
        inf.close();
      }

      if(SPIFFS.exists("/mqtt.json")) {
        File mf = SPIFFS.open("/mqtt.json", "w");
        if(mf) {
          mf.print("{");
          mf.println("\t\"mqttServer\": \"" + server.arg("mqttServer") + "\"");
          mf.println("}");
          mf.print("~");
        }
        mf.close();
      }
      
      server.send(200, "text/json", "{\"success\":true}");
      ESP.restart();
      
    } else {
      server.send (200, "text/json", "{ \"success\":false, \"message\":\"you are not authorized!\" }");
    }
  });

    server.on("/postsubgpio", HTTP_POST, [](){
    if(isAuthentified()){  
      
      if(SPIFFS.exists("/subgpio.json")) {
        File uf = SPIFFS.open("/subgpio.json", "w");
        if(uf) {
          uf.print("{");
          uf.println("\t\"gpio4\": \"" + server.arg("gpio4") + "\",");
          uf.println("\t\"gpio5\": \"" + server.arg("gpio5") + "\",");
          uf.println("\t\"gpio12\": \"" + server.arg("gpio12") + "\",");
          uf.println("\t\"gpio13\": \"" + server.arg("gpio13") + "\",");
          uf.println("\t\"gpio14\": \"" + server.arg("gpio14") + "\",");
          uf.println("\t\"gpio15\": \"" + server.arg("gpio15") + "\"");
          uf.println("}");
          uf.print("~");
        }
        uf.close();
      }
      
      server.send(200, "text/json", "{\"success\":true}");
      ESP.restart();
      
    } else {
      server.send (200, "text/json", "{ \"success\":false, \"message\":\"you are not authorized!\" }");
    }
  });

  server.on("/postpubgpio", HTTP_POST, [](){
    if(isAuthentified()){  
      
      if(SPIFFS.exists("/pubgpio.json")) {
        File uf = SPIFFS.open("/subgpio.json", "w");
        if(uf) {
          uf.print("{");
          uf.println("\t\"gpio2\": \"" + server.arg("gpio2") + "\"");
          uf.println("}");
          uf.print("~");
        }
        uf.close();
      }
      
      server.send(200, "text/json", "{\"success\":true}");
      ESP.restart();
      
    } else {
      server.send (200, "text/json", "{ \"success\":false, \"message\":\"you are not authorized!\" }");
    }
  });

  server.on("/test", []() { 
    if(isAuthentified()){
      server.send (200, "text/html", "You are authorized. Welcome!");  
    } else {
      server.send (200, "text/html", "You are not authorized.");
    }     
  });  

  server.on("/style.css", []() {
    server.send (200, "text/css", PAGE_Style);
  });

  server.onNotFound(handleNotFound);  

  server.begin();
  
  server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");  
  server.serveStatic("/js/jquery.min.js", SPIFFS, "/js/jquery.min.js");
  server.serveStatic("/js/jcookie.js", SPIFFS, "/js/jcookie.js");
  server.serveStatic("/js/signin.js", SPIFFS, "/js/signin.js");
  server.serveStatic("/js/main.js", SPIFFS, "/js/main.js");
  server.serveStatic("/js/pubgpio.js", SPIFFS, "/js/pubgpio.js");
  server.serveStatic("/js/subgpio.js", SPIFFS, "/js/subgpio.js");

  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent","Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize);
  
  Serial.println("HTTP server started");

  dht.begin();
  
  connectMqtt();
}

void loop() {
  server.handleClient();

  if (!client.connected() && i < 2) {
    reconnect();
  }
  
  client.loop();

  mqttPublish();
}
