/*******************************************************************************************************************************************
* �ļ�����client.c
* �ļ�������973�ǻ�Эͬ����SARϵͳTestBed�׼��������ͨ�ÿͻ��˳����¶�������������ˣ�Data Subscriber��
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
gcc browser.c -o browser -lpthread
4�����У����漰ԭʼ�׽��ֵ�ʹ�ã���rootȨ�ޣ�
sudo ./browser
*/

#include"client.h"

/*******************************************************************************************************************************************
*************************************�궨����������************ȫ�ֱ�������******************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************��ʼ�������������********************************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�Client_Parameterinit
* ����������Clientģ��ȫ�ֱ�������ֵ
* �����б�
* ���ؽ����
*****************************************/
void
Client_Parameterinit()
{
}

/*******************************************************************************************************************************************
*******************************************Ӧ�ò�Client��������ͨ��CLI���ƣ�******************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.���������   To.Ӧ�÷�����*********************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.Ӧ�÷����� To.���������***********************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�Client_StringTransport
* ����������
* �����б�
* ���ؽ����
*****************************************/
void
Client_StringTransport
(
)
{
	int i=0;
	char SID[SIDLEN];
	memset(SID,0,SIDLEN);
	char DATA[DATALEN];
	memset(DATA,0,DATALEN);

	printf("[SID ]  ");
	scanf("%s",SID);
	//scanf("%c",SID);
	//strcpy(SID,"wangzhaoxu");
	//printf("%s\n",SID);

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

	//���ʹ����Get��
	uint8_t sendpkg[sizeof(Trans_get)];
	Trans_get * psendpkg;
	psendpkg = (Trans_get *)sendpkg;

	memset(sendpkg,0,sizeof(Trans_get));
	
	strcpy(psendpkg->sid,SID);
	
	sendto(socket_sidsender,sendpkg,sizeof(Trans_get),0,(struct sockaddr *)&addrTo,sizeof(addrTo));
	close(socket_sidsender);

	gettimeofday(&utime,NULL);
	printf("===============Time===============\nGET  SENT %ld us\n==================================\n",1000000 * utime.tv_sec + utime.tv_usec);
	Startutime.tv_usec = utime.tv_usec;

	if(DEVETESTIMPL <= 1)
		printf("   The Client asked for Data whitch match the SID: %s\n",SID);

	//���մ����Data��
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
		printf("Client responce listening start...\n");
	
	uint8_t recvpkg[sizeof(Trans_data)];
	Trans_data * precvpkg;
	precvpkg = (Trans_data *)recvpkg;

	memset(recvpkg,0,sizeof(Trans_data));

	recvfrom(socket_datareceiver,recvpkg,sizeof(Trans_data),0,(struct sockaddr *)&sin,&sockaddr_len);
	close(socket_datareceiver);

	gettimeofday(&utime,NULL);
	printf("===============Time===============\nDATA RECV %ld us\n==================================\n",1000000 * utime.tv_sec + utime.tv_usec);
	Endutime.tv_usec = utime.tv_usec;
	if(Endutime.tv_usec > Startutime.tv_usec)
		Usedutime.tv_usec = Endutime.tv_usec - Startutime.tv_usec;
	else
		Usedutime.tv_usec =-(Endutime.tv_usec - Startutime.tv_usec);
	printf("Total Time: %ld us\n",Usedutime.tv_usec);
	
	if(DEVETESTIMPL <= 1)
		printf("Client responce listener received a new UDP pkg.\n");
	
	memcpy(DATA,precvpkg->data,DATALEN);
	printf("[DATA]  %s\n",DATA);

	//printf("Data \"%s\" has been received and it will be sent to the HTTP Client immediately./n",str);
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
* �������ƣ�Client_thread_timer
* ����������Ϊ���߳��ṩ��ʱ�ź�
* �����б�
* ���ؽ����
*****************************************/
void *
Client_thread_timer
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
* �������ƣ�Client_thread_NetworkLayertoTransportLayer
* ��������������ͨ�� - From.���������   To.Ӧ�÷�����
* �����б�
* ���ؽ����
*****************************************/
void *
Client_thread_NetworkLayertoTransportLayer
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
* �������ƣ�Client_thread_TransportLayertoNetworkLayer
* ��������������ͨ�� - From.Ӧ�÷����� To.���������
* �����б�
* ���ؽ����
*****************************************/
void *
Client_thread_TransportLayertoNetworkLayer
(
	void * fd
)
{
	sleep(1);
	unsigned long count=1;
	while(1)
	{
		printf("COUNT == %ld\n",count++);
		Client_StringTransport();
	}

	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�Client_main
* ����������Clientģ��������������ForwardingPlaneת��ƽ���̡߳�������ѭ��ִ�м�HTTP���������ܡ�
* �����б�
eg: ./sub d1sub1 d1sub1-eth1
�ڶ�����ʹ��char**argv�ķ�ʽ����������֪�ǲ����﷨����
* ���ؽ����
*****************************************/
int
Client_main
(
	int argc,
	char argv[][30]
)
{
	int i,j;

	//�������ͼ�ʱ�����߳�
	pthread_t pthread_timer;
	if(pthread_create(&pthread_timer, NULL, Client_thread_timer, NULL)!=0)
	{
		perror("Creation of timer thread failed.");
	}
	
	//����thread_NetworkLayertoTransportLayer�������߳�
	pthread_t pthread_NetworkLayertoTransportLayer;
	if(pthread_create(&pthread_NetworkLayertoTransportLayer, NULL, Client_thread_NetworkLayertoTransportLayer, NULL)!=0)
	{
		perror("Creation of NetworkLayertoTransportLayer thread failed.");
	}
	
	//����thread_TransportLayertoNetworkLayer�������߳�
	pthread_t pthread_TransportLayertoNetworkLayer;
	if(pthread_create(&pthread_TransportLayertoNetworkLayer, NULL, Client_thread_TransportLayertoNetworkLayer, NULL)!=0)
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
