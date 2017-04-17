#define PI 3.14159265

// Angle is defined from Y axis
#define X_WHEEL_ANGLE 120
#define Y_WHEEL_ANGLE 0
#define Z_WHEEL_ANGLE 240

// Motor outputs
int motorX = D3;
int motorY = TX;
int motorZ = RX;
int motorR = D2;

// Motor direction outputs
int dirMotorX = D4;
int dirMotorY = D5;
int dirMotorZ = D6;
int dirMotorR = D7;

// Gyro accelerometer I2C interface
//int SCL = D1;
//int SDA = D0;

void setup(){
  setupMotors();
  setupGyroAcc();
  setupParticleFunctions();
}

void loop(){
  // Interrupt driven app
}


/*
  Setup functions
*/

void setupMotors(void){
  // Setup motor pins for pwm output
  pinMode(motorX, OUTPUT);
  pinMode(motorY, OUTPUT);
  pinMode(motorZ, OUTPUT);
  pinMode(motorR, OUTPUT);

  // Setup motor pins for direction control
  pinMode(dirMotorX, OUTPUT);
  pinMode(dirMotorY, OUTPUT);
  pinMode(dirMotorZ, OUTPUT);
  pinMode(dirMotorR, OUTPUT);
}

void setupGyroAcc(void){
  // pass
}

void setupParticleFunctions(void){
  Particle.function("move", move);
  Particle.function("getpos", getPos);
}

/*
  Particle functions
*/

int move(String command){
  int angle = 0;
  int amount = 0;
  int speed = 0;
  int head_rotation = 0;

  Particle.publish(command);

  sscanf(command, "%d %d %d %d", &angle, &amount, &speed, &head_rotation);

  moveBody(angle, amount, speed);
  rotateHead(head_rotation);

  // Add timer interupt to stop motors

  return 0;
}

int getPos(String command){
  return 0;
}

/*
  Movement functions
*/

void moveBody(int angle, int amount, int speed){
  // Coef is used to convert degrees to radians
  float coef = PI / 180;

  // Wheel spin directional angles
  float x = 0;
  float y = 0;
  float z = 0;

  // Wheel spin directions
  bool dirX = false;
  bool dirY = false;
  bool dirZ = false;

  // Validate speed
  if(speed > 255){
    speed = 255;
  }
  else if(speed < 0){
    speed = 0;
  }

  // Validate direction
  if(angle > 360){
    angle = 360;
  }
  else if(angle < 0){
    angle = 0;
  }

  // Calculate wheel directional angles
  x = sin((angle + X_WHEEL_ANGLE) * coef);
  y = sin((angle + Y_WHEEL_ANGLE) * coef);
  z = sin((angle + Z_WHEEL_ANGLE) * coef);

  // Switch directions if values are negative
  if(x < 0){
    dirX = true;
  }
  if(y < 0){
    dirX = true;
  }
  if(z < 0){
    dirX = true;
  }

  // Speed to PWM
  x = speed * fabs(x);
  y = speed * fabs(y);
  z = speed * fabs(z);

  moveMotorX(x, dirX);
  moveMotorY(y, dirY);
  moveMotorZ(z, dirZ);
}

void rotateHead(int rotation_angle){
  bool dir = false;

  if(rotation_angle >= 0){
    dir = true;
  }

  moveMotorR(rotation_angle, dir);
}

void moveMotorX(int pulse, bool dir){
  digitalWrite(dirMotorX, dir);
  analogWrite(motorX, pulse);
}

void moveMotorY(int pulse, bool dir){
  digitalWrite(dirMotorY, dir);
  analogWrite(motorY, pulse);
}

void moveMotorZ(int pulse, bool dir){
  digitalWrite(dirMotorZ, dir);
  analogWrite(motorZ, pulse);
}

void moveMotorR(int pulse, bool dir){
  digitalWrite(dirMotorR, dir);
  analogWrite(motorR, pulse);
}

void stopMovementMotors(){
  analogWrite(motorX, 0);
  analogWrite(motorY, 0);
  analogWrite(motorZ, 0);
}

void stopHeadRotation(){
  analogWrite(motorR, 0);
}
