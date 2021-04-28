#include <PZEM004Tv30.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char* ssid = "lolipop";
const char* password = "lolipoplolipop";

HTTPClient httpsClient;

// initiate pzem object with params: pins (RX, TX)
// wemos d1 mini pins [TX: GPIO1, RX: GPIO3
// we using pins D5 and D6

PZEM004Tv30 pzem(D5, D6); 

float Power, Energy, Voltase, Current, Frequency, PF;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

}

void loop() {
  
  Power = pzem.power();
  
  if(isnan(Power)) 
  {
    Serial.println("cannot read power");
  }
  else 
  {
    Serial.print("Power : ");
    Serial.print(Power);
    Serial.println(" KW");
  }

  Energy = pzem.energy();

  if(isnan(Energy)) 
  {
    Serial.println("cannot read energy");
  }
  else 
  {
    Serial.print("Energy : ");
    Serial.print(Energy);
    Serial.println(" KWh");
  }

  Voltase = pzem.voltage();

  if(isnan(Voltase)) 
  {
    Serial.println("cannot read voltage");
  }
  else 
  {
    Serial.print("voltage : ");
    Serial.print(Voltase);
    Serial.println(" V");
  }
  
  Current = pzem.current();

  if(isnan(Current)) 
  {
    Serial.println("cannot read current");
  }
  else 
  {
    Serial.print("current: ");
    Serial.print(Current);
    Serial.println(" A");
  }

  Frequency = pzem.frequency();

  if(isnan(Frequency)) 
  {
    Serial.println("cannot read frequency");
  }
  else 
  {
    Serial.print("frequency: ");
    Serial.print(Frequency);
    Serial.println(" Hz");
  }

  PF = pzem.pf();

  if(isnan(PF)) 
  {
    Serial.println("cannot read power factor");
  }
  else 
  {
    Serial.print("power factor: ");
    Serial.print(PF);
    Serial.println("");
  }
  
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status

    HTTPClient http; //Declare object of class HTTPClient
 
    http.begin("http://guarded-shore-21924.herokuapp.com/energies"); //Specify request destination
    http.addHeader("Content-Type: ", "application/json"); //Specify content-type header

    const int energies = JSON_OBJECT_SIZE(6); // declare json variable size
    StaticJsonDocument<energies> doc; // asign it to json document for mutable 

    doc["energy"] = Energy;
    doc["power"] = Power;
    doc["current"] = Current;
    doc["voltage"] = Voltase;
    doc["frequency"] = Frequency;
    doc["power_factor"] = PF;

    char json[128]; 
    
    serializeJson(doc, json); // asign json to char variable
    
    int httpCode = http.POST(json); //Send the request
    String payload = http.getString(); //Get the response payload

    Serial.println(httpCode); //Print HTTP return code
    Serial.println(payload); //Print request response payload
 
    http.end(); //Close connection
 
  } else {
    Serial.println("Error in WiFi connection");
  }
  
  Serial.println();
  delay(5000);
}
