

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
    sendData("AT D100", 2000);
    sendData("AT D30", 2000);
    sendData("AT D10", 2000);
    sendData("AT D0", 2000);
}

String sendData(String command, const int timeout)
{
    String response = "";
    // command = command + "\r\n";

    Serial.println(command);
    mySerial2.println(command); // send the read character to the Serial

    long int time = millis();

    delay(timeout);
    while (mySerial2.available())
    {

        // The esp has data so display its output to the serial window
        char c = mySerial2.read(); // read the next character.
        response += c;
    }

    return response;
}