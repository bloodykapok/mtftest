/*******************************************************************************************************************************************
* �ļ�����server.c
* �ļ�������973�ǻ�Эͬ����SARϵͳTestBed�׼��������ͨ�÷���˳����¶��������ݷ����ˣ�Data Publisher��
* ��������ģ���������������Ϊ������ͨ��ѧ��һ�������������豸���ҹ���ʵ���������������̼�����ѵר�á������������ڴ�֣�����������ļ�������������ѵ��ģ������Ϊ���˾����뾭��Ĳ������Դ�Ĵ�����ʽ��Ϊ���߱��������˽���һ��һ���ڣ��κθ��˲����������ת����չʾ����Դ���ݡ��κ��������ѧԱ����������������ֱ����Ҫ��������������Ҫ����֮Ȩ������������Щ��Դ�����漰ʵ������Ŀ���ܣ��ݲ���������ר���������������Ȩ�����ˣ��ʸ�����Դ����ʵ�����ڲ�һ��һ���ţ��緢���й������߱��˵�һƬ�������Ϊ�����߽���������ԭ���Խ���׷�顢��֤�����ߺ������Ȩ��������Դ�ķ���Ȩ�����߱������У���������ܽ���̽�͸�޳����׵��ȳ���Ϊ���ѧҵ���ౡ֮������ϣ�ԸѧԱ�������ߵ��Ͷ��ɹ���лл������
* ���ߣ�������
* ��ݣ�������ͨ��ѧ��һ�������������豸���ҹ���ʵ���� 2013��˶�������о���
* E-mail. hellozxwang@foxmail.com
* Mobile. 18811774990
* QQ    . 535667240
* Addr  . �����к�������ֱ���ⱱ����ͨ��ѧ��е¥D706��, 100044
*******************************************************************************************************************************************/
/*******************************************************************************************************************************************
*****����˵����1.���������ڷ��Ͱ�������SID��ӦData��DATA���Ͱ�
**************2.�������������ڼ�����SAR/CoLoR�������ݰ���
**************3.�ӽ��յ���GET������ȡSID
**************4.���ز�ѯ�������ݣ��ҵ�SIDƥ���Data����
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
gcc server.c -o server -lpthread
4�����У����漰ԭʼ�׽��ֵ�ʹ�ã���rootȨ�ޣ�
sudo ./server
*/

#include"server.h"

/*******************************************************************************************************************************************
*************************************�궨����������************ȫ�ֱ�������******************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************��ʼ�������������********************************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�Server_Parameterinit
* ����������Serverģ��ȫ�ֱ�������ֵ
* �����б�
* ���ؽ����
*****************************************/
void
Server_Parameterinit()
{
}

/*******************************************************************************************************************************************
*******************************************Ӧ�ò�Server��������ͨ��CLI���ƣ�******************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.������Ӧ��   To.Ӧ�÷�����*********************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�Server_FindData
* ������������ѯSID��Ӧ��DATA
* �����б�
* ���ؽ����
*****************************************/
int
Server_FindData
(
	uint8_t * SID,
	uint8_t * DATA
)
{
	int i,j;
	//��ѯSID�����б�
	//�ļ�������
	FILE *fp;
	
	char ch;
	char buf[SIDLEN/*sid����*/ + DATALEN/*data����*/ +1/*�м�ո�*/];
	int SIDlen=0,DATAlen=0;
	uint8_t tSID[SIDLEN+1];
	uint8_t tDATA[DATALEN+1];
	memset(tSID,0,SIDLEN+1);
	memset(tDATA,0,DATALEN+1);

	strcpy(tSID,SID);//printf("[search]tSID = %s\n",tSID);
	
	int flag_sidfound;
	int file_i;
	
	for(i=0;i<SIDLEN;i++)
	{
		if(tSID[i] == 0)
			break;
	}
	SIDlen = i;
	i=0;
	
	if((fp=fopen(FILEsidname,"r"))==NULL)
	{
		printf("cannot open file!\n");
		exit(0);
	}
	file_i = 0;
	flag_sidfound = 0;
	int runcount = 0;//temp
	while(1)
	{
		if(SIDlen == 0)//����յ���SIDΪ��
		{	
			if(DEVETESTIMPL <= 1)
				printf("   SID %s not found!(SIDlen == 0)\n",tSID);
			memcpy(tDATA, "xxxx", 4);
			break;
		}
		
		ch=fgetc(fp);
		if(ch==EOF)
		{
			buf[file_i] = 0;
			if(strncmp(buf,tSID,SIDlen) == 0)
			{
				if(buf[SIDlen] != ' ')
				{
					file_i = 0;
					buf[0] = 0;
					continue;
				}

				if(DEVETESTIMPL <= 1)
					printf("   SID %s found! ",tSID);

				for(i=SIDlen+1,j=0;;i++)
				{
					if(buf[i] != 0)
					{
						tDATA[j++] = buf[i];
					}
					else
					{
						tDATA[j] = 0;
						DATAlen = j;
						flag_sidfound = 1;
						break;
					}
				}

				if(DEVETESTIMPL <= 1)
					printf("Data is: %s\n",tDATA);

				break;
			}
			else
			{
				if(DEVETESTIMPL <= 1)
					printf("   SID %s not found!(No SID in the list)\n",tSID);

				memcpy(tDATA, "xxxx", 4);
				DATAlen = 4;
				break;
			}
		}
		else if(ch=='\n')
		{
			buf[file_i] = 0;//printf("[search]buf = %s\n",buf);
			if(strncmp(buf,tSID,SIDlen) == 0)
			{
				if(buf[SIDlen] != ' ')
				{
					file_i = 0;
					buf[0] = 0;
					continue;
				}
				
				if(DEVETESTIMPL <= 1)
					printf("   SID %s found! ",tSID);

				for(i=SIDlen+1,j=0;;i++)
				{
					if(buf[i] != 0)
					{
						tDATA[j++] = buf[i];
					}
					else
					{
						tDATA[j] = 0;
						DATAlen = j;
						flag_sidfound = 1;
						break;
					}
				}

				if(DEVETESTIMPL <= 1)
					printf("Data is: %s\n",tDATA);

				break;
			}
			else
			{
				file_i = 0;
				buf[0] = 0;
				continue;
			}
		}
		else
		{
			buf[file_i++]=ch;
		}
		
		if(flag_sidfound == 1)
			break;
	}

	strcpy(DATA,tDATA);

	fclose(fp);

	return DATAlen;
}

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.Ӧ�÷����� To.������Ӧ��***********************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
**********************************************************���߳����ɳ���********************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�Server_thread_NetworkLayertoTransportLayer
* ��������������ͨ�� - From.������Ӧ��   To.Ӧ�÷�����
* �����б�
* ���ؽ����
*****************************************/
void *
Server_thread_NetworkLayertoTransportLayer
(
	void * fd
)
{
	int i,j;

	//����socket�����մ��������SID
	int socket_sidreceiver;
	socket_sidreceiver = socket(AF_INET,SOCK_DGRAM,0);

	struct sockaddr_in sin;
	int sockaddr_len=sizeof(struct sockaddr_in);
	bzero(&sin,sockaddr_len);
	sin.sin_family=AF_INET;
	//sin.sin_addr.s_addr=htonl(INADDR_ANY);//htonl�������ֽ���ת��Ϊ�����ֽ���
	sin.sin_addr.s_addr=inet_addr(LOOPIP);
	sin.sin_port=htons(NETLtoTRAL);

	bind(socket_sidreceiver,(struct sockaddr *)&sin,sockaddr_len);

	//����socket������㷢��DATA
	int socket_datasender;
	socket_datasender = socket(AF_INET,SOCK_DGRAM,0);

	struct sockaddr_in addrTo;
	bzero(&addrTo,sizeof(addrTo));
	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons(TRALtoNETL);
	addrTo.sin_addr.s_addr=inet_addr(LOOPIP);

	uint8_t SID[SIDLEN];
	uint8_t DATA[DATALEN];
	int SIDlen,DATAlen;

	uint8_t recvpkg[sizeof(Trans_get)];
	Trans_get * precvpkg;
	precvpkg = (Trans_get *)recvpkg;
	uint8_t sendpkg[sizeof(CoLoR_data)];
	Trans_data * psendpkg;
	psendpkg = (Trans_data *)sendpkg;

	while(1)
	{
		i=0;
		j=0;
		SIDlen = 0;
		DATAlen = 0;

		memset(SID,0,SIDLEN);
		memset(DATA,0,DATALEN);
		memset(recvpkg,0,sizeof(Trans_get));
		memset(sendpkg,0,sizeof(CoLoR_data));
		
		//���մ�����
		recvfrom(socket_sidreceiver,recvpkg,sizeof(Trans_get),0,(struct sockaddr *)&sin,&sockaddr_len);
		
	gettimeofday(&utime,NULL);
	printf("===============Time===============\nGET  RECV %ld us\n==================================\n",1000000 * utime.tv_sec + utime.tv_usec);

		//��ȡSID
		memcpy(SID,precvpkg->sid,SIDLEN);

		//��ѯSID��Ӧ��DATA
		printf("[SID ]  %s\n",SID);
		DATAlen = Server_FindData(SID,DATA);
		printf("[DATA]  %s\n",DATA);
		
		//��װDATA
		memcpy(psendpkg->sid,SID,SIDLEN);
		memcpy(psendpkg->data,DATA,DATALEN);

		//���ʹ�����
		sendto(socket_datasender,sendpkg,sizeof(Trans_data),0,(struct sockaddr *)&addrTo,sizeof(addrTo));

	gettimeofday(&utime,NULL);
	printf("===============Time===============\nDATA SENT %ld us\n==================================\n",1000000 * utime.tv_sec + utime.tv_usec);
	}
	close(socket_sidreceiver);
	close(socket_datasender);

	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�Server_thread_TransportLayertoNetworkLayer
* ��������������ͨ�� - From.Ӧ�÷����� To.������Ӧ��
* �����б�
* ���ؽ����
*****************************************/
void *
Server_thread_TransportLayertoNetworkLayer
(
	void * fd
)
{
	while(1)
	{
		sleep(10000);
	}

	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�Server_main
* ����������Serverģ�������������������̣߳������ṩʵ�ʹ���
* �����б��ڶ�����ʹ��char**argv�ķ�ʽ����������֪�ǲ����﷨����
* ���ؽ����
*****************************************/
//int main(int argc, char *argv[])
int
Server_main
(
	int argc,
	char argv[][30]
)
{
	int i,j;
	
	//����thread_NetworkLayertoTransportLayer�������߳�
	pthread_t pthread_NetworkLayertoTransportLayer;
	if(pthread_create(&pthread_NetworkLayertoTransportLayer, NULL, Server_thread_NetworkLayertoTransportLayer, NULL)!=0)
	{
		perror("Creation of NetworkLayertoTransportLayer thread failed.");
	}
	
	//����thread_TransportLayertoNetworkLayer�������߳�
	pthread_t pthread_TransportLayertoNetworkLayer;
	if(pthread_create(&pthread_TransportLayertoNetworkLayer, NULL, Server_thread_TransportLayertoNetworkLayer, NULL)!=0)
	{
		perror("Creation of TransportLayertoNetworkLayer thread failed.");
	}

	//����ΪPublisherģ�������ͨ�Ų�����������
	/*
	struct publisherinput
	{
		int argc;
		char argv[50][30];
	};
	*/
	struct publisherinput pubinput;
	pubinput.argc=0;
	memset(pubinput.argv,0,1500);

	pubinput.argc=argc;
	for(i=0;i<pubinput.argc;i++)
	{
		strcpy(pubinput.argv[i],argv[i]);
	}

	pthread_t pthread_publisher;

	//����Publisher���߳�
	if(pthread_create(&pthread_publisher, NULL, Publisher_main, (void *)&pubinput)!=0)
	{
		perror("Creation of publisher thread failed.");
	}

	//Publisher_main(argc,(char (*)[30])argv);

	//��ѭ�������ͻ��˴���
	while (1)
	{
		sleep(10000);
	}
}
