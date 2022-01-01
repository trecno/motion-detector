#include <SPI.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h" 
// Please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID; // Your network SSID (name)
char pass[] = SECRET_PASS; // Your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
// If you don't want to use DNS (and reduce your sketch size),
// use the numeric IP instead of the name for the server:
// IPAddress server(xx,xxx,xxx,xxx); // Numeric IP (no DNS)
char server[] = "api.telegram.org";  // Name address (using DNS)

// PIR variables
int pirPin = 2;  // PIR Out pin 
int pirStat = 0; // PIR status

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiSSLClient client;

// This method makes a HTTP connection to the server:
void httpRequest() {
  // Close any connection before send a new request.
  // This will free the socket on the NINA module
  client.stop();

  // If there's a successful connection:
  if (client.connect(server, 443)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /botTOKEN/sendMessage?chat_id=CHATID&text=¡INTRUSO! HTTP/1.1");
    client.println("Host: api.telegram.org");
    client.println("Connection: close");
    client.println();
  } else {
    // If you couldn't make a connection:
    Serial.println("Connection failed");
  }
}


void printWiFiStatus() {
  // Print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup() {
  // Initialize pirPin
  pinMode(pirPin, INPUT);
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  // Check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // Attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");
  printWiFiStatus();
}

void loop() {
  pirStat = digitalRead(pirPin); 
  
  // If motion detected
  if (pirStat == HIGH) {
    Serial.println("Hey I got you!!!");
    httpRequest();
  } else {
    Serial.println("Nothing");
  }
}
