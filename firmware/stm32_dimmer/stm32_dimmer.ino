
#include <Arduino.h>
#include <HardwareTimer.h>

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

volatile bool count_hz_flag = false;
volatile uint32_t count_hz = 0;
volatile unsigned long zc_samples[10];
volatile uint8_t zc_index = 0;
uint8_t dim_delay = 0;

uint32_t local_hz = 0;

HardwareTimer timer(TIM14);
HardwareSerial mySerial(PB7, PB6);
int led_flag = 0;



void onTimer() {
    if(zc_index < 10 && count_hz > 0) {
        zc_samples[zc_index++] = count_hz / 2;
        count_hz = 0;
    }
}


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

    timer.setOverflow(1000000, MICROSEC_FORMAT);
    timer.attachInterrupt(onTimer);
    timer.setInterruptPriority(1, 0);
    timer.resume();

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


    if(zc_index == 10) {
        timer.pause();

        for (uint8_t i = 1; i < zc_index; i++)
        {
            unsigned long key = zc_samples[i];
            int j = i - 1;
            while (j >= 0 && zc_samples[j] > key)
            {
                zc_samples[j + 1] = zc_samples[j];
                j--;
            }
            zc_samples[j + 1] = key;
        }

        uint32_t sum = 0;
        for(uint8_t i = 1; i < zc_index - 1; i++) {
            sum += zc_samples[i];
        }
        local_hz = sum / (zc_index - 2);

        if(local_hz > 47 && local_hz < 53) {
            dim_delay = 100;
        } else if(local_hz >= 57 && local_hz < 63) {
            dim_delay = 80;
        }
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

    if(zc_index < 10) {
        count_hz++;
    }

    if (dim <= DIM_MAX)
        return;
    // if (dim >= DIM_MIN)
    //     return;

    AC_CTRL_OFF;

    int dimtime = (dim_delay * dim);
    delayMicroseconds(dimtime); // Off cycle
    AC_CTRL_ON;                 // triac firing
    // delayMicroseconds(500);     // triac On propagation delay
    // triac Off
}