/*******************************************************************************************************************************************
* �ļ�����stringserver.c
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
gcc stringserver.c -o stringserver -lpthread
4�����У����漰ԭʼ�׽��ֵ�ʹ�ã���rootȨ�ޣ�
sudo ./stringserver
*/

#include"stringserver.h"

/*******************************************************************************************************************************************
*************************************�궨����������************ȫ�ֱ�������******************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************��ʼ�������������********************************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�StringServer_Parameterinit
* ����������StringServerģ��ȫ�ֱ�������ֵ
* �����б�
* ���ؽ����
*****************************************/
void
StringServer_Parameterinit()
{
}

/*******************************************************************************************************************************************
*******************************************Ӧ�ò�StringServer��������ͨ��CLI���ƣ�******************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.������Ӧ��   To.Ӧ�÷�����*********************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�StringServer_FindData
* ������������ѯSID��Ӧ��DATA
* �����б�
* ���ؽ����
*****************************************/
int
StringServer_FindData
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

	strcpy(tSID,SID);
	
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
			strcpy(tDATA, "xxxx");
			DATAlen = 0;
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
						DATAlen = j-1;
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

				strcpy(tDATA, "xxxx");
				DATAlen = 0;
				break;
			}
		}
		else if(ch=='\n')
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
						DATAlen = j-1;
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
* �������ƣ�StringServer_thread_timer
* ����������Ϊ���߳��ṩ��ʱ�ź�
* �����б�
* ���ؽ����
*****************************************/
void *
StringServer_thread_timer
(
	void * fd
)
{
	int timer_XX = 0;
	
	while(1)
	{
		sleep(10000);
		sleep(SLEEP);
		
		//time(&GMT);//��ȡGMT����ֵ��GMT
		//UTC = localtime(&GMT);//GMT-UTCת��
		//printf("GMT(long)   %ld\n",GMT);//[GMT]���1970.1.1.00:00:00��������
		//printf("UTC(human)  %s",asctime(UTC));//[UTC]����ַ�������ɶ�ʱ��
	}
	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�StringServer_thread_NetworkLayertoTransportLayer
* ��������������ͨ�� - From.������Ӧ��   To.Ӧ�÷�����
* �����б�
* ���ؽ����
*****************************************/
void *
StringServer_thread_NetworkLayertoTransportLayer
(
	void * fd
)
{
	int i,j;

	//���մ��������SID
	int socket_sidreceiver;
	socket_sidreceiver = socket(AF_INET,SOCK_DGRAM,0);

	struct sockaddr_in sin;
	int sockaddr_len=sizeof(struct sockaddr_in);
	bzero(&sin,sockaddr_len);
	sin.sin_family=AF_INET;
	//sin.sin_addr.s_addr=htonl(INADDR_ANY);
	sin.sin_addr.s_addr=inet_addr(LOOPIP);
	sin.sin_port=htons(NETLtoTRAL);

	bind(socket_sidreceiver,(struct sockaddr *)&sin,sockaddr_len);

	uint8_t message[DATALENst+SIDLENst+OFFLEN+20];
	uint8_t SID[SIDLEN];
	uint8_t DATA[DATALEN];
	int SIDlen=0,DATAlen=0;
	while(1)
	{
		i=0;
		j=0;
		SIDlen = 0;
		DATAlen = 0;
		memset(message,0,DATALENst+SIDLENst+OFFLEN+20);
		memset(SID,0,SIDLEN);
		memset(DATA,0,DATALEN);

		recvfrom(socket_sidreceiver,message,sizeof(message),0,(struct sockaddr *)&sin,&sockaddr_len);

		if(strncmp(message,"GET",3) == 0)
		{
			i+=2;
			for(j=0;;j++)
			{
				i++;
				if(!(message[i]=='O' && message[i+1]=='F' && message[i+2]=='F'))
				{
					SID[j] = message[i];
				}
				else
					break;
			}
			SID[j] = 0;
			SIDlen = j;
			//printf("[NetworkLayer to TransportLayer]SID = %s\n",SID);

			//��ѯSID��Ӧ��DATA
			DATAlen = StringServer_FindData(SID,DATA);
			
			//δ�ܲ�ѯ��SID��Ӧ��DATA
			if(DATAlen == 0)
				continue;

			//���²㷵�ز�ѯ����DATA
			//����socket
			//�׽ӿ�������
			int socket_datasender;
			socket_datasender = socket(AF_INET,SOCK_DGRAM,0);

			//��װ������Ϣ
			memset(message,0,DATALENst+SIDLENst+OFFLEN+20);
			memcpy(message,"GOT",3);
			memcpy(message+3,DATA,DATAlen);
			memcpy(message+3+DATAlen,"GET",3);
			memcpy(message+3+DATAlen+3,SID,SIDlen);
			memcpy(message+3+DATAlen+3+SIDlen,"OFF",3);
			memcpy(message+3+DATAlen+3+SIDlen+3,"",OFFLEN);

			struct sockaddr_in addrTo;
			bzero(&addrTo,sizeof(addrTo));
			addrTo.sin_family=AF_INET;
			addrTo.sin_port=htons(TRALtoNETL);
			//unsigned long IPto = 2130706433;//�ػ���ַ���� == 2130706433
			//addrTo.sin_addr.s_addr=htonl(IPto);//htonl�������ֽ���ת��Ϊ�����ֽ���
			addrTo.sin_addr.s_addr=inet_addr(LOOPIP);//htonl�������ֽ���ת��Ϊ�����ֽ���
			//����SID����
			sendto(socket_datasender,message,3+DATAlen+3+SIDlen+3+OFFLEN,0,(struct sockaddr *)&addrTo,sizeof(addrTo));
			close(socket_datasender);
		}
	}
	
	close(socket_sidreceiver);

	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�StringServer_thread_TransportLayertoNetworkLayer
* ��������������ͨ�� - From.Ӧ�÷����� To.������Ӧ��
* �����б�
* ���ؽ����
*****************************************/
void *
StringServer_thread_TransportLayertoNetworkLayer
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
* �������ƣ�StringServer_main
* ����������StringServerģ�������������������̣߳������ṩʵ�ʹ���
* �����б��ڶ�����ʹ��char**argv�ķ�ʽ����������֪�ǲ����﷨����
* ���ؽ����
*****************************************/
//int main(int argc, char *argv[])
int
StringServer_main
(
	int argc,
	char argv[][30]
)
{
	int i,j;
	
	//�������ͼ�ʱ�����߳�
	pthread_t pthread_timer;
	if(pthread_create(&pthread_timer, NULL, StringServer_thread_timer, NULL)!=0)
	{
		perror("Creation of timer thread failed.");
	}

	//����thread_NetworkLayertoTransportLayer�������߳�
	pthread_t pthread_NetworkLayertoTransportLayer;
	if(pthread_create(&pthread_NetworkLayertoTransportLayer, NULL, StringServer_thread_NetworkLayertoTransportLayer, NULL)!=0)
	{
		perror("Creation of NetworkLayertoTransportLayer thread failed.");
	}
	
	//����thread_TransportLayertoNetworkLayer�������߳�
	pthread_t pthread_TransportLayertoNetworkLayer;
	if(pthread_create(&pthread_TransportLayertoNetworkLayer, NULL, StringServer_thread_TransportLayertoNetworkLayer, NULL)!=0)
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
