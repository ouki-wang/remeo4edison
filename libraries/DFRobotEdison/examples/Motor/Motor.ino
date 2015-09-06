/*
 *Copyright (c) 2014, 成都极趣科技有限公司
 *All rights reserved
 *
 *文件名称：Motor.ino
 *文件标识：
 *摘    要：控制电机例程  
 *
 *当前版本：V1.1
 *作    者：邓卫兵(15196617738@163.com)
 *完成时间：2015.1.7
 *
 *取代版本：V1.0
 *原作者  : 邓卫兵(15196617738@163.com)
 *完成时间：2014.12.22
 */
#include <DFRobot.h>
#include <IIC1.h>

DFrobotEdison Motor;

void setup() {
  Motor.begin(M1); /*初始化电机驱动*/

}

void loop() {
  Motor.move(); /*启动电机*/
  
  delay(3000);
  Motor.setDirection(CLOCKWISE); /*电机顺时针旋转*/
  Motor.setSpeed(100); /**/
  
  delay(3000);
  Motor.setDirection(ANTICLOCKWISE); /*电机逆时针旋转*/
  Motor.setSpeed(200); /*调节电机速度*/
  
  delay(3000);
  
  Motor.stop(); /*停止电机*/
  
  delay(3000);
}