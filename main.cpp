#include "mbed.h"

//poorten LED


//poorten sensoren
DigitalIn sensorLinks(D1);
DigitalIn SensorMid(D2);
DigitalIn SensorRechts(D3);
DigitalIn SensorOnder(D4);

// poorten motors
PwmOut MotorLinksForward(D5);
PwmOut MotorLinksBackward(D6);
PwmOut MotorRechtsForward(D10);
PwmOut MotorRechtsBackward(D11);

// cooldown timer
#define Cooldown     500ms


int main()
{
    float Object;
    float ObjectL;
    float objectR;

    float ondergrond;

    float power = 0.1;
    float powerback = 0.1;

    float RemAfstand;
    float GeenVloer;
    
    while (true) {
        
        MotorLinksForward.write(power);
        MotorRechtsForward.write(power);

        //controle op ondergrond
        if (ondergrond >= GeenVloer ) {
            //stop
            MotorLinksForward.write(0);
            MotorRechtsForward.write(0);

            ThisThread::sleep_for(Cooldown);
            //backwards
            MotorLinksBackward.write(powerback);
            MotorRechtsForward.write(powerback);

            ThisThread::sleep_for(Cooldown);
            //double check
            if (ondergrond >=GeenVloer){
                //stop programa
            }
            
        }
        //check for object
        else if () {
        //stop


        //turn left

        //keep checking
        if () {
            //return to going straight
        }

        }
    }
}
