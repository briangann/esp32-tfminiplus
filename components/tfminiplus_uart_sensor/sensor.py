import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import ICON_MOTION_SENSOR, UNIT_CENTIMETER

DEPENDENCIES = ["uart"]

tfminiplus_uart_sensor_ns = cg.esphome_ns.namespace("tfminiplus_uart_sensor")
TFminiPlusUARTSensor = tfminiplus_uart_sensor_ns.class_(
    "TFminiPlusUARTSensor", cg.PollingComponent, uart.UARTDevice
)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        TFminiPlusUARTSensor,
        unit_of_measurement=UNIT_CENTIMETER,
        icon=ICON_MOTION_SENSOR,
        accuracy_decimals=1,
    )
    .extend(cv.polling_component_schema("5s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
