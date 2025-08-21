#pragma once

#include "esphome/core/component.h"
#include "esphome/components/output/float_output.h"
#include "esphome/core/log.h"


namespace esphome {
namespace arduino_dimmer {


class ArduinoDimmer : public esphome::Component, public esphome::output::FloatOutput
{
public:
    void setup() override
    {
        Serial2.begin(115200, SERIAL_8N1, 16, 17);
    }

    void write_state(float state) override
    {
        int percent = (int)(100 * state);
        String command = "AT D";
        command = command + percent;

        Serial2.println(command);
    }
};

}
}
