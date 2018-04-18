#include <SPI.h>
//Allows the ethernet cable to connect the arduino to the internet in order to connect to PubNub
#include <Ethernet.h>
//Includes the PubNub library so that data can be displayed in PubNub library
#include <PubNub.h>

// address
byte mac[] = { 0x00, 0x0E, 0xEF, 0x00, 0x00, 0x64 };

//This is so that the arduino knows when the temperature has reached a low enough temperature to change from taken to available
const int hot = 28; //high is 28 degrees C
const int cold = 25;// Low is 25 degrees C

//This is the publish key to access the PubNub account and project
//This is necessary to display the data taken from the sensors and publish it on PubNub
char pubkey[] = "pub-c-0379fb69-98c0-489c-833a-cdb4373db148";

//This is the subscribe key to access the PubNub account and project
//This is necessary to display the data taken from the sensors and publish it on PubNub
char subkey[] = "sub-c-39d5d1fa-2de9-11e8-85fd-a682db239c54";

//This is the channel name so that the data taken from the sensors is published in the right place
char channel[] = "Channel-19kfm0u0";


//The main set up for the sensors 
void setup()
{
  //Shows the pin that the heat sensor is located in so that the data can be shown accurately 
	pinMode(A0, INPUT); //stored in port A0

	Serial.begin(9600); //Start up serial printer
	Serial.println("Serial set up");//Prints confirmation in console

	while (!Ethernet.begin(mac)) { //Connects to internet 
		Serial.println("Ethernet setup error");//tells user if there is an error
		delay(1000);//delay to stop pubnub issues
	}
 //Tells us that the ethernet is working correctly and the data can be published 
	Serial.println("Ethernet set up");
//This takes the subscribe and publish keys so that PubNub can be accessed and the data can be sent
	PubNub.begin(pubkey, subkey);
 //Prints in console to let us know everything is set up correctly
	Serial.println("PubNub set up");
}
//This is where the data gets taken from the arduino and posted to PubNub
void loop()
{
	Ethernet.maintain();

	EthernetClient *client;
//The pressure sensor takes the measurement and it is posted to PubNub
	char msg[64] = "{\"eon\":{\"sensor\":";
	//This is the assigned oprt for the pressure sensor (assigned to port 0)
		sprintf(msg + strlen(msg), "%d", analogRead(0));
	//Tells the console to print the publishing message
	strcat(msg, "}}");
//Prints error if there is one so that viewer is informed that something isn't working correctly
	Serial.print("publishing message: ");
	Serial.println(msg);
	client = PubNub.publish(channel, msg);
	if (!client) {
		Serial.println("publishing error");
	} 
  //Delay for one second so that data isn't posted too often as this can effect the ability to post to PubNub
	delay(1000);
}

void temp()
{
  Ethernet.maintain();

  EthernetClient *client;
  //The temperature sensor takes the measurement and it is posted to PubNub
  char msg[64] = "{\"eon\":{\"sensor\":";
  
    sprintf(msg + strlen(msg), "%d", analogRead(0));
  
  strcat(msg, "}}");
//Prints error if there is one so that viewer is informed that something isn't working correctly
  Serial.print("publishing message: ");
  Serial.println(msg);
  //prints if there is an error
  client = PubNub.publish(channel, msg);
  if (!client) {
    Serial.println("publishing error");
  } 

  delay(1000);
}

