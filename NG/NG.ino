/******************************************************************
                   cmd     arg
Ax模拟输入口       0xAx    0 不读取电压值   非0  读取电压值
Ax数字输出口       0xDx    1 high   0 low
电机1方向          0xB1    1 逆时针 0 顺时针
电机2方向          0xB2    1 逆时针 0 顺时针
电机1速度          0xC1    value 速度值
电机2速度          0xC2    value 速度值
舵机控制           0xEx    value  占空比

‘M’  0x4D
M2
D9      ENA
D7      DIRA       

M1
D10     ENB
D8      DIRB

Version: V1.3
******************************************************************/

#include <Wire.h>
#include <Servo.h> 

Servo myservo;
unsigned char cmd=0,arg=0;
int ADC_value=0;
unsigned char Check_State=0;
#define DBG 0
//程序初始化
void setup()
{
#if (DBG==1)
  Serial.begin(115200);
#endif
  Wire.begin(4);                         // 加入 i2c 总线，设置从机地址为 0xDF
  Wire.onReceive(I2C_slave_receive_Handler);//定义收到主机命令时的处理函数
  Wire.onRequest(I2C_slave_send_Handler);   //定义收到主机命令时的上传数据函数
}

//主程序
void loop()
{
}

// 当从总线接收到主机数据时，执行该函数
void I2C_slave_receive_Handler(int None)
{
	/* 55 aa cmd arg cs*/
	int ok=0;
    int count = 0;
	unsigned char header1,header2,cmd,arg,cs;
	do{
		while( Wire.available() == 0){ // 循环执行，直到数据包只剩下最后一个字符
                  if(++count == 1000) break;
                  delay(1);
                }
		header1 = Wire.read(); //接收命令的第一个字节
#if (DBG==1)
                Serial.print("header1=");Serial.println(header1,HEX);
#endif
		if(header1 != 0x55)
			break;
                count = 0;
		while( Wire.available() == 0){
                  if(++count == 1000) break;
                  delay(1);
                }
		header2 = Wire.read();
#if (DBG==1)
                Serial.print("header2=");Serial.println(header2,HEX);
#endif
		if(header2 != 0xaa)
			break;
                count=0;
		while( Wire.available() == 0){
                  if(++count == 1000) break;
                  delay(1);
                }
		cmd = Wire.read();
#if (DBG==1)
                Serial.print("cmd=");Serial.println(cmd,HEX);
#endif
		count=0;
                while( Wire.available() == 0){
                  if(++count == 1000) break;
                  delay(1);
                }
		arg = Wire.read();
#if (DBG==1)
                Serial.print("arg=");Serial.println(arg,HEX);
#endif
                count=0;
		while( Wire.available() == 0){
                  if(++count == 1000) break;
                  delay(1);
                }
		cs = Wire.read();
#if (DBG==1)
                Serial.print("cs=");Serial.println(cs,HEX);
#endif
		if(cs == (unsigned char)(header1+header2+cmd+arg)){
			ok = 1;
		}
	}while(0);
	if(!ok){
                //Serial.println("ok == 0");
		return;
	}
        exec(cmd,arg);
	ok=0;
}
void exec(unsigned char cmd, unsigned char arg)
{
	unsigned char x = cmd &0x0F;
	switch(cmd&0xF0){
		case 0xA0:
			if(x < 4)
				pinMode(x+14, INPUT);//将Ax设为模拟输入口
                        if(arg)
                                ADC_value=analogRead(x+14);
		break;
		case 0xD0:
			if(x < 4){
				pinMode(x+14, OUTPUT);//将Ax设为数字输出口	
				digitalWrite(7,  arg);
			}
		break;
        case 0xB0: //电机方向
			if(x == 1){//设置M1电机顺时针转动
				pinMode(8, OUTPUT);
				if(arg == 0)//设置M1电机顺时针转动
					digitalWrite(8,  LOW);
				else		  //设置M1电机逆时针转动
					digitalWrite(8,  HIGH);
			}else if(x == 2){
				pinMode(7, OUTPUT);
				if(arg == 0)//设置M1电机顺时针转动
					digitalWrite(7,  LOW);
				else		  //设置M1电机逆时针转动
					digitalWrite(7,  HIGH);				
			}
		break;
		case 0xC0: //电机速度
			if(x == 1)
				analogWrite(10, arg);
			else
				analogWrite(9,  arg);
		break;
		case 0xE0: //舵机控制
			if(x < 4){
				myservo.attach(x+14);
				myservo.write(map(arg, 0, 255, 0, 180));
			}
		break;
		default:
		break;
		}
}
//当主机通知从机上传数据时，执行该事件
void I2C_slave_send_Handler()
{
        unsigned char buf[5];
        buf[0]=0x55;buf[1]=0xaa;
        buf[2]=ADC_value&0xff;buf[3]=ADC_value>>8;
        buf[4]=buf[0]+buf[1]+buf[2]+buf[3];
	Wire.write((const uint8_t *)buf,5); //向总线发送主机想要读取的数据
        //Wire.write(ADC_value>>8); //向总线发送主机想要读取的数据
        //Serial.print("ADC_value=");Serial.println(ADC_value,HEX);
}
