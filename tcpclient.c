/*******************************************************************************************************************************************
* �ļ�����tcpclient.c
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
gcc tcpclient.c -o tcpclient -lpthread
4�����У����漰ԭʼ�׽��ֵ�ʹ�ã���rootȨ�ޣ�
sudo ./tcpclient
*/

#include"tcpclient.h"

/*******************************************************************************************************************************************
*************************************�궨����������************ȫ�ֱ�������******************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************��ʼ�������������********************************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�TCPClient_Parameterinit
* ����������TCPClientģ��ȫ�ֱ�������ֵ
* �����б�
* ���ؽ����
*****************************************/
void
TCPClient_Parameterinit()
{
	SockAddrLength = sizeof(struct sockaddr_in);


	//��ʱ��ʼ
	TCPCliParaList.CountTCPClientTimingBegin_Triggering = 1;
	TCPCliParaList.CountTCPClientTimingBegin_Waiting = 1;
	TCPCliParaList.CountTCPClientTimingBegin_Triggered = 1;

	bzero(&TCPCliParaList.SENDAddrToTCPClientTimingBegin,SockAddrLength);
	TCPCliParaList.SENDAddrToTCPClientTimingBegin.sin_family=AF_INET;
	TCPCliParaList.SENDAddrToTCPClientTimingBegin.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.SENDAddrToTCPClientTimingBegin.sin_port=htons(TCPClientTimingBegin);

	bzero(&TCPCliParaList.RECVAddrToTCPClientTimingBegin,SockAddrLength);
	TCPCliParaList.RECVAddrToTCPClientTimingBegin.sin_family=AF_INET;
	TCPCliParaList.RECVAddrToTCPClientTimingBegin.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.RECVAddrToTCPClientTimingBegin.sin_port=htons(TCPClientTimingBegin);

	TCPCliParaList.SENDTCPClientTimingBegin = socket(AF_INET,SOCK_DGRAM,0);
	TCPCliParaList.RECVTCPClientTimingBegin = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPCliParaList.RECVTCPClientTimingBegin,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientTimingBegin,SockAddrLength);

	//��ʱ����
	TCPCliParaList.CountTCPClientTimingEnd_Triggering = 1;
	TCPCliParaList.CountTCPClientTimingEnd_Waiting = 1;
	TCPCliParaList.CountTCPClientTimingEnd_Triggered = 1;

	bzero(&TCPCliParaList.SENDAddrToTCPClientTimingEnd,SockAddrLength);
	TCPCliParaList.SENDAddrToTCPClientTimingEnd.sin_family=AF_INET;
	TCPCliParaList.SENDAddrToTCPClientTimingEnd.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.SENDAddrToTCPClientTimingEnd.sin_port=htons(TCPClientTimingEnd);

	bzero(&TCPCliParaList.RECVAddrToTCPClientTimingEnd,SockAddrLength);
	TCPCliParaList.RECVAddrToTCPClientTimingEnd.sin_family=AF_INET;
	TCPCliParaList.RECVAddrToTCPClientTimingEnd.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.RECVAddrToTCPClientTimingEnd.sin_port=htons(TCPClientTimingEnd);

	TCPCliParaList.SENDTCPClientTimingEnd = socket(AF_INET,SOCK_DGRAM,0);
	TCPCliParaList.RECVTCPClientTimingEnd = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPCliParaList.RECVTCPClientTimingEnd,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientTimingEnd,SockAddrLength);

	//������ʱ���
	TCPCliParaList.CountTCPClientTimingRespond_Triggering = 1;
	TCPCliParaList.CountTCPClientTimingRespond_Waiting = 1;
	TCPCliParaList.CountTCPClientTimingRespond_Triggered = 1;

	bzero(&TCPCliParaList.SENDAddrToTCPClientTimingRespond,SockAddrLength);
	TCPCliParaList.SENDAddrToTCPClientTimingRespond.sin_family=AF_INET;
	TCPCliParaList.SENDAddrToTCPClientTimingRespond.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.SENDAddrToTCPClientTimingRespond.sin_port=htons(TCPClientTimingRespond);

	bzero(&TCPCliParaList.RECVAddrToTCPClientTimingRespond,SockAddrLength);
	TCPCliParaList.RECVAddrToTCPClientTimingRespond.sin_family=AF_INET;
	TCPCliParaList.RECVAddrToTCPClientTimingRespond.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.RECVAddrToTCPClientTimingRespond.sin_port=htons(TCPClientTimingRespond);

	TCPCliParaList.SENDTCPClientTimingRespond = socket(AF_INET,SOCK_DGRAM,0);
	TCPCliParaList.RECVTCPClientTimingRespond = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPCliParaList.RECVTCPClientTimingRespond,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientTimingRespond,SockAddrLength);

	//���ջ�������
	TCPCliParaList.CountTCPClientBufReady_Triggering = 1;
	TCPCliParaList.CountTCPClientBufReady_Waiting = 1;
	TCPCliParaList.CountTCPClientBufReady_Triggered = 1;

	bzero(&TCPCliParaList.SENDAddrToTCPClientBufReady,SockAddrLength);
	TCPCliParaList.SENDAddrToTCPClientBufReady.sin_family=AF_INET;
	TCPCliParaList.SENDAddrToTCPClientBufReady.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.SENDAddrToTCPClientBufReady.sin_port=htons(TCPClientBufReady);

	bzero(&TCPCliParaList.RECVAddrToTCPClientBufReady,SockAddrLength);
	TCPCliParaList.RECVAddrToTCPClientBufReady.sin_family=AF_INET;
	TCPCliParaList.RECVAddrToTCPClientBufReady.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.RECVAddrToTCPClientBufReady.sin_port=htons(TCPClientBufReady);

	TCPCliParaList.SENDTCPClientBufReady = socket(AF_INET,SOCK_DGRAM,0);
	TCPCliParaList.RECVTCPClientBufReady = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPCliParaList.RECVTCPClientBufReady,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientBufReady,SockAddrLength);

	//���պ�������
	TCPCliParaList.CountTCPClientMoreData_Triggering = 1;
	TCPCliParaList.CountTCPClientMoreData_Waiting = 1;
	TCPCliParaList.CountTCPClientMoreData_Triggered = 1;
	
	bzero(&TCPCliParaList.SENDAddrToTCPClientMoreData,SockAddrLength);
	TCPCliParaList.SENDAddrToTCPClientMoreData.sin_family=AF_INET;
	TCPCliParaList.SENDAddrToTCPClientMoreData.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.SENDAddrToTCPClientMoreData.sin_port=htons(TCPClientMoreData);

	bzero(&TCPCliParaList.RECVAddrToTCPClientMoreData,SockAddrLength);
	TCPCliParaList.RECVAddrToTCPClientMoreData.sin_family=AF_INET;
	TCPCliParaList.RECVAddrToTCPClientMoreData.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.RECVAddrToTCPClientMoreData.sin_port=htons(TCPClientMoreData);

	TCPCliParaList.SENDTCPClientMoreData = socket(AF_INET,SOCK_DGRAM,0);
	TCPCliParaList.RECVTCPClientMoreData = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPCliParaList.RECVTCPClientMoreData,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientMoreData,SockAddrLength);

	//֪ͨ������д�뻺��
	TCPCliParaList.CountTCPClientWriteData_Triggering = 1;
	TCPCliParaList.CountTCPClientWriteData_Waiting = 1;
	TCPCliParaList.CountTCPClientWriteData_Triggered = 1;

	bzero(&TCPCliParaList.SENDAddrToTCPClientWriteData,SockAddrLength);
	TCPCliParaList.SENDAddrToTCPClientWriteData.sin_family=AF_INET;
	TCPCliParaList.SENDAddrToTCPClientWriteData.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.SENDAddrToTCPClientWriteData.sin_port=htons(TCPClientWriteData);

	bzero(&TCPCliParaList.RECVAddrToTCPClientWriteData,SockAddrLength);
	TCPCliParaList.RECVAddrToTCPClientWriteData.sin_family=AF_INET;
	TCPCliParaList.RECVAddrToTCPClientWriteData.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.RECVAddrToTCPClientWriteData.sin_port=htons(TCPClientWriteData);

	TCPCliParaList.SENDTCPClientWriteData = socket(AF_INET,SOCK_DGRAM,0);
	TCPCliParaList.RECVTCPClientWriteData = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPCliParaList.RECVTCPClientWriteData,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientWriteData,SockAddrLength);

	//�ļ�д�����
	TCPCliParaList.CountTCPClientFileClose_Triggering = 1;
	TCPCliParaList.CountTCPClientFileClose_Waiting = 1;
	TCPCliParaList.CountTCPClientFileClose_Triggered = 1;

	bzero(&TCPCliParaList.SENDAddrToTCPClientFileClose,SockAddrLength);
	TCPCliParaList.SENDAddrToTCPClientFileClose.sin_family=AF_INET;
	TCPCliParaList.SENDAddrToTCPClientFileClose.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.SENDAddrToTCPClientFileClose.sin_port=htons(TCPClientFileClose);

	bzero(&TCPCliParaList.RECVAddrToTCPClientFileClose,SockAddrLength);
	TCPCliParaList.RECVAddrToTCPClientFileClose.sin_family=AF_INET;
	TCPCliParaList.RECVAddrToTCPClientFileClose.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.RECVAddrToTCPClientFileClose.sin_port=htons(TCPClientFileClose);

	TCPCliParaList.SENDTCPClientFileClose = socket(AF_INET,SOCK_DGRAM,0);
	TCPCliParaList.RECVTCPClientFileClose = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPCliParaList.RECVTCPClientFileClose,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientFileClose,SockAddrLength);

	//����˳��1
	TCPCliParaList.CountTCPClientStartOrder1_Triggering = 1;
	TCPCliParaList.CountTCPClientStartOrder1_Waiting = 1;
	TCPCliParaList.CountTCPClientStartOrder1_Triggered = 1;
	
	bzero(&TCPCliParaList.SENDAddrToTCPClientStartOrder1,SockAddrLength);
	TCPCliParaList.SENDAddrToTCPClientStartOrder1.sin_family=AF_INET;
	TCPCliParaList.SENDAddrToTCPClientStartOrder1.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.SENDAddrToTCPClientStartOrder1.sin_port=htons(TCPClientStartOrder1);

	bzero(&TCPCliParaList.RECVAddrToTCPClientStartOrder1,SockAddrLength);
	TCPCliParaList.RECVAddrToTCPClientStartOrder1.sin_family=AF_INET;
	TCPCliParaList.RECVAddrToTCPClientStartOrder1.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.RECVAddrToTCPClientStartOrder1.sin_port=htons(TCPClientStartOrder1);

	TCPCliParaList.SENDTCPClientStartOrder1 = socket(AF_INET,SOCK_DGRAM,0);
	TCPCliParaList.RECVTCPClientStartOrder1 = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPCliParaList.RECVTCPClientStartOrder1,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientStartOrder1,SockAddrLength);

	//����˳��2
	TCPCliParaList.CountTCPClientStartOrder2_Triggering = 1;
	TCPCliParaList.CountTCPClientStartOrder2_Waiting = 1;
	TCPCliParaList.CountTCPClientStartOrder2_Triggered = 1;
	
	bzero(&TCPCliParaList.SENDAddrToTCPClientStartOrder2,SockAddrLength);
	TCPCliParaList.SENDAddrToTCPClientStartOrder2.sin_family=AF_INET;
	TCPCliParaList.SENDAddrToTCPClientStartOrder2.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.SENDAddrToTCPClientStartOrder2.sin_port=htons(TCPClientStartOrder2);

	bzero(&TCPCliParaList.RECVAddrToTCPClientStartOrder2,SockAddrLength);
	TCPCliParaList.RECVAddrToTCPClientStartOrder2.sin_family=AF_INET;
	TCPCliParaList.RECVAddrToTCPClientStartOrder2.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.RECVAddrToTCPClientStartOrder2.sin_port=htons(TCPClientStartOrder2);

	TCPCliParaList.SENDTCPClientStartOrder2 = socket(AF_INET,SOCK_DGRAM,0);
	TCPCliParaList.RECVTCPClientStartOrder2 = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPCliParaList.RECVTCPClientStartOrder2,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientStartOrder2,SockAddrLength);

	//����˳��3
	TCPCliParaList.CountTCPClientStartOrder3_Triggering = 1;
	TCPCliParaList.CountTCPClientStartOrder3_Waiting = 1;
	TCPCliParaList.CountTCPClientStartOrder3_Triggered = 1;
	
	bzero(&TCPCliParaList.SENDAddrToTCPClientStartOrder3,SockAddrLength);
	TCPCliParaList.SENDAddrToTCPClientStartOrder3.sin_family=AF_INET;
	TCPCliParaList.SENDAddrToTCPClientStartOrder3.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.SENDAddrToTCPClientStartOrder3.sin_port=htons(TCPClientStartOrder3);

	bzero(&TCPCliParaList.RECVAddrToTCPClientStartOrder3,SockAddrLength);
	TCPCliParaList.RECVAddrToTCPClientStartOrder3.sin_family=AF_INET;
	TCPCliParaList.RECVAddrToTCPClientStartOrder3.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.RECVAddrToTCPClientStartOrder3.sin_port=htons(TCPClientStartOrder3);

	TCPCliParaList.SENDTCPClientStartOrder3 = socket(AF_INET,SOCK_DGRAM,0);
	TCPCliParaList.RECVTCPClientStartOrder3 = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPCliParaList.RECVTCPClientStartOrder3,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientStartOrder3,SockAddrLength);

	//����˳��4
	TCPCliParaList.CountTCPClientStartOrder4_Triggering = 1;
	TCPCliParaList.CountTCPClientStartOrder4_Waiting = 1;
	TCPCliParaList.CountTCPClientStartOrder4_Triggered = 1;
	
	bzero(&TCPCliParaList.SENDAddrToTCPClientStartOrder4,SockAddrLength);
	TCPCliParaList.SENDAddrToTCPClientStartOrder4.sin_family=AF_INET;
	TCPCliParaList.SENDAddrToTCPClientStartOrder4.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.SENDAddrToTCPClientStartOrder4.sin_port=htons(TCPClientStartOrder4);

	bzero(&TCPCliParaList.RECVAddrToTCPClientStartOrder4,SockAddrLength);
	TCPCliParaList.RECVAddrToTCPClientStartOrder4.sin_family=AF_INET;
	TCPCliParaList.RECVAddrToTCPClientStartOrder4.sin_addr.s_addr=inet_addr(LOOPIP);
	TCPCliParaList.RECVAddrToTCPClientStartOrder4.sin_port=htons(TCPClientStartOrder4);

	TCPCliParaList.SENDTCPClientStartOrder4 = socket(AF_INET,SOCK_DGRAM,0);
	TCPCliParaList.RECVTCPClientStartOrder4 = socket(AF_INET,SOCK_DGRAM,0);

	bind(TCPCliParaList.RECVTCPClientStartOrder4,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientStartOrder4,SockAddrLength);

}

/*******************************************************************************************************************************************
*******************************************Ӧ�ò�TCPClient��������ͨ��CLI���ƣ�**************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.���������   To.Ӧ�÷�����********************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.Ӧ�÷����� To.���������**********************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
**********************************************************���߳����ɳ���********************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�TCPClient_thread_timer
* ����������Ϊ���߳��ṩ��ʱ�ź�
* �����б�
* ���ؽ����
*****************************************/
void *
TCPClient_thread_timer
(
	void * fd
)
{
	int timer_XX = 0;
/*
	while(1)
	{
		sleep(SLEEP);
		
		time(&GMT);//��ȡGMT����ֵ��GMT
		UTC = localtime(&GMT);//GMT-UTCת��
		printf("GMT(long)   %ld\n",GMT);//[GMT]���1970.1.1.00:00:00��������
		printf("UTC(human)  %s",asctime(UTC));//[UTC]����ַ�������ɶ�ʱ��
	}
*/

	time(&GMT);//��ȡGMT����ֵ��GMT
	StartTime = GMT;

	//�ر��߳�
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�TCPClient_thread_TCP
* ��������������ͨ�� - TCP�ͻ���������������߳�
* �����б�
* ���ؽ����
*****************************************/
void *
TCPClient_thread_TCP
(
	void * fd
)
{
	sleep(1);
	int i,j;

	uint8_t message[DATALENst+SIDLENst+OFFLEN+20];
	memset(message,0,DATALENst+SIDLENst+OFFLEN+20);

	uint8_t SID[SIDLEN];
	memset(SID,0,SIDLEN);
	int SIDlen=0;
	
	uint8_t DATA[DATALEN];
	memset(DATA,0,DATALEN);
	int DATAlen;

	uint8_t REQoffset[OFFLEN];
	memset(REQoffset,0,OFFLEN);
	unsigned long REQoffsetlong;

	uint8_t RESoffset[OFFLEN];
	memset(RESoffset,0,OFFLEN);
	unsigned long RESoffsetlong;

	//������ʼ��
	memset(TCPCliParaList.SID,0,SIDLEN);//�����������SID
	TCPCliParaList.SIDLen = 0;//SID��ʵ�ʳ���

	TCPCliParaList.BUF = NULL;//���ͻ�������ͷ
	TCPCliParaList.BUFEnd = NULL;//���ͻ�������β
	TCPCliParaList.BUFFileEnd = NULL;//��ȡ�ļ�����ʱ�����������ļ�ĩβ���ڵ�ַ

	TCPCliParaList.BUFRenewCount = 0;//���������´���
	TCPCliParaList.BUFLength = TCPBUFSIZE;//���ͻ��峤��

	TCPCliParaList.FileWritePoint = 0;//�ļ�д�����
	TCPCliParaList.FileLength = 0;//�ļ��ܳ���

	TCPCliParaList.PacketDataLength = PKTDATALEN;//�������ݶγ���
	TCPCliParaList.LastPacketDataLength = 0;//����������������һ�����ݰ�ʵ�ʳ���

	TCPCliParaList.OffsetConfirmDATA = 0;//�������ļ���ȷ���յ����ֽڶ���
	TCPCliParaList.OffsetPacket = 0;//�������ļ��е�ǰ����ֽ���
	TCPCliParaList.OffsetWaitDATA = PKTDATALEN;//�������ļ��еȴ�ȷ���յ����ֽ���������ACK���ݰ�ƫ�����ֶε���д���ݣ�

	TCPCliParaList.AddrConfirmDATA = NULL;//�ڻ���������ȷ��ƫ�������ڵ�ַ
	TCPCliParaList.AddrPacket = NULL;//�ڻ������е�ǰ���ƫ�������ڵ�ַ
	TCPCliParaList.AddrWaitDATA = NULL;//�ڻ������д�ȷ��ƫ�������ڵ�ַ

	TCPCliParaList.RTOs = 0;//��������ʱ�ӣ�RTO����1�벿�֣���λ�룩
	TCPCliParaList.RTOus = 0;//��������ʱ�ӣ�RTOС��1�벿�֣���λ΢�룩
	TCPCliParaList.RTOns = 0;//��������ʱ�ӣ�RTOС��1�벿�֣���λ���룩

	TCPCliParaList.FlagInputBUF = 0;//�Ƿ��յ�������д�뻺��

	TCPCliParaList.TimingState = 0;//��ʱ״̬��0��ʱ��1��ʱ
	TCPCliParaList.ThreadState = 0;//����״̬�����ڽ������̵Ŀ��ƣ�

printf("xxx waiting...\n");
printf("xxx triggering...\n");
printf("xxx triggered!\n");

	//����FTP�߳̿�ʼ
	sendto(TCPCliParaList.SENDTCPClientStartOrder1,"StartOrder1",20,0,(struct sockaddr *)&TCPCliParaList.SENDAddrToTCPClientStartOrder1,SockAddrLength);
	
	//printf("[%d] StartOrder1 triggering...\n",TCPCliParaList.CountTCPClientStartOrder1_Triggering);
	TCPCliParaList.CountTCPClientStartOrder1_Triggering++;

	//����TCP�����߳̿�ʼ
	//printf("[%d] StartOrder3 waiting...\n",TCPCliParaList.CountTCPClientStartOrder3_Waiting);
	TCPCliParaList.CountTCPClientStartOrder3_Waiting++;
	
	recvfrom(TCPCliParaList.RECVTCPClientStartOrder3,Trigger,20,0,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientStartOrder3,&SockAddrLength);
	
	//printf("[%d] StartOrder3 triggered!\n",TCPCliParaList.CountTCPClientStartOrder3_Triggered);
	TCPCliParaList.CountTCPClientStartOrder3_Triggered++;

	//���������߳̿�ʼ
	sendto(TCPCliParaList.SENDTCPClientStartOrder4,"StartOrder4",20,0,(struct sockaddr *)&TCPCliParaList.SENDAddrToTCPClientStartOrder4,SockAddrLength);
	
	//printf("[%d] StartOrder4 triggering...\n",TCPCliParaList.CountTCPClientStartOrder4_Triggering);
	TCPCliParaList.CountTCPClientStartOrder4_Triggering++;

	//ѭ������������Դ�ɹ�������
	int socket_datareceiver;
	socket_datareceiver = socket(AF_INET,SOCK_DGRAM,0);

	struct sockaddr_in sin;
	int sockaddr_len=SockAddrLength;
	bzero(&sin,sockaddr_len);
	sin.sin_family=AF_INET;
	//sin.sin_addr.s_addr=htonl(INADDR_ANY);
	sin.sin_addr.s_addr=inet_addr(LOOPIP);
	sin.sin_port=htons(TRALtoAPPL);

	bind(socket_datareceiver,(struct sockaddr *)&sin,sockaddr_len);
		
	int flag_sidgot = 0;
	int counter_sidget = 0;

	if(DEVETESTIMPL <= 1)
		printf("[Transport Layer] TCPClient responce listening start...\n");
	
	uint8_t recvpkg[sizeof(Trans_data)];
	Trans_data * precvpkg;
	precvpkg = (Trans_data *)recvpkg;

	int FoundSIDNum = -1;
	while(1)
	{
		i=0;
		j=0;

		//��ջ��壬Ϊ����������׼��
		DATAlen = 0;
		REQoffsetlong = 0;
		RESoffsetlong = 0;
		memset(DATA,0,DATALEN);
		memset(recvpkg,0,sizeof(Trans_data));

		FoundSIDNum = -1;

		//����
		recvfrom(socket_datareceiver,recvpkg,sizeof(Trans_data),0,(struct sockaddr *)&sin,&sockaddr_len);

		if(transDEVETESTIMPL <= 1)
			printf("[Transport Layer] TCPClient responce listener received a new UDP message.\n");
		
		//��ȡDATA��offset
		memcpy(DATA,precvpkg->data,DATALEN);
		memcpy(RESoffset,precvpkg->offset,OFFLEN);
		DATAlen = DATALEN;

		//�յ������ݳ����ۻ�����Ԥ֪�ļ����ȣ��ݴ���ƣ����ǽ������ƣ����Գ����ж�������Ҫ���ڽ���������
		if((long)TCPCliParaList.OffsetConfirmDATA - (long)DATALEN >= FILELEN)//ǿ��long��ת���Ǳ�������������������
		{
			printf("Received File Length   = %ld\n",(long)TCPCliParaList.OffsetConfirmDATA - (long)DATALEN);
			printf("File Length Ever known = %ld\n",(unsigned long)FILELEN);
			printf("[Deadly Error] Received File Length is Bigger Than the File Length Ever known.\n");
			
			TCPCliParaList.FlagInputBUF = 0;
			sendto(TCPCliParaList.SENDTCPClientTimingEnd,"TimingEnd",20,0,(struct sockaddr *)&TCPCliParaList.SENDAddrToTCPClientTimingEnd,SockAddrLength);

			printf("[%d] TimingEnd triggering...\n",TCPCliParaList.CountTCPClientTimingEnd_Triggering);
			TCPCliParaList.CountTCPClientTimingEnd_Triggering++;
		}
	
		if(transDEVETESTIMPL <= 5)
		{
			printf("[DATAlen]\n%d\n",DATAlen);
			printf("[DATA]\n%s\n",DATA);
			printf("[DATA-END]\n");
		}

		//����TCP��������нӿ����ã��ӿ���Ϣ��ʽGOTdataOFFoffset
		//�˴�ȱ�ٶ�������ȷ�Ե�У��
		
		//�ж�OFFSET�ĺϷ��ԣ�������Ҫ�������
		if(RESoffset[0] >= 0 && RESoffset[1] >= 0 && RESoffset[2] >= 0 && RESoffset[3] >= 0)
		{
			RESoffsetlong = (unsigned long)RESoffset[0] * 256 * 256 * 256 + 
					(unsigned long)RESoffset[1] * 256 * 256 + 
					(unsigned long)RESoffset[2] * 256 + 
					(unsigned long)RESoffset[3];
			
			//������SID��Ӧ�������̱߳��
			if(strncmp(TCPCliParaList.SID,SID,SIDLEN) == 0)
			{
				FoundSIDNum = 0;
			}
			if(FoundSIDNum == -1)
			{
				printf("An unknown ACK Packet received, SID not found.\n");
				exit(0);
			}

			TCPCliParaList.SIDLen = SIDlen;
			//if(RESoffsetlong >= 10000)exit(0);
			
			if(transDEVETESTIMPL <= 5)
			{
				printf("[RECV]RESoffsetlong  = %ld\n",RESoffsetlong);
				printf("[RECV]OffsetWaitDATA = %ld\n",TCPCliParaList.OffsetWaitDATA);
			}
			
			if(RESoffsetlong == TCPCliParaList.OffsetWaitDATA)//�յ���ǰ�������ƫ������д����ջ��壬����ȷ��ƫ����ֵ
			{
				//printf("New Data Input!\n");
				//�򻺳��������ո��յ��������ֶ�
				
				//�յ����ݶγ��ȴ��ڱ���TCP�涨�ı�׼�������ݶγ��ȣ�
				//��ζ���ļ���δ�������ļ���СǡΪPacketDataLength��������������⣩
				if(DATAlen == TCPCliParaList.PacketDataLength)
				{
					//���յ������ݿ���������
					memcpy(TCPCliParaList.AddrConfirmDATA,DATA,TCPCliParaList.PacketDataLength);

					//�ƶ����ඨλ��ʶ
					TCPCliParaList.OffsetConfirmDATA = RESoffsetlong;
					TCPCliParaList.OffsetWaitDATA    = RESoffsetlong + DATAlen;
					TCPCliParaList.AddrConfirmDATA  += TCPCliParaList.PacketDataLength;

					//�������ػ�����
					if(TCPCliParaList.AddrConfirmDATA >= TCPCliParaList.BUFEnd)
					{
						TCPCliParaList.AddrConfirmDATA = TCPCliParaList.BUF + ( TCPCliParaList.AddrConfirmDATA - TCPCliParaList.BUFEnd );
					}
				}
				//�յ����ݶγ���С�ڱ���TCP�涨�ı�׼�������ݶγ��ȣ�
				//��ζ�������ļ������һ������Ӧ���������ļ����ɺͽ�������Ĵ���
				/*
				else if(DATAlen < TCPCliParaList.PacketDataLength)
				{
					//���յ������ݿ���������
					memcpy(TCPCliParaList.AddrConfirmDATA,DATA,DATAlen);
					
					//�ƶ����ඨλ��ʶ
					TCPCliParaList.OffsetWaitDATA = RESoffsetlong + DATAlen;
					TCPCliParaList.AddrConfirmDATA += TCPCliParaList.PacketDataLength;

					//֪ͨ���ļ�д������
					sendto(TCPCliParaList.SENDTCPClientWriteData,"WriteData",20,0,(struct sockaddr *)&TCPCliParaList.SENDAddrToTCPClientWriteData,SockAddrLength);

					printf("[%d] WriteData triggering...\n",TCPCliParaList.CountTCPClientWriteData_Triggering);
					TCPCliParaList.CountTCPClientWriteData_Triggering++;
				}
				*/
				//�յ����ݶγ��Ȳ����ڱ���TCP�涨�ı�׼�������ݶγ��ȣ�
				//��������Э�������ƥ�䣬��Ҫ�޸�����
				else
				{
					printf("Deadly error!(DATAlen == %d > TCPCliParaList.PacketDataLength)\n",DATAlen);
					exit(0);
				}

				TCPCliParaList.FlagInputBUF = 1;
				
				//֪ͨ������ʱ
				sendto(TCPCliParaList.SENDTCPClientTimingEnd,"TimingEnd",20,0,(struct sockaddr *)&TCPCliParaList.SENDAddrToTCPClientTimingEnd,SockAddrLength);

				//printf("[%d] TimingEnd triggering...\n",TCPCliParaList.CountTCPClientTimingEnd_Triggering);
				TCPCliParaList.CountTCPClientTimingEnd_Triggering++;
			}
			else if(RESoffsetlong != TCPCliParaList.OffsetWaitDATA)//�յ��ǵ�ǰ�������ƫ�������ظ����͵�ǰ�������ƫ����
			{
				TCPCliParaList.FlagInputBUF = 0;

				sendto(TCPCliParaList.SENDTCPClientTimingEnd,"TimingEnd",20,0,(struct sockaddr *)&TCPCliParaList.SENDAddrToTCPClientTimingEnd,SockAddrLength);

				//printf("[%d] TimingEnd triggering...\n",TCPCliParaList.CountTCPClientTimingEnd_Triggering);
				TCPCliParaList.CountTCPClientTimingEnd_Triggering++;
			}
		}
		else//offsetֵ�Ƿ�
		{
			//����ACK�ش�
			TCPCliParaList.FlagInputBUF = 0;

			sendto(TCPCliParaList.SENDTCPClientTimingEnd,"TimingEnd",20,0,(struct sockaddr *)&TCPCliParaList.SENDAddrToTCPClientTimingEnd,SockAddrLength);

			//printf("[%d] TimingEnd triggering...\n",TCPCliParaList.CountTCPClientTimingEnd_Triggering);
			TCPCliParaList.CountTCPClientTimingEnd_Triggering++;
		}
	}
	
	close(socket_datareceiver);


	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);	
}

/*****************************************
* �������ƣ�TCPClient_thread_FTP
* ������������ȡ�ļ��̣߳�FTPǰ���������ȡ�ļ���ά�ַ��ͻ���ĸ���
* �����б�
* ���ؽ����
*****************************************/
void *
TCPClient_thread_FTP
(
	void * fd
)
{
	//��ȡ������
	//int ThreadNum = *((int *)fd);printf("[FTP]ThreadNum = %d\n",ThreadNum);
	
	//����FTP�߳̿�ʼ
	//printf("[%d] StartOrder1 waiting...\n",TCPCliParaList.CountTCPClientStartOrder1_Waiting);
	TCPCliParaList.CountTCPClientStartOrder1_Waiting++;

	recvfrom(TCPCliParaList.RECVTCPClientStartOrder1,Trigger,20,0,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientStartOrder1,&SockAddrLength);

	//printf("[%d] StartOrder1 triggered!\n",TCPCliParaList.CountTCPClientStartOrder1_Triggered);
	TCPCliParaList.CountTCPClientStartOrder1_Triggered++;

	//Ϊ����������ջ����ڴ�
	TCPCliParaList.BUF = (uint8_t *)malloc(TCPBUFSIZE);
	memset(TCPCliParaList.BUF,0,TCPBUFSIZE);
	
	//������ؿ��Ʋ���
	TCPCliParaList.BUFEnd = TCPCliParaList.BUF + TCPBUFSIZE;//���ͻ�������β

	TCPCliParaList.BUFRenewCount = 0;//���������´���
	TCPCliParaList.BUFLength = TCPBUFSIZE;//���ͻ��峤��

	TCPCliParaList.FileWritePoint = 0;//�ļ�д�����
	TCPCliParaList.FileLength = 0;//�ļ��ܳ���

	TCPCliParaList.PacketDataLength = PKTDATALEN;//�������ݶγ���
	TCPCliParaList.LastPacketDataLength = 0;//����������������һ�����ݰ�ʵ�ʳ���
	
	TCPCliParaList.AddrConfirmDATA = TCPCliParaList.BUF;//�ڻ���������ȷ��ƫ�������ڵ�ַ
	TCPCliParaList.AddrPacket = TCPCliParaList.BUF;//�ڻ������е�ǰ���ƫ�������ڵ�ַ
	TCPCliParaList.AddrWaitDATA = TCPCliParaList.BUF;//�ڻ������д�ȷ��ƫ�������ڵ�ַ

	//���������Ŷ�Ӧ�Ĳ�����SIDѰ�Ҷ�Ӧ���ļ�
	
	
	//���ļ�����ȡ�������
	int fp;
	unsigned long filelen = FILELEN;//debuglog.txt���ļ���С
	
	//�������򿪿ͻ��˽����ļ�
	fp = open(FTPClientPath,O_RDWR | O_CREAT | O_APPEND | O_TRUNC);
	if(fp == -1)
	{
		printf("[Deadly Error] The Client File \"%s\" opening failed.\n",FTPClientPath);
		exit(0);
	}
	
	//֪ͨ����㣬���ͻ����Ѿ�׼����������ʱ���ã�
	sendto(TCPCliParaList.SENDTCPClientBufReady,"BufReady",20,0,(struct sockaddr *)&TCPCliParaList.SENDAddrToTCPClientBufReady,SockAddrLength);

	//printf("[%d] BufReady triggering...\n",TCPCliParaList.CountTCPClientBufReady_Triggering);
	TCPCliParaList.CountTCPClientBufReady_Triggering++;

	//�������м�ʱ�߳̿�ʼ
	//sendto(TCPCliParaList.SENDTCPClientStartOrder2,"StartOrder2",20,0,(struct sockaddr *)&TCPCliParaList.SENDAddrToTCPClientStartOrder2,SockAddrLength);

	//printf("[%d] StartOrder2 triggering...\n",TCPCliParaList.CountTCPClientStartOrder2_Triggering);
	
	//��ʼ����Ϊ����㹩Ӧ����
	while(1)
	{
		//printf("[%d] WriteData waiting...\n",TCPCliParaList.CountTCPClientWriteData_Waiting);
		TCPCliParaList.CountTCPClientWriteData_Waiting++;

		//�����ȴ�����㴥�����ϴ����ݵ�Ҫ��
		recvfrom(TCPCliParaList.RECVTCPClientWriteData,Trigger,20,0,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientWriteData,&SockAddrLength);
		
		//printf("[%d] WriteData triggered!\n",TCPCliParaList.CountTCPClientWriteData_Triggered);
		TCPCliParaList.CountTCPClientWriteData_Triggered++;

		TCPCliParaList.BUFRenewCount++;
		
		//һ���ϴ����ջ�����һ������ݣ�ǰ��κͺ��ν����ϴ�
		if(TCPCliParaList.BUFRenewCount % 2 == 1)//�ж��ϴ�ǰ���
		{
			//��黺��ʣ�೤���Ƿ��㹻һ���ϴ�
			if(TCPCliParaList.FileWritePoint + TCPCliParaList.BUFLength/2 > filelen)//����һ���ϴ������¼β����ʶ���ϴ���������
			{
				write(fp,TCPCliParaList.BUF,filelen - TCPCliParaList.FileWritePoint);
				
				TCPCliParaList.BUFFileEnd = TCPCliParaList.BUF + ( filelen - TCPCliParaList.FileWritePoint );
				TCPCliParaList.ThreadState = 1;
				break;
			}
			else//�㹻һ���ϴ������ϴ�
			{
				write(fp,TCPCliParaList.BUF,TCPCliParaList.BUFLength/2);
				
				TCPCliParaList.FileWritePoint += TCPCliParaList.BUFLength/2;
				//printf("TCPCliParaList.FileWritePoint = %ld\n",TCPCliParaList.FileWritePoint);
			}
		}
		else if(TCPCliParaList.BUFRenewCount % 2 == 0)//�ж��ϴ�����
		{
			//����ļ�ʣ�೤���Ƿ��㹻һ���ϴ�
			if(TCPCliParaList.FileWritePoint + TCPCliParaList.BUFLength/2 > filelen)//����һ���ϴ������¼β����ʶ���ϴ���������
			{
				//write(fp,TCPCliParaList.BUF + TCPCliParaList.BUFLength/2,filelen - TCPCliParaList.FileWritePoint);
				write(fp,TCPCliParaList.BUF + TCPCliParaList.BUFLength/2,filelen - TCPCliParaList.FileWritePoint);
				
				TCPCliParaList.BUFFileEnd = TCPCliParaList.BUF + TCPCliParaList.BUFLength/2 + ( filelen - TCPCliParaList.FileWritePoint );
				TCPCliParaList.ThreadState = 1;
				break;
			}
			else//�㹻һ���ϴ������ϴ�
			{
				write(fp,TCPCliParaList.BUF + TCPCliParaList.BUFLength/2,TCPCliParaList.BUFLength/2);
				
				TCPCliParaList.FileWritePoint += TCPCliParaList.BUFLength/2;
				//printf("TCPCliParaList.FileWritePoint = %ld\n",TCPCliParaList.FileWritePoint);
			}
		}
		printf("Data already received: %ld Byte\n",TCPCliParaList.FileWritePoint);
	}

	//������ͨ��֪ͨ�ļ�д�����
	sendto(TCPCliParaList.SENDTCPClientFileClose,"FileClose",20,0,(struct sockaddr *)&TCPCliParaList.SENDAddrToTCPClientFileClose,SockAddrLength);

	//printf("[%d] FileClose triggering...\n",TCPCliParaList.CountTCPClientFileClose_Triggering);
	TCPCliParaList.CountTCPClientFileClose_Triggering++;
	
	printf("[FTP]File Transport End.\n");

	//�������������ܹ���������
	time(&GMT);//��ȡGMT����ֵ��GMT
	EndTime = GMT;
	UsedTime = EndTime - StartTime;
	printf("Total Transport Delay   %ld\n",UsedTime);//������������ܹ���������

	exit(0);
	close(fp);
}

/*****************************************
* �������ƣ�TCPClient_thread_NetworkLayertoTransportLayer
* ��������������ͨ�� - From.���������   To.Ӧ�÷�����  �ṩ��ʱ
* �����б�
* ���ؽ����
*****************************************/
void *
TCPClient_thread_NetworkLayertoTransportLayer
(
	void * fd
)
{
	//��ȡ������
	//int ThreadNum = *((int *)fd);printf("[UP]ThreadNum = %d\n",ThreadNum);

	//�������м�ʱ�߳̿�ʼ
	//printf("[%d] BufReady waiting...\n",TCPCliParaList.CountTCPClientBufReady_Waiting);
	TCPCliParaList.CountTCPClientBufReady_Waiting++;
	
	recvfrom(TCPCliParaList.RECVTCPClientBufReady,Trigger,20,0,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientBufReady,&SockAddrLength);
	
	//printf("[%d] BufReady triggered!\n",TCPCliParaList.CountTCPClientBufReady_Triggered);
	TCPCliParaList.CountTCPClientBufReady_Triggered++;
	
	//printf("[%d] StartOrder2 waiting...\n",TCPCliParaList.CountTCPClientStartOrder2_Waiting);
	//TCPCliParaList.CountTCPClientStartOrder2_Waiting++;
	
	//recvfrom(TCPCliParaList.RECVTCPClientStartOrder2,Trigger,20,0,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientStartOrder2,&SockAddrLength);
	
	//printf("[%d] StartOrder2 triggered!\n",TCPCliParaList.CountTCPClientStartOrder2_Triggered);
	//TCPCliParaList.CountTCPClientStartOrder2_Triggered++;
	
	//��ʱ����ز�����ʼ��
	TCPCliParaList.RTOs = RTOS;//��������ʱ�ӣ�RTO����1�벿�֣���λ�룩
	TCPCliParaList.RTOus = RTOUS;//��������ʱ�ӣ�RTOС��1�벿�֣���λ΢�룩
	TCPCliParaList.RTOns = RTONS;//��������ʱ�ӣ�RTOС��1�벿�֣���λ���룩
	time_t GMT;//���ʱ�׼ʱ�䣬ʵ����time_t�ṹ(typedef long time_t;)
	struct timespec delaytime;

	//����TCP�����߳̿�ʼ
	sendto(TCPCliParaList.SENDTCPClientStartOrder3,"StartOrder3",20,0,(struct sockaddr *)&TCPCliParaList.SENDAddrToTCPClientStartOrder3,SockAddrLength);

	//printf("[%d] StartOrder3 triggering...\n",TCPCliParaList.CountTCPClientStartOrder3_Triggering);
	TCPCliParaList.CountTCPClientStartOrder3_Triggering++;

	while(1)
	{
		//��������
		TCPCliParaList.TimingState = 0;

		//printf("[%d] TimingBegin waiting...\n",TCPCliParaList.CountTCPClientTimingBegin_Waiting);
		TCPCliParaList.CountTCPClientTimingBegin_Waiting++;
		
		//�����ȴ���ʱ����
		recvfrom(TCPCliParaList.RECVTCPClientTimingBegin,Trigger,20,0,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientTimingBegin,&SockAddrLength);

		//printf("[%d] TimingBegin triggered!\n",TCPCliParaList.CountTCPClientTimingBegin_Triggered);
		TCPCliParaList.CountTCPClientTimingBegin_Triggered++;
		
		//��ʱ�������ڴ�������ͨ��������ʱ��ʾ

		//printf("[%d] TimingEnd waiting...\n",TCPCliParaList.CountTCPClientTimingEnd_Waiting);
		TCPCliParaList.CountTCPClientTimingEnd_Waiting++;

		//����ʱ��UDP��Ϣ����
		//���ó�ʱ
		struct timeval delaytime;
		delaytime.tv_sec=TCPCliParaList.RTOs;
		delaytime.tv_usec=TCPCliParaList.RTOus;

		//��¼sockfd
		fd_set readfds;//�Ѹ��ú��������ⲿ����ָ��
		FD_ZERO(&readfds);
		FD_SET(TCPCliParaList.RECVTCPClientTimingEnd,&readfds);

		select(TCPCliParaList.RECVTCPClientTimingEnd+1,&readfds,NULL,NULL,&delaytime);
	
		//��ʱ����
		if(FD_ISSET(TCPCliParaList.RECVTCPClientTimingEnd,&readfds))
		{
			if(0 <= recvfrom(TCPCliParaList.RECVTCPClientTimingEnd,Trigger,20,0,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientTimingEnd,&SockAddrLength))
			{
				if(transDEVETESTIMPL <= 0)
					printf("[TimingEndDelay]  %.3f ms\n",(double)(TCPCliParaList.RTOus - (int)delaytime.tv_usec)/(double)1000);
			}
		}
		else
		{
			if(transDEVETESTIMPL <= 0)
				printf("[TimingEndDelay]  %.3f ms   (Timeout)\n",(double)(TCPCliParaList.RTOus - (int)delaytime.tv_usec)/(double)1000);
		}

		//printf("[%d] TimingEnd triggered!\n",TCPCliParaList.CountTCPClientTimingEnd_Triggered);
		TCPCliParaList.CountTCPClientTimingEnd_Triggered++;

		//֪ͨ����ͨ�����������η�����ʱ�ѵó����
		sendto(TCPCliParaList.SENDTCPClientTimingRespond,"TimingRespond",20,0,(struct sockaddr *)&TCPCliParaList.SENDAddrToTCPClientTimingRespond,SockAddrLength);

		//printf("[%d] TimingRespond triggering...\n",TCPCliParaList.CountTCPClientTimingRespond_Triggering);
		TCPCliParaList.CountTCPClientTimingRespond_Triggering++;
	}
	
	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�TCPClient_thread_TransportLayertoNetworkLayer
* ��������������ͨ�� - From.Ӧ�÷����� To.���������
* �����б�
* ���ؽ����
*****************************************/
void *
TCPClient_thread_TransportLayertoNetworkLayer
(
	void * fd
)
{
	//��ȡ������
	//int ThreadNum = *((int *)fd);printf("[DOWN]ThreadNum = %d\n",ThreadNum);

	//���������߳̿�ʼ
	//printf("[%d] StartOrder4 waiting...\n",TCPCliParaList.CountTCPClientStartOrder4_Waiting);
	TCPCliParaList.CountTCPClientStartOrder4_Waiting++;

	recvfrom(TCPCliParaList.RECVTCPClientStartOrder4,Trigger,20,0,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientStartOrder4,&SockAddrLength);

	//printf("[%d] StartOrder4 triggered!\n",TCPCliParaList.CountTCPClientStartOrder4_Triggered);
	TCPCliParaList.CountTCPClientStartOrder4_Triggered++;

	//����������һ��SID�ļ�
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
	bzero(&addrTo,SockAddrLength);
	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons(APPLtoTRAL);
	//unsigned long IPto = 2130706433;//�ػ���ַ���� == 2130706433
	//addrTo.sin_addr.s_addr=htonl(IPto);//htonl�������ֽ���ת��Ϊ�����ֽ���
	addrTo.sin_addr.s_addr=inet_addr(LOOPIP);//htonl�������ֽ���ת��Ϊ�����ֽ���
	
	//��ֵSID 
	strcpy(SID,"wangzhaoxu");
	if(transDEVETESTIMPL <= 0)
		printf("\n[SID  ]  %s\n",SID);
	
	uint8_t offset[OFFLEN];
	unsigned long filelen = FILELEN;//debuglog.txt���ļ���С

	uint8_t sendpkg[sizeof(Trans_get)];
	Trans_get * psendpkg;
	psendpkg = (Trans_get *)sendpkg;

	while(TCPCliParaList.OffsetConfirmDATA <= filelen)
	{
		memset(sendpkg,0,sizeof(Trans_get));

		if(TCPCliParaList.TimingState == 0)//δ��ʱ
		{
			TCPCliParaList.OffsetWaitDATA += 0;
		}
		else if(TCPCliParaList.TimingState == 1)//��ʱ
		{
		}
		//printf("[Down]OffsetWaitDATA = %ld\n",TCPCliParaList.OffsetWaitDATA);
		
		//�����ȷ�ϵ�ƫ����ֵ
		offset[3] = TCPCliParaList.OffsetWaitDATA % 256;
		offset[2] = TCPCliParaList.OffsetWaitDATA / 256 % 256;
		offset[1] = TCPCliParaList.OffsetWaitDATA / 256 / 256 % 256;
		offset[0] = TCPCliParaList.OffsetWaitDATA / 256 / 256 / 256 % 256;
		
		//��װ�����get��
		strcpy(psendpkg->sid,SID);
		memcpy(psendpkg->offset,offset,OFFLEN);
	
		//���ʹ����get��
		sendto(socket_sidsender,sendpkg,sizeof(Trans_get),0,(struct sockaddr *)&addrTo,SockAddrLength);

		if(DEVETESTIMPL <= 1)
			printf("[Transport Layer] The TCPClient asked for Data whitch match the SID: %s\n",SID);

		//����������ʱ
		sendto(TCPCliParaList.SENDTCPClientTimingBegin,"TimingBegin",20,0,(struct sockaddr *)&TCPCliParaList.SENDAddrToTCPClientTimingBegin,SockAddrLength);

		//printf("[%d] TimingBegin triggering...\n",TCPCliParaList.CountTCPClientTimingBegin_Triggering);
		TCPCliParaList.CountTCPClientTimingBegin_Triggering++;

		//printf("[%d] TimingRespond waiting...\n",TCPCliParaList.CountTCPClientTimingRespond_Waiting);
		TCPCliParaList.CountTCPClientTimingRespond_Waiting++;

		//�ȴ���������ACK
		recvfrom(TCPCliParaList.RECVTCPClientTimingRespond,Trigger,20,0,(struct sockaddr *)&TCPCliParaList.RECVAddrToTCPClientTimingRespond,&SockAddrLength);

		//printf("[%d] TimingRespond triggered!\n",TCPCliParaList.CountTCPClientTimingRespond_Triggered);
		TCPCliParaList.CountTCPClientTimingRespond_Triggered++;
		
		//�����жϣ��յ������ݳ����ۻ�����Ԥ֪�ļ����ȵ��߼����̷�Χ���߼����̷�Χ���ļ�����+�������ȣ�
		if(TCPCliParaList.OffsetConfirmDATA >= filelen)
		{
			printf("[END message]Received File Length is Bigger Than the File Length Ever known.\n");
			
			TCPCliParaList.FlagInputBUF = 0;

			//֪ͨ���ļ�д������
			sendto(TCPCliParaList.SENDTCPClientWriteData,"WriteData",20,0,(struct sockaddr *)&TCPCliParaList.SENDAddrToTCPClientWriteData,SockAddrLength);

			//printf("[%d] WriteData triggering...\n",TCPCliParaList.CountTCPClientWriteData_Triggering);
			TCPCliParaList.CountTCPClientWriteData_Triggering++;
		}
		//д���ļ��ж�
		else if(TCPCliParaList.OffsetConfirmDATA / ( TCPCliParaList.BUFLength / 2 ) >= TCPCliParaList.BUFRenewCount + 1)
		{
			//֪ͨ���ļ�д������
			sendto(TCPCliParaList.SENDTCPClientWriteData,"WriteData",20,0,(struct sockaddr *)&TCPCliParaList.SENDAddrToTCPClientWriteData,SockAddrLength);

			//printf("[%d] WriteData triggering...\n",TCPCliParaList.CountTCPClientWriteData_Triggering);
			TCPCliParaList.CountTCPClientWriteData_Triggering++;
		}
	}
	
	printf("[Down]File transport end.\n");
	sleep(100);//�ȴ�һ���룬����Ȼδ����д���ļ����߳��йرճ���˵������������������⣬Ӧ���Զ��ر�

	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�TCPClient_main
* ����������TCPClientģ��������������ForwardingPlaneת��ƽ���̡߳�������ѭ��ִ�м�HTTP���������ܡ�
* �����б�
eg: ./sub d1sub1 d1sub1-eth1
�ڶ�����ʹ��char**argv�ķ�ʽ����������֪�ǲ����﷨����
* ���ؽ����
*****************************************/
int
TCPClient_main
(
	int argc,
	char argv[][30]
)
{
	int i,j;

	TCPClient_Parameterinit();

	//�������ͼ�ʱ�����߳�
	pthread_t pthread_timer;
	if(pthread_create(&pthread_timer, NULL, TCPClient_thread_timer, NULL)!=0)
	{
		perror("Creation of timer thread failed.");
	}
	
	//����TCP�����߳�
	pthread_t pthread_TCP;
	if(pthread_create(&pthread_TCP, NULL, TCPClient_thread_TCP, NULL)!=0)
	{
		perror("Creation of TCP thread failed.");
	}

	//����thread_FTP���߳�
	pthread_t pthread_FTP;
	if(pthread_create(&pthread_FTP, NULL, TCPClient_thread_FTP, NULL)!=0)
	{
		perror("Creation of FTP thread failed.");
	}
	
	//����thread_NetworkLayertoTransportLayer�������߳�
	pthread_t pthread_NetworkLayertoTransportLayer;
	if(pthread_create(&pthread_NetworkLayertoTransportLayer, NULL, TCPClient_thread_NetworkLayertoTransportLayer, NULL)!=0)
	{
		perror("Creation of NetworkLayertoTransportLayer thread failed.");
	}
	
	//����thread_TransportLayertoNetworkLayer�������߳�
	pthread_t pthread_TransportLayertoNetworkLayer;
	if(pthread_create(&pthread_TransportLayertoNetworkLayer, NULL, TCPClient_thread_TransportLayertoNetworkLayer, NULL)!=0)
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
