/*****************************
**
**  visa资源查找
**
******************************/
#define  IN_CLASSROOM
//#define  IN_LABROOM

#include <ansi_c.h>
#include <visa.h>
#include <stdio.h>

ViStatus status;	  //定义状态变量
ViSession  defRM;	  //定义系统资源会话变量
ViSession  instr;	  //定义仪器会话变量
ViFindList List;      //定义资源列表
ViChar     desc[VI_FIND_BUFLEN]; //定义资源描述符缓冲区
ViUInt32   numInstrs;			 //定义存放找到仪器数量的变量
//modified by feature-f
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
	status = viFindRsrc(defRM,"?*::INSTR", &List, &numInstrs, desc);   //教室	  地址
#endif
	if( status != VI_SUCCESS )
	{
		printf( "Error...\n");getchar();
		viClose(defRM);
		return;
	}
	printf("找到 %d 个仪器\n\n",numInstrs);
	for( i=0; i<numInstrs; i++ )
	{
		status = viOpen(defRM,desc,VI_NULL,VI_NULL, &instr);
		status = viWrite(instr,"*IDN?",5,&cnt);
		status = viRead(instr, inbuf, 100, &cnt);
		inbuf[cnt]=0;
		printf( "仪器-%d-[%s]is:\n %s \n", i+1,desc,inbuf);
		viClose(instr);
		viFindNext(List,desc);
	}
	
	viClose(defRM);  //关闭系统资源
	printf( "\npress anykey to exit...");
	getchar();
	
	//
}
