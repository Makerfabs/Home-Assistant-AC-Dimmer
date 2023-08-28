#define SCR_Pin PA7
#define RELAY_PIN PC15
#define LED_PIN PA6
#define ZCD_PIN PA0

#define DIM_MAX 10
#define DIM_MIN 90

#define AC_CTRL_OFF digitalWrite(SCR_Pin, LOW)
#define AC_CTRL_ON digitalWrite(SCR_Pin, HIGH)

#define RELAY_OFF digitalWrite(RELAY_PIN, LOW)
#define RELAY_ON digitalWrite(RELAY_PIN, HIGH)

#define LED_OFF digitalWrite(LED_PIN, HIGH)
#define LED_ON digitalWrite(LED_PIN, LOW)

unsigned char dim = 0;

HardwareSerial mySerial(PB7, PB6);
int led_flag = 0;

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(SCR_Pin, OUTPUT);
    pinMode(ZCD_PIN, INPUT);
    LED_OFF;
    RELAY_OFF;
    AC_CTRL_OFF;

    mySerial.begin(115200);
    delay(1000);
    mySerial.println("Start");

    attachInterrupt(ZCD_PIN, zero_cross_int, RISING); // CHANGE FALLING RISING
}

void loop()
{
    if (led_flag % 2 == 0)
        LED_ON;
    else
        LED_OFF;

    led_flag++;

    char c;
    String str = "";
    while (mySerial.available() > 0)
    {
        c = mySerial.read();
        if (c == '\n')
        {
            mySerial.println("Get line:");
            mySerial.println(str);
            at_explain(str);
        }
        else
            str = str + c;
    }

    delay(1000);
}

// AT D50
void at_explain(String command)
{
    if (command.startsWith("AT"))
    {
        int var_1 = 0;

        sscanf(command.c_str(), "AT D%d", &var_1);
        mySerial.println(var_1);

        set_power(var_1);
    }
}

void set_power(int level)
{
    dim = map(level, 0, 100, DIM_MIN, DIM_MAX);
    if (level == 0)
    {
        RELAY_OFF;
    }
    else
        RELAY_ON;
}

void zero_cross_int() // function to be fired at the zero crossing to dim the light
{

    if (dim <= DIM_MAX)
        return;
    // if (dim >= DIM_MIN)
    //     return;

    AC_CTRL_OFF;

    int dimtime = (100 * dim);
    delayMicroseconds(dimtime); // Off cycle
    AC_CTRL_ON;                 // triac firing
    // delayMicroseconds(500);     // triac On propagation delay
    // triac Off
}
