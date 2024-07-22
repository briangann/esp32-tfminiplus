#include "esphome/core/log.h"
#include "tfminiplus_uart_sensor.h"
#include "TFMPlus.h"

namespace esphome
{
    namespace tfminiplus_uart_sensor
    {
        static const char *TAG = "tfminiplus_uart_sensor.sensor";
        TFMPlus tfmP;

        void TFminiPlusUARTSensor::setup()
        {
            ESP_LOGCONFIG(TAG, "Setting up TFmini Plus Sensor '%s'...", this->get_name().c_str());
            // ESP_LOGCONFIG(TAG, "DONE Setting up TFmini Plus Sensor '%s'...", this->get_name().c_str());
            /*
                        //printf("\r\nTFMPlus Library ESP32HOME\r\n"); // say 'hello'
                        ESP_LOGCONFIG(TAG, "===== TFMiniPlus ===== \r\n");
                        */
            Serial2.begin(115200); // Initialize TFMPLus device serial port.
            delay(20);             // Give port time to initalize
            tfmP.begin(&Serial2);  // Initialize device library object and...
                                   // pass device serial port to the object.

            // Send some example commands to the TFMini-Plus
            // - - Perform a system reset - - - - - - - - - - -
            ESP_LOGCONFIG(TAG, "Soft reset: ");
            if (tfmP.sendCommand(SOFT_RESET, 0))
            {
                ESP_LOGCONFIG(TAG, "passed.\r\n");
            }
            else
                tfmP.printReply();

            delay(500); // added to allow the System Rest enough time to complete

            // - - Display the firmware version - - - - - - - - -
            ESP_LOGCONFIG(TAG, "Firmware version: ");
            if (tfmP.sendCommand(GET_FIRMWARE_VERSION, 0))
            {
                ESP_LOGCONFIG(TAG, "%1u.", tfmP.version[0]); // print three single numbers
                ESP_LOGCONFIG(TAG, "%1u.", tfmP.version[1]); // each separated by a dot
                ESP_LOGCONFIG(TAG, "%1u\r\n", tfmP.version[2]);
            }
            else
                tfmP.printReply();
            // - - Set the data frame-rate to 20Hz - - - - - - - -
            ESP_LOGCONFIG(TAG, "Data-Frame rate: ");
            if (tfmP.sendCommand(SET_FRAME_RATE, FRAME_20))
            {
                ESP_LOGCONFIG(TAG, "%2uHz.\r\n", FRAME_20);
            }
            else
                tfmP.printReply();
            // - - - - - - - - - - - - - - - - - - - - - - - -

            /*  // - - - - - - - - - - - - - - - - - - - - - - - -
                // The next two commands may be used to switch the device
                // into I2C mode.  This sketch will no longer receive UART
                // (serial) data.  The 'TFMPI2C_example' sketch in the
                // TFMPI2C Library can be used to switch the device back
                // to UART mode.
                // Don't forget to switch the cables, too.
                // - - - - - - - - - - - - - - - - - - - - - - - -
                printf( "Set I2C Mode: ");
                if( tfmP.sendCommand( SET_I2C_MODE, 0))
                {
                    printf( "mode set.\r\n");
                }
                else tfmP.printReply();
                printf( "Save settings: ");
                if( tfmP.sendCommand( SAVE_SETTINGS, 0))
                {
                    printf( "saved.\r\n");
                }
                else tfmP.printReply();
                // - - - - - - - - - - - - - - - - - - - - - - - -
            */

            delay(500); // And wait for half a second.
        }
        int16_t tfDist = 0; // Distance to object in centimeters
        int16_t tfFlux = 0; // Strength or quality of return signal
        int16_t tfTemp = 0; // Internal temperature of Lidar sensor chip

        void TFminiPlusUARTSensor::update()
        {
            ESP_LOGI(TAG,"UPDATE Dist:%04icm ", tfDist); // display distance,
               // ESP_LOGCONFIG(TAG,"Flux:%05i ", tfFlux);   // display signal strength/quality,
               // ESP_LOGCONFIG(TAG,"Temp:%2i C", tfTemp);  // display temperature,
            this->publish_state(tfDist);
        }


        void TFminiPlusUARTSensor::loop()
        {
            if (!this->update_)
                return;
            //delay(50); // Loop delay to match the 20Hz data frame rate
            delay(20);
            if (!tfmP.getData(tfDist, tfFlux, tfTemp)) // Get data from the device.
            {
                ESP_LOGW(TAG, "No data from TFMini Plus");
                tfmP.printFrame(); // display the error and HEX dataa
                Serial.println("no data");
            }

            //Serial.println("test");
            //tfmP.getData(tfDist);
            //Serial.println(tfDist);
        }

        void TFminiPlusUARTSensor::dump_config()
        {
            ESP_LOGCONFIG(TAG, "TFmini Plus");
            LOG_SENSOR("", "", this);
            LOG_UPDATE_INTERVAL(this);
        }

    } // namespace tfminiplus_uart_sensor
} // namespace esphome
