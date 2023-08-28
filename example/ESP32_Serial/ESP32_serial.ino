

#include <Arduino.h>

HardwareSerial mySerial2(2);

#define AT_RX 16
#define AT_TX 17

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    mySerial2.begin(115200, SERIAL_8N1, AT_RX, AT_TX);
}
void loop()
{
    // put your main code here, to run repeatedly:
    while (Serial.available() > 0)
    {
        mySerial2.write(Serial.read());
        yield();
    }
    while (mySerial2.available() > 0)
    {
        Serial.write(mySerial2.read());
        yield();
    }
}
