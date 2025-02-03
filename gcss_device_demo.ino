// name: gcss_device_demo.ino
// desc: this is a demonstration of communication between device and gcss
// creator: @kpers
// date: 31.01.2025

// initializes WiFi libraries
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

WiFiUDP udp;

char packet_buffer[255];

// client info
unsigned int local_port = 9999;

// server info
char *server_ip = "192.168.1.104";
unsigned int server_port = 8888;

// WiFi info
const char *ssid = "";
const char *password = "";

// board info
const char *board_info = "";

// cache
bool paired = false;

// send_server function
// sends signal to the server
// returns void
void send_server(const char *data_to_send) {
  udp.beginPacket(server_ip, server_port); // establishes a connection with a remote device to send data via UDP

  char buffer[30];

  sprintf(buffer, data_to_send); // debugs data

  udp.printf(buffer); // sends data to the server

  udp.endPacket(); // finish sending data to the server
}

// get_from_server function
// receives signal from server
// returns data
char *get_from_server() {
  int packet_size = udp.parsePacket(); // checks for the presence of a received UDP packet and returns its size

  if (packet_size) {
    Serial.print("[DEBUG]: Received packet from: "); Serial.println(udp.remoteIP()); // debugs info about sender

    int len = udp.read(packet_buffer, 255); // reads UDP data into the specified buffer

    Serial.printf("Data: %s\n", packet_buffer); //
                                                // debugs data
    Serial.println();                           //

    return packet_buffer; // returns received data
  }
}

void setup() {
  Serial.begin(115200); // initializes Serial
  
  WiFi.begin(ssid, password); // initializes Wifi
  
  while (WiFi.status() != WL_CONNECTED) { //
    delay(500); Serial.print(F("."));     // loops until board connects to the Wifi
  }                                       //

  udp.begin(local_port); // initializes port update
  
  Serial.printf("[DEBUG]: Connected to the WiFi with local port: %s:%i \n", WiFi.localIP().toString().c_str(), local_port); // debugs successful connection

  while (paired == false) {                             //
    send_server(board_info);                            //
                                                        //
    Serial.print("[DEBUG]: Waiting for a response..."); //
    Serial.println();                                   //
                                                        //
    delay(2000);                                        //
                                                        // loops until board pairs with gcss
    if (String(get_from_server()) == "ps") {            //
      Serial.print("[DEBUG]: Pairing successful");      //
      Serial.println();                                 //
                                                        //
      break;                                            //
    }                                                   //
  }                                                     //
}

void loop() {
  // your code here
}
