/*******************************************************************************************************************************************
* �ļ�����stringclient.c
* �ļ�������973�ǻ�Эͬ����SARϵͳTestBed�׼��������ַ�������ˣ��¶�������������ˣ�Data Subscriber��
* ��������ģ���������������Ϊ������ͨ��ѧ��һ�������������豸���ҹ���ʵ���������������̼�����ѵר�á������������ڴ�֣�����������ļ�������������ѵ��ģ������Ϊ���˾����뾭��Ĳ������Դ�Ĵ�����ʽ��Ϊ���߱��������˽���һ��һ���ڣ��κθ��˲����������ת����չʾ����Դ���ݡ��κ��������ѧԱ����������������ֱ����Ҫ��������������Ҫ����֮Ȩ������������Щ��Դ�����漰ʵ������Ŀ���ܣ��ݲ���������ר���������������Ȩ�����ˣ��ʸ�����Դ����ʵ�����ڲ�һ��һ���ţ��緢���й������߱��˵�һƬ�������Ϊ�����߽���������ԭ���Խ���׷�顢��֤�����ߺ������Ȩ��������Դ�ķ���Ȩ�����߱������У���������ܽ���̽�͸�޳����׵��ȳ���Ϊ���ѧҵ���ౡ֮������ϣ�ԸѧԱ�������ߵ��Ͷ��ɹ���лл������
* ���ߣ�������
* ��ݣ�������ͨ��ѧ��һ�������������豸���ҹ���ʵ���� 2013��˶�������о���
* E-mail. hellozxwang@foxmail.com
* Mobile. 18811774990
* QQ    . 535667240
* Addr  . �����к�������ֱ���ⱱ����ͨ��ѧ��е¥D706��, 100044
*******************************************************************************************************************************************/
/*******************************************************************************************************************************************
*****����˵����1.CLI��ʽ�༭SID������
**************2.����SID��Ӧ��DATA����ʾ
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
gcc stringclient.c -o stringclient -lpthread
4�����У����漰ԭʼ�׽��ֵ�ʹ�ã���rootȨ�ޣ�
sudo ./stringclient
*/

#include"stringclient.h"

/*******************************************************************************************************************************************
*************************************�궨����������************ȫ�ֱ�������******************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************��ʼ�������������********************************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�StringClient_Parameterinit
* ����������StringClientģ��ȫ�ֱ�������ֵ
* �����б�
* ���ؽ����
*****************************************/
void
StringClient_Parameterinit()
{
}

/*******************************************************************************************************************************************
*******************************************Ӧ�ò�StringClient��������ͨ��CLI���ƣ�******************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.���������   To.Ӧ�÷�����*********************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.Ӧ�÷����� To.���������***********************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�StringClient_StringTransport
* ����������
* �����б�
* ���ؽ����
*****************************************/
void
StringClient_StringTransport
(
	struct timeval * tv,
	fd_set * readfds
)
{
	int i=0,j=0;
	
	uint8_t message[DATALENst+SIDLENst+OFFLEN+20];
	memset(message,0,DATALENst+SIDLENst+OFFLEN+20);

	uint8_t SID[SIDLEN];
	memset(SID,0,SIDLEN);
	int SIDlen=0;
	
	uint8_t DATA[DATALEN];
	memset(DATA,0,DATALEN);

	uint8_t REQoffset[OFFLEN];
	memset(REQoffset,0,OFFLEN);

	uint8_t RESoffset[OFFLEN];
	memset(RESoffset,0,OFFLEN);

	static int num=-1;
	static int TimeoutNum=-1;
	static int LargestDelay=0;
	num++;

	if(transDEVETESTIMPL <= 0)
	{
		printf("Pkg     Count == %d\n",num);
		printf("Timeout Count == %d\n",TimeoutNum);
		printf("Timeout Limit == %d ms\n",RTOUS/1000);
		printf("LargestDelay  == %.3f ms\n",(double)LargestDelay/(double)1000);
		static double PkgLossRate=0;
		PkgLossRate = 100 * (double)TimeoutNum / (double)num;
		printf("PkgLossRate   == %.3f %%\n",PkgLossRate);
	}

	//usleep(100000);

	//printf("\n[SID  ]  ");
	//scanf("%s",SID);

	//getchar();

	//if(num%500 == 0)
		//RTOUS = 20000;

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
	socket_sidsender = socket(AF_INET,SOCK_DGRAM,0);
		
	struct sockaddr_in addrTo;
	bzero(&addrTo,sizeof(addrTo));
	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons(APPLtoTRAL);
	//unsigned long IPto = 2130706433;//�ػ���ַ���� == 2130706433
	//addrTo.sin_addr.s_addr=htonl(IPto);//htonl�������ֽ���ת��Ϊ�����ֽ���
	addrTo.sin_addr.s_addr=inet_addr(LOOPIP);//htonl�������ֽ���ת��Ϊ�����ֽ���

	//��ֵSID 
	strcpy(SID,"dropping");
	if(transDEVETESTIMPL <= 0)
		printf("\n[SID  ]  %s\n",SID);

	//ȷ��SID��ʵ��ĩβ��ΪSIDlen���¸�ֵ��Ĭ��SID�м䲻�����ASCII��Ϊ0���ֽڣ�
	int pp=0;
	while(1)
	{
		if(SID[pp] == 0)
		{
			SIDlen = pp;
			break;
		}
		pp++;
	}

	//��װ������Ϣ
	memset(message,0,DATALENst+SIDLENst+OFFLEN+20);
	memcpy(message,"GET",3);
	memcpy(message+3,SID,SIDlen);
	memcpy(message+3+SIDlen,"OFF",3);
	memcpy(message+3+SIDlen+3,REQoffset,OFFLEN);
	
	//����SID����
	sendto(socket_sidsender,message,3+SIDlen+3+OFFLEN,0,(struct sockaddr *)&addrTo,sizeof(addrTo));
	close(socket_sidsender);

	if(DEVETESTIMPL <= 1)
		printf("[Transport Layer] The StringClient asked for Data whitch match the SID: %s\n",SID);
	
	//��ѭ������������Դ�ɹ�������
	int socket_datareceiver;
	socket_datareceiver = socket(AF_INET,SOCK_DGRAM,0);

	struct sockaddr_in sin;
	int sockaddr_len=sizeof(struct sockaddr_in);
	bzero(&sin,sockaddr_len);
	sin.sin_family=AF_INET;
	//sin.sin_addr.s_addr=htonl(INADDR_ANY);
	sin.sin_addr.s_addr=inet_addr(LOOPIP);
	sin.sin_port=htons(TRALtoAPPL);

	bind(socket_datareceiver,(struct sockaddr *)&sin,sockaddr_len);
		
	int flag_sidgot = 0;
	int counter_sidget = 0;

	if(DEVETESTIMPL <= 1)
		printf("[Transport Layer] StringClient responce listening start...\n");
	
	//��ջ���message��Ϊ����������׼��
	memset(message,0,DATALENst+SIDLENst+OFFLEN+20);

	//����ʱ��UDP��Ϣ����
	//���ó�ʱ
	//struct timeval tv;//�Ѹ��ú��������ⲿ����ָ��
	tv->tv_sec=RTOUS/1000000;
	tv->tv_usec=RTOUS%1000000;

	//��¼sockfd
	//fd_set readfds;//�Ѹ��ú��������ⲿ����ָ��
	FD_ZERO(readfds);
	FD_SET(socket_datareceiver,readfds);

	select(socket_datareceiver+1,readfds,NULL,NULL,tv);
	
	//��ʱ����
	int n;
	n=0;
	if(FD_ISSET(socket_datareceiver,readfds))
	{
		if((n=recvfrom(socket_datareceiver,message,sizeof(message),0,(struct sockaddr *)&sin,&sockaddr_len))>=0)
		{
			//if(transDEVETESTIMPL <= 0)
				printf("[Delay]  %.3f ms\n",(double)(RTOUS - (int)tv->tv_usec)/(double)1000);
			if(LargestDelay<RTOUS - (int)tv->tv_usec)
				LargestDelay = RTOUS - (int)tv->tv_usec;
		}
	}
	else
	{
		//if(transDEVETESTIMPL <= 0)
			printf("[Delay]  %.3f ms   (Timeout)\n",(double)(RTOUS - (int)tv->tv_usec)/(double)1000);
		TimeoutNum++;
		//RTOUS = RTOUS;
	}

	//recvfrom(socket_datareceiver,message,sizeof(message),0,(struct sockaddr *)&sin,&sockaddr_len);
	close(socket_datareceiver);

	if(DEVETESTIMPL <= 1)
		printf("[Transport Layer] StringClient responce listener received a new UDP message.\n");
	//���ܵ�����ϢΪ ��GOT��
	if(strncmp(message,"GOT",3) == 0)
	{
		i+=2;
		for(j=0;;j++)
		{
			i++;
			if(!(message[i]=='O' && message[i+1]=='F' && message[i+2]=='F'))
			{
				DATA[j] = message[i];
			}
			else
				break;
		}
		DATA[j] = 0;
		//printf("DATA = %s\n",DATA);

		i+=2;
		for(j=0;j<OFFLEN;j++)
		{
			i++;
			RESoffset[j] = message[i];
		}
		RESoffset[j] = 0;
	}
	
	if(transDEVETESTIMPL <= 0)
		printf("[DATA ]  %s\n",DATA);

	/*
	sleep(1);
	counter_sidget++;
	if(counter_sidget >= MAX_GETSIDTIME)//�����ȡSID��Ӧ��Դ�����󳬹�MAX_GETSIDTIME��Ԥ��ʱ��û�лظ�������Ϊ����ʧ��
	{
		break;
	}
	*/
		
	/*
	����ӿڣ�ֱ�ӽ�ȡ�ص��ļ�����DEFAULTDIR�������뿪ͷ�궨�壩��ָ��ĸ�Ŀ¼����������Ĵ����ȡ�ļ�
	������Դ��CoLoRЭ��ȡ�ص��ļ�
	����չ�ԣ������ļ���ȡ�����صĻ���ʵ��������˻���Ļ������ܣ�δ���������ڳ�������ӻ��������б���ϻ�����
	*/
}

/*******************************************************************************************************************************************
**********************************************************���߳����ɳ���********************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�StringClient_thread_timer
* ����������Ϊ���߳��ṩ��ʱ�ź�
* �����б�
* ���ؽ����
*****************************************/
void *
StringClient_thread_timer
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
* �������ƣ�StringClient_thread_NetworkLayertoTransportLayer
* ��������������ͨ�� - From.���������   To.Ӧ�÷�����
* �����б�
* ���ؽ����
*****************************************/
void *
StringClient_thread_NetworkLayertoTransportLayer
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
* �������ƣ�StringClient_thread_TransportLayertoNetworkLayer
* ��������������ͨ�� - From.Ӧ�÷����� To.���������
* �����б�
* ���ؽ����
*****************************************/
void *
StringClient_thread_TransportLayertoNetworkLayer
(
	void * fd
)
{
	int PacketFlowCount = 0;
	
	struct timeval tv;
	fd_set readfds;
	while(1)
	{
		//system("clear");
		StringClient_StringTransport(&tv,&readfds);
		PacketFlowCount++;
		if(PacketFlowCount >= 10)
		{
			PacketFlowCount = 0;
			//sleep(10);
		}
	}

	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�StringClient_main
* ����������StringClientģ��������������ForwardingPlaneת��ƽ���̡߳�������ѭ��ִ�м�HTTP���������ܡ�
* �����б�
eg: ./sub d1sub1 d1sub1-eth1
�ڶ�����ʹ��char**argv�ķ�ʽ����������֪�ǲ����﷨����
* ���ؽ����
*****************************************/
int
StringClient_main
(
	int argc,
	char argv[][30]
)
{
	int i,j;

	//�������ͼ�ʱ�����߳�
	pthread_t pthread_timer;
	if(pthread_create(&pthread_timer, NULL, StringClient_thread_timer, NULL)!=0)
	{
		perror("Creation of timer thread failed.");
	}
	
	//����thread_NetworkLayertoTransportLayer�������߳�
	pthread_t pthread_NetworkLayertoTransportLayer;
	if(pthread_create(&pthread_NetworkLayertoTransportLayer, NULL, StringClient_thread_NetworkLayertoTransportLayer, NULL)!=0)
	{
		perror("Creation of NetworkLayertoTransportLayer thread failed.");
	}
	
	//����thread_TransportLayertoNetworkLayer�������߳�
	pthread_t pthread_TransportLayertoNetworkLayer;
	if(pthread_create(&pthread_TransportLayertoNetworkLayer, NULL, StringClient_thread_TransportLayertoNetworkLayer, NULL)!=0)
	{
		perror("Creation of TransportLayertoNetworkLayer thread failed.");
	}

	//����ΪSubscriberģ�������ͨ�Ų�����������
	/*
	struct subscriberinput
	{
		int argc;
		char argv[50][30];
	};
	*/
	struct subscriberinput subinput;
	subinput.argc=0;
	memset(subinput.argv,0,1500);

	subinput.argc=argc;
	for(i=0;i<subinput.argc;i++)
	{
		strcpy(subinput.argv[i],argv[i]);
	}

	pthread_t pthread_subscriber;

	//����Subscriber���߳�
	if(pthread_create(&pthread_subscriber, NULL, Subscriber_main, (void *)&subinput)!=0)
	{
		perror("Creation of subscriber thread failed.");
	}

	//Subscriber_main(argc,(char (*)[30])argv);

	//��ѭ�������ͻ��˴���
	while (1)
	{
		sleep(10000);
	}
}
