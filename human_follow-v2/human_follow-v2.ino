#include<NewPing.h>                        
#include<AFMotor.h>

#define RIGHT A2
#define LEFT A3
#define TRIGGER_PIN A1
#define ECHO_PIN A0
#define MAX_DISTANCE 500
#define MAX_CAPACITY 1750   //array size

unsigned int distance = 0;
unsigned int Right_Value = 0;
unsigned int Left_Value = 0;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

AF_DCMotor Motor1(1,MOTOR12_1KHZ);
AF_DCMotor Motor2(2,MOTOR12_1KHZ);
AF_DCMotor Motor3(3,MOTOR34_1KHZ);
AF_DCMotor Motor4(4,MOTOR34_1KHZ);

char path[MAX_CAPACITY];
int i = 0;
int currentSize = 0;

void setup() {

  Serial.begin(9600);

  pinMode(RIGHT, INPUT);
  pinMode(LEFT, INPUT);

}

void loop() {
  
  delay(50);
  distance = sonar.ping_cm();
  Serial.print("distance");
  Serial.println(distance);

  Right_Value = digitalRead(RIGHT);
  Left_Value = digitalRead(LEFT);

  Serial.print("RIGHT");
  Serial.println(Right_Value);
  Serial.print("LEFT");
  Serial.println(Left_Value);

  if((distance > 15) && (distance < 200)){
  
  //Move Forward:
  Motor1.setSpeed(130);
  Motor1.run(FORWARD);
  Motor2.setSpeed(130);
  Motor2.run(FORWARD);
  Motor3.setSpeed(130);
  Motor3.run(FORWARD);
  Motor4.setSpeed(130);
  Motor4.run(FORWARD);

  path[i] = 'F';
  Serial.println(path[i]);
  i++;
  currentSize++;
  
}else if((Right_Value==0) && (Left_Value==1)) {
  
  //Turn Left                                                
  Motor1.setSpeed(150);
  Motor1.run(FORWARD);
  Motor2.setSpeed(150);
  Motor2.run(FORWARD);
  Motor3.setSpeed(150);
  Motor3.run(BACKWARD);
  Motor4.setSpeed(150);
  Motor4.run(BACKWARD);
  delay(150);

  path[i] = 'L';
  Serial.println(path[i]);
  i++;
  currentSize++;
  
}else if((Right_Value==1)&&(Left_Value==0)) {
  
  //Turn Right
  Motor1.setSpeed(150);
  Motor1.run(BACKWARD);
  Motor2.setSpeed(150);
  Motor2.run(BACKWARD);
  Motor3.setSpeed(150);
  Motor3.run(FORWARD);
  Motor4.setSpeed(150);
  Motor4.run(FORWARD);
  delay(150);

  path[i] = 'R';
  Serial.println(path[i]);
  i++;
  currentSize++;
  
}else if(distance > 200) {
  
  //Stop
  Motor1.setSpeed(0);
  Motor1.run(RELEASE);
  Motor2.setSpeed(0);
  Motor2.run(RELEASE);
  Motor3.setSpeed(0);
  Motor3.run(RELEASE);
  Motor4.setSpeed(0);
  Motor4.run(RELEASE);

}else if(distance < 10) {
  
  //Stop
  Motor1.setSpeed(0);
  Motor1.run(RELEASE);
  Motor2.setSpeed(0);
  Motor2.run(RELEASE);
  Motor3.setSpeed(0);
  Motor3.run(RELEASE);
  Motor4.setSpeed(0);
  Motor4.run(RELEASE);

}else if ((Right_Value==1)&&(Left_Value==1)){
  // return path
  for (int i = currentSize - 1; i >= 0; i--) {
    if(path[i]== 'F'){
      // BACKWARD
      Motor1.setSpeed(130);
      Motor1.run(BACKWARD);
      Motor2.setSpeed(130);
      Motor2.run(BACKWARD);
      Motor3.setSpeed(130);
      Motor3.run(BACKWARD);
      Motor4.setSpeed(130);
      Motor4.run(BACKWARD);

    }else if (path[i]== 'L'){
      //Turn Right
      Motor1.setSpeed(150);
      Motor1.run(BACKWARD);
      Motor2.setSpeed(150);
      Motor2.run(BACKWARD);
      Motor3.setSpeed(150);
      Motor3.run(FORWARD);
      Motor4.setSpeed(150);
      Motor4.run(FORWARD);

    }else if (path[i]== 'R'){
      //Turn Left                                                
      Motor1.setSpeed(150);
      Motor1.run(FORWARD);
      Motor2.setSpeed(150);
      Motor2.run(FORWARD);
      Motor3.setSpeed(150);
      Motor3.run(BACKWARD);
      Motor4.setSpeed(150);
      Motor4.run(BACKWARD);
    }
  }
}
}
