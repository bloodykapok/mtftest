/*******************************************************************************************************************************************
* �ļ�����main.c
* �ļ�������973�ǻ�Эͬ����SARϵͳTestBed�׼�����������
* ��������ģ���������������Ϊ������ͨ��ѧ��һ�������������豸���ҹ���ʵ���������������̼�����ѵר�á������������ڴ�֣�����������ļ�������������ѵ��ģ������Ϊ���˾����뾭��Ĳ������Դ�Ĵ�����ʽ��Ϊ���߱��������˽���һ��һ���ڣ��κθ��˲����������ת����չʾ����Դ���ݡ��κ��������ѧԱ����������������ֱ����Ҫ��������������Ҫ����֮Ȩ������������Щ��Դ�����漰ʵ������Ŀ���ܣ��ݲ���������ר���������������Ȩ�����ˣ��ʸ�����Դ����ʵ�����ڲ�һ��һ���ţ��緢���й������߱��˵�һƬ�������Ϊ�����߽���������ԭ���Խ���׷�顢��֤�����ߺ������Ȩ��������Դ�ķ���Ȩ�����߱������У���������ܽ���̽�͸�޳����׵��ȳ���Ϊ���ѧҵ���ౡ֮������ϣ�ԸѧԱ�������ߵ��Ͷ��ɹ���лл������
* ���ߣ�������
* ��ݣ�������ͨ��ѧ��һ�������������豸���ҹ���ʵ���� 2013��˶�������о���
* E-mail. hellozxwang@foxmail.com
* Mobile. 18811774990
* QQ    . 535667240
* Addr  . �����к�������ֱ���ⱱ����ͨ��ѧ��е¥D706��, 100044
*******************************************************************************************************************************************/
/*******************************************************************************************************************************************
*****����˵����1.�������**********************************************************************************************************
*******************************************************************************************************************************************/
/*
�������ò��裺
1���궨���޸�

2��ϵͳ����
��Fedoraϵͳ������Ҫʹ��ԭʼ�׽��ַ����Զ����ʽ�����ݰ�����ر�Fedora�ķ���ǽ�����
sudo systemctl stop firewalld.service
��Ubuntuϵͳ�������κβ���
3����������
make
4������
���漰ԭʼ�׽��ֵ�ʹ�ã���rootȨ��
sudo ./xxx��xxx�����ϲ���ñ��ļ��ĳ��������
*/

#include"cacherouter.h"
#include"borderrouter.h"
#include"resourcemanager.h"
//#include"subscriber.h"
//#include"publisher.h"
#include"client.h"
#include"server.h"
#include"stringclient.h"
#include"stringserver.h"
#include"tcpclient.h"
#include"tcpserver.h"
#include"cnfclient.h"
#include"cnfserver.h"

#include"browserclient.h"

///////////////////////////////////////////////////////////////BUG����////////////////////////////////////////////////////////////////
/*
1��ͬһ���ͻ��˼��������ͬʱ�������������Ͽͻ�������򣬰�����򴿿ͻ��ˡ���������ļ�����������������DATA�ٳ�
����ԭ��ͬԴ�Ŀͻ��˳���ʹ�õ�UDP�˿���ͬ����ɼ����˿ڳ�ͻ
*/

/*****************************************
* �������ƣ�pc_main
* ����������ʵ�������ģʽ�µ�������
* �����б�eg: ./sar cr d1cr1 d1cr1-eth1 d1cr1-eth2 d1cr1-eth3
* ���ؽ����
*****************************************/
void
pc_main
(
	int argc,
	char * argv[]
)
{
	int i,ii;

	if(DEVETESTIMPL <= 0)
	{
		printf("argc == %d\n",argc);
		for(i=0;i<argc;i++)
			printf("argv[%d] == %s\n",i,argv[i]);
	}

	int argcx;
	char argvx[50][30];

	argcx=0;
	memset(argvx,0,1500);

	argcx=argc-1;
	strcpy(argvx[0],"./");
	strcpy(argvx[0]+2,argv[1]);

	i=0;
	ii=0;

	for(i=1;i<argcx;i++)
	{
		strcpy(argvx[i],argv[i+1]);
	}
	//���ˣ����������Ϊ��: ./cr d1cr1 d1cr1-eth1 d1cr1-eth2 d1cr1-eth3
	if(DEVETESTIMPL <= 0)
	{
		printf("argcx == %d\n",argcx);
		for(i=0;i<argcx;i++)
			printf("argvx[%d] == %s\n",i,argvx[i]);
	}

	//�ж��������ڵ��ɫģ��
	int re;

	if(strncmp(argv[1],"hahaha",6)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("SB\n");
	}
	else if(strncmp(argv[1],"cnfcli",6)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("CNF Client Starting...\n");
		re = CNFClientStart_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"cnfser",6)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("CNF Server Starting...\n");
		re = CNFServerStart_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"tcpcli",6)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("TCP Client Starting...\n");
		re = TCPClient_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"tcpser",6)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("TCP Server Starting...\n");
		re = TCPServer_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"strcli",6)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("String Client Starting...\n");
		re = StringClient_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"strser",6)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("String Server Starting...\n");
		re = StringServer_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"http",4)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("HTTP client Starting...\n");
		re = BrowserClient_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"cli",3)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("Client Starting...\n");
		re = Client_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"ser",3)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("Server Starting...\n");
		re = Server_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"sub",3)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("Content Subscriber Starting...\n");
		//re = Subscriber_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"pub",3)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("Content Publisher Starting...\n");
		//re = Publisher_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"cr",2)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("Cache Router Starting...\n");
		re = CacheRouter_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"br",2)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("Border Router Starting...\n");
		re = BorderRouter_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"rm",2)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("Resource Manager Starting...\n");
		re = ResourceManager_main(argcx,(char (*)[30])argvx);
	}
	else
	{
		if(DEVETESTIMPL <= 1)
			printf("Unknown device name!\n");
	}
}

/*****************************************
* �������ƣ�pc_main
* ����������mininet����ƽ̨ģʽ�µ�������
* �����б�eg: ./sar cr d1cr1 1 2 3
* ���ؽ����
*****************************************/
void
mininet_main
(
	int argc,
	char * argv[]
)
{
	int i,ii;

	if(DEVETESTIMPL <= 0)
	{
		printf("argc == %d\n",argc);
		for(i=0;i<argc;i++)
			printf("argv[%d] == %s\n",i,argv[i]);
	}

	int argcx;
	char argvx[50][30];

	argcx=0;
	memset(argvx,0,1500);

	argcx=argc-1;
	strcpy(argvx[0],"./");
	strcpy(argvx[0]+2,argv[1]);
	strcpy(argvx[1],argv[2]);

	i=0;
	ii=0;

	for(i=2;i<argcx;i++)
	{
		strcpy(argvx[i],argvx[1]);
		for(ii=0;ii<30;ii++)
		{
			if(argvx[i][ii]==0)
			{
				strcpy(argvx[i]+ii,"-eth");
				strcpy(argvx[i]+ii+4,argv[i+1]);
				break;
			}
		}
	}
	//���ˣ����������Ϊ��: ./cr d1cr1 d1cr1-eth1 d1cr1-eth2 d1cr1-eth3
	if(DEVETESTIMPL <= 0)
	{
		printf("argcx == %d\n",argcx);
		for(i=0;i<argcx;i++)
			printf("argvx[%d] == %s\n",i,argvx[i]);
	}

	//�ж��������ڵ��ɫģ��
	int re;
	
	if(strncmp(argv[1],"hahaha",6)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("SB\n");
	}
	else if(strncmp(argv[1],"cnfcli",6)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("CNF Client Starting...\n");
		re = CNFClientStart_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"cnfser",6)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("CNF Server Starting...\n");
		re = CNFServerStart_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"tcpcli",6)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("TCP Client Starting...\n");
		re = TCPClient_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"tcpser",6)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("TCP Server Starting...\n");
		re = TCPServer_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"strcli",6)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("String Client Starting...\n");
		re = StringClient_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"strser",6)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("String Server Starting...\n");
		re = StringServer_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"http",4)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("HTTP client Starting...\n");
		re = BrowserClient_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"cli",3)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("Client Starting...\n");
		re = Client_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"ser",3)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("Server Starting...\n");
		re = Server_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"sub",3)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("Content Subscriber Starting...\n");
		//re = Subscriber_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"pub",3)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("Content Publisher Starting...\n");
		//re = Publisher_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"cr",2)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("Cache Router Starting...\n");
		re = CacheRouter_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"br",2)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("Border Router Starting...\n");
		re = BorderRouter_main(argcx,(char (*)[30])argvx);
	}
	else if(strncmp(argv[1],"rm",2)==0)
	{
		if(DEVETESTIMPL <= 1)
			printf("Resource Manager Starting...\n");
		re = ResourceManager_main(argcx,(char (*)[30])argvx);
	}
	else
	{
		if(DEVETESTIMPL <= 1)
			printf("Unknown device name!\n");
	}
}

/*****************************************
* �������ƣ�main
* ����������������
* �����б�eg: ./sar cr d1cr1 1 2 3
* ���ؽ����
*****************************************/
void
main
(
	int argc,
	char * argv[]
)
{
	switch(GLOBALTEST)
	{
		case 0:
			if(DEVETESTIMPL <= 1)
				printf("[main]Do not support the bus network anymore.\n");
			break;
		case 1:
			if(DEVETESTIMPL <= 1)
				printf("[main]Network run on mininet emulator platform.\n");
			mininet_main(argc,(char **)argv);
			break;
		case 2:
			if(DEVETESTIMPL <= 1)
				printf("[main]Network run on real individual physical devices.\n");
			pc_main(argc,(char **)argv);
			break;
		default:
			if(DEVETESTIMPL <= 1)
				printf("[main]GLOBALTEST Configure error.\n");
			break;
	}
}




















//
