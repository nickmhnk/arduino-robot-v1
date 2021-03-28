#include <IRremote.h>
#include <Servo.h>
#include <NewPing.h>

#define ENABLE_LED_FEEDBACK true
#define USE_DEFAULT_FEEDBACK_LED_PIN 0


const int LeftMotorForward = 7;
const int LeftMotorBackward = 6;
const int RightMotorForward = 5;
const int RightMotorBackward = 4;
const int servoPin = 11;
const int IR_RECEIVE_PIN = 10;

#define TRIGGER_PIN  13  
#define ECHO_PIN     12  
#define MAX_DISTANCE 200


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
Servo servo_motor;

  
enum class DIRECTION {
  FORWARD,
  BACKWARD,
  STOP
};


auto moveState = DIRECTION::STOP;
boolean state = false;
const int TURN_DELAY = 100;

void setup() {  
  Serial.begin(115200);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);

  Serial.print(F("Ready to receive IR signals at pin "));

  pinMode(LED_BUILTIN, OUTPUT);

  servo_motor.attach(servoPin);
  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);
  
}




void loop() {

  int dist = sonar.ping_cm();
  if ((dist < 5) && (moveState == DIRECTION::FORWARD)) {
    moveStop();
  }
  
  if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial);
  
    if (IrReceiver.decodedIRData.command == 0x52) {
      moveBackward();
      
    } else if (IrReceiver.decodedIRData.command == 0x18) {
      moveForward();          
      
    } else if (IrReceiver.decodedIRData.command == 0x8) {
      turnLeft();
      
    } else if (IrReceiver.decodedIRData.command == 0x5A) {
      turnRight();
      
    } else if (IrReceiver.decodedIRData.command == 0x1C) {
      moveStop();
    } 

    IrReceiver.resume();
  }
}


void moveStop() {

  moveState = DIRECTION::STOP;
  
  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}


void moveForward() {
  if (moveState != DIRECTION::FORWARD) {
    moveStop();
    delay(50);
  }
  
  moveState = DIRECTION::FORWARD;
  
  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);

  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW); 

}

void moveBackward() {

  if (moveState != DIRECTION::BACKWARD) {
    moveStop();
    delay(50);
  }
  
  moveState = DIRECTION::BACKWARD;
  
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);
  
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);
}


void turnLeft() {

  if (moveState == DIRECTION::FORWARD) {

    // stop left motor
    digitalWrite(LeftMotorForward, LOW);
    
    // same as forward
    digitalWrite(RightMotorForward, HIGH);
    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW);

    delay(TURN_DELAY);

    //activat left motro
    digitalWrite(LeftMotorForward, HIGH);

    
  } else if (moveState == DIRECTION::BACKWARD) {

    // stop right motor
    digitalWrite(RightMotorBackward, LOW);

    // same as backward
    digitalWrite(LeftMotorBackward, HIGH);
    digitalWrite(LeftMotorForward, LOW);
    digitalWrite(RightMotorForward, LOW);

    delay(TURN_DELAY);

    //activate back
    digitalWrite(RightMotorBackward, HIGH);
    
  } else {
    digitalWrite(LeftMotorForward, LOW);
    digitalWrite(RightMotorForward, HIGH);
    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW);
  }

}

void turnRight() {

  if (moveState == DIRECTION::FORWARD) {

    digitalWrite(RightMotorForward, LOW);
    
    // same as forward
    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW);

    delay(TURN_DELAY);

    digitalWrite(RightMotorForward, HIGH);

    
  } else if (moveState == DIRECTION::BACKWARD) {

    // stop right motor
    digitalWrite(LeftMotorBackward, LOW);

    // same as backward
    digitalWrite(RightMotorBackward, HIGH);
    
    digitalWrite(LeftMotorForward, LOW);
    digitalWrite(RightMotorForward, LOW);

    delay(TURN_DELAY);

    //activate back
    digitalWrite(LeftMotorBackward, HIGH);
    
  } else {
    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(RightMotorForward, LOW);
    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW);
  }

}
