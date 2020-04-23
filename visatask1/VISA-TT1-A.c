#include <visa.h>
#include <stdio.h>

ViStatus status;	  //定义状态变量
ViSession  defRM;	  //定义系统资源会话变量
int cnt; //定义计数器变量

char inbuf[128];  //定义输入缓冲区
int main()
{

	//打开系统资源会话
	status = viOpenDefaultRM(&defRM);	 //教室管理中心216 （资源管理器） 调用，接口	 传址
	if( status != VI_SUCCESS )  //Err < 0, Success == 0, Warning > 0
	{
		printf( "ERROR: viOpenDefaultRM failed with error = 0x%lx\n", status );
		printf( "press anykey to exit...");
		getchar();
		return;
	}
	else
	{
		printf( "打开系统资源会话成功。\n" );
		printf( "press anykey to exit...");
		getchar();
	}
	//打开系统资源会话成功
	
	//
	viClose(defRM);  //关闭系统资源   传值

}
