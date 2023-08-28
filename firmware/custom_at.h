// Vincent create
#include "esphome.h"

class MyCustom_AT : public Component, public FloatOutput
{
public:
    void setup() override
    {
        Serial2.begin(115200, SERIAL_8N1, 16, 17);
    }

    void write_state(float state) override
    {
        this->percent = 100 * state;
        Serial.print("percent:");
        Serial.println(this->percent);

        String command = "AT D";
        command = command + percent;

        Serial2.println(command);
    }

    int percent = 0;
};
