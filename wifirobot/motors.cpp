#include "iface.h"
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
const int SERVO_CHANNEL=0;
const int MOTOR_PWM_BASE=12;
const int LEFT_ENCODER=D6;
const int RIGHT_ENCODER=D5;
const int LED_PWM_BASE = 4;
const int SERVO_MIN = 150;
const int SERVO_MAX = 600;

int left_last=0,right_last=0;
int left_dir=1,right_dir=1;
int left_slits=0,right_slits=0;
int left_limit=0,right_limit=0;


void left_tick_isr()
{
  left_slits+=left_dir;
}

void right_tick_isr()
{
  right_slits+=right_dir;
}


void pwm_setup()
{
  pinMode(LEFT_ENCODER,INPUT);
  pinMode(RIGHT_ENCODER,INPUT);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(10);
  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER),left_tick_isr,RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER),right_tick_isr,RISING);
}

void drive_motor(int ch, int a, int b)
{
  pwm.setPWM(ch+0, 0, a);
  pwm.setPWM(ch+1, 0, b);
}

void drive_motor_channel(int offset, int v, int* dir)
{
  if (v>=0) 
  {
    drive_motor(MOTOR_PWM_BASE+offset,v,0);
    *dir = 1;
  }
  else
  {
    drive_motor(MOTOR_PWM_BASE+offset,0,-v);
    *dir = -1;
  }
}

void drive_left(int v)
{
  drive_motor_channel(0,v,&left_dir);
  if (v==0) left_slits=0;
}

void drive_right(int v)
{
  drive_motor_channel(2,v,&right_dir);
  if (v==0) right_slits=0;
}

void drive(int l, int r)
{
  drive_left(l);
  drive_right(r);
}

void process_drive_command(int* params, int n)
{
  if (n<2) 
  {
    Serial.println("Invalid drive command");
    return;
  }
  Serial.print("Drive: ");
  Serial.print(params[0]);
  Serial.print(",");
  Serial.println(params[1]);
  left_limit=9999;
  right_limit=9999;
  //left_slits=0;
  //right_slits=0;
  drive(params[0],params[1]);
}

void process_step_command(int* params, int n)
{
  if (n<4) return;
  Serial.println("Step");
  //left_slits=0;
  //right_slits=0;
  drive(params[0],params[2]);
  left_limit=left_slits + left_dir * params[1];
  right_limit=right_slits + right_dir * params[3];
}

void process_servo_command(int* params, int n)
{
  if (n<1) 
  {
    Serial.println("Invalid servo command");
    return;
  }
  int p=params[0];
  Serial.print("Servo: ");
  Serial.println(p);
  if (p<SERVO_MIN) p=SERVO_MIN;
  if (p>SERVO_MAX) p=SERVO_MAX;
  //setServoPulse(SERVO_CHANNEL,p+1000.0);
  pwm.setPWM(SERVO_CHANNEL, 0, p);
}

void poll_encoder(int pin, int* last, int* slits)
{
  /*
  int v=digitalRead(pin);
  if (v!=(*last))
  {
    Serial.print("Encoder ");
    Serial.print(pin);
    Serial.print(" changed to ");
    Serial.print(v);
    Serial.print(" slits=");
    *last=v;
    if (v!=0) (*slits)++;
    Serial.println(*slits);
  }
  */
}

void poll_encoders()
{
  if (left_last!=left_slits || right_last!=right_slits)
  {
    Serial.print("Enc: ");
    Serial.print(left_slits);
    Serial.print("  ");
    Serial.println(right_slits);
    left_last=left_slits;
    right_last=right_slits;
  }
  
  //poll_encoder(LEFT_ENCODER,&left_last,&left_slits);
  //poll_encoder(RIGHT_ENCODER,&right_last,&right_slits);
  if (left_limit!=0 && abs(left_slits)>=abs(left_limit))
  {
    Serial.println("Stopping left (limit)");
    drive_left(0);
    left_limit=0;
  }
  if (right_limit!=0 && abs(right_slits)>=abs(right_limit))
  {
    Serial.println("Stopping right (limit)");
    drive_right(0);
    right_limit=0;
  }
}
