#include "mbed.h"

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
DigitalOut sensorLinksTrigger(D2);
DigitalIn sensorlinksEcho(D3);

// sensor mid
DigitalOut SensorMidTrigger(D4);
DigitalIn SensorMidEcho(D7);

// sensor rechts
DigitalOut SensorRechtsTrigger(D8);
DigitalIn SensorRechtsEcho(D9);

// sensor onder
DigitalIn SensorOnder(A0);

// poorten motors
PwmOut MotorLinksForward(D10);
PwmOut MotorLinksBackward(D11);
PwmOut MotorRechtsForward(D5);
PwmOut MotorRechtsBackward(D6);

// cases
enum robert { Forward, Object, Floor, turnaround };

// movement functions

void MoveForward() {
  MotorRechtsForward = 1;
  MotorLinksForward = 1;
  MotorLinksBackward = 0;
  MotorRechtsBackward = 0;
}

void MoveBackward() {
  MotorRechtsForward = 0;
  MotorLinksForward = 0;
  MotorLinksBackward = 1;
  MotorRechtsBackward = 1;
}

void TurnLeft() {
  MotorRechtsForward = 1;
  MotorLinksForward = 0;
  MotorLinksBackward = 1;
  MotorRechtsBackward = 0;
}

void TurnRight() {
  MotorRechtsForward = 0;
  MotorLinksForward = 1;
  MotorLinksBackward = 0;
  MotorRechtsBackward = 1;
}

void stop() {
  MotorRechtsForward = 0;
  MotorLinksForward = 0;
  MotorLinksBackward = 0;
  MotorRechtsBackward = 0;
}

bool objectfound() {
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
  while (SensorMidEcho.read() == 0)
    ;
  timer1.start();
  while (SensorMidEcho.read() == 1)
    ;
  timer1.stop();

  int time = timer1.read_us();
  int distance = time / 58;
  if (distance >= objectFound) {
    object = true;
  }
  return Object;
}

bool Objectlinks() {
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
  if (distance >= objectFound) {
    Objectlinks = true;
  }

  return Objectlinks;
}
bool objectrechts() {
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
  if (distance >= objectFound) {
    Objectrechts = true;
  }

  return Objectrechts;
}

bool floor() {
  bool Nofloor = false;
  float Spaning_max = 3.3;
  float voltage = SensorOnder.read();

  if (voltage * Spaning_max <= 1) {
    Nofloor = true;
  }

  return Nofloor;
}

// main function
int main() {
  ThisThread::sleep_for(5000ms);

  // standard variables
  int turntime = 1000;
  double speed = power;

  // case structure
  robert robert = robert::Forward;
  bool entry = true;

  // object variables
  bool ObjectFound;
  bool objectlinks;
  bool objectrechts;

  // floor variables
  bool nofloor;

  while (true) {
    // case structure
    switch (robert) {
    case Forward:
      // go forward
      if (entry) {
        MoveForward();

        entry = false;
      }
      // check for floor
      if (nofloor == true) {
        entry = true;
        robert = robert::Floor;
      }
      // check object
      if (ObjectFound == true) {
        robert = robert::Object;
        entry = true;
      }
      break;
    case Object:
      // start turn
      if (entry) {
        TurnRight();
        timer1.start();
        entry = false;
      }
      // stop turn and start forward
      if (timer1 >= turntime)
        ;
      {
        MoveForward();
        timer1.stop();
        timer1.reset();
        // check if there needs to be a turnaround
        if (Object == true) {
          robert = robert::turnaround;
        }
        // if object has been passed/ starting turn
        if (objectlinks == false) {
          ThisThread::sleep_for(100ms);
          timer2.start();
          TurnLeft();
        }
        // end turn and return to forward case
        if (timer2 >= turntime) {
          stop();

          timer2.stop();
          timer2.reset();

          entry = true;
          robert = robert::Forward;
        }
      }
      break;
    case turnaround:
      if (entry) {
        TurnRight();

        timer1.start();
        entry = false;
      }

      if (timer1 >= turntime * 2) {
        MoveForward();

        timer1.stop();
        timer1.reset();
      }

      if (objectlinks == false) {
        ThisThread::sleep_for(100ms);
        timer2.start();
        TurnRight();
      }
      if (timer2 >= turntime) {
        stop();

        timer2.stop();
        timer2.reset();

        entry = true;
        robert = robert::Forward;
      }
      break;
    case Floor:
      if (entry) {
        MoveBackward();

        entry = false;
        timer1.stop();
      }
      if (timer1 >= 500 && nofloor == true) {

        stop();

        timer1.reset();
        timer1.stop();
      } else if (timer1 >= 500) {
        TurnRight();

        timer1.stop();
        timer2.start();
      }

      if (timer2 >= turntime) {
        robert = robert::Forward;
      }
      break;
    }
  }
}