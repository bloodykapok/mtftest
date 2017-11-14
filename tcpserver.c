/*******************************************************************************************************************************************
* �ļ�����tcpserver.c
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
gcc tcpserver.c -o tcpserver -lpthread
4�����У����漰ԭʼ�׽��ֵ�ʹ�ã���rootȨ�ޣ�
sudo ./tcpserver
*/

#include"tcpserver.h"

/*******************************************************************************************************************************************
*************************************�궨����������************ȫ�ֱ�������******************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************��ʼ�������������********************************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�TCPServer_Parameterinit
* ����������TCPServerģ��ȫ�ֱ�������ֵ
* �����б�
* ���ؽ����
*****************************************/
void
TCPServer_Parameterinit()
{
	SockAddrLength = sizeof(struct sockaddr_in);

	TCPSerParaList.LostPacketCount=0;
	TCPSerParaList.InputCount=0;
	TCPSerParaList.TotalWindowLength=0;



	//���ڴ��俪ʼ
	TCPSerParaList.CountTCPServerWindowBegin_Triggering = 1;
	TCPSerParaList.CountTCPServerWindowBegin_Waiting = 1;
	TCPSerParaList.CountTCPServerWindowBegin_Triggered = 1;
/*
	printf("[%d] WindowBegin triggering...\n",TCPSerParaList.CountTCPServerWindowBegin_Triggering);
	TCPSerParaList.CountTCPServerWindowBegin_Triggering++;
	printf("[%d] WindowBegin waiting...\n",TCPSerParaList.CountTCPServerWindowBegin_Waiting);
	TCPSerParaList.CountTCPServerWindowBegin_Waiting++;
	printf("[%d] WindowBegin triggered!\n",TCPSerParaList.CountTCPServerWindowBegin_Triggered);
	TCPSerParaList.CountTCPServerWindowBegin_Triggered++;
*/
	bzero(&TCPSerParaList.SENDAddrToTCPServerWindowBegin,SockAddrLength);
	TCPSerParaList.SENDAddrToTCPServerWindowBegin.sin_family=AF_INET;
	TCPSerParaList.SENDAddrToTCPServerWindowBegin.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPSerParaList.SENDAddrToTCPServerWindowBegin.sin_port=htons(TCPServerWindowBegin);

	bzero(&TCPSerParaList.RECVAddrToTCPServerWindowBegin,SockAddrLength);
	TCPSerParaList.RECVAddrToTCPServerWindowBegin.sin_family=AF_INET;
	TCPSerParaList.RECVAddrToTCPServerWindowBegin.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPSerParaList.RECVAddrToTCPServerWindowBegin.sin_port=htons(TCPServerWindowBegin);

	TCPSerParaList.SENDTCPServerWindowBegin = socket(AF_INET,SOCK_DGRAM,0);
	TCPSerParaList.RECVTCPServerWindowBegin = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPSerParaList.RECVTCPServerWindowBegin,(struct sockaddr *)&TCPSerParaList.RECVAddrToTCPServerWindowBegin,SockAddrLength);

	//���ڴ�������ͨ��������ʱ
	TCPSerParaList.CountTCPServerWindowTimingEnd_Triggering = 1;
	TCPSerParaList.CountTCPServerWindowTimingEnd_Waiting = 1;
	TCPSerParaList.CountTCPServerWindowTimingEnd_Triggered = 1;
/*
	printf("[%d] WindowTimingEnd triggering...\n",TCPSerParaList.CountTCPServerWindowTimingEnd_Triggering);
	TCPSerParaList.CountTCPServerWindowTimingEnd_Triggering++;
	printf("[%d] WindowTimingEnd waiting...\n",TCPSerParaList.CountTCPServerWindowTimingEnd_Waiting);
	TCPSerParaList.CountTCPServerWindowTimingEnd_Waiting++;
	printf("[%d] WindowTimingEnd triggered!\n",TCPSerParaList.CountTCPServerWindowTimingEnd_Triggered);
	TCPSerParaList.CountTCPServerWindowTimingEnd_Triggered++;
*/
	bzero(&TCPSerParaList.SENDAddrToTCPServerWindowTimingEnd,SockAddrLength);
	TCPSerParaList.SENDAddrToTCPServerWindowTimingEnd.sin_family=AF_INET;
	TCPSerParaList.SENDAddrToTCPServerWindowTimingEnd.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPSerParaList.SENDAddrToTCPServerWindowTimingEnd.sin_port=htons(TCPServerWindowTimingEnd);

	bzero(&TCPSerParaList.RECVAddrToTCPServerWindowTimingEnd,SockAddrLength);
	TCPSerParaList.RECVAddrToTCPServerWindowTimingEnd.sin_family=AF_INET;
	TCPSerParaList.RECVAddrToTCPServerWindowTimingEnd.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPSerParaList.RECVAddrToTCPServerWindowTimingEnd.sin_port=htons(TCPServerWindowTimingEnd);

	TCPSerParaList.SENDTCPServerWindowTimingEnd = socket(AF_INET,SOCK_DGRAM,0);
	TCPSerParaList.RECVTCPServerWindowTimingEnd = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPSerParaList.RECVTCPServerWindowTimingEnd,(struct sockaddr *)&TCPSerParaList.RECVAddrToTCPServerWindowTimingEnd,SockAddrLength);

	//���ڴ�������ͨ���������
	TCPSerParaList.CountTCPServerWindowRespond_Triggering = 1;
	TCPSerParaList.CountTCPServerWindowRespond_Waiting = 1;
	TCPSerParaList.CountTCPServerWindowRespond_Triggered = 1;
/*
	printf("[%d] WindowRespond triggering...\n",TCPSerParaList.CountTCPServerWindowRespond_Triggering);
	TCPSerParaList.CountTCPServerWindowRespond_Triggering++;
	printf("[%d] WindowRespond waiting...\n",TCPSerParaList.CountTCPServerWindowRespond_Waiting);
	TCPSerParaList.CountTCPServerWindowRespond_Waiting++;
	printf("[%d] WindowRespond triggered!\n",TCPSerParaList.CountTCPServerWindowRespond_Triggered);
	TCPSerParaList.CountTCPServerWindowRespond_Triggered++;
*/
	bzero(&TCPSerParaList.SENDAddrToTCPServerWindowRespond,SockAddrLength);
	TCPSerParaList.SENDAddrToTCPServerWindowRespond.sin_family=AF_INET;
	TCPSerParaList.SENDAddrToTCPServerWindowRespond.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPSerParaList.SENDAddrToTCPServerWindowRespond.sin_port=htons(TCPServerWindowRespond);

	bzero(&TCPSerParaList.RECVAddrToTCPServerWindowRespond,SockAddrLength);
	TCPSerParaList.RECVAddrToTCPServerWindowRespond.sin_family=AF_INET;
	TCPSerParaList.RECVAddrToTCPServerWindowRespond.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPSerParaList.RECVAddrToTCPServerWindowRespond.sin_port=htons(TCPServerWindowRespond);

	TCPSerParaList.SENDTCPServerWindowRespond = socket(AF_INET,SOCK_DGRAM,0);
	TCPSerParaList.RECVTCPServerWindowRespond = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPSerParaList.RECVTCPServerWindowRespond,(struct sockaddr *)&TCPSerParaList.RECVAddrToTCPServerWindowRespond,SockAddrLength);

	//���ͻ�������
	TCPSerParaList.CountTCPServerBufReady_Triggering = 1;
	TCPSerParaList.CountTCPServerBufReady_Waiting = 1;
	TCPSerParaList.CountTCPServerBufReady_Triggered = 1;
/*
	printf("[%d] BufReady triggering...\n",TCPSerParaList.CountTCPServerBufReady_Triggering);
	TCPSerParaList.CountTCPServerBufReady_Triggering++;
	printf("[%d] BufReady waiting...\n",TCPSerParaList.CountTCPServerBufReady_Waiting);
	TCPSerParaList.CountTCPServerBufReady_Waiting++;
	printf("[%d] BufReady triggered!\n",TCPSerParaList.CountTCPServerBufReady_Triggered);
	TCPSerParaList.CountTCPServerBufReady_Triggered++;
*/
	bzero(&TCPSerParaList.SENDAddrToTCPServerBufReady,SockAddrLength);
	TCPSerParaList.SENDAddrToTCPServerBufReady.sin_family=AF_INET;
	TCPSerParaList.SENDAddrToTCPServerBufReady.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPSerParaList.SENDAddrToTCPServerBufReady.sin_port=htons(TCPServerBufReady);

	bzero(&TCPSerParaList.RECVAddrToTCPServerBufReady,SockAddrLength);
	TCPSerParaList.RECVAddrToTCPServerBufReady.sin_family=AF_INET;
	TCPSerParaList.RECVAddrToTCPServerBufReady.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPSerParaList.RECVAddrToTCPServerBufReady.sin_port=htons(TCPServerBufReady);

	TCPSerParaList.SENDTCPServerBufReady = socket(AF_INET,SOCK_DGRAM,0);
	TCPSerParaList.RECVTCPServerBufReady = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPSerParaList.RECVTCPServerBufReady,(struct sockaddr *)&TCPSerParaList.RECVAddrToTCPServerBufReady,SockAddrLength);

	//��Ӧ��������
	TCPSerParaList.CountTCPServerMoreData_Triggering = 1;
	TCPSerParaList.CountTCPServerMoreData_Waiting = 1;
	TCPSerParaList.CountTCPServerMoreData_Triggered = 1;
/*
	printf("[%d] MoreData triggering...\n",TCPSerParaList.CountTCPServerMoreData_Triggering);
	TCPSerParaList.CountTCPServerMoreData_Triggering++;
	printf("[%d] MoreData waiting...\n",TCPSerParaList.CountTCPServerMoreData_Waiting);
	TCPSerParaList.CountTCPServerMoreData_Waiting++;
	printf("[%d] MoreData triggered!\n",TCPSerParaList.CountTCPServerMoreData_Triggered);
	TCPSerParaList.CountTCPServerMoreData_Triggered++;
*/
	bzero(&TCPSerParaList.SENDAddrToTCPServerMoreData,SockAddrLength);
	TCPSerParaList.SENDAddrToTCPServerMoreData.sin_family=AF_INET;
	TCPSerParaList.SENDAddrToTCPServerMoreData.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPSerParaList.SENDAddrToTCPServerMoreData.sin_port=htons(TCPServerMoreData);

	bzero(&TCPSerParaList.RECVAddrToTCPServerMoreData,SockAddrLength);
	TCPSerParaList.RECVAddrToTCPServerMoreData.sin_family=AF_INET;
	TCPSerParaList.RECVAddrToTCPServerMoreData.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPSerParaList.RECVAddrToTCPServerMoreData.sin_port=htons(TCPServerMoreData);

	TCPSerParaList.SENDTCPServerMoreData = socket(AF_INET,SOCK_DGRAM,0);
	TCPSerParaList.RECVTCPServerMoreData = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPSerParaList.RECVTCPServerMoreData,(struct sockaddr *)&TCPSerParaList.RECVAddrToTCPServerMoreData,SockAddrLength);

	//�ļ���ȡ����
	TCPSerParaList.CountTCPServerFileClose_Triggering = 1;
	TCPSerParaList.CountTCPServerFileClose_Waiting = 1;
	TCPSerParaList.CountTCPServerFileClose_Triggered = 1;
/*
	printf("[%d] FileClose triggering...\n",TCPSerParaList.CountTCPServerFileClose_Triggering);
	TCPSerParaList.CountTCPServerFileClose_Triggering++;
	printf("[%d] FileClose waiting...\n",TCPSerParaList.CountTCPServerFileClose_Waiting);
	TCPSerParaList.CountTCPServerFileClose_Waiting++;
	printf("[%d] FileClose triggered!\n",TCPSerParaList.CountTCPServerFileClose_Triggered);
	TCPSerParaList.CountTCPServerFileClose_Triggered++;
*/
	bzero(&TCPSerParaList.SENDAddrToTCPServerFileClose,SockAddrLength);
	TCPSerParaList.SENDAddrToTCPServerFileClose.sin_family=AF_INET;
	TCPSerParaList.SENDAddrToTCPServerFileClose.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPSerParaList.SENDAddrToTCPServerFileClose.sin_port=htons(TCPServerFileClose);

	bzero(&TCPSerParaList.RECVAddrToTCPServerFileClose,SockAddrLength);
	TCPSerParaList.RECVAddrToTCPServerFileClose.sin_family=AF_INET;
	TCPSerParaList.RECVAddrToTCPServerFileClose.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPSerParaList.RECVAddrToTCPServerFileClose.sin_port=htons(TCPServerFileClose);

	TCPSerParaList.SENDTCPServerFileClose = socket(AF_INET,SOCK_DGRAM,0);
	TCPSerParaList.RECVTCPServerFileClose = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPSerParaList.RECVTCPServerFileClose,(struct sockaddr *)&TCPSerParaList.RECVAddrToTCPServerFileClose,SockAddrLength);

	//����ͨ�������հ�
	TCPSerParaList.CountTCPServerContinueRECV_Triggering = 1;
	TCPSerParaList.CountTCPServerContinueRECV_Waiting = 1;
	TCPSerParaList.CountTCPServerContinueRECV_Triggered = 1;
/*
	printf("[%d] ContinueRECV triggering...\n",TCPSerParaList.CountTCPServerContinueRECV_Triggering);
	TCPSerParaList.CountTCPServerContinueRECV_Triggering++;
	printf("[%d] ContinueRECV waiting...\n",TCPSerParaList.CountTCPServerContinueRECV_Waiting);
	TCPSerParaList.CountTCPServerContinueRECV_Waiting++;
	printf("[%d] ContinueRECV triggered!\n",TCPSerParaList.CountTCPServerContinueRECV_Triggered);
	TCPSerParaList.CountTCPServerContinueRECV_Triggered++;
*/
	bzero(&TCPSerParaList.SENDAddrToTCPServerContinueRECV,SockAddrLength);
	TCPSerParaList.SENDAddrToTCPServerContinueRECV.sin_family=AF_INET;
	TCPSerParaList.SENDAddrToTCPServerContinueRECV.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPSerParaList.SENDAddrToTCPServerContinueRECV.sin_port=htons(TCPServerContinueRECV);

	bzero(&TCPSerParaList.RECVAddrToTCPServerContinueRECV,SockAddrLength);
	TCPSerParaList.RECVAddrToTCPServerContinueRECV.sin_family=AF_INET;
	TCPSerParaList.RECVAddrToTCPServerContinueRECV.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPSerParaList.RECVAddrToTCPServerContinueRECV.sin_port=htons(TCPServerContinueRECV);

	TCPSerParaList.SENDTCPServerContinueRECV = socket(AF_INET,SOCK_DGRAM,0);
	TCPSerParaList.RECVTCPServerContinueRECV = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPSerParaList.RECVTCPServerContinueRECV,(struct sockaddr *)&TCPSerParaList.RECVAddrToTCPServerContinueRECV,SockAddrLength);

}

/*******************************************************************************************************************************************
*******************************************Ӧ�ò�TCPServer��������ͨ��CLI���ƣ�******************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.������Ӧ��   To.Ӧ�÷�����*********************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.Ӧ�÷����� To.������Ӧ��***********************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
**********************************************************���߳����ɳ���********************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�TCPServer_thread_timer
* ����������Ϊ���߳��ṩ��ʱ�ź�
* �����б�
* ���ؽ����
*****************************************/
void *
TCPServer_thread_timer
(
	void * fd
)
{
	TCPSerParaList.timer_XX = 0;
	
	while(1)
	{
		//sleep(10000);
		sleep(SLEEP);
		TCPSerParaList.timer_XX++;
		
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
* �������ƣ�TCPServer_thread_TCP
* ��������������ͨ�� - TCP�����������������߳�
* �����б�
* ���ؽ����
*****************************************/
void *
TCPServer_thread_TCP
(
	void * fd
)
{
	int i,j;

	//���ڶ�����TCP���߳���
	pthread_t pthread_NetworkLayertoTransportLayer;
	pthread_t pthread_TransportLayertoNetworkLayer;
	pthread_t pthread_FTP;

	//�������մ��������SID���׽���
	int socket_sidreceiver;
	socket_sidreceiver = socket(AF_INET,SOCK_DGRAM,0);

	struct sockaddr_in sin;
	int sockaddr_len=SockAddrLength;
	bzero(&sin,sockaddr_len);
	sin.sin_family=AF_INET;
	//sin.sin_addr.s_addr=htonl(INADDR_ANY);
	sin.sin_addr.s_addr=inet_addr(LOOPIP);
	sin.sin_port=htons(NETLtoTRAL);

	bind(socket_sidreceiver,(struct sockaddr *)&sin,sockaddr_len);

	//��������SID����Ĵ���洢����
	uint8_t SID[SIDLEN];
	uint8_t OFFSET[OFFLEN];
	unsigned long offsetlong;
	uint8_t DATA[DATALEN];
	int SIDlen=0,DATAlen=0;
	
	uint8_t recvpkg[sizeof(Trans_get)];
	Trans_get * precvpkg;
	precvpkg = (Trans_get *)recvpkg;

	int SingleThreadAlreadyStart = 0;
	int FoundSIDNum;
	while(1)
	{
		i=0;
		j=0;

		offsetlong = 0;
		SIDlen = 0;
		DATAlen = 0;
		memset(SID,0,SIDLEN);
		memset(OFFSET,0,OFFLEN);
		memset(DATA,0,DATALEN);
		memset(recvpkg,0,sizeof(Trans_get));

		FoundSIDNum=-1;

		recvfrom(socket_sidreceiver,recvpkg,sizeof(Trans_get),0,(struct sockaddr *)&sin,&sockaddr_len);

		//��ȡSID��offset
		memcpy(SID,precvpkg->sid,SIDLEN);
		memcpy(OFFSET,precvpkg->offset,OFFLEN);
		SIDlen = 10;

		if(transDEVETESTIMPL <= 4)
		{
			printf("[NetworkLayer to TransportLayer]SID = %s\n",SID);
			printf("[NetworkLayer to TransportLayer]OFF = %d %d %d %d\n",OFFSET[0],OFFSET[1],OFFSET[2],OFFSET[3]);
		}

		//����TCP��������нӿ�����
		
		//�ж�OFFSET��0���Ǵ���0������ж��յ���GET��Ϣ�� ������������Ϣ ���� ĳ���������ACK
		if(OFFSET[0] == 0 && OFFSET[1] == 0 && OFFSET[2] == 3 && OFFSET[3] == 232 && SingleThreadAlreadyStart == 0)//����������������ʱ������������Ϊ1��
		{
			printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>A New Service Started.\n");
			SingleThreadAlreadyStart = 1;
			
			//��ʼ�������߳̿��Ʋ�����
			memset(TCPSerParaList.SID,0,SIDLEN);
			memcpy(TCPSerParaList.SID,SID,SIDlen);
			TCPSerParaList.SIDLen = SIDlen;
	
			TCPSerParaList.BUF = NULL;//���ͻ�������ͷ
			TCPSerParaList.BUFEnd = NULL;//���ͻ�������β
			TCPSerParaList.BUFFileEnd = NULL;//��ȡ�ļ�����ʱ�����������ļ�ĩβ���ڵ�ַ

			TCPSerParaList.BUFRenewCount = 1;//���������´���
			TCPSerParaList.BUFLength = TCPBUFSIZE;//���ͻ��峤��

			TCPSerParaList.FileReadPoint = 0;//�ļ���ȡ����
			TCPSerParaList.FileLength = 0;//�ļ��ܳ���

			TCPSerParaList.PacketDataLength = PKTDATALEN;//�������ݶγ���
			TCPSerParaList.LastPacketDataLength = PKTDATALEN;//����������������һ�����ݰ�ʵ�ʳ���

			TCPSerParaList.WindowLength = 1;//���ڳ���
			TCPSerParaList.WindowThreshold = TCPBUFSIZE;//��������ֵ

			TCPSerParaList.OffsetWindowHead = 0;//�������ļ��д���ͷ����ָ��ƫ����
			TCPSerParaList.OffsetWindowEnd = PKTDATALEN;//�������ļ��д���β����ָ��ƫ����
			TCPSerParaList.OffsetConfirmACK = 0;//�������ļ�����ȷ��ƫ��������ƫ����
			TCPSerParaList.OffsetRequestACK = 0;//�������ļ��нӵ������ƫ����
			TCPSerParaList.OffsetPacket = PKTDATALEN;//�������ļ��е�ǰ���ƫ�������ش����ݰ�ƫ�����ֶε���д���ݣ�
			TCPSerParaList.OffsetWaitACK = PKTDATALEN;//�������ļ��еȴ�ȷ���յ���ƫ��������һ�ڣ�

			TCPSerParaList.AddrWindowHead = NULL;//�ڻ������д���ͷ�����ڵ�ַ
			TCPSerParaList.AddrWindowEnd = NULL;//�ڻ������д���β�����ڵ�ַ
			TCPSerParaList.AddrConfirmACK = NULL;//�ڻ���������ȷ��ƫ�������ڵ�ַ
			TCPSerParaList.AddrRequestACK = NULL;//�ڻ������нӵ���������ڵ�ַ
			TCPSerParaList.AddrPacket = NULL;//�ڻ������е�ǰ���ƫ�������ڵ�ַ
			TCPSerParaList.AddrWaitACK = NULL;//�ڻ������д�ȷ��ƫ�������ڵ�ַ

			TCPSerParaList.RTOs = 0;//��������ʱ�ӣ�RTO����1�벿�֣���λ�룩
			TCPSerParaList.RTOus = 0;//��������ʱ�ӣ�RTOС��1�벿�֣���λ΢�룩
			TCPSerParaList.RTOns = 0;//��������ʱ�ӣ�RTOС��1�벿�֣���λ���룩

			TCPSerParaList.FlagTrebleAck = 0;//����������¼�յ���ͬƫ����ACK�ĸ����������ж�����ACK����״��

			TCPSerParaList.WindowState = 0;//����״̬��1ACK�������µ�δ�ﴰ����Ҫ����Ͻ磻2���ڴ���ɹ���3����ACK��4��ʱ
			TCPSerParaList.ThreadState = 0;//����״̬�����ڽ������̵Ŀ��ƣ���0δ��ɣ�1�ļ���ȡ��ϣ�2����¼����ϣ�3���ڷ�����ϣ�4����ȷ����ϣ�������ֹ��
			
			//����thread_FTP���߳�
			if(pthread_create(&pthread_FTP, NULL, TCPServer_thread_FTP, NULL)!=0)
			{
				perror("Creation of FTP thread failed.");
			}
			
			//����thread_NetworkLayertoTransportLayer�������߳�
			if(pthread_create(&pthread_NetworkLayertoTransportLayer, NULL, TCPServer_thread_NetworkLayertoTransportLayer, NULL)!=0)
			{
				perror("Creation of NetworkLayertoTransportLayer thread failed.");
			}
	
			//����thread_TransportLayertoNetworkLayer�������߳�
			if(pthread_create(&pthread_TransportLayertoNetworkLayer, NULL, TCPServer_thread_TransportLayertoNetworkLayer, NULL)!=0)
			{
				perror("Creation of TransportLayertoNetworkLayer thread failed.");
			}
			
			//֪ͨӦ�ò����SID���ݲ����뷢�ͻ���
			//����ʱ�Ե��������򻯿�����
			printf("[RECV new]offsetlong = %ld\n",(unsigned long)1000);
			printf("[RECV new]OffsetRequestACK  = %ld\n",TCPSerParaList.OffsetRequestACK);
		}
		else//�������������ACK
		{
			//printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>There is an ACK for a Single Thread which Already Started.\n");

			offsetlong    = (unsigned long)OFFSET[0] * 256 * 256 * 256 + 
					(unsigned long)OFFSET[1] * 256 * 256 + 
					(unsigned long)OFFSET[2] * 256 + 
					(unsigned long)OFFSET[3];

			//ƥ��SID��������SID��Ӧ�������̱߳��
			if(strncmp(TCPSerParaList.SID,SID,SIDLEN) == 0)
			{
				FoundSIDNum = 0;
			}
			if(FoundSIDNum == -1)
			{
				//printf("An unknown ACK Packet received, SID not found.\n");
				continue;
			}
			
			TCPSerParaList.SIDLen = SIDlen;

			if(transDEVETESTIMPL <= 5)
			{
				printf("[RECV]offsetlong       = %ld\n",offsetlong);
				printf("[LAST]OffsetRequestACK = %ld\n",TCPSerParaList.OffsetRequestACK);
				printf("[NOW ]OffsetWindowEnd  = %ld\n",TCPSerParaList.OffsetWindowEnd);
			}

			if(offsetlong > TCPSerParaList.OffsetRequestACK)//�µ�����ƫ����������ȷ��ƫ����ֵ
			{//printf("New ACK\n");
				TCPSerParaList.OffsetRequestACK = offsetlong;
				TCPSerParaList.OffsetConfirmACK = TCPSerParaList.OffsetRequestACK - TCPSerParaList.PacketDataLength;
				
				TCPSerParaList.FlagTrebleAck = 0;
				
				TCPSerParaList.WindowState = 1;
				
				if(TCPSerParaList.OffsetRequestACK == TCPSerParaList.OffsetWindowEnd)//�յ�ACK�������ȴ����ڵ��Ͻ�
				{
					TCPSerParaList.WindowState = 2;
				}
			}
			else if(offsetlong == TCPSerParaList.OffsetRequestACK)//�յ��ظ�ƫ������ACK
			{//printf("Same ACK\n");printf("[RECV]offsetlong       = %ld\n",offsetlong);
				TCPSerParaList.FlagTrebleAck++;
				
				if(TCPSerParaList.FlagTrebleAck >= 3)//�ظ�ACK��3��
				{
					//printf("TrebleAck Reached!\n");
					TCPSerParaList.WindowState = 3;
					TCPSerParaList.FlagTrebleAck = 0;
					
					continue;
				}
				else
				{
					continue;
				}
			}
			else//�յ�����ƫ����С�ڵ�ǰȷ��ƫ������ACK���������д���������������������
			{
				continue;
			}

			sendto(TCPSerParaList.SENDTCPServerWindowTimingEnd,"WindowTimingEnd",20,0,(struct sockaddr *)&TCPSerParaList.SENDAddrToTCPServerWindowTimingEnd,SockAddrLength);

			//printf("[%d] WindowTimingEnd triggering...\n",TCPSerParaList.CountTCPServerWindowTimingEnd_Triggering);
			TCPSerParaList.CountTCPServerWindowTimingEnd_Triggering++;
			
			//����Ƿ�������������ﴰ���Ͻ������ACK���Ĵ���WindowTimingEnd�����ų�����ACK���²��ȴ�����ͨ�����ͼ����հ���ָ��

			if(TCPSerParaList.WindowState != 1)
			{
				//�����ȴ�����ͨ�����ͼ����հ���ָ��
				//printf("[%d] ContinueRECV waiting...\n",TCPSerParaList.CountTCPServerContinueRECV_Waiting);
				TCPSerParaList.CountTCPServerContinueRECV_Waiting++;

				recvfrom(TCPSerParaList.RECVTCPServerContinueRECV,Trigger,20,0,(struct sockaddr *)&TCPSerParaList.RECVAddrToTCPServerContinueRECV,&SockAddrLength);

				//printf("[%d] ContinueRECV triggered.\n",TCPSerParaList.CountTCPServerContinueRECV_Triggered);
				TCPSerParaList.CountTCPServerContinueRECV_Triggered++;
			}

		}
	}
	
	close(socket_sidreceiver);

	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);	
}

/*****************************************
* �������ƣ�TCPServer_thread_FTP
* ������������ȡ�ļ��̣߳�FTPǰ���������ȡ�ļ���ά�ַ��ͻ���ĸ���
* �����б�
* ���ؽ����
*****************************************/
void *
TCPServer_thread_FTP
(
	void * fd
)
{
	//��ȡ������
	//int ThreadNum = *((int *)fd);printf("[FTP]ThreadNum = %d\n",ThreadNum);
	
	//Ϊ�������뷢�ͻ����ڴ�
	TCPSerParaList.BUF = (uint8_t *)malloc(TCPBUFSIZE);
	memset(TCPSerParaList.BUF,0,TCPBUFSIZE);
	
	//���������Ŷ�Ӧ�Ĳ�����SIDѰ�Ҷ�Ӧ���ļ�
	
	
	//���ļ�����ȡ�������
	int fp;
	unsigned long filelen;
	
	fp = open(FTPServerPath,O_RDONLY);
	filelen = lseek(fp,0,SEEK_END);
	//������鿽���ļ��ܳ���
	TCPSerParaList.FileLength = filelen;
	
	printf("FileLength = %ld\n",TCPSerParaList.FileLength);
	
	TCPSerParaList.BUFEnd = TCPSerParaList.BUF + TCPSerParaList.BUFLength;

	//����ļ��ܳ���С�ڻ��������ȣ����С�ļ���
	int readlength = 0;
	if(TCPSerParaList.FileLength <= TCPSerParaList.BUFLength)
	{
		printf("File is very small, smaller than the BUFFER.\n");
		readlength = TCPSerParaList.FileLength;
		TCPSerParaList.BUFFileEnd = TCPSerParaList.BUF + TCPSerParaList.FileLength;
		TCPSerParaList.ThreadState = 1;
	}
	else
	{
		readlength = TCPBUFSIZE;
	}

	//��ȡ��һ������
	lseek(fp,0,SEEK_SET);
	read(fp,TCPSerParaList.BUF,readlength);
	//��Ӧ���޸��ļ����Ʋ���
	TCPSerParaList.BUFRenewCount = 0;
	TCPSerParaList.FileReadPoint = TCPBUFSIZE;
	
	//printf("BUF = %s\n",TCPSerParaList.BUF);
	
	//֪ͨ����㣬���ͻ����Ѿ�׼������
	sendto(TCPSerParaList.SENDTCPServerBufReady,"BufReady",20,0,(struct sockaddr *)&TCPSerParaList.SENDAddrToTCPServerBufReady,SockAddrLength);

	printf("[%d] BufReady triggering...\n",TCPSerParaList.CountTCPServerBufReady_Triggering);
	TCPSerParaList.CountTCPServerBufReady_Triggering++;
	
	//��ʼ����Ϊ����㹩Ӧ����
	while(TCPSerParaList.ThreadState == 0)
	{
		//�����ȴ�����㴥���Թ�Ӧ�������ݵ�Ҫ��
		//printf("[%d] MoreData waiting...\n",TCPSerParaList.CountTCPServerMoreData_Waiting);
		TCPSerParaList.CountTCPServerMoreData_Waiting++;

		recvfrom(TCPSerParaList.RECVTCPServerMoreData,Trigger,20,0,(struct sockaddr *)&TCPSerParaList.RECVAddrToTCPServerMoreData,&SockAddrLength);

		//printf("[%d] MoreData triggered.\n",TCPSerParaList.CountTCPServerMoreData_Triggered);
		TCPSerParaList.CountTCPServerMoreData_Triggered++;

		TCPSerParaList.BUFRenewCount++;

		printf("FILE Reloading...(Per BUFLEN/2 for once)\n");

		//һ�θ��·��ͻ�����һ������ݣ�ǰ��κͺ��ν������
		if(TCPSerParaList.BUFRenewCount % 2 == 1)//ָʾ����ǰ���
		{
			//����ļ�ʣ�೤���Ƿ��㹻һ�β���
			if(TCPSerParaList.FileReadPoint + TCPBUFSIZE/2 > filelen)//����һ�β��䣬�����������ݣ�����¼β����ʶ
			{
				lseek(fp,TCPSerParaList.FileReadPoint,SEEK_SET);
				read(fp,TCPSerParaList.BUF,filelen - TCPSerParaList.FileReadPoint);
				
				TCPSerParaList.BUFFileEnd = TCPSerParaList.BUF + ( filelen - TCPSerParaList.FileReadPoint );
				TCPSerParaList.ThreadState = 1;
				break;
			}
			else//�㹻һ�β��䣬�򲹳�
			{
				lseek(fp,TCPSerParaList.FileReadPoint,SEEK_SET);
				read(fp,TCPSerParaList.BUF,TCPBUFSIZE/2);
			
				TCPSerParaList.FileReadPoint += TCPBUFSIZE/2;
			}
		}
		else if(TCPSerParaList.BUFRenewCount % 2 == 0)//ָʾ���º���
		{
			//����ļ�ʣ�೤���Ƿ��㹻һ�β���
			if(TCPSerParaList.FileReadPoint + TCPBUFSIZE/2 > filelen)//����һ�β��䣬�����������ݣ�����¼β����ʶ
			{
				lseek(fp,TCPSerParaList.FileReadPoint,SEEK_SET);
				read(fp,TCPSerParaList.BUF + TCPBUFSIZE/2,filelen - TCPSerParaList.FileReadPoint);
				
				TCPSerParaList.BUFFileEnd = TCPSerParaList.BUF + TCPBUFSIZE/2 + ( filelen - TCPSerParaList.FileReadPoint );
				TCPSerParaList.ThreadState = 1;
				break;
			}
			else//�㹻һ�β��䣬�򲹳�
			{
				lseek(fp,TCPSerParaList.FileReadPoint,SEEK_SET);
				read(fp,TCPSerParaList.BUF + TCPBUFSIZE/2,TCPBUFSIZE/2);
			
				TCPSerParaList.FileReadPoint += TCPBUFSIZE/2;
			}
		}
		printf("FILE Reloaded. Already loaded: %ld Byte\n",TCPSerParaList.FileReadPoint);
	}

	//������ͨ��֪ͨ�ļ���ȡ����
	sendto(TCPSerParaList.SENDTCPServerFileClose,"FileClose",20,0,(struct sockaddr *)&TCPSerParaList.SENDAddrToTCPServerFileClose,SockAddrLength);

	//printf("[%d] FileClose triggering...\n",TCPSerParaList.CountTCPServerFileClose_Triggering);
	TCPSerParaList.CountTCPServerFileClose_Triggering++;
	
	close(fp);
}

/*****************************************
* �������ƣ�TCPServer_thread_NetworkLayertoTransportLayer
* ��������������ͨ�� - From.������Ӧ��   To.Ӧ�÷�����������ACK�Ľ��պͼ�ʱ
* �����б�
* ���ؽ����
*****************************************/
void *
TCPServer_thread_NetworkLayertoTransportLayer
(
	void * fd
)
{
	//��ȡ������
	//int ThreadNum = *((int *)fd);printf("[UP]ThreadNum = %d\n",ThreadNum);
	
	//��ʱ����ʼ��

	//���ó�ʱ
	//struct timespec delaytime;
	struct timeval delaytime;
	
	fd_set readfds;

	int judge = 0;
	int TimingCount = 0;
	while(1)
	{
		//��������
		judge = 0;
		TimingCount++;
		//printf("TimingCount = %d\n",TimingCount);
		
		if(TCPSerParaList.WindowState != 1)
		{
			//�����ȴ���ʱ����

			//printf("[%d] WindowBegin waiting...\n",TCPSerParaList.CountTCPServerWindowBegin_Waiting);
			TCPSerParaList.CountTCPServerWindowBegin_Waiting++;

			recvfrom(TCPSerParaList.RECVTCPServerWindowBegin,Trigger,20,0,(struct sockaddr *)&TCPSerParaList.RECVAddrToTCPServerWindowBegin,&SockAddrLength);

			//printf("[%d] WindowBegin triggered!\n",TCPSerParaList.CountTCPServerWindowBegin_Triggered);
			TCPSerParaList.CountTCPServerWindowBegin_Triggered++;
		}
		
		//��ʱ�������ڴ�������ͨ��������ʱ��ʾ

		//printf("[%d] WindowTimingEnd waiting...\n",TCPSerParaList.CountTCPServerWindowTimingEnd_Waiting);
		TCPSerParaList.CountTCPServerWindowTimingEnd_Waiting++;

		//����ʱ��UDP��Ϣ����
		//���ó�ʱ
		delaytime.tv_sec=TCPSerParaList.RTOs;
		delaytime.tv_usec=TCPSerParaList.RTOus;

		//��¼sockfd
		FD_ZERO(&readfds);
		FD_SET(TCPSerParaList.RECVTCPServerWindowTimingEnd,&readfds);

		select(TCPSerParaList.RECVTCPServerWindowTimingEnd+1,&readfds,NULL,NULL,&delaytime);
	
		//��ʱ����
		if(FD_ISSET(TCPSerParaList.RECVTCPServerWindowTimingEnd,&readfds))
		{
			if(0 <= recvfrom(TCPSerParaList.RECVTCPServerWindowTimingEnd,Trigger,20,0,(struct sockaddr *)&TCPSerParaList.RECVAddrToTCPServerWindowTimingEnd,&SockAddrLength))
			{
				if(transDEVETESTIMPL <= 0)
					printf("[TimingEndDelay]  %.3f ms\n",(double)(RTOUS - (int)delaytime.tv_usec)/(double)1000);
				judge = 1;
			}
		}
		else
		{
			if(transDEVETESTIMPL <= 0)
				printf("[TimingEndDelay]  %.3f ms   (Timeout)\n",(double)(RTOUS - (int)delaytime.tv_usec)/(double)1000);
		}

		//printf("[%d] WindowTimingEnd triggered!\n",TCPSerParaList.CountTCPServerWindowTimingEnd_Triggered);
		TCPSerParaList.CountTCPServerWindowTimingEnd_Triggered++;

		//��ʾ���
		if(judge > 0)//��ʱ���
		{
			//printf("On Time!\n");
			
			//��ʱ���յ��µ�ACK������������δ����ɹ������ڿ������ȴ�������������ö�ʱ��
			if(TCPSerParaList.WindowState == 1)
			{
				continue;
			}
			//���������2���ڰ�ʱ����ɹ� 3����ACK������Ҫ���������ڴ��䴦��
		}
		else//��ʱ
		{
			//printf("Time Out!\n");
			TCPSerParaList.WindowState = 4;
			//printf("WindowState = %d\n",TCPSerParaList.WindowState);
		}

		//֪ͨ����ͨ�����������δ��ڴ����ѵó����
		sendto(TCPSerParaList.SENDTCPServerWindowRespond,"WindowRespond",20,0,(struct sockaddr *)&TCPSerParaList.SENDAddrToTCPServerWindowRespond,SockAddrLength);

		//printf("[%d] WindowRespond triggering...\n",TCPSerParaList.CountTCPServerWindowRespond_Triggering);
		TCPSerParaList.CountTCPServerWindowRespond_Triggering++;
	}
	
	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�TCPServer_thread_TransportLayertoNetworkLayer
* ��������������ͨ�� - From.Ӧ�÷����� To.������Ӧ�ˣ����𴰿ڵļ����뻬������ʱ����Ӧ����װ���ݲ����µݽ�
* �����б�
* ���ؽ����
*****************************************/
void *
TCPServer_thread_TransportLayertoNetworkLayer
(
	void * fd
)
{
	//��ȡ������
	//int ThreadNum = *((int *)fd);printf("[DOWN]ThreadNum = %d\n",ThreadNum);
	
	//��������ȴ����ͻ���׼������

	printf("[%d] BufReady waiting...\n",TCPSerParaList.CountTCPServerBufReady_Waiting);
	TCPSerParaList.CountTCPServerBufReady_Waiting++;

	recvfrom(TCPSerParaList.RECVTCPServerBufReady,Trigger,20,0,(struct sockaddr *)&TCPSerParaList.RECVAddrToTCPServerBufReady,&SockAddrLength);

	printf("[%d] BufReady triggered!\n",TCPSerParaList.CountTCPServerBufReady_Triggered);
	TCPSerParaList.CountTCPServerBufReady_Triggered++;

	int i,j;
	
	//������ʼ���������ʼ�������˵�ʱ���Ѿ���ζ���յ�ƫ��ΪPKTDATALEN������

	TCPSerParaList.BUFLength = TCPBUFSIZE;//���ͻ��峤��

	TCPSerParaList.PacketDataLength = PKTDATALEN;//�������ݶγ���
	TCPSerParaList.LastPacketDataLength = PKTDATALEN;//����������������һ�����ݰ�ʵ�ʳ���

	TCPSerParaList.WindowLength = 2;//���ڳ���
	TCPSerParaList.WindowThreshold = TCPBUFSIZE;//��������ֵ

	TCPSerParaList.OffsetWindowHead = 0;//�������ļ��д���ͷ����ָ��ƫ����
	TCPSerParaList.OffsetWindowEnd = PKTDATALEN + TCPSerParaList.WindowLength * TCPSerParaList.PacketDataLength;//�������ļ��д���β����ָ��ƫ����
	TCPSerParaList.OffsetConfirmACK = 0;//�������ļ�����ȷ��ƫ��������ƫ����
	TCPSerParaList.OffsetRequestACK = PKTDATALEN;//�������ļ��нӵ������ƫ����
	TCPSerParaList.OffsetPacket = PKTDATALEN;//�������ļ��е�ǰ���ƫ�������ش����ݰ�ƫ�����ֶε���д���ݣ�
	TCPSerParaList.OffsetWaitACK = PKTDATALEN;//�������ļ��еȴ�ȷ���յ���ƫ��������һ�ڣ�

	TCPSerParaList.AddrWindowHead = TCPSerParaList.BUF;//�ڻ������д���ͷ�����ڵ�ַ
	TCPSerParaList.AddrWindowEnd = TCPSerParaList.BUF + TCPSerParaList.WindowLength * TCPSerParaList.PacketDataLength;//�ڻ������д���β�����ڵ�ַ
	TCPSerParaList.AddrConfirmACK = TCPSerParaList.BUF;//�ڻ���������ȷ��ƫ�������ڵ�ַ
	TCPSerParaList.AddrRequestACK = TCPSerParaList.BUF;//�ڻ������нӵ���������ڵ�ַ
	TCPSerParaList.AddrPacket = TCPSerParaList.BUF;//�ڻ������е�ǰ���ƫ�������ڵ�ַ
	TCPSerParaList.AddrWaitACK = TCPSerParaList.BUF;//�ڻ������д�ȷ��ƫ�������ڵ�ַ

	TCPSerParaList.RTOs = RTOS;//��������ʱ�ӣ�RTO����1�벿�֣���λ�룩
	TCPSerParaList.RTOus = RTOUS;//��������ʱ�ӣ�RTOС��1�벿�֣���λ΢�룩
	TCPSerParaList.RTOns = RTONS;//��������ʱ�ӣ�RTOС��1�벿�֣���λ���룩

	TCPSerParaList.FlagTrebleAck = 0;//����������¼�յ���ͬƫ����ACK�ĸ����������ж�����ACK����״��

	TCPSerParaList.WindowState = 0;//����״̬��1ACK�������µ�δ�ﴰ����Ҫ����Ͻ磻2���ڴ���ɹ���3����ACK��4��ʱ
	//TCPSerParaList.ThreadState = 0;//����״̬��ʱ�Ѿ��ı䣨FTP�߳��ж�ȡС�ļ�ʱ�ᷢ���ı䣬�ʲ����ٴγ�ʼ����



	//������̱���

	uint8_t DATA[DATALEN];
	unsigned long offsetlong;
	uint8_t offset[OFFLEN];
	
	uint8_t sendpkg[sizeof(CoLoR_data)];
	Trans_data * psendpkg;
	psendpkg = (Trans_data *)sendpkg;

	while(1)
	{
		TCPSerParaList.FlagTrebleAck = 0;

		//����ļ��Ѿ���ȡ��ϣ������鷢�͹����Ƿ��Ѿ����ļ�ĩβ
		//printf("ThreadState == %d\n",TCPSerParaList.ThreadState);
		//printf("[SET TAIL]TCPSerParaList.AddrWindowEnd   = %d\n",TCPSerParaList.AddrWindowEnd - TCPSerParaList.BUF);
		//printf("ThreadState = %d\n",TCPSerParaList.ThreadState);
		if(TCPSerParaList.ThreadState >= 1)
		{
/*
			printf("[CHECK TAIL]OffsetConfirmACK == %ld\n",TCPSerParaList.OffsetConfirmACK);
			printf("[CHECK TAIL]OffsetRequestACK == %ld\n",TCPSerParaList.OffsetRequestACK);
			printf("[CHECK TAIL]OffsetPacket + WindowLength * PacketDataLength == %ld\n",TCPSerParaList.OffsetPacket + TCPSerParaList.WindowLength * TCPSerParaList.PacketDataLength);
*/
			if( TCPSerParaList.OffsetPacket - PKTDATALEN + TCPSerParaList.WindowLength * TCPSerParaList.PacketDataLength > TCPSerParaList.FileLength )
			{
				TCPSerParaList.ThreadState = 2;
				
				//�����ļ�ʣ�೤�����¼��㴰�ڴ�С
				TCPSerParaList.WindowLength = ( TCPSerParaList.FileLength - TCPSerParaList.OffsetConfirmACK ) / TCPSerParaList.PacketDataLength;
				printf("WindowLength = %ld\n",TCPSerParaList.WindowLength);
				if(( TCPSerParaList.FileLength - TCPSerParaList.OffsetConfirmACK ) % TCPSerParaList.PacketDataLength != 0)
					TCPSerParaList.WindowLength++;
				printf("WindowLength = %ld\n",TCPSerParaList.WindowLength);

				//���´���β��
				TCPSerParaList.OffsetWindowEnd = TCPSerParaList.OffsetWindowHead + TCPSerParaList.WindowLength * TCPSerParaList.PacketDataLength;
				TCPSerParaList.AddrWindowEnd   = TCPSerParaList.AddrWindowHead   + TCPSerParaList.WindowLength * TCPSerParaList.PacketDataLength;
				printf("[SET TAIL]TCPSerParaList.OffsetWindowEnd = %ld\n",TCPSerParaList.OffsetWindowEnd);
				printf("[SET TAIL]TCPSerParaList.AddrWindowEnd   = %d\n",TCPSerParaList.AddrWindowEnd - TCPSerParaList.BUF);
			}
		}
		
		//ѭ�����ʹ����ڵ����ݰ�


		//��¼���ڳ��ȱ仯����
		TCPSerParaList.InputCount++;
		TCPSerParaList.TotalWindowLength += TCPSerParaList.WindowLength;
		//printf("[%d] ",TCPSerParaList.InputCount);
		//printf("WindowLength = %ld\n",TCPSerParaList.WindowLength);
		
/*
		itoa((int)TCPSerParaList.WindowLength,TCPSerParaList.NumStr,10);
		if(TCPSerParaList.WindowLength<10)
			write(TCPSerParaList.LogFilePoint,TCPSerParaList.NumStr,1);
		else if(TCPSerParaList.WindowLength<100)
			write(TCPSerParaList.LogFilePoint,TCPSerParaList.NumStr,2);
		else if(TCPSerParaList.WindowLength<1000)
			write(TCPSerParaList.LogFilePoint,TCPSerParaList.NumStr,3);
		write(TCPSerParaList.LogFilePoint," ",1);
*/

		for(i=0;i<TCPSerParaList.WindowLength;i++)
		{
			if(transDEVETESTIMPL <= 5)
				printf("WindowLength = %ld\n",TCPSerParaList.WindowLength);

			memset(sendpkg,0,sizeof(CoLoR_data));
			
			//��ȡ��������
			if(i == TCPSerParaList.WindowLength - 1 && TCPSerParaList.ThreadState >= 2)//����Ѿ�¼�뵽������������������һ����
			{printf("The last Packet!!!!\n");

				printf("[LAST PACKET]AddrWindowEnd - BUF        = %d\n",TCPSerParaList.AddrWindowEnd - TCPSerParaList.BUF);
				printf("[LAST PACKET]AddrWindowEnd - AddrPacket = %d\n",TCPSerParaList.AddrWindowEnd - TCPSerParaList.AddrPacket);

				memcpy(DATA,TCPSerParaList.AddrPacket,TCPSerParaList.AddrWindowEnd - TCPSerParaList.AddrPacket);

				TCPSerParaList.LastPacketDataLength = TCPSerParaList.AddrWindowEnd - TCPSerParaList.AddrPacket;
				
				//����offset����������
				offsetlong = TCPSerParaList.OffsetPacket;
				for(j=OFFLEN-1;j>=0;j--)
				{
					offset[j] = (char)offsetlong%256;
					offsetlong/=256;
				}
				printf("Sent Offset = %ld\n",TCPSerParaList.OffsetPacket);
				//printf("Sent Offset = %d %d %d %d\n",(int)offset[0],(int)offset[1],(int)offset[2],(int)offset[3]);

				//��װSID��DATA��offset
				memcpy(psendpkg->sid,TCPSerParaList.SID,SIDLEN);
				memcpy(psendpkg->data,DATA,TCPSerParaList.AddrWindowEnd - TCPSerParaList.AddrPacket);
				memcpy(psendpkg->offset,offset,OFFLEN);
				/*
				//��װ��Ϣ
				memcpy(message,"GOT",3);
				memcpy(message + 3,DATA,TCPSerParaList.AddrWindowEnd - TCPSerParaList.AddrPacket);
				memcpy(message + 3 + TCPSerParaList.LastPacketDataLength,"GET",3);
				memcpy(message + 3 + TCPSerParaList.LastPacketDataLength + 3,TCPSerParaList.SID,TCPSerParaList.SIDLen);
				memcpy(message + 3 + TCPSerParaList.LastPacketDataLength + 3 + TCPSerParaList.SIDLen,"OFF",3);
				memcpy(message + 3 + TCPSerParaList.LastPacketDataLength + 3 + TCPSerParaList.SIDLen + 3,offset,OFFLEN);
				*/
			
				//�������
				//����socket
				//�׽ӿ�������
				int socket_datasender;
				socket_datasender = socket(AF_INET,SOCK_DGRAM,0);
		
				struct sockaddr_in addrTo;
				bzero(&addrTo,SockAddrLength);
				addrTo.sin_family=AF_INET;
				addrTo.sin_port=htons(TRALtoNETL);
				//unsigned long IPto = 2130706433;//�ػ���ַ���� == 2130706433
				//addrTo.sin_addr.s_addr=htonl(IPto);//htonl�������ֽ���ת��Ϊ�����ֽ���
				addrTo.sin_addr.s_addr=inet_addr(LOOPIP);//htonl�������ֽ���ת��Ϊ�����ֽ���

				//���һ��ʱ�䷢����һ����
				usleep(FlowSlot);
			
				//����SID����
				sendto(socket_datasender,sendpkg,sizeof(Trans_data),0,(struct sockaddr *)&addrTo,SockAddrLength);
				printf("DATAsent\n");
				//����RTO��ʱ��
				if(i == 0)
				{
					sendto(TCPSerParaList.SENDTCPServerWindowBegin,"WindowBegin",20,0,(struct sockaddr *)&TCPSerParaList.SENDAddrToTCPServerWindowBegin,SockAddrLength);

					//printf("[%d] WindowBegin triggering...\n",TCPSerParaList.CountTCPServerWindowBegin_Triggering);
					TCPSerParaList.CountTCPServerWindowBegin_Triggering++;
				}
			
				close(socket_datasender);
			
				//�����仯
				//TCPSerParaList.OffsetPacket += TCPSerParaList.PacketDataLength;
				//TCPSerParaList.AddrPacket   += TCPSerParaList.PacketDataLength;
			
				//�������ػ�����
				if(TCPSerParaList.AddrPacket >= TCPSerParaList.BUFEnd)
				{
					TCPSerParaList.AddrPacket = TCPSerParaList.BUF + ( TCPSerParaList.AddrPacket - TCPSerParaList.BUFEnd );
				}

				TCPSerParaList.OffsetWaitACK = TCPSerParaList.OffsetPacket;
				TCPSerParaList.AddrWaitACK   = TCPSerParaList.AddrPacket;
				
				//����������ĩ�˲����ã�У���ۼӵ��ļ������Ƿ���ʵ���ļ��������
				if(TCPSerParaList.OffsetWaitACK > TCPSerParaList.FileLength + PKTDATALEN)
				{
					printf("Deadly Error! [ OffsetWaitACK > FileLength + PKTDATALEN ] In The End!\n");
					exit(0);
				}
				else
				{
					//printf("File Length Checking passed!.\n");
				}

				TCPSerParaList.ThreadState = 3;//�޸�����״̬Ϊ��3���ڷ������
			}
			else//���͵����ݰ���δ��������������һ��
			{
/*
				printf("[NOT END]AddrWindowEnd - BUF        = %d\n",TCPSerParaList.AddrWindowEnd - TCPSerParaList.BUF);
				printf("[NOT END]BUFFileEnd    - AddrPacket = %d\n",TCPSerParaList.BUFFileEnd - TCPSerParaList.AddrPacket);
				printf("[NOT END]AddrWindowEnd - AddrPacket = %d\n",TCPSerParaList.AddrWindowEnd - TCPSerParaList.AddrPacket);
				printf("[NOT END]OffsetPacket               = %ld\n",TCPSerParaList.OffsetPacket);
*/
				memcpy(DATA,TCPSerParaList.AddrPacket,TCPSerParaList.PacketDataLength);

				//printf("DATA = \n");
				int testnum;
				for(testnum=0;testnum<TCPSerParaList.PacketDataLength;testnum++)
				{
					//printf("%c",DATA[testnum]);
				}
				//printf("\n");
				//����offset����������
				offsetlong = TCPSerParaList.OffsetPacket;
				for(j=OFFLEN-1;j>=0;j--)
				{
					offset[j] = (char)offsetlong%256;
					offsetlong/=256;
				}

				if(transDEVETESTIMPL <= 5)
					printf("Sent Offset = %ld\n",TCPSerParaList.OffsetPacket);

				//printf("Sent Offset = %d %d %d %d\n",(int)offset[0],(int)offset[1],(int)offset[2],(int)offset[3]);

				//printf("OFFSET = %d %d %d %d\n",offset[0],offset[1],offset[2],offset[3]);
				//printf("PacketDataLength = %ld\n",TCPSerParaList.PacketDataLength);
				//printf("SIDLen = %d\n",TCPSerParaList.SIDLen);
				
				//��װSID��DATA��offset
				memcpy(psendpkg->sid,TCPSerParaList.SID,SIDLEN);
				memcpy(psendpkg->data,DATA,TCPSerParaList.PacketDataLength);
				memcpy(psendpkg->offset,offset,OFFLEN);
				
				/*
				//��װ��Ϣ
				memcpy(message,"GOT",3);
				memcpy(message + 3,DATA,TCPSerParaList.PacketDataLength);
				memcpy(message + 3 + TCPSerParaList.PacketDataLength,"GET",3);
				memcpy(message + 3 + TCPSerParaList.PacketDataLength + 3,TCPSerParaList.SID,TCPSerParaList.SIDLen);
				memcpy(message + 3 + TCPSerParaList.PacketDataLength + 3 + TCPSerParaList.SIDLen,"OFF",3);
				memcpy(message + 3 + TCPSerParaList.PacketDataLength + 3 + TCPSerParaList.SIDLen + 3,offset,OFFLEN);
				*/
			
				//�������
				//����socket
				//�׽ӿ�������
				int socket_datasender;
				socket_datasender = socket(AF_INET,SOCK_DGRAM,0);
		
				struct sockaddr_in addrTo;
				bzero(&addrTo,SockAddrLength);
				addrTo.sin_family=AF_INET;
				addrTo.sin_port=htons(TRALtoNETL);
				//unsigned long IPto = 2130706433;//�ػ���ַ���� == 2130706433
				//addrTo.sin_addr.s_addr=htonl(IPto);//htonl�������ֽ���ת��Ϊ�����ֽ���
				addrTo.sin_addr.s_addr=inet_addr(LOOPIP);//htonl�������ֽ���ת��Ϊ�����ֽ���

				//���һ��ʱ�䷢����һ����
				usleep(FlowSlot);
			
				//����SID����
				sendto(socket_datasender,sendpkg,sizeof(Trans_data),0,(struct sockaddr *)&addrTo,SockAddrLength);
				
				//����RTO��ʱ��
				if(i == 0)
				{
					sendto(TCPSerParaList.SENDTCPServerWindowBegin,"WindowBegin",20,0,(struct sockaddr *)&TCPSerParaList.SENDAddrToTCPServerWindowBegin,SockAddrLength);

					//printf("[%d] WindowBegin triggering...\n",TCPSerParaList.CountTCPServerWindowBegin_Triggering);
					TCPSerParaList.CountTCPServerWindowBegin_Triggering++;
				}
			
				close(socket_datasender);
			
				//�����仯
				TCPSerParaList.OffsetPacket += TCPSerParaList.PacketDataLength;
				TCPSerParaList.AddrPacket   += TCPSerParaList.PacketDataLength;
			
				//�������ػ�����
				if(TCPSerParaList.AddrPacket >= TCPSerParaList.BUFEnd)
				{
					TCPSerParaList.AddrPacket = TCPSerParaList.BUF + ( TCPSerParaList.AddrPacket - TCPSerParaList.BUFEnd );
				}

				TCPSerParaList.OffsetWaitACK = TCPSerParaList.OffsetPacket;
				TCPSerParaList.AddrWaitACK   = TCPSerParaList.AddrPacket;
/*
				printf("[PARA CHANGE]OffsetPacket     = %ld\n",TCPSerParaList.OffsetPacket);
				printf("[PARA CHANGE]AddrPacket - BUF = %d\n",TCPSerParaList.AddrPacket - TCPSerParaList.BUF);
				printf("[PARA CHANGE]OffsetWaitACK    = %ld\n",TCPSerParaList.OffsetWaitACK);
				printf("[PARA CHANGE]AddrWaitACK- BUF = %d\n",TCPSerParaList.AddrWaitACK - TCPSerParaList.BUF);
*/
			}
		}
		
		//�����������ȴ�����ͨ���ź�

		//printf("[%d] WindowRespond waiting...\n",TCPSerParaList.CountTCPServerWindowRespond_Waiting);
		TCPSerParaList.CountTCPServerWindowRespond_Waiting++;

		recvfrom(TCPSerParaList.RECVTCPServerWindowRespond,Trigger,20,0,(struct sockaddr *)&TCPSerParaList.RECVAddrToTCPServerWindowRespond,&SockAddrLength);

		//printf("[%d] WindowRespond triggered!\n",TCPSerParaList.CountTCPServerWindowRespond_Triggered);
		TCPSerParaList.CountTCPServerWindowRespond_Triggered++;
		
		//��������������
		
		//���ڴ�С�����ڻ���������ֵ����ز������㣬���ݲ�����TCPSerParaList.WindowState��TCPSerParaList.OffsetRequestACK
		//printf("WindowState = %d\n",TCPSerParaList.WindowState);

		if(TCPSerParaList.WindowState == 1)//ACK�������µ�δ�ﴰ����Ҫ����Ͻ�
		{
			//printf("After WindowRespond, WindowState == 1. Maybe not normal. Check the code.\n");
			TCPSerParaList.WindowState = 0;
		}
		else if(TCPSerParaList.WindowState == 2)//��������˳���������
		{
			TCPSerParaList.WindowState = 0;
			
/*��δ���ÿͻ��˶����һ�����ɹ����յķ������ʴ˶δ��벻���ܵõ�ִ�У���ʱ���ڳ�ʱ��ִ��
			//����������ɹ�
			if(TCPSerParaList.ThreadState >= 3)
			{
				TCPSerParaList.ThreadState = 4;
				printf("[END] File Transmission Succeed!!\n");
				break;
			}
*/
			
			if(TCPSerParaList.WindowLength * 2 <= TCPSerParaList.WindowThreshold)//�������׶�
				TCPSerParaList.WindowLength *= 2;
			else//ӵ������׶�
				TCPSerParaList.WindowLength ++;

			//���ⴰ�ڳ��ȳ�����������������
			/*
			if(TCPSerParaList.WindowLength > TCPSerParaList.BUFLength / TCPSerParaList.PacketDataLength / 2)
			{
				TCPSerParaList.WindowLength = TCPSerParaList.BUFLength / TCPSerParaList.PacketDataLength / 2;
			}
			*/
			//���ⴰ�ڳ��ȳ�����󴰿ڳ���
			if(TCPSerParaList.WindowLength > MAXWndLen)
			{
				TCPSerParaList.WindowLength = MAXWndLen;
			}

			//���´���ͷ��
			TCPSerParaList.OffsetWindowHead = TCPSerParaList.OffsetWindowEnd;
			TCPSerParaList.AddrWindowHead   = TCPSerParaList.AddrWindowEnd;
			//���´���β��
			TCPSerParaList.OffsetWindowEnd += TCPSerParaList.WindowLength * TCPSerParaList.PacketDataLength;
			TCPSerParaList.AddrWindowEnd   += TCPSerParaList.WindowLength * TCPSerParaList.PacketDataLength;
/*
			printf("[RENEW WND HEAD1]AddrWindowHead - BUF = %d\n",TCPSerParaList.AddrWindowHead - TCPSerParaList.BUF);
			printf("[RENEW WND TAIL1]OffsetWindowEnd     = %ld\n",TCPSerParaList.OffsetWindowEnd);
			printf("[RENEW WND TAIL1]AddrWindowEnd - BUF = %d\n",TCPSerParaList.AddrWindowEnd - TCPSerParaList.BUF);
*/
			//�������ػ�����
			if(TCPSerParaList.AddrWindowEnd >= TCPSerParaList.BUFEnd)
			{
				TCPSerParaList.AddrWindowEnd = TCPSerParaList.BUF + ( TCPSerParaList.AddrWindowEnd - TCPSerParaList.BUFEnd );
			}
		}
/*
		else if(TCPSerParaList.WindowState == 3)//����ACK����ȶ�����
		{printf("Treble ACK!\n");
			TCPSerParaList.WindowState = 0;
			
			//����״̬����
			if(TCPSerParaList.ThreadState >= 2)
			{
				TCPSerParaList.ThreadState = 1;
			}

			TCPSerParaList.WindowThreshold = TCPSerParaList.WindowLength / 2;//������ֵ����Ϊ��ǰ���ڴ�С��һ��
			
			TCPSerParaList.WindowLength = TCPSerParaList.WindowThreshold;//�Ѵ���ֵ����Ϊ����ֵ
			
			if(TCPSerParaList.WindowThreshold == 0)
				TCPSerParaList.WindowThreshold = 1;
			if(TCPSerParaList.WindowLength == 0)
				TCPSerParaList.WindowLength = 1;
			
			printf("TCPSerParaList.WindowThreshold = %ld\n",TCPSerParaList.WindowThreshold);

			//���´���ͷ��
			TCPSerParaList.OffsetWindowHead = TCPSerParaList.OffsetRequestACK;
			TCPSerParaList.AddrWindowHead   = TCPSerParaList.AddrWaitACK - ( TCPSerParaList.OffsetWaitACK - TCPSerParaList.OffsetRequestACK );

			//�������ػ�����
			if(TCPSerParaList.AddrWindowHead < TCPSerParaList.BUF)
			{
				TCPSerParaList.AddrWindowHead = TCPSerParaList.BUFEnd - ( TCPSerParaList.BUF - TCPSerParaList.AddrWindowHead );
			}

			//���´���β��
			TCPSerParaList.OffsetWindowEnd = TCPSerParaList.OffsetWindowHead + 
							 TCPSerParaList.WindowLength * TCPSerParaList.PacketDataLength;
			TCPSerParaList.AddrWindowEnd   = TCPSerParaList.AddrWindowHead   + 
							 TCPSerParaList.WindowLength * TCPSerParaList.PacketDataLength;

			//printf("[RENEW WND HEAD2]AddrWindowHead - BUF = %d\n",TCPSerParaList.AddrWindowHead - TCPSerParaList.BUF);
			//printf("[RENEW WND TAIL2]OffsetWindowEnd     = %ld\n",TCPSerParaList.OffsetWindowEnd);
			//printf("[RENEW WND TAIL2]AddrWindowEnd - BUF = %d\n",TCPSerParaList.AddrWindowEnd - TCPSerParaList.BUF);

			//�������ػ�����
			if(TCPSerParaList.AddrWindowEnd >= TCPSerParaList.BUFEnd)
			{
				TCPSerParaList.AddrWindowEnd = TCPSerParaList.BUF + ( TCPSerParaList.AddrWindowEnd - TCPSerParaList.BUFEnd );
			}
		}
*/
		else if(TCPSerParaList.WindowState == 3 || TCPSerParaList.WindowState == 4)//����ACK��ʱ���ض�ӵ����
		{//printf("Time Out!\n");
			TCPSerParaList.LostPacketCount++;//��¼��������
			
			TCPSerParaList.WindowState = 0;
			
			//����������ɹ�
			if(TCPSerParaList.ThreadState >= 3)
			{
				TCPSerParaList.ThreadState = 4;
				printf("[END] File Transmission Succeed!!\n");
				break;
			}

			//����״̬����
			else if(TCPSerParaList.ThreadState >= 2)
			{
				TCPSerParaList.ThreadState = 1;
			}

			TCPSerParaList.WindowThreshold = TCPSerParaList.WindowLength / 2;//������ֵ����Ϊ��ǰ���ڴ�С��һ��
			
			TCPSerParaList.WindowLength = 1;//�Ѵ��ڴ�С��������Ϊһ�����Ķ�

			if(TCPSerParaList.WindowThreshold == 0)
				TCPSerParaList.WindowThreshold = 1;

			//printf("TCPSerParaList.WindowThreshold = %ld\n",TCPSerParaList.WindowThreshold);
			
			//���´���ͷ��
			TCPSerParaList.OffsetWindowHead = TCPSerParaList.OffsetRequestACK;
			TCPSerParaList.AddrWindowHead   = TCPSerParaList.AddrWaitACK - ( TCPSerParaList.OffsetWaitACK - TCPSerParaList.OffsetRequestACK );

			//�������ػ�����
			if(TCPSerParaList.AddrWindowHead < TCPSerParaList.BUF)
			{
				TCPSerParaList.AddrWindowHead = TCPSerParaList.BUFEnd - ( TCPSerParaList.BUF - TCPSerParaList.AddrWindowHead );
			}

			//���´���β��
			TCPSerParaList.OffsetWindowEnd = TCPSerParaList.OffsetWindowHead + 
							 TCPSerParaList.WindowLength * TCPSerParaList.PacketDataLength;
			TCPSerParaList.AddrWindowEnd   = TCPSerParaList.AddrWindowHead   + 
							 TCPSerParaList.WindowLength * TCPSerParaList.PacketDataLength;
/*
			printf("[RENEW WND HEAD3]AddrWindowHead - BUF = %d\n",TCPSerParaList.AddrWindowHead - TCPSerParaList.BUF);
			printf("[RENEW WND TAIL3]OffsetWindowEnd     = %ld\n",TCPSerParaList.OffsetWindowEnd);
			printf("[RENEW WND TAIL3]AddrWindowEnd - BUF = %d\n",TCPSerParaList.AddrWindowEnd - TCPSerParaList.BUF);
*/
			//�������ػ�����
			if(TCPSerParaList.AddrWindowEnd >= TCPSerParaList.BUFEnd)
			{
				TCPSerParaList.AddrWindowEnd = TCPSerParaList.BUF + ( TCPSerParaList.AddrWindowEnd - TCPSerParaList.BUFEnd );
			}
		}
		else//��������WindowState����״ֵ̬�쳣
		{
			//printf("[Deadly Error] (WindowState == %d)\n",TCPSerParaList.WindowState);
			TCPSerParaList.LostPacketCount++;
			continue;
			//exit(0);
		}

		TCPSerParaList.OffsetPacket = TCPSerParaList.OffsetRequestACK;
		TCPSerParaList.AddrPacket   = TCPSerParaList.AddrWindowHead;
		//printf("[PARA CHANGE]AddrPacket - BUF = %d\n",TCPSerParaList.AddrPacket - TCPSerParaList.BUF);
		
		//��Ӧ�ò�֪ͨ�Թ�Ӧ�������ݵ�Ҫ��
		if( TCPSerParaList.OffsetRequestACK / ( TCPSerParaList.BUFLength / 2 ) == TCPSerParaList.BUFRenewCount + 1 )
		{
			sendto(TCPSerParaList.SENDTCPServerMoreData,"MoreData",20,0,(struct sockaddr *)&TCPSerParaList.SENDAddrToTCPServerMoreData,SockAddrLength);

			//printf("[%d] MoreData triggering...\n",TCPSerParaList.CountTCPServerMoreData_Triggering);
			TCPSerParaList.CountTCPServerMoreData_Triggering++;
		}
		else if( TCPSerParaList.OffsetRequestACK / ( TCPSerParaList.BUFLength / 2 ) > TCPSerParaList.BUFRenewCount + 1 )
		{
			printf("[Deadly Error]This is not normal. BUFRenewCount has problems to solve.\n1. Maybe a MoreData message has been lost.\n2. WindowLength is larger than BUFLength/2.\n");
			exit(0);
		}

		//��������ͨ�������հ���ָ��
		sendto(TCPSerParaList.SENDTCPServerContinueRECV,"ContinueRECV",20,0,(struct sockaddr *)&TCPSerParaList.SENDAddrToTCPServerContinueRECV,SockAddrLength);
		
		//printf("[%d] ContinueRECV triggering...\n",TCPSerParaList.CountTCPServerContinueRECV_Triggering);
		TCPSerParaList.CountTCPServerContinueRECV_Triggering++;

	}

	//sleep(100);
	printf("[DOWN]File Transport End.\n");
	printf("==========DATA Results==========\n");
	printf("Total Time = %ld seconds\n",TCPSerParaList.timer_XX);
	printf("Transport Rate = %.3f Byte/s\n",(double)FILELEN / (double)TCPSerParaList.timer_XX);
	printf("Average Window Length = %ld\n",TCPSerParaList.TotalWindowLength / (unsigned long)TCPSerParaList.InputCount);
	printf("TCPSerParaList.LostPacketCount  = %ld\n",TCPSerParaList.LostPacketCount);
	printf("TotalPacketCount = %ld\n",TCPSerParaList.FileLength / TCPSerParaList.PacketDataLength + 1);
	printf("Lost Rate = %.3f%%\n",((double)TCPSerParaList.LostPacketCount / ((double)TCPSerParaList.FileLength / (double)TCPSerParaList.PacketDataLength)) * (double)100);
	printf("================================\n");

	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�TCPServer_main
* ����������TCPServerģ�������������������̣߳������ṩʵ�ʹ���
* �����б��ڶ�����ʹ��char**argv�ķ�ʽ����������֪�ǲ����﷨����
* ���ؽ����
*****************************************/
//int main(int argc, char *argv[])
int
TCPServer_main
(
	int argc,
	char argv[][30]
)
{
	int i,j;
	
	TCPServer_Parameterinit();
	
	//�������ͼ�ʱ�����߳�
	pthread_t pthread_timer;
	if(pthread_create(&pthread_timer, NULL, TCPServer_thread_timer, NULL)!=0)
	{
		perror("Creation of timer thread failed.");
	}

	//����TCP�����߳�
	pthread_t pthread_TCP;
	if(pthread_create(&pthread_TCP, NULL, TCPServer_thread_TCP, NULL)!=0)
	{
		perror("Creation of TCP thread failed.");
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
