//此处为了兼容其他的多串口Arduino板子
#define GpsSerial  Serial
#define DebugSerial Serial
int L = 13; //LED指示灯引脚

struct
{
	char GPS_Buffer[80];
	bool isGetData;		//是否获取到GPS数据
	bool isParseData;	//是否解析完成
	char UTCTime[11];		//UTC时间
	char latitude[11];		//纬度
	char N_S[2];		//N/S
	char longitude[12];		//经度
	char E_W[2];		//E/W
	bool isUsefull;		//定位信息是否有效
} Save_Data;

const unsigned int gpsRxBufferLength = 600;
char gpsRxBuffer[gpsRxBufferLength];
unsigned int ii = 0;


void setup()	//初始化内容
{
	GpsSerial.begin(9600);			//定义波特率9600，使其与模块的输出频率相等
	DebugSerial.begin(9600);

	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
}

void loop()		//主循环
{
	gpsRead();	//获取GPS数据
	parseGpsBuffer();//解析GPS数据
	printGpsBuffer();//输出解析后的数据
	// DebugSerial.println("\r\n\r\nloop\r\n\r\n");
}

void errorLog(int num)
{
	DebugSerial.print("ERROR");
	DebugSerial.println(num);
	while (1)
	{
		digitalWrite(L, HIGH);
		delay(300);
		digitalWrite(L, LOW);
		delay(300);
	}
}

void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
		DebugSerial.print("Save_Data.UTCTime = ");
		DebugSerial.println(Save_Data.UTCTime);


		
	}
}

void parseGpsBuffer()//获取数据后解析，筛选有效信息
{
	char *subString;
	char *subStringNext;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		DebugSerial.println("**************");
		DebugSerial.println(Save_Data.GPS_Buffer);

		
		for (int i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W

						default:break;
					}
        

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//解析错误
				}
			}


		}
	}

  if(Save_Data.latitude[3]=='.') {char temp;temp=Save_Data.latitude[3];Save_Data.latitude[3]=Save_Data.latitude[1];Save_Data.latitude[1]=temp;}
 if(Save_Data.latitude[4]=='.') {char temp;temp=Save_Data.latitude[4];Save_Data.latitude[4]=Save_Data.latitude[2];Save_Data.latitude[2]=temp;}
 if(Save_Data.latitude[5]=='.') {char temp;temp=Save_Data.latitude[5];Save_Data.latitude[5]=Save_Data.latitude[3];Save_Data.latitude[3]=temp;}
 
 if(Save_Data.longitude[3]=='.') {char temp;temp=Save_Data.longitude[3];Save_Data.longitude[3]=Save_Data.longitude[1];Save_Data.longitude[1]=temp;}
 if(Save_Data.longitude[4]=='.') {char temp;temp=Save_Data.longitude[4];Save_Data.longitude[4]=Save_Data.longitude[2];Save_Data.longitude[2]=temp;}
 if(Save_Data.longitude[5]=='.') {char temp;temp=Save_Data.longitude[5];Save_Data.longitude[5]=Save_Data.longitude[3];Save_Data.longitude[3]=temp;}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
}//转换获取坐标的格式方便小程序解析








void gpsRead()
{
	while (GpsSerial.available())
	{
		gpsRxBuffer[ii++] = GpsSerial.read();
		if (ii == gpsRxBufferLength)clrGpsRxBuffer();
	}

	char* GPS_BufferHead;
	char* GPS_BufferTail;
	if ((GPS_BufferHead = strstr(gpsRxBuffer, "$GPRMC,")) != NULL || (GPS_BufferHead = strstr(gpsRxBuffer, "$GNRMC,")) != NULL )
	{
		if (((GPS_BufferTail = strstr(GPS_BufferHead, "\r\n")) != NULL) && (GPS_BufferTail > GPS_BufferHead))
		{
			memcpy(Save_Data.GPS_Buffer, GPS_BufferHead, GPS_BufferTail - GPS_BufferHead);
			Save_Data.isGetData = true;

			clrGpsRxBuffer();
		}
	}
}

void clrGpsRxBuffer(void)
{
	memset(gpsRxBuffer, 0, gpsRxBufferLength);      //清空
	ii = 0;
}
