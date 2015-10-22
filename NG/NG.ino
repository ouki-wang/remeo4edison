/******************************************************************
                           cmd     arg
Ax Analog input port       0xAx    0 do not get voltage value , 1 get voltage value
Dx Digital input port      0xDx    1 high   0 low
Moter1 Direction           0xB1    1 anticlockwise 0 clockwise
Moter2 Direction           0xB2    1 anticlockwise 0 clockwise
Moter1 Speed               0xC1    speed value
Moter2 Speed               0xC2    speed value 
servo control              0xEx    duty

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

void setup()
{
#if (DBG == 1)
  Serial.begin(115200);
#endif
  Wire.begin(4);                         //set slave address  4
  Wire.onReceive(I2C_slave_receive_Handler);//Specify the function when i2c slave receives commands 
  Wire.onRequest(I2C_slave_send_Handler);   //Specify the function when i2c slave send data to host
}

void loop()
{
}

// When receiving data from the bus, execute the function
void I2C_slave_receive_Handler(int None)
{
	/* 55 aa cmd arg cs*/
	int ok=0;
    int count = 0;
	unsigned char header1,header2,cmd,arg,cs;
	do{
		while( Wire.available() == 0){ // The loop is executed, until the packets left with only one character
                  if(++count == 1000) break;
                  delay(1);
                }
		header1 = Wire.read(); //receives the last character
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
			if(x > 3) x+=2;
			if(x < 8)
				pinMode(x+14, INPUT);//Set the Ax for analog input port
                        if(arg)
                                ADC_value=analogRead(x+14);
		break;
		case 0xD0:
			if(x > 3) x+=2;
			if(x < 8){
				pinMode(x+14, OUTPUT);//Set the Ax for digital output port
				digitalWrite(x+14,  arg);
			}
		break;
        case 0xB0: //moter direction
			if(x == 1){//Setting M1 motor direction of rotation
				pinMode(8, OUTPUT);
				if(arg == 0)//Setting M1 motor clockwise rotation
					digitalWrite(8,  LOW);
				else		  //Setting M1 motor anticlockwise rotation
					digitalWrite(8,  HIGH);
			}else if(x == 2){
				pinMode(7, OUTPUT);
				if(arg == 0)//Setting M2 motor clockwise rotation
					digitalWrite(7,  LOW);
				else		  //Setting M2 motor anticlockwise rotation
					digitalWrite(7,  HIGH);				
			}
		break;
		case 0xC0: //set moter speed
			if(x == 1) //M1
				analogWrite(10, arg);
			else //M2
				analogWrite(9,  arg);
		break;
		case 0xE0: //servo control
			if(x < 4){
				myservo.attach(x+14);
				myservo.write(map(arg, 0, 255, 0, 180));
			}
		break;
		default:
		break;
		}
}
//When the host control slave uploads the data, execute the function
void I2C_slave_send_Handler()
{
        unsigned char buf[5];
        buf[0]=0x55;buf[1]=0xaa;
        buf[2]=ADC_value&0xff;buf[3]=ADC_value>>8;
        buf[4]=buf[0]+buf[1]+buf[2]+buf[3];
		Wire.write((const uint8_t *)buf,5); //Sending expected data to i2c master
        //Serial.print("ADC_value=");Serial.println(ADC_value,HEX);
}
