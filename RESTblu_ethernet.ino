/*

Octoblu REST interface to Send messages to Meshblu

by Moheeb Zara @ Octoblu
Feb/2015

*/


#include <SPI.h>
#include <Ethernet.h>

#define SITE_URL      "meshblu.octoblu.com"  
#define UUID          "uuid"
#define TOKEN         "token"

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = SITE_URL;

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,0,177);

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
/*
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
  
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  } */
}

unsigned long time = 0;
int val;
String payload;

void loop()
{
  
  //This sends an analog value as the payload
  
val = analogRead(0);
payload = String(val);

//UUID you want to send this message to. * to broadcast to all listening
String target = "*";

  
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
  delay(1000);
  // disconnect from server
  client.stop();
  // add delay to prevent connect again too fast
  delay(300);

  unsigned long t2 = millis();
  time = t2-t1;
  Serial.print("Total time spend (we will send this to server next time): ");
  Serial.println(time);
}
