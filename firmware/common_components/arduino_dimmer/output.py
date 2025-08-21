import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_ID

arduino_dimmer_ns = cg.esphome_ns.namespace('arduino_dimmer')
ArduinoDimmer = arduino_dimmer_ns.class_('ArduinoDimmer', cg.Component, output.FloatOutput)

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
       cv.Required(CONF_ID): cv.declare_id(ArduinoDimmer),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await output.register_output(var, config)
