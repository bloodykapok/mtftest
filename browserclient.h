/*******************************************************************************************************************************************
* �ļ�����browserclient.h
* �ļ�������973�ǻ�Эͬ����SARϵͳTestBed�׼��������������Э��ת�Ӽ����϶�����������������¶�������������ˣ�Data Subscriber��
* ��������ģ���������������Ϊ������ͨ��ѧ��һ�������������豸���ҹ���ʵ���������������̼�����ѵר�á������������ڴ�֣�����������ļ�������������ѵ��ģ������Ϊ���˾����뾭��Ĳ������Դ�Ĵ�����ʽ��Ϊ���߱��������˽���һ��һ���ڣ��κθ��˲����������ת����չʾ����Դ���ݡ��κ��������ѧԱ����������������ֱ����Ҫ��������������Ҫ����֮Ȩ������������Щ��Դ�����漰ʵ������Ŀ���ܣ��ݲ���������ר����������������Ȩ�����ˣ��ʸ�����Դ����ʵ�����ڲ�һ��һ���ţ��緢���й������߱��˵�һƬ�������Ϊ�����߽���������ԭ���Խ���׷�顢��֤�����ߺ������Ȩ��������Դ�ķ���Ȩ�����߱������У����������ܽ���̽�͸�޳����׵��ȳ���Ϊ���ѧҵ���ౡ֮������ϣ�ԸѧԱ�������ߵ��Ͷ��ɹ���лл������
* ���ߣ�������
* ���ݣ�������ͨ��ѧ��һ�������������豸���ҹ���ʵ���� 2013��˶�������о���
* E-mail. hellozxwang@foxmail.com
* Mobile. 18811774990
* QQ    . 535667240
* Addr  . �����к�������ֱ���ⱱ����ͨ��ѧ��е¥D706��, 100044
*******************************************************************************************************************************************/
/*******************************************************************************************************************************************
*****����˵����1.�䵱�϶�����������ı���HTTP������
**************2.���¶���������ˣ�Data Subscriber���ṩ�������SID������DATA���������������HTTPЭ�鷵��
*******************************************************************************************************************************************/
/*
�������ò��裺
1���궨���޸�
DEFAULTDIRָͨ����ҳ�����ļ�ϵͳ�ĸ�·����ע��ϵͳ���Ƿ���/home��û�еĻ��������о����޸�Ϊĳ��·��������������û��̫��Ӱ��
PhysicalPortָCoLoRЭ�鷢��Get���ͽ���Data���������˿ڣ�ע��������Ĭ�����߶˿������Ƿ�Ϊeth0����Fedora20ϵͳ�е�Ĭ������Ϊem1����ע��ʶ��
2��ϵͳ����
��Fedoraϵͳ������Ҫʹ��ԭʼ�׽��ַ����Զ����ʽ�����ݰ�����ر�Fedora�ķ���ǽ�����
sudo systemctl stop firewalld.service
��Ubuntuϵͳ�������κβ���
3����������
gcc browserclient.c -o browserclient -lpthread
4�����У����漰ԭʼ�׽��ֵ�ʹ�ã���rootȨ�ޣ�
sudo ./browserclient
*/

#include"subscriber.h"

/*******************************************************************************************************************************************
*************************************�궨����������************ȫ�ֱ�������******************************************************************
*******************************************************************************************************************************************/
//HTTP�������ⲿ����
#define DEFAULTIP     "127.0.0.1"             //����HTTP����ı���IP��ַ��������ַ��
#define DEFAULTPORT   "80"                    //����HTTP����ı��ض˿�
#define DEFAULTDIR    "/home"                 //HTTPĿ¼���ʷ���ĸ�·��

//HTTP�������ڲ�����
#define DEFAULTLOG    "./run.log" //������־·��
#define DEFAULTBACK   "10"                    //����������󲢷���������
#define MAX_GETSIDTIME 10                     //�ȴ�����ȡ��Data�ʱ��

//Ӧ�ò㣨HTTP�������� - ���������뺯��
void prterrmsg(char *msg);
#define prterrmsg(msg)         {perror(msg); abort();}
void wrterrmsg(char *msg);
#define wrterrmsg(msg)         {fputs(msg, logfp); fputs(strerror(errno), logfp);fflush(logfp); abort();}


void prtinfomsg(char *msg);
#define prtinfomsg(msg)        {fputs(msg, stdout);}
void wrtinfomsg(char *msg);
#define wrtinfomsg(msg)        {fputs(msg, logfp); fflush(logfp);}

#define MAXPATH 150
#define MAXBUF  1024

char buffer[MAXBUF + 1];
char *host;
char *port;
char *back;
char *dirroot;
char *logdir;
uint8_t daemon_y_n;

FILE *logfp;

/*******************************************************************************************************************************************
*******************************************��ʼ�������������********************************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�BrowserClient_Parameterinit
* ����������BrowserClientģ��ȫ�ֱ�������ֵ
* �����б���
* ���ؽ����
*****************************************/
void
BrowserClient_Parameterinit();

/*******************************************************************************************************************************************
*******************************************Ӧ�ò㣨HTTP������������browserclient��ѭ�����úͿ��ƣ�***********************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�BrowserClient_dir_up
* ��������������dirpath��ָĿ¼����һ��Ŀ¼
* �����б���
* ���ؽ����
*****************************************/
char *
BrowserClient_dir_up
(
	char * dirpath
);

/*****************************************
* �������ƣ�BrowserClient_AllocateMemory
* ��������������ռ䲢��d��ָ�����ݸ���
* �����б���
* ���ؽ����
*****************************************/
void
BrowserClient_AllocateMemory
(
	char ** s,
	int l,
	char * d
);

/*****************************************
* �������ƣ�BrowserClient_GiveResponse
* ������������Path��ָ�����ݷ��͵�client_sockȥ
* �����б���
* ���ؽ����
���Path��һ��Ŀ¼�����г�Ŀ¼����
���Path��һ���ļ����������ļ�
*****************************************/
void
BrowserClient_GiveResponse
(
	FILE * client_sock,
	char * Path
);

/*****************************************
* �������ƣ�BrowserClient_getoption
* ��������������ȡ������Ĳ���
* �����б���
* ���ؽ����
*****************************************/
void
BrowserClient_getoption
(
	int argc,
	char argv[][30]
);
/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.���������   To.Ӧ�÷�����*********************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.Ӧ�÷����� To.���������***********************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
**********************************************************���߳����ɳ���********************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�BrowserClient_main
* ����������BrowserClientģ��������������ForwardingPlaneת��ƽ���̡߳�������ѭ��ִ�м�HTTP���������ܡ�
* �����б���
eg: ./sub d1sub1 d1sub1-eth1
�ڶ�����ʹ��char**argv�ķ�ʽ������������֪�ǲ����﷨����
* ���ؽ����
*****************************************/
int
BrowserClient_main
(
	int argc,
	char argv[][30]
);