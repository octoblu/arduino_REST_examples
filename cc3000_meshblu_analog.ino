/****************************************************************

Meshblu 




Shawn Hymel @ SparkFun Electronics
March 1, 2014
https://github.com/sparkfun/SFE_CC3000_Library

The security mode is defined by one of the following:
WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA, WLAN_SEC_WPA2

Hardware Connections:
 
 Uno Pin    CC3000 Board    Function
 
 +5V        VCC or +5V      5V
 GND        GND             GND
 2          INT             Interrupt
 7          EN              WiFi Enable
 10         CS              SPI Chip Select
 11         MOSI            SPI MOSI
 12         MISO            SPI MISO
 13         SCK             SPI Clock

Resources:
Include SPI.h, SFE_CC3000.h, and SFE_CC3000_Client.h

Development environment specifics:
Written in Arduino 1.0.5
Tested with Arduino UNO R3


Distributed as-is; no warranty is given.
****************************************************************/

#include <SPI.h>
#include <SFE_CC3000.h>
#include <SFE_CC3000_Client.h>

// Pins
#define CC3000_INT      2   // Needs to be an interrupt pin (D2/D3)
#define CC3000_EN       7   // Can be any digital pin
#define CC3000_CS       10  // Preferred is pin 10 on Uno

// Connection info data lengths
#define IP_ADDR_LEN     4   // Length of IP address in bytes

// Constants
#define SITE_URL      "meshblu.octoblu.com"  
#define UUID          "YOUR DEVICE UUID"
#define TOKEN         "YOUR DEVICE TOKEN"

// Add an extra delay (without extra delay messages send 1 per second.
#define addDelay 0

char ap_ssid[] = "WIFI SSID";                  // SSID of network
char ap_password[] = "PASSWORD";          // Password of network
unsigned int ap_security = WLAN_SEC_WPA2; // Security of network
unsigned int timeout = 30000;             // Milliseconds
char server[] = SITE_URL;        // Remote host site


// Global Variables
SFE_CC3000 wifi = SFE_CC3000(CC3000_INT, CC3000_EN, CC3000_CS);
SFE_CC3000_Client client = SFE_CC3000_Client(wifi);

void setup() {
  
  ConnectionInfo connection_info;
  int i;
  
  // Initialize Serial port
  Serial.begin(115200);
  
  // Initialize CC3000 (configure SPI communications)
  if ( wifi.init() ) {
    Serial.println("CC3000 initialization complete");
  } else {
    Serial.println("Something went wrong during CC3000 init!");
  }
  
  // Connect using DHCP
  Serial.print("Connecting to SSID: ");
  Serial.println(ap_ssid);
  if(!wifi.connect(ap_ssid, ap_security, ap_password, timeout)) {
    Serial.println("Error: Could not connect to AP");
  }
  
  // Gather connection details and print IP address
  if ( !wifi.getConnectionInfo(connection_info) ) {
    Serial.println("Error: Could not obtain connection details");
  } else {
    Serial.print("IP Address: ");
    for (i = 0; i < IP_ADDR_LEN; i++) {
      Serial.print(connection_info.ip_address[i]);
      if ( i < IP_ADDR_LEN - 1 ) {
        Serial.print(".");
      }
    }
    Serial.println();
  }
  
  
}


unsigned long time = 0;
int val;
int val2;
String payload;


void loop()
{
  
  
 //This sends an analog value as the payload.
  
val = analogRead(0);
val2 = analogRead(1);

// To send on value uncomment below and comment the other instance of payload.
//payload = String(val); 

// To send multiple values, we manually format the JSON string adding in our values.
payload = "{\"x\": " + String(val) + ", \"y\": " + String(val2) + "}";

//UUID you want to send this message to. * to broadcast to all listening
String target = "*";

// Code stuff that takes care of sending the message.
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
  delay(addDelay);
  // disconnect from server
 // client.close();
 

  unsigned long t2 = millis();
  time = t2-t1;
  Serial.print("Total time spend (we will send this to server next time): ");
  Serial.println(time);
}
