#include <Servo.h>
// #include <math.h>

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}


void forwardKinematics() {
  float theta1F = theta1 * PI / 180;   // degrees to radians
  float theta2F = theta2 * PI / 180;
  xP = round(L1 * cos(theta1F) + L2 * cos(theta1F + theta2F));
  yP = round(L1 * sin(theta1F) + L2 * sin(theta1F + theta2F));
}

// INVERSE KINEMATICS
void inverseKinematics(float x, float y) {
  float theta2 = acos((sq(x) + sq(y) - sq(L1) - sq(L2)) / (2 * L1 * L2));
  if (x < 0 & y < 0) {
    theta2 = (-1) * theta2;
  }
  
  float theta1 = atan(x / y) - atan((L2 * sin(theta2)) / (L1 + L2 * cos(theta2)));
  
  theta2 = (-1) * theta2 * 180 / PI;
  theta1 = theta1 * 180 / PI;

 // Angles adjustment depending in which quadrant the final tool coordinate x,y is
  if (x >= 0 & y >= 0) {       // 1st quadrant
    theta1 = 90 - theta1;
  }
  if (x < 0 & y > 0) {       // 2nd quadrant
    theta1 = 90 - theta1;
  }
  if (x < 0 & y < 0) {       // 3d quadrant
    theta1 = 270 - theta1;
    phi = 270 - theta1 - theta2;
    phi = (-1) * phi;
  }
  if (x > 0 & y < 0) {       // 4th quadrant
    theta1 = -90 - theta1;
  }
  if (x < 0 & y == 0) {
    theta1 = 270 + theta1;
  }
  
  // Calculate "phi" angle so gripper is parallel to the X axis
  phi = 90 + theta1 + theta2;
  phi = (-1) * phi;

  // Angle adjustment depending in which quadrant the final tool coordinate x,y is
  if (x < 0 & y < 0) {       // 3d quadrant
    phi = 270 - theta1 - theta2;
  }
  if (abs(phi) > 165) {
    phi = 180 + phi;
  }

  theta1=round(theta1);
  theta2=round(theta2);
  phi=round(phi);
  
  cp5.getController("j1Slider").setValue(theta1);
  cp5.getController("j2Slider").setValue(theta2);
  cp5.getController("j3Slider").setValue(phi);
  cp5.getController("zSlider").setValue(zP);
}
