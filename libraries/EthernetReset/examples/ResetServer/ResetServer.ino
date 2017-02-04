// Reset Web Server
//
// A simple sketch that creates a web servers for remote resetting the
// Industruino D21G board thourgh a GET request to a specific URL:
//
//    http://{ip}[:{port}]/{reset_path}/reset
//
// {reset_path} is used instead of a password.  This is not a proper/secure
// authentication method, just a way to differentiate between different boards
// on the same local network if you need to.
//
// Originally created 14 Sep 2012 by Stelios Tsampas
// Adapted for Industruino by Claudio Indellicati

#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetReset.h>

// Networking parameters for the reset server
byte macAddr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress locIp(192, 168, 0, 200);
IPAddress dnsIp(192, 168, 0, 1);
IPAddress gwIp(192, 168, 0, 1);
IPAddress netMask(255, 255, 255, 0);
int port = 8080;
char *reset_path = "password";

// Create the reset server
EthernetReset reset(port, reset_path);


void setup()
{
  // Starting the reset server.
  // The 'begin()' method takes care of everything, from initializing the
  // Ethernet Module to starting the web server for resetting.
  // This is why you should always start it before any other server you might
  // need to run
  reset.begin(macAddr, locIp, dnsIp, gwIp, netMask);

  // Put the rest of your setup code here
}

void loop()
{
  // After the reset server is running the only thing needed is this command at
  // the beginning of the 'loop()' function in your sketch and it will take care
  // of checking for any reset request
  reset.check();

  // Put the rest of your code here
}
