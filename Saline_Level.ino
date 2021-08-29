#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "DHT.h"   //library for dht 11
#define DHTPIN D2
#define DHTTYPE DHT11   // DHT 11


DHT dht(DHTPIN, DHTTYPE); 
const char* ssid = "Rutuja"; // SSID i.e. Service Set Identifier is the name of your WIFI
const char* password = "deshmukh"; // Your Wifi password, in case you have open network comment the whole statement.

WiFiServer server(80); // Creates a server that listens for incoming connections on the specified port, here in this case port is 80.





int hbpin=A0;
int hb=0;
int trigPin=D6;
int echoPin=D5;
int dist=0;
int duration=0;
int distance=0;
 
void setup() {
  Serial.begin(115200);


  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(A0,INPUT);
  dht.begin();
 

  // Connect to WiFi network
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(".");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP()); //Gets the WiFi shield's IP address and Print the IP address of serial monitor
  Serial.println("/");
 
}
 

void loop() {
   WiFiClient client = server.available();
   float temp= dht.readTemperature();
  hb=analogRead(hbpin);  //pulse sensor
  int heartbeat=PulseSensor(hb);
  dist=calculateDistance();
  
  Serial.print("Temperature= ");  //temperature sensor
  Serial.print(temp);
  Serial.print(" Analog=");
  Serial.print(hb);
  Serial.print(" HB-");
  Serial.print(heartbeat);
  Serial.print(" SalineLevel-");
  Serial.println(dist);

    

 //client.println("</html>");

              HTTPClient http;
                    String url = "http://192.168.43.50:8084/SalineLevel/UpdateBin?field2="+String(temp)+"&field3="+String(heartbeat)+"&field1="+String(dist); //access the php file to write data
                    Serial.println(url);     
                    http.begin(url);
              
                    //using GET method to write to sql
                    int httpCode = http.GET();
                    if(httpCode > 0)
                    {
                        Serial.printf("[HTTP] GET...code: %d\n", httpCode);
                          if(httpCode == HTTP_CODE_OK)
                          {
                            String payload = http.getString();
                            Serial.println(payload);
                          }
                    }
                       else
                       {
                          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
                       }
                        http.end();
                     delay(3000);

 
}


int PulseSensor(int j) //Function for Pulse Senosr

{

int i;  
if((j>=0 & j<=599) or j==0  )
{
i=0;
  }


else if(j>=560 & j<=1200)
{
i=random(60,90);  
  }
return i;
  
}

int calculateDistance(){  //function to calculate distance 
  
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance= duration*0.034/2;
  return distance;
}



 
