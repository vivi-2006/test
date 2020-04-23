/*****************************
**
** visa资源查找与仪器自动识别
**
******************************/
//#define IN_CLASSROOM
#define IN_LABROOM

#include <ansi_c.h>
#include <visa.h>
#include <stdio.h>

ViStatus status;	  //定义状态变量
ViSession  defRM;	  //定义系统资源会话变量
ViSession  instr;	  //定义仪器会话变量
ViFindList List;     //
ViChar     desc[VI_FIND_BUFLEN];
ViUInt32   numInstrs;

ViSession  dso=VI_NULL, afg=VI_NULL;

int cnt; //定义计数器变量
char inbuf[128];  //定义输入缓冲区
main()
{
	int i;
	//打开系统资源会话
	status = viOpenDefaultRM(&defRM);
	if( status != VI_SUCCESS )  //Err < 0, Success == 0, Warning > 0
	{
		printf( "ERROR: viOpenDefaultRM failed with error = 0x%lx\n", status );
		printf( "press anykey to exit...");
		getchar();
		return;
	}
	//打开系统资源会话成功
	
	//搜索所有系统资源
#ifdef IN_CLASSROOM
	status = viFindRsrc(defRM,"?*INSTR", &List, &numInstrs, desc);
#endif
#ifdef IN_LABROOM
	status = viFindRsrc(defRM,"TCPIP?*INSTR", &List, &numInstrs, desc);
#endif
	if( status != VI_SUCCESS )
	{
		printf("Error\n"); getchar();
		viClose(defRM);
		return;
	}
	printf("找到 %d 个仪器\n",numInstrs);
	for( i=0; i<numInstrs; i++ )
	{
		status = viOpen(defRM,desc,VI_NULL,VI_NULL, &instr);
		status = viWrite(instr,"*IDN?",5,&cnt);
		status = viRead(instr, inbuf, 100, &cnt);
		inbuf[cnt]=0;
		printf( "INSTR %d is: %s \n", i+1,inbuf);
#ifdef IN_CLASSROOM
		if( strstr(inbuf,"TDS 2012") ) dso = instr;
		if( strstr(inbuf,"AFG310") ) afg = instr;
#endif
#ifdef IN_LABROOM
		if( strstr(inbuf,"DSO5012") ) dso = instr;
		if( strstr(inbuf,"33220A") ) afg = instr;
#endif
		viFindNext(List,desc);
	}
	
	printf("\n\n----------------------------\n");
	
	if( dso != VI_NULL )
	{
		status = viWrite(dso,"*IDN?",5,&cnt);
		status = viRead(dso, inbuf, 100, &cnt);
		inbuf[cnt]=0;
		printf( "示波器是： %s \n", inbuf);
	}
	
	if( afg != VI_NULL )
	{
		status = viWrite(afg,"*IDN?",5,&cnt);
		status = viRead(afg, inbuf, 100, &cnt);
		inbuf[cnt]=0;
		printf( "信号源是： %s \n", inbuf);
	}
	
	viClose(defRM);  //关闭系统资源
	printf( "\npress anykey to exit...");
	getchar();
	
	//
}
