/*****************************
**
**  visa��Դ����
**
******************************/
#define  IN_CLASSROOM
//#define  IN_LABROOM

#include <ansi_c.h>
#include <visa.h>
#include <stdio.h>

ViStatus status;	  //����״̬����
ViSession  defRM;	  //����ϵͳ��Դ�Ự����
ViSession  instr;	  //���������Ự����
ViFindList List;      //������Դ�б�
ViChar     desc[VI_FIND_BUFLEN]; //������Դ������������
ViUInt32   numInstrs;			 //�������ҵ����������ı���
//modified by feature-f
int cnt; //�������������
char inbuf[128];  //�������뻺����
main()
{
	int i;
	//��ϵͳ��Դ�Ự
	status = viOpenDefaultRM(&defRM);
	if( status != VI_SUCCESS )  //Err < 0, Success == 0, Warning > 0
	{
		printf( "ERROR: viOpenDefaultRM failed with error = 0x%lx\n", status );
		printf( "press anykey to exit...");
		getchar();
		return;
	}
	//��ϵͳ��Դ�Ự�ɹ�
	
	//��������ϵͳ��Դ
#ifdef IN_CLASSROOM
	status = viFindRsrc(defRM,"?*INSTR", &List, &numInstrs, desc);
#endif
#ifdef IN_LABROOM
	status = viFindRsrc(defRM,"?*::INSTR", &List, &numInstrs, desc);   //����	  ��ַ
#endif
	if( status != VI_SUCCESS )
	{
		printf( "Error...\n");getchar();
		viClose(defRM);
		return;
	}
	printf("�ҵ� %d ������\n\n",numInstrs);
	for( i=0; i<numInstrs; i++ )
	{
		status = viOpen(defRM,desc,VI_NULL,VI_NULL, &instr);
		status = viWrite(instr,"*IDN?",5,&cnt);
		status = viRead(instr, inbuf, 100, &cnt);
		inbuf[cnt]=0;
		printf( "����-%d-[%s]is:\n %s \n", i+1,desc,inbuf);
		viClose(instr);
		viFindNext(List,desc);
	}
	
	viClose(defRM);  //�ر�ϵͳ��Դ
	printf( "\npress anykey to exit...");
	getchar();
	
	//
}
