/*******************************************************************************************************************************************
* �ļ�����videoserver.c
* �ļ�������973�ǻ�Эͬ����SARϵͳTestBed�׼��������ɷ�����������HTTP������
* ��������ģ���������������Ϊ������ͨ��ѧ��һ�������������豸���ҹ���ʵ���������������̼�����ѵר�á������������ڴ�֣�����������ļ�������������ѵ��ģ������Ϊ���˾����뾭��Ĳ������Դ�Ĵ�����ʽ��Ϊ���߱��������˽���һ��һ���ڣ��κθ��˲����������ת����չʾ����Դ���ݡ��κ��������ѧԱ����������������ֱ����Ҫ��������������Ҫ����֮Ȩ������������Щ��Դ�����漰ʵ������Ŀ���ܣ��ݲ���������ר���������������Ȩ�����ˣ��ʸ�����Դ����ʵ�����ڲ�һ��һ���ţ��緢���й������߱��˵�һƬ�������Ϊ�����߽���������ԭ���Խ���׷�顢��֤�����ߺ������Ȩ��������Դ�ķ���Ȩ�����߱������У���������ܽ���̽�͸�޳����׵��ȳ���Ϊ���ѧҵ���ౡ֮������ϣ�ԸѧԱ�������ߵ��Ͷ��ɹ���лл������
* ���ߣ�������
* ��ݣ�������ͨ��ѧ��һ�������������豸���ҹ���ʵ���� 2013��˶�������о���
* E-mail. hellozxwang@foxmail.com
* Mobile. 18811774990
* QQ    . 535667240
* Addr  . �����к�������ֱ���ⱱ����ͨ��ѧ��е¥D706��, 100044
*******************************************************************************************************************************************/
/*******************************************************************************************************************************************
*****����˵����1.�����ɷ�����������HTTP������
*******************************************************************************************************************************************/
/*
�������ò��裺
1���궨���޸�
CACHEPATHָ�洢SID��Dataƥ���ϵ���ļ���Ĭ���ļ���cache.log��·����Ҫ���иó������Ա���о�����������ļ�ʵ�ʴ��ڵ�λ�ö��Ϻž�����
PhysicalPortָCoLoRЭ�鷢��Get���ͽ���Data���������˿ڣ�ע��������Ĭ�����߶˿������Ƿ�Ϊeth0����Fedora20ϵͳ�е�Ĭ������Ϊem1����ע��ʶ��
2��ϵͳ����
��Fedoraϵͳ������Ҫʹ��ԭʼ�׽��ַ����Զ����ʽ�����ݰ�����ر�Fedora�ķ���ǽ�����
sudo systemctl stop firewalld.service
��Ubuntuϵͳ�������κβ���
3����������
gcc tcpserver.c -o tcpserver -lpthread
4�����У����漰ԭʼ�׽��ֵ�ʹ�ã���rootȨ�ޣ�
sudo ./tcpserver
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include <net/if.h>
#include <net/ethernet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <netpacket/packet.h>
#include <netdb.h>

#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <resolv.h>
#include <signal.h>
#include <getopt.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

//Proxy�ⲿͨ��
//�����ⲿIPv4����
#define DEFAULTIP     "127.0.0.1"             //����HTTP����ı���IP��ַ��������ַ��
#define DEFAULTPORT   "3390"                  //����HTTP����ı��ض˿�
#define DEFAULTDIR    "/home"                 //HTTPĿ¼���ʷ���ĸ�·��
//�����ڲ�CoLoR����
#define Physical_Port "eth0"                  //�����˿�
#define LOCALTEST     1                       //�Ƿ�Ϊ����˫�˲��ԣ�������Ϊ��0ֵ����������Ϊ0
#define PORTNUM       0                       //������ʹ�õĶ˿ں�

//Proxy�ڲ�ͨ��
#define PORTtoMAC   6001                      //[UDP�˿ں�]HTTPЭ��ջ  ���� CoLoRЭ��ջ
#define PORTtoHTTP  6002                      //[UDP�˿ں�]CoLoRЭ��ջ ���� HTTPЭ��ջ
#define SIDDES "127.0.0.1"

//Proxy�ڲ�����
#define DATAFLOWPATH  "/home/zxwang/vedioserver/rich.mp3"//��ý���ļ�·��
#define DEFAULTLOG    "/home/HTTP_Server.log" //������־·��
#define DEFAULTBACK   "10"                    //����������󲢷���������
#define MAX_GETSIDTIME 10                     //�ȴ�����ȡ��Data�ʱ��

//Э�����
#define SIDLEN    20                          //SID����
#define NIDLEN    16                          //NID����
#define PIDN      0                           //PID����
#define DATALEN   20                          //Data����
#define PUBKEYLEN 16                          //��Կ����
#define MTU       1500                        //����䵥Ԫ

//ȫ�ֱ���
char temp[100];              //ר���ڸ����ַ�����ʽ�����м��ݴ����
uint8_t tempdata[DATALEN];
uint8_t tempSID[SIDLEN];
uint8_t tempsid[SIDLEN];
uint8_t SID[SIDLEN];//��֪��������������HTTP����������ú���֮�����ֵ����

int flag_localtest = LOCALTEST;

unsigned char local_mac[7];
unsigned char local_ip[5];
char dest_ip[16]={0};
unsigned char broad_mac[7]={0xff,0xff,0xff,0xff,0xff,0xff,0x00};

int test_count=0;

//CoLoRЭ�����������жϵ��ֶΣ���ֹ���̶��ײ�������Version/Type�ֶ�ΪGet����Data����Register����ͨ�ã�
typedef struct _Ether_VersionType Ether_VersionType;
struct _Ether_VersionType
{
	//ethernetͷ
	uint8_t ether_dhost[6]; //Ŀ��Ӳ����ַ
	uint8_t ether_shost[6]; //ԴӲ����ַ
	uint16_t ether_type; //��������
	
	//CoLoR-Dataͷ
	uint8_t version_type;////�汾4λ������4λ
	uint8_t ttl;//����ʱ��
	uint16_t total_len;//�ܳ���
	
	uint16_t port_no;//�˿ں�
	uint16_t checksum;//�����
	
	uint8_t sid_len;//SID����
	uint8_t nid_len;//NID����
	uint8_t pid_n;//PID����
	uint8_t options_static;//�̶��ײ�ѡ��
};
Ether_VersionType tempVersionType;

//CoLoRЭ��Get���ײ���PID֮ǰ���ֶγ��ȹ̶������ڷ�װ
typedef struct _Ether_CoLoR_get Ether_CoLoR_get;
struct _Ether_CoLoR_get
{
	//ethernetͷ
	uint8_t ether_dhost[6]; //Ŀ��Ӳ����ַ
	uint8_t ether_shost[6]; //ԴӲ����ַ
	uint16_t ether_type; //��������
	
	//CoLoR-Getͷ
	uint8_t version_type;////�汾4λ������4λ
	uint8_t ttl;//����ʱ��
	uint16_t total_len;//�ܳ���
	
	uint16_t port_no;//�˿ں�
	uint16_t checksum;//�����
	
	uint8_t sid_len;//SID����
	uint8_t nid_len;//NID����
	uint8_t pid_n;//PID����
	uint8_t options_static;//�̶��ײ�ѡ��
	
	uint16_t publickey_len;//��Կ����
	uint16_t mtu;//����䵥Ԫ
	
	uint8_t sid[SIDLEN];//SID
	uint8_t nid[NIDLEN];//NID
	
	uint8_t data[DATALEN];//Data
	
	uint8_t publickey[PUBKEYLEN];//��Կ
};

//CoLoRЭ��Get���ײ���PID֮ǰ���ֶγ��ȿɱ䣬���ڽ���
typedef struct _Ether_CoLoR_get_parse Ether_CoLoR_get_parse;
struct _Ether_CoLoR_get_parse
{
	//ethernetͷ
	uint8_t ether_dhost[6]; //Ŀ��Ӳ����ַ
	uint8_t ether_shost[6]; //ԴӲ����ַ
	uint16_t ether_type; //��������
	
	//CoLoR-Getͷ
	uint8_t version_type;////�汾4λ������4λ
	uint8_t ttl;//����ʱ��
	uint16_t total_len;//�ܳ���
	
	uint16_t port_no;//�˿ں�
	uint16_t checksum;//�����
	
	uint8_t sid_len;//SID����
	uint8_t nid_len;//NID����
	uint8_t pid_n;//PID����
	uint8_t options_static;//�̶��ײ�ѡ��
	
	uint16_t publickey_len;//��Կ����
	uint16_t mtu;//����䵥Ԫ
	
	uint8_t* sid;//SID
	uint8_t* nid;//NID
	
	uint8_t* data;//Data
	
	uint8_t* publickey;//��Կ
};
Ether_CoLoR_get_parse tempGet;

//CoLoRЭ��Data���ײ���PID֮ǰ���ֶγ��ȹ̶������ڷ�װ
typedef struct _Ether_CoLoR_data Ether_CoLoR_data;
struct _Ether_CoLoR_data
{
	//ethernetͷ
	uint8_t ether_dhost[6]; //Ŀ��Ӳ����ַ
	uint8_t ether_shost[6]; //ԴӲ����ַ
	uint16_t ether_type; //��������
	
	//CoLoR-Dataͷ
	uint8_t version_type;////�汾4λ������4λ
	uint8_t ttl;//����ʱ��
	uint16_t total_len;//�ܳ���
	
	uint16_t port_no;//�˿ں�
	uint16_t checksum;//�����
	
	uint8_t sid_len;//SID����
	uint8_t nid_len;//NID����
	uint8_t pid_n;//PID����
	uint8_t options_static;//�̶��ײ�ѡ��
	
	uint8_t signature_algorithm;//ǩ���㷨
	uint8_t if_hash_cache;//�Ƿ��ϣ4λ���Ƿ񻺴�4λ
	uint16_t options_dynamic;//�ɱ��ײ�ѡ��
	
	uint8_t sid[SIDLEN];//SID
	uint8_t nid[NIDLEN];//NID
	
	uint8_t data[DATALEN];//Data
	
	uint8_t data_signature[16];//����ǩ��
};

//CoLoRЭ��Data���ײ���PID֮ǰ���ֶγ��ȿɱ䣬���ڽ���
typedef struct _Ether_CoLoR_data_parse Ether_CoLoR_data_parse;
struct _Ether_CoLoR_data_parse
{
	//ethernetͷ
	uint8_t ether_dhost[6]; //Ŀ��Ӳ����ַ
	uint8_t ether_shost[6]; //ԴӲ����ַ
	uint16_t ether_type; //��������
	
	//CoLoR-Dataͷ
	uint8_t version_type;////�汾4λ������4λ
	uint8_t ttl;//����ʱ��
	uint16_t total_len;//�ܳ���
	
	uint16_t port_no;//�˿ں�
	uint16_t checksum;//�����
	
	uint8_t sid_len;//SID����
	uint8_t nid_len;//NID����
	uint8_t pid_n;//PID����
	uint8_t options_static;//�̶��ײ�ѡ��
	
	uint8_t signature_algorithm;//ǩ���㷨
	uint8_t if_hash_cache;//�Ƿ��ϣ4λ���Ƿ񻺴�4λ
	uint16_t options_dynamic;//�ɱ��ײ�ѡ��
	
	uint8_t* sid;//SID
	uint8_t* nid;//NID
	
	uint8_t* data;//Data
	
	uint8_t data_signature[16];//����ǩ��
};
Ether_CoLoR_data_parse tempData;

//CoLoRЭ��Register���ײ���PID֮ǰ��
typedef struct _Ether_CoLoR_register Ether_CoLoR_register;
struct _Ether_CoLoR_register
{
	//ethernetͷ
	uint8_t ether_dhost[6]; //Ŀ��Ӳ����ַ
	uint8_t ether_shost[6]; //ԴӲ����ַ
	uint16_t ether_type; //��������
	
	//CoLoR-Registerͷ
	uint8_t version_type;////�汾4λ������4λ
	//�������ʽ������������
};
Ether_CoLoR_register tempRegister;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////���£����̵߳��������뺯������
///////////////////////////////////////////////////////////////////////////////////////////////////////////////HTTP_Server���������뺯��
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
char *host               = 0;
char *port               = 0;
char *back               = 0;
char *dirroot            = 0;
char *logdir             = 0;
unsigned char daemon_y_n = 0;

FILE *logfp;

/*****************************************
* �������ƣ�dir_up
* ����������
����dirpath��ָĿ¼����һ��Ŀ¼
* �����б�
* ���ؽ����
*****************************************/
char *dir_up(char *dirpath)
{
	static char Path[MAXPATH];
	int len;
	
	strcpy(Path, dirpath);
	len = strlen(Path);
	if (len > 1 && Path[len - 1] == '/')
		len--;
	while (Path[len - 1] != '/' && len > 1)
		len--;
	Path[len] = 0;
	return Path;
}


/*****************************************
* �������ƣ�AllocateMemory
* ����������
����ռ䲢��d��ָ�����ݸ���
* �����б�
* ���ؽ����
*****************************************/
void AllocateMemory(char **s, int l, char *d)
{
	*s = malloc(l + 1);
	bzero(*s, l + 1);
	memcpy(*s, d, l);
}

/*****************************************
* �������ƣ�GiveResponse
* ����������
��Path��ָ�����ݷ��͵�client_sockȥ
* �����б�
* ���ؽ����
���Path��һ��Ŀ¼�����г�Ŀ¼����
���Path��һ���ļ����������ļ�
*****************************************/
void GiveResponse(FILE * client_sock, char *Path)
{
	int i=0;
	int j=0;
	char str[30];
	
	
	struct dirent *dirent;
	struct stat info;
	char Filename[MAXPATH];
	DIR *dir;
	int fd, len, ret;
	char *p, *realPath, *realFilename, *nport;
	
	//���ʵ�ʹ���Ŀ¼���ļ�
	len = strlen(dirroot) + strlen(Path) + 1;
	realPath = malloc(len + 1);
	bzero(realPath, len + 1);
	sprintf(realPath, "%s/%s", dirroot, Path);
	
	//���ʵ�ʹ����˿�
	len = strlen(port) + 1;
	nport = malloc(len + 1);
	bzero(nport, len + 1);
	sprintf(nport, ":%s", port);
	
	//����յ�������·��������
    if (stat(realPath, &info))
	{
	/*
	�ڴ˴����SIDת�Ӵ���
	����realPath��ΪSID
	����ӿڣ�SID
	�������CoLoRЭ��GET����װ����
	�����ʽ��1���ļ���ȡ�����ӳٽϴ󲢲����飩��2��socket���ͱ��ػػ���Ϣ
		*/
		//����socket
		//�׽ӿ�������
		int socket_sidsender;
		socket_sidsender=socket(AF_INET,SOCK_DGRAM,0);
		
		sprintf(temp,"GET%s",SID);
		
		struct sockaddr_in addrTo;
		bzero(&addrTo,sizeof(addrTo));
		addrTo.sin_family=AF_INET;
		addrTo.sin_port=htons(PORTtoHTTP);
		//unsigned long IPto = 2130706433;//�ػ���ַ���� == 2130706433
		//addrTo.sin_addr.s_addr=htonl(IPto);//htonl�������ֽ���ת��Ϊ�����ֽ���
		addrTo.sin_addr.s_addr=inet_addr(SIDDES);//htonl�������ֽ���ת��Ϊ�����ֽ���
		//����SID����
		sendto(socket_sidsender,temp,sizeof(temp),0,(struct sockaddr *)&addrTo,sizeof(addrTo));
		printf("   The HTTP Client asked for Data whitch match the SID: %s\n",SID);
		//��ѭ������������Դ�ɹ�������
		int sin_len;
		char message[100];
		int socket_sidreceiver;
		
		struct sockaddr_in sin;
		bzero(&sin,sizeof(sin));
		sin.sin_family=AF_INET;
		sin.sin_addr.s_addr=htonl(INADDR_ANY);
		sin.sin_port=htons(PORTtoMAC);
		sin_len=sizeof(sin);
		
		socket_sidreceiver=socket(AF_INET,SOCK_DGRAM,0);
		bind(socket_sidreceiver,(struct sockaddr *)&sin,sizeof(sin));
		
		int flag_sidgot = 0;
		int counter_sidget = 0;
		while(1)
		{
			recvfrom(socket_sidreceiver,message,sizeof(message),0,(struct sockaddr *)&sin,&sin_len);
			//���ܵ�����ϢΪ ��GOT��
			if(strncmp(message,"GOT",3) == 0)
			{
				flag_sidgot = 1;
				i=2;
				for(j=0;;j++)
				{
					if(message[++i]!='\0')
					{
						str[j] = message[i];
					}
					else
						break;
				}
				str[j]='\0';
				break;
			}printf("Data \"%s\" has been received and it will be sent to the HTTP Client immediately./n",str);
			/*
			sleep(1);
			counter_sidget++;
			if(counter_sidget >= MAX_GETSIDTIME)//�����ȡSID��Ӧ��Դ�����󳬹�MAX_GETSIDTIME��Ԥ��ʱ��û�лظ�������Ϊ����ʧ��
			{
			break;
			}
			*/
		}
		/*
		����ӿڣ�ֱ�ӽ�ȡ�ص��ļ�����DEFAULTDIR�������뿪ͷ�궨�壩��ָ��ĸ�Ŀ¼����������Ĵ����ȡ�ļ�
		������Դ��CoLoRЭ��ȡ�ص��ļ�
		����չ�ԣ������ļ���ȡ�����صĻ���ʵ��������˻���Ļ������ܣ�δ���������ڳ�������ӻ��������б���ϻ�����
		*/
		
		if(flag_sidgot == 1)//�����ȡ������Դ���Ѿ��洢�������ļ���ֱ����ʾ��ҳ����
		{
			fprintf(client_sock,
				"HTTP/1.1 200 OK\r\nServer:SONG\r\nConnection: close\r\nContent-Type:text/html; charset=UTF-8\r\n\r\n<html><head><title>%d - %s</title></head>"
				"<body><font size=+4>[NGI Lab] 973 Project Testing Server - coded by zxWang</font><br><hr width=\"100%%\"><br><center>"
				"<table border cols=3 width=\"100%%\">", errno,
				strerror(errno));
			fprintf(client_sock,
				"</table><font color=\"CC0000\" size=+2>Success: SID \"%s\" was recognized successfully! The Data is: \"%s\"</font></body></html>",
				SID,str);
			goto out;
		}
		else if(flag_sidgot == 0)
		{
			fprintf(client_sock,
				"HTTP/1.1 200 OK\r\nServer:SONG\r\nConnection: close\r\nContent-Type:text/html; charset=UTF-8\r\n\r\n<html><head><title>%d - %s</title></head>"
				"<body><font size=+4>[NGI Lab] 973 Project Testing Server - coded by zxWang</font><br><hr width=\"100%%\"><br><center>"
				"<table border cols=3 width=\"100%%\">", errno,
				strerror(errno));
			fprintf(client_sock,
				"</table><font color=\"CC0000\" size=+2>Error: \n%s %s</font></body></html>",
				Path, strerror(errno));
			goto out;
		}
	}
	//��������ļ����󣬼������ļ�
    if (S_ISREG(info.st_mode))
	{
		fd = open(realPath, O_RDONLY);
		len = lseek(fd, 0, SEEK_END);
		p = (char *) malloc(len + 1);
		bzero(p, len + 1);
		lseek(fd, 0, SEEK_SET);
		ret = read(fd, p, len);
		close(fd);
		fprintf(client_sock,
			"HTTP/1.1 200 OK\r\nServer:SONG\r\nConnection: keep-alive\r\nContent-type: application/*\r\nContent-Length:%d\r\n\r\n",
			len);
			/*
			printf("len = %d\n",len);
			int dd=0;
			char c;
			while(dd<100)
			{
			c = *(p+dd);
			printf("%d",c/128);
			c%=128;
			printf("%d",c/64);
			c%=64;
			printf("%d",c/32);
			c%=32;
			printf("%d",c/16);
			c%=16;
			printf("%d",c/8);
			c%=8;
			printf("%d",c/4);
			c%=4;
			printf("%d",c/2);
			c%=2;
			printf("%d",c);
			printf(" ");
			dd++;
			}
		*/
		fwrite(p, len, 1, client_sock);
free(p);
	} 
	else if (S_ISDIR(info.st_mode))
	{
		//�������Ŀ¼����
		dir = opendir(realPath);
		fprintf(client_sock,
			"HTTP/1.1 200 OK\r\nServer:SONG\r\nConnection: close\r\nContent-Type:text/html; charset=UTF-8\r\n\r\n<html><head><title>%s</title></head>"
			"<body><font size=+4>[NGI Lab] 973 Project Testing Server - coded by zxWang-dir</font><br><hr width=\"100%%\"><br><center>"
			"<table border cols=3 width=\"100%%\">", Path);
		fprintf(client_sock,
			"<caption><font size=+3>Dir %s</font></caption>\n",
			Path);
		fprintf(client_sock,
			"<tr><td>Name</td><td>Size</td><td>Time</td></tr>\n");
		if (dir == 0)
		{
			fprintf(client_sock,
				"</table><font color=\"CC0000\" size=+2>%s</font></body></html>",
				strerror(errno));
			return;
		}
		//��ȡĿ¼�����������
		while ((dirent = readdir(dir)) != 0)
		{
			if (strcmp(Path, "/") == 0)
				sprintf(Filename, "/%s", dirent->d_name);
			else
				sprintf(Filename, "%s/%s", Path, dirent->d_name);
			fprintf(client_sock, "<tr>");
			len = strlen(dirroot) + strlen(Filename) + 1;
			realFilename = malloc(len + 1);
			bzero(realFilename, len + 1);
			sprintf(realFilename, "%s/%s", dirroot, Filename);
			if (stat(realFilename, &info) == 0)
			{
				if (strcmp(dirent->d_name, "..") == 0)
				{
					fprintf(client_sock,
						"<td><a href=\"http://%s%s%s\">(parent)</a></td>",
						host, atoi(port) == 80 ? "" : nport,
						dir_up(Path));
				}
				else
				{
					fprintf(client_sock,
						"<td><a href=\"http://%s%s%s\">%s</a></td>",
						host, atoi(port) == 80 ? "" : nport, Filename,
						dirent->d_name);
				}
				if (S_ISDIR(info.st_mode))
					//fprintf(client_sock, "<td>Ŀ¼</td>");
fprintf(client_sock, "<td>Dir</td>");
else if (S_ISREG(info.st_mode))
fprintf(client_sock, "<td>%d</td>", (int)info.st_size);
				else if (S_ISLNK(info.st_mode))
					//fprintf(client_sock, "<td>����</td>");
					fprintf(client_sock, "<td>Link</td>");
				else if (S_ISCHR(info.st_mode))
					//fprintf(client_sock, "<td>�ַ��豸</td>");
					fprintf(client_sock, "<td>Char Device</td>");
				else if (S_ISBLK(info.st_mode))
					//fprintf(client_sock, "<td>���豸</td>");
					fprintf(client_sock, "<td>Block Device</td>");
else if (S_ISFIFO(info.st_mode))
					fprintf(client_sock, "<td>FIFO</td>");
				else if (S_ISSOCK(info.st_mode))
					fprintf(client_sock, "<td>Socket</td>");
				else
					//fprintf(client_sock, "<td>(δ֪)</td>");
					fprintf(client_sock, "<td>(Unknown)</td>");
				fprintf(client_sock, "<td>%s</td>", ctime(&info.st_ctime));
			}
			fprintf(client_sock, "</tr>\n");
			free(realFilename);
		}
fprintf(client_sock, "</table></center></body></html>");
	}
	else
	{
		//�ȷǳ����ļ��ַ�Ŀ¼����ֹ����
		fprintf(client_sock,
			"HTTP/1.1 200 OK\r\nServer:SONG\r\nConnection: close\r\nContent-Type:text/html; charset=UTF-8\r\n<html><head><title>permission denied</title></head>"
			"<body><font size=+4>[NGI Lab] 973 Project Testing Server - coded by zxWang-wrong</font><br><hr width=\"100%%\"><br><center>"
			"<table border cols=3 width=\"100%%\">");
		fprintf(client_sock,
			"</table><font color=\"CC0000\" size=+2>Path '%s' denied</font></body></html& gt;",
			Path);
	}
out:
	free(realPath);
	free(nport);
}

/*****************************************
* �������ƣ�getoption
* ����������
����ȡ������Ĳ���
* �����б�
* ���ؽ����
*****************************************/
void getoption(int argc, char **argv)
{
	int c, len;
	char *p = 0;
	
	opterr = 0;
	while (1)
	{
		int option_index = 0;
		static struct option long_options[] =
		{
			{"host", 1, 0, 0},
			{"port", 1, 0, 0},
			{"back", 1, 0, 0},
			{"dir", 1, 0, 0},
			{"log", 1, 0, 0},
			{"daemon", 0, 0, 0},
			{0, 0, 0, 0}
		};
		//������֧����һЩ������
		//--host   IP��ַ        ��  -H  IP��ַ
		//--port   �˿�          ��  -P  �˿�
		//--back   ��������      ��  -B  ��������
		//--dir    ��վ��Ŀ¼    ��  -D  ��վ��Ŀ¼
		//--log    ��־���·��  ��  -L  ��־���·��
		//--daemon ��̨ģʽ
		c = getopt_long(argc, argv, "H:P:B:D:L",
			long_options, &option_index);
		if (c == -1 || c == '?')
			break;
		
		if(optarg)
			len = strlen(optarg);
		else
			len = 0;
		
		if ((!c
			&& !(strcasecmp(long_options[option_index].name, "host")))
			|| c == 'H')
			p = host = malloc(len + 1);
		else if ((!c
			&& !(strcasecmp(long_options[option_index].name, "port")))
			|| c == 'P')
			p = port = malloc(len + 1);
		else if ((!c
			&& !(strcasecmp(long_options[option_index].name, "back")))
			|| c == 'B')
			p = back = malloc(len + 1);
		else if ((!c
			&& !(strcasecmp(long_options[option_index].name, "dir")))
			|| c == 'D')
			p = dirroot = malloc(len + 1);
		else if ((!c
			&& !(strcasecmp(long_options[option_index].name, "log")))
			|| c == 'L')
			p = logdir = malloc(len + 1);
		else if ((!c
			&& !(strcasecmp(long_options[option_index].name, "daemon"))))
		{
			daemon_y_n = 1;
			continue;
		}
		else
			break;
		bzero(p, len + 1);
		memcpy(p, optarg, len);
	}
}

/*******************************************************************************************************************************************
*******************************************ԭʼ�׽��ֽ������ݰ���������MAC�����ϵ���������****************************************************
*******************************************************************************************************************************************/
int CoLoR_Sendpkg ( char * mac,char * broad_mac,char * ip,char * dest,uint16_t port );

//���ջ�������������ԭʼ���ݣ���С
#define RCV_BUF_SIZE     1024 * 5

//���ջ�����
static int g_iRecvBufSize = RCV_BUF_SIZE;
static char g_acRecvBuf[RCV_BUF_SIZE] = {0};

//���������ӿ�,��Ҫ���ݾ�������޸�
static const char *g_szIfName = Physical_Port;

/*****************************************
* �������ƣ�Ethernet_SetPromisc
* ����������������������ģʽ���Բ���
* �����б�
const char *pcIfName
int fd
int iFlags
* ���ؽ����
static int
*****************************************/
static int Ethernet_SetPromisc(const char *pcIfName, int fd, int iFlags)
{
	int iRet = -1;
	struct ifreq stIfr;
	
	//��ȡ�ӿ����Ա�־λ
	strcpy(stIfr.ifr_name, pcIfName);
	iRet = ioctl(fd, SIOCGIFFLAGS, &stIfr);
	if (0 > iRet)
	{
		perror("[Error]Get Interface Flags");   
		return -1;
	}
	
	if (0 == iFlags)
	{
		//ȡ������ģʽ
		stIfr.ifr_flags &= ~IFF_PROMISC;
	}
	else
	{
		//����Ϊ����ģʽ
		stIfr.ifr_flags |= IFF_PROMISC;
	}
	
	iRet = ioctl(fd, SIOCSIFFLAGS, &stIfr);
	if (0 > iRet)
	{
		perror("[Error]Set Interface Flags");
		return -1;
	}
	
	return 0;
}

/*****************************************
* �������ƣ�Ethernet_InitSocket
* ��������������ԭʼ�׽���
* �����б�
* ���ؽ����
static int
*****************************************/
static int Ethernet_InitSocket()
{
	int iRet = -1;
	int fd = -1;
	struct ifreq stIf;
	struct sockaddr_ll stLocal = {0};
	
	//����SOCKET
	fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (0 > fd)
	{
		perror("[Error]Initinate L2 raw socket");
		return -1;
	}
	
	//��������ģʽ����
	Ethernet_SetPromisc(g_szIfName, fd, 1);
	
	//����SOCKETѡ��
	iRet = setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &g_iRecvBufSize,sizeof(int));
	if (0 > iRet)
	{
		perror("[Error]Set socket option");
		close(fd);
		return -1;
	}
	
	//��ȡ���������ӿ�����
	strcpy(stIf.ifr_name, g_szIfName);
	iRet = ioctl(fd, SIOCGIFINDEX, &stIf);
	if (0 > iRet)
	{
		perror("[Error]Ioctl operation");
		close(fd);
		return -1;
	}
	
	//����������
	stLocal.sll_family = PF_PACKET;
	stLocal.sll_ifindex = stIf.ifr_ifindex;
	stLocal.sll_protocol = htons(ETH_P_ALL);
	iRet = bind(fd, (struct sockaddr *)&stLocal, sizeof(stLocal));
	if (0 > iRet)
	{
		perror("[Error]Bind the interface");
		close(fd);
		return -1;
	}
	
	return fd;   
}

/*****************************************
* �������ƣ�CoLoR_SeeType
* �����������ж��Ƿ�Ϊ��Ϊ����CoLoR��ͷ
* �����б�
const Ether_CoLoR_get *pkg
* ���ؽ����
static int
*****************************************/
static int CoLoR_SeeType(const Ether_VersionType *pkg)
{
	int i;
	struct protoent *pstIpProto = NULL;
	
	if (NULL == pkg)
	{
		return -1;
	}
	//����Ϊ�жϣ�ȷ�������������ݰ������Լ������ģ����Ա����������Զ˽��в��Ե�ʱ���뽫flag_localtest�ж�ֵ��Ϊ����ֵ
	if(flag_localtest != 0)
	{
		if((strncmp((char*)pkg->ether_shost,(char*)local_mac,6)==0) && (pkg->port_no == PORTNUM))
		{
			return -1;
		}
	}
	
	//�汾��Э������
	//int version = pkg->version_type / 16;//ȡversion_type�ֶ�8λ����������ǰ��λ����ȡIP�汾��
	//int type    = pkg->version_type % 16;//ȡversion_type�ֶ�8λ���������ĺ���λ����ȡCoLoR�����ͺ�
	if(pkg->version_type == 160)//����ʾIP�汾��Ϊ10��������Ϊ0��ר��CoLoR-Get��  �ֶ�Ϊ1010 0000���İ�
	{
		return 0;
	}
	if(pkg->version_type == 161)//����ʾIP�汾��Ϊ10��������Ϊ1��ר��CoLoR-Data�� �ֶ�Ϊ1010 0001���İ�
	{
		return 1;
	}
	if(pkg->version_type == 161)//����ʾIP�汾��Ϊ10��������Ϊ2��ר��CoLoR-Register�� �ֶ�Ϊ1010 0010���İ�
	{
		return 2;
	}
	
	
	return -1;
}

/*****************************************
* �������ƣ�CoLoR_ParseGet
* ��������������CoLoR-Get��ͷ
* �����б�
const Ether_CoLoR_get *pkg
* ���ؽ����
static int
*****************************************/
static int CoLoR_ParseGet(const Ether_CoLoR_get *pkg)
{
	int i;
	
	char* p;
	p = (char*)pkg;
	
	if (NULL == pkg)
	{
		return -1;
	}
	
	//����Get�����ֶ�
	free(tempGet.sid);
	free(tempGet.nid);
	free(tempGet.data);
	free(tempGet.publickey);
	
	memcpy((uint8_t*)tempGet.ether_dhost,(uint8_t*)pkg->ether_dhost,6);
	memcpy((uint8_t*)tempGet.ether_shost,(uint8_t*)pkg->ether_shost,6);
	tempGet.ether_type = pkg->ether_type;
	
	printf (">>>CoLoR-Get Received.\n");
	printf ("   |=================Packet=================|\n");
	tempGet.version_type = pkg->version_type;
	printf("   |version_type = %d\n",tempGet.version_type);
	tempGet.ttl = pkg->ttl;
	printf("   |ttl = %d\n",tempGet.ttl);
	tempGet.total_len = pkg->total_len;
	printf("   |total_len = %d\n",tempGet.total_len);
	tempGet.port_no = pkg->port_no;
	printf("   |port_no = %d\n",tempGet.port_no);
	tempGet.checksum = pkg->checksum;
	printf("   |checksum = %d\n",tempGet.checksum);
	tempGet.sid_len = pkg->sid_len;
	printf("   |sid_len = %d\n",tempGet.sid_len);
	tempGet.nid_len = pkg->nid_len;
	printf("   |nid_len = %d\n",tempGet.nid_len);
	tempGet.pid_n = pkg->pid_n;
	printf("   |pid_n = %d\n",tempGet.pid_n);
	tempGet.options_static = pkg->options_static;
	printf("   |options_static = %d\n",tempGet.options_static);
	
	p += sizeof(uint8_t) * (14 + 12);
	
	tempGet.publickey_len = (uint16_t)(*p);
	printf("   |publickey_len = %d\n",tempGet.publickey_len);
	tempGet.mtu = (uint16_t)(*(p+sizeof(uint16_t)));
	printf("   |mtu = %d\n",tempGet.mtu);
	
	p += sizeof(uint16_t) * 2;
	
	tempGet.sid = (uint8_t*)calloc(tempGet.sid_len,sizeof(uint8_t));
	memcpy((uint8_t*)tempGet.sid,(uint8_t*)p,tempGet.sid_len);
	printf("   |sid = %s\n",tempGet.sid);
	
	p += sizeof(uint8_t) * tempGet.sid_len;
	
	tempGet.nid = (uint8_t*)calloc(tempGet.nid_len,sizeof(uint8_t));
	memcpy((uint8_t*)tempGet.nid,(uint8_t*)p,tempGet.nid_len);
	printf("   |nid = %s\n",tempGet.nid);
	
	p += sizeof(uint8_t) * tempGet.nid_len;
	
	uint16_t data_len = tempGet.total_len - 16 - tempGet.sid_len - tempGet.nid_len - tempGet.pid_n*4 - tempGet.publickey_len;
	tempGet.data = (uint8_t*)calloc(data_len+1/*+1��������Ϊ�������ر�Ԥ��*/,sizeof(uint8_t));
	memcpy((uint8_t*)tempGet.data,(uint8_t*)p,data_len);
	*(tempGet.data+data_len) = '\0';//�����ΪʲôΨ��data����Ҫ�ر��ڳ�1byte��Ϊ�����������Խ���ȡ
	printf("   |data = %s\n",tempGet.data);
	
	p += sizeof(uint8_t) * data_len;
	
	tempGet.publickey = (uint8_t*)calloc(tempGet.publickey_len,sizeof(uint8_t));
	memcpy((uint8_t*)tempGet.publickey,(uint8_t*)p,tempGet.publickey_len);
	printf("   |publickey = %s\n",tempGet.publickey);
	
	memcpy(tempsid, tempGet.sid, tempGet.sid_len/*SID����*/);
	printf ("   |========================================|\n");
	
	//������Get������Ӧ
	if(tempGet.port_no == 4141)//����mplayer��Get����
	{
		CoLoR_Sendpkg ( local_mac,broad_mac,local_ip,dest_ip,4141 );
	}
	
	
	return 0;
}

/*****************************************
* �������ƣ�CoLoR_ParseData
* ��������������CoLoR-Data��ͷ
* �����б�
const Ether_CoLoR_get *pkg
* ���ؽ����
static int
*****************************************/
static int CoLoR_ParseData(const Ether_CoLoR_data *pkg)
{
	int i;
	
	char* p;
	p = (char*)pkg;
	
	if (NULL == pkg)
	{
		return -1;
	}
	
	//����Data�����ֶ�
	free(tempData.sid);
	free(tempData.nid);
	free(tempData.data);
	
	printf (">>>CoLoR-Data Received.\n");
	printf ("   |=================Packet=================|\n");
	memcpy((uint8_t*)tempData.ether_dhost,(uint8_t*)pkg->ether_dhost,6);
	memcpy((uint8_t*)tempData.ether_shost,(uint8_t*)pkg->ether_shost,6);
	tempData.ether_type = pkg->ether_type;
	tempData.version_type = pkg->version_type;
	printf("   |version_type = %d\n",tempData.version_type);
	tempData.ttl = pkg->ttl;
	printf("   |ttl = %d\n",tempData.ttl);
	tempData.total_len = pkg->total_len;
	printf("   |total_len = %d\n",tempData.total_len);
	tempData.port_no = pkg->port_no;
	printf("   |port_no = %d\n",tempData.port_no);
	tempData.checksum = pkg->checksum;
	printf("   |checksum = %d\n",tempData.checksum);
	tempData.sid_len = pkg->sid_len;
	printf("   |sid_len = %d\n",tempData.sid_len);
	tempData.nid_len = pkg->nid_len;
	printf("   |nid_len = %d\n",tempData.nid_len);
	tempData.pid_n = pkg->pid_n;
	printf("   |pid_n = %d\n",tempData.pid_n);
	tempData.options_static = pkg->options_static;
	printf("   |options_static = %d\n",tempData.options_static);
	
	p += sizeof(uint8_t) * (14 + 12);
	
	tempData.signature_algorithm = (uint8_t)(*p);
	printf("   |signature_algorithm = %d\n",tempData.signature_algorithm);
	tempData.if_hash_cache = (uint8_t)(*(p+sizeof(uint8_t)));
	printf("   |if_hash_cache = %d\n",tempData.if_hash_cache);
	tempData.options_dynamic = (uint16_t)(*(p+sizeof(uint16_t)));
	printf("   |options_dynamic = %d\n",tempData.options_dynamic);
	
	p += sizeof(uint32_t)-1;//-1�������в����pΪʲô�����������һλ
	
	tempData.sid = (uint8_t*)calloc(tempData.sid_len,sizeof(uint8_t));
	memcpy((uint8_t*)tempData.sid,(uint8_t*)p,tempData.sid_len);
	printf("   |sid = %s\n",tempData.sid);
	
	p += sizeof(uint8_t) * tempData.sid_len;
	
	tempData.nid = (uint8_t*)calloc(tempData.nid_len,sizeof(uint8_t));
	memcpy((uint8_t*)tempData.nid,(uint8_t*)p,tempData.nid_len);
	printf("   |nid = %s\n",tempData.nid);
	
	p += sizeof(uint8_t) * tempData.nid_len;
	
	uint16_t data_len = tempData.total_len - sizeof(uint8_t)*32 - tempData.sid_len - tempData.nid_len - tempData.pid_n*sizeof(uint8_t)*4;
	tempData.data = (uint8_t*)calloc(data_len+1/*+1��������Ϊ�������ر�Ԥ��*/,sizeof(uint8_t));
	memcpy((uint8_t*)tempData.data,(uint8_t*)p,data_len);
	*(tempData.data+data_len) = '\0';//�����ΪʲôΨ��data����Ҫ�ر��ڳ�1Byte��Ϊ�����������Խ���ȡ
	printf("   |data = %s\n",tempData.data);
	
	p += sizeof(uint8_t) * data_len;
	
	for(i=0;i<16;i++)
	{
		tempData.data_signature[i] = (uint8_t)(*(p+sizeof(uint8_t)*i));
	}
	printf("   |data_signature = %s\n",tempData.data_signature);
	
	//��HTTP_Server����Got����
	int socket_sidsender;
	socket_sidsender=socket(AF_INET,SOCK_DGRAM,0);
	
	printf ("   |========================================|\n");
	
	sprintf(temp,"GOT%s",pkg->data);//When this code file is moved to CoLoR, change "wangzhaoxu" to a char* witch contains the SID.
	
	struct sockaddr_in addrTo;
	bzero(&addrTo,sizeof(addrTo));
	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons(PORTtoMAC);
	addrTo.sin_addr.s_addr=inet_addr(SIDDES);
	
	sendto(socket_sidsender,temp,sizeof(temp),0,(struct sockaddr *)&addrTo,sizeof(addrTo));
	
	return 0;
}

/*****************************************
* �������ƣ�CoLoR_ParseRegister
* ��������������CoLoR-Register��ͷ
* �����б�
const Ether_CoLoR_get *pkg
* ���ؽ����
static int
*****************************************/
static int CoLoR_ParseRegister(const Ether_CoLoR_register *pkg)
{
	int i;
	
	if (NULL == pkg)
	{
		return -1;
	}
	//����Register�����ֶ�
	//�����Register����ʽ�������
	
	
	printf (">>>CoLoR-Register from Somewhere. Type : %d\n",pkg->version_type%16);
	
	return 0;
}

/*****************************************
* �������ƣ�Ethernet_ParseFrame
* ��������������֡��������
* �����б�
const char *pcFrameData
* ���ؽ����
static int
*****************************************/
static int Ethernet_ParseFrame(const char *pcFrameData)
{
	//��鱾��mac��IP��ַ
	memset ( local_mac,0,sizeof ( local_mac ) );
	memset ( local_ip,0,sizeof ( local_ip ) );
	memset ( dest_ip,0,sizeof ( dest_ip ) );
	
	if ( GetLocalMac ( Physical_Port,local_mac,local_ip ) ==-1 )
		return ( -1 );
	
	
	int iType = -1;
	int iRet = -1;
	
	struct ether_header *pstEthHead = NULL;
	Ether_VersionType *pkgvt = NULL;
	Ether_CoLoR_get *pkgget = NULL;
	Ether_CoLoR_data *pkgdata = NULL;
	Ether_CoLoR_register *pkgregister = NULL;
	
	//���յ���ԭʼ��������ֵΪ��̫��ͷ
	pstEthHead = (struct ether_header*)g_acRecvBuf;
	
	//�ж�CoLoR���ݰ�����
	pkgvt = (Ether_VersionType *)(pstEthHead + 0);
	iType = CoLoR_SeeType(pkgvt);
	
	if(iType == 0)//�յ�Get��
	{
		pkgget  = (Ether_CoLoR_get *)(pstEthHead + 0);
		iRet = CoLoR_ParseGet(pkgget);
	}
	else if(iType == 1)//�յ�Data��
	{
		pkgdata  = (Ether_CoLoR_data *)(pstEthHead + 0);
		iRet = CoLoR_ParseData(pkgdata);
	}
	else if(iType == 2)//�յ�Register��
	{
		pkgregister  = (Ether_CoLoR_register *)(pstEthHead + 0);
		iRet = CoLoR_ParseRegister(pkgregister);
	}
	else//�����Ͳ�����CoLoRЭ��
	{
	}
	
	return iRet;
}

/*****************************************
* �������ƣ�Ethernet_StartCapture
* ����������������������֡
* �����б�
const int fd
* ���ؽ����void
*****************************************/
static void Ethernet_StartCapture(const int fd)
{
	int iRet = -1;
	socklen_t stFromLen = 0;
	
	//ѭ������
	while(1)
	{
		//��ս��ջ�����
		memset(g_acRecvBuf, 0, RCV_BUF_SIZE);
		
		//��������֡
		iRet = recvfrom(fd, g_acRecvBuf, g_iRecvBufSize, 0, NULL, &stFromLen);
		if (0 > iRet)
		{
			continue;
		}
		
		//��������֡
		Ethernet_ParseFrame(g_acRecvBuf);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////���ϣ����߳�back���������뺯��
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////���£����߳�get���������뺯��

/*****************************************
* �������ƣ�GetLocalMac
* ����������
�õ�������mac��ַ��ip��ַ
Ϊ���ݰ���װʱmac��Դ��ַ�ֶ��ṩ����
* �����б�
const char *device
char *mac
char *ip
* ���ؽ����
int
*****************************************/
int GetLocalMac ( const char *device,char *mac,char *ip )
{
	int sockfd;
	struct ifreq req;
	struct sockaddr_in * sin;
	
	if ( ( sockfd = socket ( PF_INET,SOCK_DGRAM,0 ) ) ==-1 )
	{
		fprintf ( stderr,"Sock Error:%s\n\a",strerror ( errno ) );
		return ( -1 );
	}
	
	memset ( &req,0,sizeof ( req ) );
	strcpy ( req.ifr_name,device );
	if ( ioctl ( sockfd,SIOCGIFHWADDR, ( char * ) &req ) ==-1 )
	{
		fprintf ( stderr,"ioctl SIOCGIFHWADDR:%s\n\a",strerror ( errno ) );
		close ( sockfd );
		return ( -1 );
	}
	memcpy ( mac,req.ifr_hwaddr.sa_data,6 );
	
	req.ifr_addr.sa_family = PF_INET;
	if ( ioctl ( sockfd,SIOCGIFADDR, ( char * ) &req ) ==-1 )
	{
		fprintf ( stderr,"ioctl SIOCGIFADDR:%s\n\a",strerror ( errno ) );
		close ( sockfd );
		return ( -1 );
	}
	sin = ( struct sockaddr_in * ) &req.ifr_addr;
	memcpy ( ip, ( char * ) &sin->sin_addr,4 );
	
	return ( 0 );
}


/*****************************************
* �������ƣ�CoLoR_Sendpkg
* ��������������mac����װ�����ݰ���Ŀǰ���ڷ���Get���Ĺ��̣�
* �����б�
char * mac
char * broad_mac
char * ip
char * dest
* ���ؽ����
int
*****************************************/

int CoLoR_Sendpkg ( char * mac,char * broad_mac,char * ip,char * dest,uint16_t port )
{
	
	if(port == 1)//ΪHTTP��SIDƥ��������Get��
	{
		int i;
		Ether_CoLoR_get pkg;
		struct hostent *host =NULL;
		struct sockaddr sa;
		int sockfd,len;
		unsigned char temp_ip[5];
		memset ( ( char * ) &pkg,'\0',sizeof ( pkg ) );
		
		//���ethernet����
		memcpy ( ( char * ) pkg.ether_shost, ( char * ) mac,6 );
		memcpy ( ( char * ) pkg.ether_dhost, ( char * ) broad_mac,6 );
		//pkg.ether_type = htons ( ETHERTYPE_ARP );
		pkg.ether_type = htons ( 0x0800 );
		
		//���CoLoR-Get����
		pkg.version_type = 160;//�汾4λ������4λ����Ϊ���ó�CoLoR_Get��
		pkg.ttl = 255;//����ʱ��
		//pkg.data_len = htons(4);
		pkg.total_len = SIDLEN + NIDLEN + PIDN*4 + DATALEN + PUBKEYLEN + 16;//�ܳ���
		
		pkg.port_no = 0;//�˿ں�
		pkg.checksum = 0;//�����
		
		pkg.sid_len = SIDLEN;//SID����
		pkg.nid_len = NIDLEN;//NID����
		pkg.pid_n = PIDN;//PID����
		pkg.options_static = 0;//�̶��ײ�ѡ��
		
		memcpy(pkg.sid, tempSID, SIDLEN);//SID
		char nid[NIDLEN] = {'I',' ','a','m',' ','t','h','e',' ','h','o','s','t','~','~','!'};		
		memcpy(pkg.nid, nid, NIDLEN);//NID
		
		pkg.publickey_len = PUBKEYLEN;
		pkg.mtu = MTU;
		
		char data[DATALEN] = {'I',' ','a','m',' ','t','h','e',' ','d','a','t','a','~','~','~','~','~','~','!'};
		memcpy(pkg.data, data, DATALEN);//Data
		
		char publickey[PUBKEYLEN] = {'I',' ','a','m',' ','t','h','e',' ','p','u','b','k','e','y','!'};
		memcpy(pkg.publickey, publickey, PUBKEYLEN);//��Կ
		
		
		fflush ( stdout );
		memset ( temp_ip,0,sizeof ( temp_ip ) );
		if ( inet_aton ( dest, ( struct in_addr * ) temp_ip ) ==0 )
		{
			if ( ( host = gethostbyname ( dest ) ) ==NULL )
			{
				fprintf ( stderr,"Fail! %s\n\a",hstrerror ( h_errno ) );
				return ( -1 );
			}
			memcpy ( ( char * ) temp_ip,host->h_addr,4 );
		}
		
		//ʵ��Ӧ��ʹ��PF_PACKET
		if ( ( sockfd = socket ( PF_PACKET/*PF_INET*/,SOCK_PACKET,htons ( ETH_P_ALL ) ) ) ==-1 )
		{
			fprintf ( stderr,"Socket Error:%s\n\a",strerror ( errno ) );
			return ( 0 );
		}
		
		memset ( &sa,'\0',sizeof ( sa ) );
		strcpy ( sa.sa_data,Physical_Port );
		
		len = sendto ( sockfd,&pkg,sizeof ( pkg ),0,&sa,sizeof ( sa ) );//����Get����mac��
		
		printf ("   CoLoR-Get  to   Cache Terminal. SID : %s\n",pkg.sid);//���CoLoR-Get��MAC�㷢������ʾ
		
		if ( len != sizeof ( pkg ) )//������ͳ�����ʵ�ʰ���ƥ�䣬����ʧ��
		{
			fprintf ( stderr,"Sendto Error:%s\n\a",strerror ( errno ) );
			close(sockfd);
			return ( 0 );
		}
		
		close(sockfd);
	}
	
	
	if(port == 4141)//Ϊmplayer����Data
	{
		int i;
		//
		//CoLoRЭ��Data���ײ���PID֮ǰ���ֶγ��ȹ̶������ڷ�װ-mplayer����HTTP����ͷר��
		typedef struct _Ether_CoLoR_data_mplayer_head Ether_CoLoR_data_mplayer_head;
		struct _Ether_CoLoR_data_mplayer_head
		{
			//ethernetͷ
			uint8_t ether_dhost[6]; //Ŀ��Ӳ����ַ
			uint8_t ether_shost[6]; //ԴӲ����ַ
			uint16_t ether_type; //��������
			
			//CoLoR-Dataͷ
			uint8_t version_type;////�汾4λ������4λ
			uint8_t ttl;//����ʱ��
			uint16_t total_len;//�ܳ���
			
			uint16_t port_no;//�˿ں�
			uint16_t checksum;//�����
			
			uint8_t sid_len;//SID����
			uint8_t nid_len;//NID����
			uint8_t pid_n;//PID����
			uint8_t options_static;//�̶��ײ�ѡ��
			
			uint8_t signature_algorithm;//ǩ���㷨
			uint8_t if_hash_cache;//�Ƿ��ϣ4λ���Ƿ񻺴�4λ
			uint16_t options_dynamic;//�ɱ��ײ�ѡ��
			
			uint8_t sid[SIDLEN];//SID
			uint8_t nid[NIDLEN];//NID
			
			uint8_t data[110];//Data
			
			uint8_t data_signature[16];//����ǩ��
		};
		//CoLoRЭ��Data���ײ���PID֮ǰ���ֶγ��ȹ̶������ڷ�װ-mplayer������Ƶ��Ƶ����ר��
		typedef struct _Ether_CoLoR_data_mplayer_data Ether_CoLoR_data_mplayer_data;
		struct _Ether_CoLoR_data_mplayer_data
		{
			//ethernetͷ
			uint8_t ether_dhost[6]; //Ŀ��Ӳ����ַ
			uint8_t ether_shost[6]; //ԴӲ����ַ
			uint16_t ether_type; //��������
			
			//CoLoR-Dataͷ
			uint8_t version_type;////�汾4λ������4λ
			uint8_t ttl;//����ʱ��
			uint16_t total_len;//�ܳ���
			
			uint16_t port_no;//�˿ں�
			uint16_t checksum;//�����
			
			uint8_t sid_len;//SID����
			uint8_t nid_len;//NID����
			uint8_t pid_n;//PID����
			uint8_t options_static;//�̶��ײ�ѡ��
			
			uint8_t signature_algorithm;//ǩ���㷨
			uint8_t if_hash_cache;//�Ƿ��ϣ4λ���Ƿ񻺴�4λ
			uint16_t options_dynamic;//�ɱ��ײ�ѡ��
			
			uint8_t sid[SIDLEN];//SID
			uint8_t nid[NIDLEN];//NID
			
			uint8_t data[1000];//Data
			
			uint8_t data_signature[16];//����ǩ��
		};
		//
		Ether_CoLoR_data_mplayer_head pkg_head;
		struct sockaddr sa1;
		int sockfd_head,len_head;
		memset((char*)&pkg_head,'\0',sizeof(pkg_head));
		
		//��ȡ�ļ�
		int fd, datalen, ret;
		char *p, *realPath, data_head[110];
		
		fd = open(DATAFLOWPATH, O_RDONLY);
		datalen = lseek(fd, 0, SEEK_END);
		p = (char *) malloc(datalen + 1);
		bzero(p, datalen + 1);
		lseek(fd, 0, SEEK_SET);
		ret = read(fd, p, datalen);
		
		close(fd);
		sprintf(data_head,
			"HTTP/1.1 200 OK\r\nServer:SONG\r\nConnection: keep-alive\r\nContent-type: application/*\r\nContent-Length:%d\r\n\r\n",
			110);
		
		//����HTTP����ͷ
		//���ethernet����
		memcpy ( ( char * ) pkg_head.ether_shost, ( char * ) mac,6 );
		memcpy ( ( char * ) pkg_head.ether_dhost, ( char * ) broad_mac,6 );
		//pkg_head.ether_type = htons ( ETHERTYPE_ARP );
		pkg_head.ether_type = htons ( 0x0800 );
		
		//���CoLoR-Get����
		pkg_head.version_type = 161;//�汾4λ������4λ����Ϊ���ó�CoLoR_Data��
		pkg_head.ttl = 255;//����ʱ��
		//pkg_head.data_len = htons(4);
		pkg_head.total_len = SIDLEN + NIDLEN + PIDN*4 + 110 + PUBKEYLEN + 16;//�ܳ���
		
		pkg_head.port_no = 4141;//�˿ں�
		pkg_head.checksum = 0;//�����
		
		pkg_head.sid_len = SIDLEN;//SID����
		pkg_head.nid_len = NIDLEN;//NID����
		pkg_head.pid_n = PIDN;//PID����
		pkg_head.options_static = 0;//�̶��ײ�ѡ��
		
		pkg_head.signature_algorithm=1;//ǩ���㷨
		pkg_head.if_hash_cache=0;//�Ƿ��ϣ4λ���Ƿ񻺴�4λ
		pkg_head.options_dynamic=0;//�ɱ��ײ�ѡ��
		
		memcpy(pkg_head.sid, tempSID, SIDLEN);//SID
		char nid[NIDLEN] = {'I',' ','a','m',' ','t','h','e',' ','h','o','s','t','~','~','!'};		
		memcpy(pkg_head.nid, nid, NIDLEN);//NID
		
		
		
		memcpy(pkg_head.data, data_head, 110);//Data
		
		
		//ʵ��Ӧ��ʹ��PF_PACKET
		if ( ( sockfd_head = socket ( PF_PACKET/*PF_INET*/,SOCK_PACKET,htons ( ETH_P_ALL ) ) ) ==-1 )
		{
			fprintf ( stderr,"Socket Error:%s\n\a",strerror ( errno ) );
			return ( 0 );
		}
		
		memset ( &sa1,'\0',sizeof ( sa1 ) );
		strcpy ( sa1.sa_data,Physical_Port );
		
		len_head = sendto ( sockfd_head,&pkg_head,sizeof ( pkg_head ),0,&sa1,sizeof ( sa1 ) );//����Get����mac��
		
		printf(">>>CoLoR-Data Sent\n");//���CoLoR-Get��MAC�㷢������ʾ
		printf ("   |=================HTTPhead=================|\n");
        printf("%s\n",pkg_head.data);
		printf ("   |==========================================|\n");
		
		if ( len_head != sizeof ( pkg_head ) )//������ͳ�����ʵ�ʰ���ƥ�䣬����ʧ��
		{
			fprintf ( stderr,"Sendto Error:%s\n\a",strerror ( errno ) );
			close(sockfd_head);
			return ( 0 );
		}
		
		close(sockfd_head);
		/*
		//����ȡ�ļ����ĵ�ͷ100���ֽ�
		printf("datalen = %d\ndata = \n",datalen);
		int dd=0;
		char c;
		while(dd<100)
		{
		c = *(p+dd);
		printf("%d",c/128);
		c%=128;
		printf("%d",c/64);
		c%=64;
		printf("%d",c/32);
		c%=32;
		printf("%d",c/16);
		c%=16;
		printf("%d",c/8);
		c%=8;
		printf("%d",c/4);
		c%=4;
		printf("%d",c/2);
		c%=2;
		printf("%d",c);
		printf(" ");
		dd++;
		}
		printf("\n");
		*/
		
		//fwrite(p, datalen, 1, client_sock);//��p�е��ַ����Գ���Ϊdatalenд��client_sock
		////////////////////////////////////////////////////////
		Ether_CoLoR_data_mplayer_data pkg_data;
		struct sockaddr sa2;
		int sockfd_data;
		int len_data;
		memset((char*)&pkg_data,'\0',sizeof(pkg_data));
		
		//��ȡ�ļ�
		char data_data[1000];
		
		int point=0;
		int totallen = (datalen/1000+1)*1000;
		
		
		while(point<totallen)
		{//printf("||roundstart:\n");
			char d;
			scanf("%c",&d);
			
			memcpy(data_data,p+point,1000);
			printf(">>>CoLoR-Data Sent\n");//���CoLoR-Get��MAC�㷢������ʾ
			printf("   datalen = %d; totallen = %d; point = %d\n",datalen,totallen,point);
			
			point+=1000;
			
			//����dataflow����
			//���ethernet����
			memcpy ( ( char * ) pkg_data.ether_shost, ( char * ) mac,6 );
			memcpy ( ( char * ) pkg_data.ether_dhost, ( char * ) broad_mac,6 );
			//pkg_data.ether_type = htons ( ETHERTYPE_ARP );
			pkg_data.ether_type = htons ( 0x0800 );
			
			//���CoLoR-Get����
			pkg_data.version_type = 161;//�汾4λ������4λ����Ϊ���ó�CoLoR_Data��
			pkg_data.ttl = 255;//����ʱ��
			//pkg_data.data_len = htons(4);
			pkg_data.total_len = SIDLEN + NIDLEN + PIDN*4 + 1000 + PUBKEYLEN + 16;//�ܳ���
			
			pkg_data.port_no = 4141;//�˿ں�
			pkg_data.checksum = 0;//�����
			
			pkg_data.sid_len = SIDLEN;//SID����
			
			pkg_data.nid_len = NIDLEN;//NID����
			pkg_data.pid_n = PIDN;//PID����
			pkg_data.options_static = 0;//�̶��ײ�ѡ��
			
			pkg_data.signature_algorithm=1;//ǩ���㷨
			pkg_data.if_hash_cache=0;//�Ƿ��ϣ4λ���Ƿ񻺴�4λ
			pkg_data.options_dynamic=0;//�ɱ��ײ�ѡ��
			
			memcpy(pkg_data.sid, tempSID, SIDLEN);//SID
			//char nid[NIDLEN] = {'I',' ','a','m',' ','t','h','e',' ','h','o','s','t','~','~','!'};		
			//memcpy(pkg_data.nid, nid, NIDLEN);//NID
			
			
			
			memcpy(pkg_data.data, data_data, 1000);//Data
			
			
			
			printf ("   |=================HTTPdata=================|\n");
			//
			int dd=0;
			char c;
			while(dd<1000)
			{
				c = *(pkg_data.data+dd);
				printf("%d",c/128);
				c%=128;
				printf("%d",c/64);
				c%=64;
				printf("%d",c/32);
				c%=32;
				printf("%d",c/16);
				c%=16;
				printf("%d",c/8);
				c%=8;
				printf("%d",c/4);
				c%=4;
				printf("%d",c/2);
				c%=2;
				printf("%d",c);
				printf(" ");
				dd++;
			}
			printf ("\n   |==========================================|\n");
			
			//ʵ��Ӧ��ʹ��PF_PACKET
			if ( ( sockfd_data = socket ( PF_PACKET/*PF_INET*/,SOCK_PACKET,htons ( ETH_P_ALL ) ) ) ==-1 )
			{
				fprintf ( stderr,"Socket Error:%s\n\a",strerror ( errno ) );
				return ( 0 );
			}
			
			memset ( &sa2,'\0',sizeof ( sa2 ) );
			strcpy ( sa2.sa_data,Physical_Port );
			
			len_data = sendto ( sockfd_data,&pkg_data,sizeof ( pkg_data ),0,&sa2,sizeof ( sa2 ) );//����Get����mac��
			
			//printf ("   CoLoR-Data to   mplayer.\n");//���CoLoR-Data��MAC�㷢������ʾ
			
			if ( len_data != sizeof ( pkg_data ) )//������ͳ�����ʵ�ʰ���ƥ�䣬����ʧ��
			{
				fprintf ( stderr,"Sendto Error:%s\n\a",strerror ( errno ) );
				close(sockfd_data);
				return ( 0 );
			}printf(">>>flow pkg count = %d\n",++test_count);
			
			close(sockfd_data);
		}
		/////////////////////////////////////////////////////////
        free(p);
		
		
}

return 1;
}


/*******************************************************************************************************************************************
**********************************************************���̼߳�ʱ������ģ��***************************************************************
*******************************************************************************************************************************************/

//�����������
//#define PORTtoHTTP 6001
//#define PORTtoMAC 6002

//��ʱ��
#define SLEEP 1
#define SLEEP_XX 1

int flag_timer_XX = 0;      //XX�߳�ִ�б�ʶ��

time_t GMT;                     //���ʱ�׼ʱ�䣬ʵ����time_t�ṹ(typedef long time_t;)
struct tm *UTC;                 //����ʱ��ʱ�䣬ʵ����tm�ṹָ��(tm��¼����������ʱ�����int�ͱ�ʾ)

//�̺߳���
void *thread_timer(void *fd);
void *thread_XX(void *fd);
void *thread_recvdata(void *fd);
void *thread_sendget(void *fd);

/*****************************************
* �������ƣ�main
* ���������������������������̣߳������ṩʵ�ʹ���
* �����б�
* ���ؽ����
*****************************************/
int main(int argc,char *argv[])
{
	int i,j;
	
	//�������ͼ�ʱ�����߳�
	pthread_t pthread_timer;
	if(pthread_create(&pthread_timer, NULL, thread_timer, NULL)!=0)
	{
		perror("Creation of timer thread failed.");
	}
	
	//����XX���߳�
	pthread_t pthread_XX;
	if(pthread_create(&pthread_XX, NULL, thread_XX, NULL)!=0)
	{
		perror("Creation of XX thread failed.");
	}
	
	//����thread_recvdata���߳�
	pthread_t pthread_recvdata;
	if(pthread_create(&pthread_recvdata, NULL, thread_recvdata, NULL)!=0)
	{
		perror("Creation of recvdata thread failed.");
	}
	
	//����thread_sendget���߳�
	pthread_t pthread_sendget;
	if(pthread_create(&pthread_sendget, NULL, thread_sendget, NULL)!=0)
	{
		perror("Creation of sendget thread failed.");
	}
	/*
	//ά�����̵߳����У�û��ʵ�ʹ���
	while(1)
	{
	}
	*/
	
	struct sockaddr_in addr;
	struct sockaddr_in my_addr;
	my_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	int sock_fd, addrlen;
	
	//��ó������Ĳ������� IP ���˿ڡ�����������ҳ��Ŀ¼��Ŀ¼���λ�õ�
	getoption(argc, argv);
	
	if (!host)
	{
		addrlen = strlen(DEFAULTIP);
		AllocateMemory(&host, addrlen,DEFAULTIP);
	}
	if (!port)
	{
		addrlen = strlen(DEFAULTPORT);
		AllocateMemory(&port, addrlen, DEFAULTPORT);
	}
	if (!back)
	{
		addrlen = strlen(DEFAULTBACK);
		AllocateMemory(&back, addrlen, DEFAULTBACK);
	}
	if (!dirroot)
	{
		addrlen = strlen(DEFAULTDIR);
		AllocateMemory(&dirroot, addrlen, DEFAULTDIR);
	}
	if (!logdir)
	{
		addrlen = strlen(DEFAULTLOG);
		AllocateMemory(&logdir, addrlen, DEFAULTLOG);
	}
	
	
	printf("host=%s \nport=%s \nback=%s \ndirroot=%s \nlogdir=%s \n%sthe background model(Process ID: %d)\n",
		host, port, back, dirroot, logdir, daemon_y_n?"This is ":"This is not ", getpid());
	
	//fork() ���δ��ں�̨����ģʽ��
	if (daemon_y_n)
	{
		if (fork())
			exit(0);
		if (fork())
			exit(0);
		close(0), close(1), close(2);
		logfp = fopen(logdir, "a+");
		if (!logfp)
			exit(0);
	}
	
	//�����ӽ����˳����������ʬ����
	signal(SIGCHLD, SIG_IGN);
	
	//���� socket
	/*
	PF_INET is for BSD
	AF_INET is for POSIX
	This location maybe the very place the bug which occured in Debian but not occured in Ubuntu lies.
	*/
	if ((sock_fd = socket(AF_INET/*ԭΪPF_INET*/, SOCK_STREAM, 0)) < 0)
	{
		if (!daemon_y_n)
		{
			prterrmsg("socket()");
		}
		else
		{
			wrterrmsg("socket()");
		}
	}
	
	//���ö˿ڿ�������
	addrlen = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &addrlen,
		sizeof(addrlen));
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(host);
	addrlen = sizeof(struct sockaddr_in);
	//�󶨵�ַ���˿ڵ���Ϣ
	if (bind(sock_fd, (struct sockaddr *) &addr, addrlen) < 0)
	{
		if (!daemon_y_n)
		{
			prterrmsg("bind()");
		}
		else
		{
			wrterrmsg("bind()");
		}
	}
	
	//��������
	if (listen(sock_fd, atoi(back)) < 0)
	{
		if (!daemon_y_n)
		{
			prterrmsg("listen()");
		}
		else
		{
			wrterrmsg("listen()");
		}
	}
	while (1)
	{
		int len;
		int new_fd;
		addrlen = sizeof(struct sockaddr_in);
		//��������������
		new_fd = accept(sock_fd, (struct sockaddr *) &addr, &addrlen);
		if (new_fd < 0) 
		{
			if (!daemon_y_n) 
			{
				prterrmsg("accept()");
			} 
			else
			{
				wrterrmsg("accept()");
			}
			break;
		}
		bzero(buffer, MAXBUF + 1);
		sprintf(buffer, ">>>Connection from: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
		if (!daemon_y_n) 
		{
			prtinfomsg(buffer);
		} 
		else 
		{
			wrtinfomsg(buffer);
		}
		//����һ���ӽ���ȥ�������󣬵�ǰ���̼����ȴ��µ����ӵ���
		if (!fork()) 
		{
			bzero(buffer, MAXBUF + 1);
			if ((len = recv(new_fd, buffer, MAXBUF, 0)) > 0) 
			{
				FILE *ClientFP = fdopen(new_fd, "w");
				if (ClientFP == NULL) 
				{
					if (!daemon_y_n) 
					{
						prterrmsg("fdopen()");
					} 
					else 
					{
						prterrmsg("fdopen()");
					}
				} 
				else 
				{
					char Req[MAXPATH + 1] = "";
					sscanf(buffer, "GET %s HTTP", Req);
					bzero(buffer, MAXBUF + 1);
					//��ȡSID
					for(i=1,j=0;;i++)
					{
						if(Req[i]!='\0')
						{
							SID[j++]=Req[i];
						}
						else
						{
							SID[j] = '\0';
							break;
						}
					}
					if (strcmp(SID, "favicon.ico") == 0)
					{
						continue;
					}
					sprintf(buffer, "");
					if (!daemon_y_n) 
					{
						prtinfomsg(buffer);
					} 
					else
					{
						wrtinfomsg(buffer);
					}
					//�����û�����
					GiveResponse(ClientFP, Req);
					fclose(ClientFP);
				}
			}
			exit(0);
		}
		close(new_fd);
	}
	close(sock_fd);
	return 0;
	
	exit(0);
	return (EXIT_SUCCESS);
}

/*****************************************
* �������ƣ�thread_timer
* ����������Ϊ���߳��ṩ��ʱ�ź�
* �����б�fd���������׽���
* ���ؽ����void
*****************************************/
void *thread_timer(void *fd)
{
	int timer_XX = 0;
	
	while(1)
	{
		sleep(SLEEP);
		
		time(&GMT);//��ȡGMT����ֵ��GMT
		UTC = localtime(&GMT);//GMT-UTCת��
		//printf("GMT(long)   %ld\n",GMT);//[GMT]���1970.1.1.00:00:00��������
		//printf("UTC(human)  %s",asctime(UTC));//[UTC]����ַ�������ɶ�ʱ��
		
		//Ϊthread_XX��ʱ
		if(++timer_XX == SLEEP_XX)
		{
			flag_timer_XX = 1;
			timer_XX = 0;
		}
	}
	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);
}

/*******************************************************************************************************************************************
*************************************************************���̺߳���*********************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�thread_XX
* ����������
* �����б�fd���������׽���
* ���ؽ����void
*****************************************/
void *thread_XX(void *fd)
{
	static int count_runtime = 0;
	int end = 0;
	while(!end)
	{
		if(flag_timer_XX == 1)
		{/*
		 printf("UTC(human)  %s",asctime(UTC));//[UTC]����ַ�������ɶ�ʱ��
		 count_runtime++;
		 printf("runtime:%d\n",count_runtime);//������̺߳��Ĵ���ִ�д���
			flag_timer_XX = 0;*/
		}
	}
	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);
}


/*****************************************
* �������ƣ�thread_recvdata
* ����������
* �����б�fd���������׽���
* ���ؽ����void
*****************************************/
void *thread_recvdata(void *fd)
{
    int iRet = -1;
    int fd_socket   = -1;
	
    //��ʼ��SOCKET
    fd_socket = Ethernet_InitSocket();
    if(0 > fd)//SOCKET��ʼ��ʧ��
    {
        printf("Error: thread_sendget() - socket initiate failed./n");
		//�ر��߳�
		exit(0);
		free(fd);
		pthread_exit(NULL);
    }
	
    //�������ݰ�
    Ethernet_StartCapture(fd_socket);
	
    //�ر�SOCKET
    close(fd_socket);
	
	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);
}



/*****************************************
* �������ƣ�thread_sendget
* ����������
* �����б�fd���������׽���
* ���ؽ����void
*****************************************/
void *thread_sendget(void *fd)
{
	int sin_len;
	char message[100];
	
	int socket_sidreceiver;
	
	struct sockaddr_in sin;
	bzero(&sin,sizeof(sin));
	sin.sin_family=AF_INET;
	sin.sin_addr.s_addr=htonl(INADDR_ANY);
	sin.sin_port=htons(PORTtoHTTP);
	sin_len=sizeof(sin);
	
	socket_sidreceiver=socket(AF_INET,SOCK_DGRAM,0);
	bind(socket_sidreceiver,(struct sockaddr *)&sin,sizeof(sin));
	
	while(1)
	{
		while(1)
		{
			recvfrom(socket_sidreceiver,message,sizeof(message),0,(struct sockaddr *)&sin,&sin_len);
			
			int i=0;
			int j=0;
			int flag_GET = 0;
			if(strncmp(message,"GET",3) == 0)
			{
				flag_GET = 1;
				i+=2;
				for(j=0;;j++)
				{
					if(message[++i]!='\0')
					{
						tempSID[j] = message[i];
					}
					else
						break;
				}
				tempSID[j]=0;
				
				//��װCoLoR-GetЭ��
				struct timeval tvafter,tvpre;
				struct timezone tz;
				gettimeofday ( &tvpre , &tz );
				
				unsigned char mac[7];
				unsigned char ip[5];
				char dest[16]={0};
				unsigned char broad_mac[7]={0xff,0xff,0xff,0xff,0xff,0xff,0x00};
				
				memset ( mac,0,sizeof ( mac ) );
				memset ( ip,0,sizeof ( ip ) );
				
				if ( GetLocalMac ( Physical_Port,mac,ip ) ==-1 )
				{
					//�ر��߳�
					exit(0);
					free(fd);
					pthread_exit(NULL);
				}
				
				int i=0;
				{
					sprintf ( dest,"255.255.255.255",i );
					CoLoR_Sendpkg ( mac,broad_mac,ip,dest,1 );
				}
				gettimeofday ( &tvafter , &tz );
				
				break;
			}
		}
	}
	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);
}

