
#include <Servo.h>
#include <math.h>
Servo arm1;// create servo object to control a servo
Servo arm2;
Servo arm3;
Servo gripper;
int enB = 5;
int in3 = 2;
int in4 = 3;
// twelve servo objects can be created on most boards

int pos_1 = 0;    // variable to store the servo position
int pos_2 = 105;
const float pi = 3.14159;
float l1 = 97;// length of first arm in mm
float l2 = 114.486;// length of second arm in mm ((120*cos(30)+6)^2+30^2))^0.5
float l2_for12 = 103.021;
int x1[4] = {-40, -40, -120, -120}; // target x value third is -120-5 and fourth is -120 for rectangular -120-10 for rest
int y1[4] = {145+10, 65 ,145+5, 65+10}; // target y value first one is 145+15 for rectangular 145+10 for rest

int x2 = 80; //box x value
int y2 = 80; //box y value
//unsigned long MOVING_TIME = 3000; // moving time is 3 seconds
//unsigned long moveStartTime;
int i = 0;

void setup() {
  Serial.begin(9600);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  arm1.attach(7);  // attaches the servo on pin 9 to the servo object
  arm2.attach(8);
  arm3.attach(9);
  gripper.attach(10);
  Serial.println(l2);
  
for (i = 0; i<4; i++){
  // open the gripper
  for (pos_1 = 0; pos_1 <=22; pos_1 += 1){ //pos = 20 for rectangular =23 for rest
    gripper.write(pos_1);
    delay(50);
  }
   
  if(i == 0){
  goUp();
  delay(6000);
  turnOff();
  } // 6 seconds   
  
  if(i == 2){
  goDown();
  delay(6000); // 6 seconds
  turnOff();
  }
  
  if(i == 0 && i ==1){   
  float theta2 = th2_12(x1[i],y1[i]);
  Serial.println(theta2);
  float theta1 = th1_12(x1[i],y1[i]);
  Serial.println(theta1);
  float th1_degree = theta1*180/pi;
  float th2_degree = theta2*180/pi+15.62;
  move_arm1(th1_degree);
  move_arm2(th2_degree);
  //move down arm3
  for (pos_2 = 105; pos_2 >=70; pos_2 -= 1){ //70 for rectan 80 for rest
    arm3.write(map(pos_2,0,270,0,180));
    delay(40);
  }
  // close the gripper
  for (pos_1 = 22; pos_1 >=0; pos_1 -= 1){ 
    gripper.write(pos_1);
    delay(40);
  }
  //move up arm3
  for (pos_2 = 70; pos_2 <= 135; pos_2 += 1){
    arm3.write(map(pos_2,0,270,0,180));
    delay(40);
  }
  //rotate to box position
  float theta2_tobox = th2(x2+i*random(-5, 5),y2+i*random(-5, 5));
  float theta1_tobox = th1(x2+i*random(-5, 5),y2+i*random(-5, 5));
  float th1_degree_tobox = theta1_tobox*180/pi;
  float th2_degree_tobox = theta2_tobox*180/pi+15;
  move_arm1_tobox(th1_degree,th1_degree_tobox);
  move_arm2_tobox(th2_degree,th2_degree_tobox);
  }
  
  else{   
  float theta2 = th2(x1[i],y1[i]);
  Serial.println(theta2);
  float theta1 = th1(x1[i],y1[i]);
  Serial.println(theta1);
  float th1_degree = theta1*180/pi;
  float th2_degree = theta2*180/pi+15.62;
  move_arm1(th1_degree);
  move_arm2(th2_degree);
  //move down arm3
  for (pos_2 = 135; pos_2 >=85; pos_2 -= 1){
    arm3.write(map(pos_2,0,270,0,180));
    delay(40);
  }
  // close the gripper
  for (pos_1 = 22; pos_1 >=0; pos_1 -= 1){
    gripper.write(pos_1);
    delay(40);
  }
  //move up arm3
  for (pos_2 = 85; pos_2 <= 135; pos_2 += 1){
    arm3.write(map(pos_2,0,270,0,180));
    delay(40);
  }
  //rotate to box position
  float theta2_tobox = th2(x2,y2);
  float theta1_tobox = th1(x2,y2);
  float th1_degree_tobox = theta1_tobox*180/pi;
  float th2_degree_tobox = theta2_tobox*180/pi+15;
  move_arm1_tobox(th1_degree,th1_degree_tobox);
  move_arm2_tobox(th2_degree,th2_degree_tobox);
  }  
 //open gripper at begnning 
  //Serial.println(th1_degree);
  //Serial.println(th2_degree);
  // map(235,0,270,0,180) is the 0 position of arm 1 in the inverse kinematics
  // 135 for arm1 165 for arm2 
  //arm1.write(map(225,0,270,0,180));
  //delay(40);
  //arm2.write(map(165,0,270,0,180));
  //delay(40);
} 
  for (pos_1 = 22; pos_1 >=0; pos_1 -= 1){
    gripper.write(pos_1);
    delay(40);
  }
}

void loop() {

}

// imply inverse kinematics

float th2(double x,double y){
  float c2 = (pow(x,2) + pow(y,2) - pow(l1,2) - pow(l2,2))/(2*l1*l2);
  //Serial.println("c2");
  //Serial.println(c2);
  float s2 = sqrt(1 - pow(c2,2));
  float th2 = atan2(s2, c2); // theta2 is deduced
  return th2;
}

float th1(double x,double y){
  float theta2 = th2(x,y);
  float c2 = (pow(x,2) + pow(y,2) - pow(l1,2) - pow(l2,2))/(2*l1*l2);
  float s2 = sqrt(1 - pow(c2,2));  
  float k1 = l1 + l2*c2;
  float k2 = l2*s2;
  float th1 = atan2(y, x) - atan2(k2, k1); // theta1 is deduce
  return th1;
}

float th2_12(double x,double y){
  float c2 = (pow(x,2) + pow(y,2) - pow(l1,2) - pow(l2_for12,2))/(2*l1*l2_for12);
  //Serial.println("c2");
  //Serial.println(c2);
  float s2 = sqrt(1 - pow(c2,2));
  float th2 = atan2(s2, c2); // theta2 is deduced
  return th2;
}

float th1_12(double x,double y){
  float theta2 = th2(x,y);
  float c2 = (pow(x,2) + pow(y,2) - pow(l1,2) - pow(l2_for12,2))/(2*l1*l2_for12);
  float s2 = sqrt(1 - pow(c2,2));  
  float k1 = l1 + l2_for12*c2;
  float k2 = l2_for12*s2;
  float th1 = atan2(y, x) - atan2(k2, k1); // theta1 is deduce
  return th1;
}

void move_arm1(double th1_degree){
  for(int pos = 235; pos >= (235-th1_degree); pos-= 1){
    arm1.write(map(pos,0,270,0,180));
    delay(50);
  }
}

void move_arm2(double th2_degree){
  for(int pos = 145; pos >= (145-th2_degree); pos-=1){
    arm2.write(map(pos,0,270,0,180));
    delay(50);
  }
}

void move_arm1_tobox(double th1_degree, double moveangle){
  if (th1_degree < moveangle){
    for (int pos = 235-th1_degree; pos >= 235-moveangle; pos -= 1){
      arm1.write(map(pos,0,270,0,180));
      delay(50);
    }
  }
  else {
    for (int pos = 235-th1_degree; pos <= 235-moveangle; pos += 1){
    arm1.write(map(pos,0,270,0,180));
    delay(50);
    }      
  }
}

void move_arm2_tobox(double th2_degree, double moveangle){
  if (th2_degree < moveangle){
    for (int pos = 145-th2_degree; pos >= 145-moveangle; pos -= 1){
      arm2.write(map(pos,0,270,0,180));
      delay(50);
    }
  }
  else {
    for (int pos = 145-th2_degree; pos <= 145-moveangle; pos += 1){
    arm2.write(map(pos,0,270,0,180));
    delay(50);
    }      
  }
}

void goDown() {
  // turn on motor A

  digitalWrite(in3, LOW);

  digitalWrite(in4, HIGH);

  // set speed to 200 out of possible range 0~255

  analogWrite(enB, 255);

}
void goUp() {

  // turn on motor A

  digitalWrite(in3, HIGH);

  digitalWrite(in4, LOW);

  // set speed to 200 out of possible range 0~255

  analogWrite(enB, 255);
}
void turnOff() {  // turn off motors

  digitalWrite(in3, LOW);

  digitalWrite(in4, LOW);

}
