/*
 *Copyright (c) 2014, Chengdu Geeker Technology Co., Ltd.
 *All rights reserved
 *
 *FileName : Motor.ino
 *File Tag :
 *Summary  : moter control example  
 *
 *Version  : V1.2
 *Author   : ouki.wang(ouki.wang@dfrobot.com)
 *Time     : 2015.10.10
 
 *Version  : V1.1
 *Author   : weibing.deng(15196617738@163.com)
 *Time     : 2015.1.7
 *
 *Version  : V1.0
 *Author   : weibing.deng(15196617738@163.com)
 *Time     : 2014.12.22
 */
#include <DFRobot.h>
#include <IIC1.h>
DFrobotEdison Motor1;
DFrobotEdison Motor2;

void setup() {
  Motor1.begin(M1); /*Initialize motor1*/
  Motor2.begin(M2); /*Initialize motor2*/
}

void loop() {
  Motor2.move(); /*motor rotate with the fastest speed*/
  Motor1.move();
  delay(3000);
  Motor2.setDirection(CLOCKWISE); /*Motor M2 clockwise rotation*/
  Motor1.setDirection(CLOCKWISE); /*Motor M1 clockwise rotation*/

  Motor2.setSpeed(100); /*set speed 100(0-255)*/
  Motor1.setSpeed(100); /*set speed 100(0-255)*/  
  delay(3000);
  
  Motor2.setDirection(ANTICLOCKWISE); /*Motor M2 anticlockwise rotation*/
  Motor2.setSpeed(200); /*set speed 200(0-255)*/
  Motor1.setDirection(ANTICLOCKWISE); /*Motor M1 anticlockwise rotation*/
  Motor1.setSpeed(200); /*set speed 300(0-255)*/  
  delay(3000);
  
  Motor2.stop(); /*stop moter*/
  Motor1.stop(); /*stop moter*/  
  delay(3000);
}