
#include "particle-osc.h"
#include "OSCBundle.h"
#include "OSCMessage.h"

IPAddress computerIPAddress = IPAddress(192,168,0,193); // put the IP address of your computer here.
IPAddress particleIPAddress;
OSCMessage particleIPMessage("/coreip");

#define LOCALPORT  8000 // the port your Particle device can receive messages on.
#define REMOTEPORT 1234 // the port on your computer where the OSC messages will be sent.

int led = D7;
int num =0;

UDP UdpConnection;

void setup() {
  UdpConnection.begin(LOCALPORT);

  particleIPAddress = WiFi.localIP();
  particleIPMessage.add(particleIPAddress[0]).add(particleIPAddress[1]).add(particleIPAddress[2]).add(particleIPAddress[3]);

  UdpConnection.beginPacket(computerIPAddress, REMOTEPORT);
  particleIPMessage.send(UdpConnection);
  UdpConnection.endPacket();

    pinMode(led, OUTPUT);
}

void loop() {

  OSCMessage testMsg_Received;

		int bytesToRead = UdpConnection.parsePacket();	// how many bytes are available via UDP
		if (bytesToRead > 0) {
			while(bytesToRead--) {
				testMsg_Received.fill(UdpConnection.read());	// filling the OSCMessage with the incoming data
			}
			if(!testMsg_Received.hasError()) { // if the address corresponds to a command, we dispatch it to the corresponding function

       // if you recievie...
       if(testMsg_Received.fullMatch("/test",0)){

        // send confirmation...
        OSCMessage testMsg_toSend("/testmessage");
        testMsg_toSend.add((float)2.6).add(150).add("hahaha").add(-1000);

        UdpConnection.beginPacket(computerIPAddress, REMOTEPORT);
        testMsg_toSend.send(UdpConnection); // send the bytes
        UdpConnection.endPacket();
        testMsg_toSend.empty(); // empty the message to free room for a new one

        digitalWrite(led, HIGH);
      }
      if(!testMsg_Received.fullMatch("/test",0)){

                digitalWrite(led, LOW);
      }
			}

		}
}
