
//This is the original code that we created before attempting to get both sensors displaying in PubNub console. 
//This currently only uses the pressure sensor data as this is the mnost important data for the project
//We only need to know whether someone is sat on the seat but the heat sensor was a further development 


#include <SPI.h> 
//Allows the ethernet cable to connect the arduino to the internet in order to connect to PubNub
#include <Ethernet.h>
//Includes the PubNub library so that data can be displayed in PubNub library
#include <PubNub.h>


// address
byte mac[] = { 0x00, 0x0E, 0xEF, 0x00, 0x00, 0x64 };
//connects the ardunio to the port where the pressure sensor is connected (port 2)
const int pubLedPin = 2;

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
{    //
    pinMode(pubLedPin, OUTPUT);
    digitalWrite(pubLedPin, LOW);
    Serial.begin(9600);
    Serial.println("Serial set up");
    while (!Ethernet.begin(mac)) {
        Serial.println("Ethernet setup error");
        delay(1000);
    }
    //Tells us that the ethernet is working correctly and the data can be published 
    Serial.println("Ethernet set up");
    //This takes the subscribe and publish keys so that PubNub can be accessed and the data can be sent
    PubNub.begin(pubkey, subkey);
    //Prints in console to let us know everything is set up correctly
    Serial.println("PubNub set up");
}

//This was an addition before connecting to during early prototype phases where light would flash when the temperature reaches a lower and higher point
void flash(int ledPin)
{
    /* Flash LED three times. */
    for (int i = 0; i < 3; i++) {
        digitalWrite(ledPin, HIGH);
        delay(100);
        digitalWrite(ledPin, LOW);
        delay(100);
    }
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
    
    strcat(msg, "}}");
    //Tells the console to print the publishing message 
    Serial.print("publishing message: ");
    Serial.println(msg);
    //Prints error if there is one so that viewer is informed that something isn't working correctly
    client = PubNub.publish(channel, msg);
    //
    if (!client) {
        Serial.println("publishing error");
    } else {
        flash(pubLedPin);
        client->stop();
    }
    //Delay for one second so that data isn't posted too often as this can effect the ability to post to PubNub
    delay(1000);
}
