#include <dht11.h>
#include <SPI.h>
#include <Ethernet.h>

dht11 DHT11;

int DHT11PIN = 2;

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
EthernetServer server(80);
unsigned int iPadPort = 9003;
IPAddress iPadIP(192,168,1,103);

void setup()
{
  Serial.begin(9600);
  Serial.println("DHT11 TEST PROGRAM ");
  Ethernet.begin(mac);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  pinMode(DHT11PIN, OUTPUT);
}

int Fahrenheit(double celsius)
{
	return 1.8 * celsius + 32;
}

void loop()
{
  EthernetClient client = server.available();
  if(client.connect(iPadIP, iPadPort)) {
    //client.println("Connected to Arduino");
    Serial.println("Connected to iPad");
    while(client.connected()){
        int chk = DHT11.read(DHT11PIN);
        
        Serial.print("Read sensor: ");
        switch (chk)
        {
          case 0: client.print("1"); client.print("|"); Serial.println("OK"); break;
          case -1: client.print("0"); client.print("|"); Serial.println("Checksum error"); break;
          case -2: client.print("0"); client.print("|"); Serial.println("Time out error"); break;
          default: client.print("0"); client.print("|"); Serial.println("Unknown error"); break;
        }
        
        client.print((int)DHT11.humidity);
        client.print("|");
        Serial.print("Humidity (%): ");
        Serial.println((float)DHT11.humidity);
        
        client.println(Fahrenheit(DHT11.temperature));
        Serial.print("Temperature (oF): ");
        Serial.println(Fahrenheit(DHT11.temperature));
        delay(2000);
    }
    delay(1);
    //close connection
    client.stop();
    Serial.println("client disonnected");
  }
  else
    Serial.println("No Connection");
}





