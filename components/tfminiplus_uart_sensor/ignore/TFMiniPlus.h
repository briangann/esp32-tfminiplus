#include "esphome.h"
#include "TFMPlus.h"
// ...

// Initialize variables
int16_t tfDist = 0;    // Distance to object in centimeters
int16_t tfFlux = 0;    // Strength or quality of return signal
int16_t tfTemp = 0;    // Internal temperature of Lidar sensor chip

// Define some variables we need later
float distance;

class MyDistanceSensor : public PollingComponent, public Sensor {
 public:
    
    //TFmini tfmini;
    TFMPlus tfmP;
    
    MyDistanceSensor() : PollingComponent(1000) { }

    void setup() override {
    
        Serial2.begin( 115200);  // Initialize TFMPLus device serial port.
        delay(20);               // Give port time to initalize
        tfmP.begin( &Serial2);   // Initialize device library object and...
    
        delay(1000);  // added to allow the System Rest enough time to complete
        
        // Send some example commands to the TFMini-Plus
        // - - Perform a system reset - - - - - - - - - - -
        Serial.printf( "===== TFMiniPlus ===== \r\n");
        Serial.printf( "Soft reset: ");
        if( tfmP.sendCommand( SOFT_RESET, 0))
        {
            Serial.printf( "passed.\r\n");
        }
        else tfmP.printReply();
      
        delay(500);  // added to allow the System Rest enough time to complete
    
        // - - Display the firmware version - - - - - - - - -
        Serial.printf( "Firmware version: ");
        if( tfmP.sendCommand( GET_FIRMWARE_VERSION, 0))
        {
            Serial.printf( "%1u.", tfmP.version[ 0]); // print three single numbers
            Serial.printf( "%1u.", tfmP.version[ 1]); // each separated by a dot
            Serial.printf( "%1u\r\n", tfmP.version[ 2]);
        }
        else tfmP.printReply();
        // - - Set the data frame-rate to 20Hz - - - - - - - -
        Serial.printf( "Data-Frame rate: ");
        if( tfmP.sendCommand( SET_FRAME_RATE, FRAME_20))
        {
            Serial.printf( "%2uHz.\r\n", FRAME_20);
        }
        else tfmP.printReply();
        // - - - - - - - - - - - - - - - - - - - - - - - -
    
        delay(500);            // And wait for half a second.

    }

    void update() override {
        
        if( tfmP.getData( tfDist, tfFlux, tfTemp)) // Get data from the device.
        {
            Serial.println("==== Medindo Distância ====");   // display distance in log
            Serial.print( "Dist: ");   // display distance in log
            Serial.print(tfDist);   // display distance in log
            Serial.println(" cm");   // display distance in log
            Serial.println("============ // ===========");
        }
        else                  // If the command fails...
        {
            tfmP.printFrame();  // display the error and HEX dataa
        }
    
        distance = tfDist;

        if (isnan(distance)) {
            distance = 0;
        }
  
    publish_state(distance);
    }
};

// ...
