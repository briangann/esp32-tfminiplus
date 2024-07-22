//#include <LibPrintf.h>

// Arduino Nano ESP32 https://docs.arduino.cc/resources/datasheets/ABX00083-datasheet.pdf

/* File Name: TFMP_example.ino
 * Developer: Bud Ryerson
 * Inception: 29JAN2019
 * Last work: 19MAY2021

 * Description: Arduino sketch to test the Benewake TFMini Plus
 * time-of-flight Lidar ranging sensor using the TFMPlus library.

 * Default settings for the TFMini Plus are a 115200 serial baud rate
 * and a 100Hz measurement frame rate. The device will begin returning
 * measurement data right away:
 *   Distance in centimeters,
 *   Signal strength in arbitrary units,
 *   and an encoded number for Temperature in degrees centigrade.

 * Use the 'sendCommand()' to send commands and return a status code.
 * Commands are selected from the library's list of defined commands.
 * Parameters can be entered directly (115200, 250, etc) but for
 * safety, they should be chosen from the library's defined lists.
 */

#include <TFMPlus.h>  // Include TFMini Plus Library v1.5.0
TFMPlus tfmP;         // Create a TFMini Plus object

                                    
void setup()
{
    Serial.begin( 9600);   // Intialize terminal serial port
    delay(20);               // Give port time to initalize
    //printf_begin();          // Initialize printf.
    printf("\r\nTFMPlus Library Example - 19MAY2021\r\n");  // say 'hello'

    Serial2.begin( 115200);  // Initialize TFMPLus device serial port.
    delay(20);               // Give port time to initalize
    tfmP.begin( &Serial2);   // Initialize device library object and...
                             // pass device serial port to the object.

    // Send some example commands to the TFMini-Plus
    // - - Perform a system reset - - - - - - - - - - -
    printf( "System reset: ");
    if( tfmP.sendCommand( SOFT_RESET, 0))
    {
        printf( "passed.\r\n");
    }
    else tfmP.printReply();
  
    delay(500);  // added to allow the System Rest enough time to complete

  // - - Display the firmware version - - - - - - - - -
    printf( "Firmware version: ");
    if( tfmP.sendCommand( GET_FIRMWARE_VERSION, 0))
    {
        printf( "%1u.", tfmP.version[ 0]); // print three single numbers
        printf( "%1u.", tfmP.version[ 1]); // each separated by a dot
        printf( "%1u\r\n", tfmP.version[ 2]);
    }
    else tfmP.printReply();
    // - - Set the data frame-rate to 20Hz - - - - - - - -
    printf( "Data-Frame rate: ");
    if( tfmP.sendCommand( SET_FRAME_RATE, FRAME_20))
    {
        printf( "%2uHz.\r\n", FRAME_20);
    }
    else tfmP.printReply();
    // - - - - - - - - - - - - - - - - - - - - - - - -

    delay(500);            // And wait for half a second.
}

// Initialize variables
int16_t tfDist = 0;    // Distance to object in centimeters
int16_t tfFlux = 0;    // Strength or quality of return signal
int16_t tfTemp = 0;    // Internal temperature of Lidar sensor chip

// Use the 'getData' function to pass back device data.
void loop()
{
    delay(50);   // Loop delay to match the 20Hz data frame rate

    if( tfmP.getData( tfDist, tfFlux, tfTemp)) // Get data from the device.
    {
      printf( "Dist:%04icm ", tfDist);   // display distance,
      printf( "Flux:%05i ",   tfFlux);   // display signal strength/quality,
      printf( "Temp:%2i\°C",  tfTemp);   // display temperature,
      printf( "\r\n");                   // end-of-line.
    }
    else                  // If the command fails...
    {
      printf( "No data from TFMini Plus");
      tfmP.printFrame();  // display the error and HEX dataa
      Serial.println("no data");
    }

    Serial.println("test");
    tfmP.getData( tfDist );
    Serial.println( tfDist );
}
