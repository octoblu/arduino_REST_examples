
/*
 
 Octoblu REST Interface for sending messages to Meshblu
 
 by Moheeb Zara
 
 Feb/2015
 
 */


#include <SPI.h>
#include <WiFi.h>

#define SITE_URL      "meshblu.octoblu.com"  
#define UUID          "7f776ba0-b64c-11e4-a109-158267812842"
#define TOKEN         "16a7bc03f95e2542ba0df697726cbdc8b20f6694"

char ssid[] = "yourNetwork"; //  your network SSID (name)
char pass[] = "secretPassword";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = SITE_URL;    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if ( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  

  
}

unsigned long time = 0;
int val;
int val2;
String payload;

void loop()
{
  
  //This sends an analog value as the payload
  
val = analogRead(0);
val2 = analogRead(1);

payload = "{\"x\": " + String(val) + ", \"y\": " + String(val2) + "}";

//UUID you want to send this message to. * to broadcast to all listening
String target = "*";


//*******************************************************
  
  unsigned long t1 = millis();

  Serial.print("try connect ");
  Serial.println(SITE_URL);
  
  while (!client.connect(server, 80))
  {
    Serial.println("retry connect");
    delay(100);
  }

  
    Serial.println("send POST request");
 



  // construct a HTTP PUT request
 String PostData = "{\"devices\": \"" + target + "\", \"payload\" : " + payload + "}";
 Serial.println(PostData);
  // if there's a successful connection:

    // send the HTTP PUT request:
  client.println("POST /messages HTTP/1.1");
  client.println("Host: meshblu.octoblu.com");
 // client.print(server);
  client.println("User-Agent: chrome");
  client.print("skynet_auth_uuid: ");
  client.println(UUID);
  client.print("skynet_auth_token: ");
  client.println(TOKEN);
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(PostData.length());
  client.println();
  client.println(PostData);
  client.println();

  // this is required delay, to allow server response
  delay(300);
  // disconnect from server
 // client.close();
  // add delay to prevent connect again too fast
  //delay(300);

  unsigned long t2 = millis();
  time = t2-t1;
  Serial.print("Total time spend (we will send this to server next time): ");
  Serial.println(time);
}
