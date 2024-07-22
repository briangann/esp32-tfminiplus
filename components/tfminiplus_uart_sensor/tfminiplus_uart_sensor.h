#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/hal.h"

#include "esphome/components/uart/uart.h"

namespace esphome
{
  namespace tfminiplus_uart_sensor
  {

    class TFminiPlusUARTSensor : public sensor::Sensor, public PollingComponent, public uart::UARTDevice
    {
    public:
      void set_distance_sensor(sensor::Sensor *value) { distance_sensor_ = value; }
      void set_temperature_sensor(sensor::Sensor *value) { internal_temperature_ = value; }
      void set_signal_quality_sensor(sensor::Sensor *value) { signal_quality_sensor_ = value; }

      void setup() override;
      void update() override;
      void loop() override;
      void dump_config() override;
    protected:
      bool update_{true};
      sensor::Sensor *distance_sensor_{nullptr};
      sensor::Sensor *signal_quality_sensor_{nullptr};
      sensor::Sensor *internal_temperature_{nullptr};
    };
  } // namespace tfminiplus_uart_sensor
} // namespace esphome
