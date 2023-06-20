#include "mbed.h"
#include <cstdio>
#include <ctime>
#include <utility>

// variablen
#define power 0.1
#define powerback 0.1
#define objectFound 30

// Timers
Timer timer1;
Timer timer2;

// poorten LED

// poorten sensoren
// sensor links
DigitalOut sensorLinksTrigger(D1);
DigitalIn sensorlinksEcho(D2);

// sensor mid
DigitalOut SensorMidTrigger(D3);
DigitalIn SensorMidEcho(D4);

// sensor rechts
DigitalOut SensorRechtsTrigger(D7);
DigitalIn SensorRechtsEcho(D8);

// sensor onder
DigitalIn SensorOnder(D9);

// poorten motors
PwmOut MotorLinksForward(D5);
PwmOut MotorLinksBackward(D6);
PwmOut MotorRechtsForward(D10);
PwmOut MotorRechtsBackward(D11);

// cases
enum robert { Forward, Object, Floor, turnaround };

void Print_speed() {
  double speed = power;
  printf("speed:%f\n", speed);
}

bool objectfound(){
    bool object = false;

    timer1.reset();
  ThisThread::sleep_for(500ms);
  int timeout = 26100;
  // Clears the trigPin
  SensorMidTrigger = 0;
  wait_us(3);
  // Sets the trigPin on HIGH state for 10 micro seconds
  SensorMidTrigger = 1;
  wait_us(10);
  SensorMidTrigger = 0;
  // Reads the echoPin, returns the sound wave travel time in microseconds

  int width = 0;
  while (SensorMidEcho.read() == 0);
  timer1.start();
  while (SensorMidEcho.read() == 1);
  timer1.stop();

    int time = timer1.read_us();
    int distance = time / 58;
    if (distance >= objectFound){
        object = true;
    }
    return Object;
}

bool Objectlinks(){
    bool Objectlinks;

        timer1.reset();
  ThisThread::sleep_for(500ms);
  int timeout = 26100;
  // Clears the trigPin
  sensorLinksTrigger = 0;
  wait_us(3);
  // Sets the trigPin on HIGH state for 10 micro seconds
  sensorLinksTrigger = 1;
  wait_us(10);
  sensorLinksTrigger = 0;
  // Reads the echoPin, returns the sound wave travel time in microseconds

  int width = 0;
  while (sensorlinksEcho.read() == 0);
  timer1.start();
  while (sensorlinksEcho.read() == 1);
  timer1.stop();

    int time = timer1.read_us();
    int distance = time / 58;
    if (distance >= objectFound){
        Objectlinks = true;
    }

    return Objectlinks;
}
bool objectrechts(){
    bool Objectrechts;

    timer1.reset();
  ThisThread::sleep_for(500ms);
  int timeout = 26100;
  // Clears the trigPin
  SensorRechtsTrigger = 0;
  wait_us(3);
  // Sets the trigPin on HIGH state for 10 micro seconds
  SensorRechtsTrigger = 1;
  wait_us(10);
  SensorRechtsTrigger = 0;
  // Reads the echoPin, returns the sound wave travel time in microseconds

  int width = 0;
  while (SensorRechtsEcho.read() == 0);
  timer1.start();
  while (SensorRechtsEcho.read() == 1);
  timer1.stop();

    int time = timer1.read_us();
    int distance = time / 58;
    if (distance >= objectFound){
        Objectrechts = true;
    }

    return Objectrechts;
}

bool floor(){
    bool Nofloor;
    return Nofloor;
}

//main function
int main() {

//standard variables
  int turntime = 1000;
  float Power;

//case structure
  robert robert = robert::Forward;
  bool entry = true;

  //object variables
  bool ObjectFound;
  bool objectlinks;
  bool objectrechts;

  //floor variables
  bool nofloor;

  while (true) {
      Print_speed();
    //case structure
    switch (robert) {
    case Forward:
    //go forward
      if (entry) {
        MotorRechtsForward = 1;
        MotorLinksForward = 1;
        MotorLinksBackward = 0;
        MotorRechtsBackward = 0;

        entry = false;
      }
      //check for floor
      if(nofloor == true){
          entry = true;
          robert = robert::Floor;
      }
      //check object
      if (ObjectFound == true) {
        robert = robert::Object;
        entry = true;
      }
      break;
    case Object:
    //start turn
      if (entry) {
        MotorRechtsForward = 0;
        MotorLinksForward = 1;
        MotorRechtsBackward = 1;
        MotorLinksBackward = 0;
        timer1.start();
        entry = false;
      }
      //stop turn and start forward
      if (timer1 >= turntime)
        ;
      {
        MotorRechtsForward = 1;
        MotorLinksForward = 1;
        MotorRechtsBackward = 0;
        MotorLinksBackward = 0;
        timer1.stop();
        timer1.reset();
        //check if there needs to be a turnaround
        if (Object == true) {
          robert = robert::turnaround;
        }
        // if object has been passed/ starting turn
        if (objectrechts == false) {
          ThisThread::sleep_for(100ms);
          timer2.start();
          MotorRechtsForward = 0;
          MotorLinksForward = 1;
          MotorRechtsBackward = 1;
          MotorLinksBackward = 0;
        }
        //end turn and return to forward case
        if (timer2 >= turntime) {
          MotorRechtsForward = 0;
          MotorLinksForward = 0;
          MotorRechtsBackward = 0;
          MotorLinksBackward = 0;

          timer2.stop();
          timer2.reset();

          entry = true;
          robert = robert::Forward;
        }
      }
      break;
    case turnaround:
    if(entry){
        MotorRechtsForward = 0;
          MotorLinksForward = 1;
          MotorRechtsBackward = 1;
          MotorLinksBackward = 0;

        timer1.start();
        entry = false;
    }

    if(timer1 >= turntime *2){
        MotorRechtsForward = 1;
          MotorLinksForward = 1;
          MotorRechtsBackward = 0;
          MotorLinksBackward = 0;

          timer1.stop();
          timer1.reset();
    }

    if (objectlinks == false) {
        ThisThread::sleep_for(100ms);
          timer2.start();
          MotorRechtsForward = 1;
          MotorLinksForward = 0;
          MotorRechtsBackward = 0;
          MotorLinksBackward = 1;
    }
    if (timer2 >= turntime){
          MotorRechtsForward = 0;
          MotorLinksForward = 0;
          MotorRechtsBackward = 0;
          MotorLinksBackward = 0;

          timer2.stop();
          timer2.reset();

          entry = true;
          robert = robert::Forward;
    }
    break;
    case Floor:
    if (entry){
        MotorLinksBackward = 1;
        MotorRechtsBackward = 1;
        MotorLinksForward = 0;
        MotorRechtsForward = 0;

        entry = false;
        timer1.stop();
    }
    if(timer1 >= 500 && nofloor == true){

          MotorRechtsForward = 0;
          MotorLinksForward = 0;
          MotorRechtsBackward = 0;
          MotorLinksBackward = 0;


        timer1.reset();
        timer1.stop();
    }
    else if (timer1 >= 500) {
    MotorRechtsForward = 1;
    MotorLinksBackward = 1;
    MotorLinksForward = 0;
    MotorRechtsBackward = 0;

    timer1.stop();
    timer2.reset();

    timer2.start();
    }

    if (timer2 >= turntime){
        robert = robert::Forward;
    }
      break;
    }
  }
}
