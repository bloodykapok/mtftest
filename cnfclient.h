/*******************************************************************************************************************************************
* �ļ�����cnfclient.h
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
gcc cnfclient.c -o cnfclient -lpthread
4�����У����漰ԭʼ�׽��ֵ�ʹ�ã���rootȨ�ޣ�
sudo ./cnfclient
*/

#include"library.h"
#include"packet.h"
#include"basic.h"

#include"cnf.h"

/*******************************************************************************************************************************************
*************************************�궨����������************ȫ�ֱ�������******************************************************************
*******************************************************************************************************************************************/
#ifndef _CNFCLI_
#define _CNFCLI_

//���״̬�����Ƿ���Ҫ������׶����е����ݣ�������٣�
//�����׶� == 0
//���Խ׶� == 1
//�а��׶� == 2
//΢���׶� == 3
//���ο����׶� == 4
//����׶� == 5
//CNF���� == 6
//CNF������� == 7
#define cnfcliDEVETESTIMPL cnfDEVETESTIMPL

//�Ƿ����AIMD���Ƶ�ÿһ��������ʾ
#define CNFclientTRIGGERTEST CNFTRIGGERTEST

#define CNFBUFSIZE       1000000    //CNF�������ͻ����С��1MB��

//CNF�����߳̿��Ʋ�����
//Tip.�ź������������ܴ������ַ�ϣ��ڽṹ���У���������Ĵ洢�����ֽڶ���ķ�ʽ���Ż�������ַ������
struct CNFCliPara
{
	//�ڲ��̴߳�����SEMͨ�ű���
	sem_t semCNFClientTimingBegin;
	sem_t semCNFClientTimingEnd;
	sem_t semCNFClientTimingRespond;
	sem_t semCNFClientBufReady;
	sem_t semCNFClientMoreData;//����
	sem_t semCNFClientWriteData;
	sem_t semCNFClientFileClose;//����
	sem_t semCNFClientStartOrder1;
	sem_t semCNFClientStartOrder2;
	sem_t semCNFClientStartOrder3;
	sem_t semCNFClientStartOrder4;
/*
	if(cnfINTRACOMMUNICATION == 0)
	{
		
	}
	else if(cnfINTRACOMMUNICATION == 1)
	{
		sem_post(&semCNFClientStartOrder);
	}
*/


	uint8_t SID[SIDLEN];//�����������SID
	int SIDLen;//SID��ʵ�ʳ���
	
	uint8_t * BUF;//���ͻ�������ͷ
	uint8_t * BUFEnd;//���ͻ�������β
	uint8_t * BUFFileEnd;//��ȡ�ļ�����ʱ�����������ļ�ĩβ���ڵ�ַ

	int BUFRenewCount;//���������´���
	unsigned long BUFLength;//���ͻ��峤��

	unsigned long FileWritePoint;//�ļ�д�����
	unsigned long FileLength;//�ļ��ܳ���

	unsigned long PacketDataLength;//�������ݶγ���
	unsigned long LastPacketDataLength;//����������������һ�����ݰ�ʵ�ʳ���	

	unsigned long OffsetConfirmDATA;//�������ļ���ȷ���յ����ֽڶ���
	unsigned long OffsetPacket;//�������ļ��е�ǰ����ֽ�������ʱû�ã�
	unsigned long OffsetWaitDATA;//�������ļ��еȴ�ȷ���յ����ֽ���������ACK���ݰ�ƫ�����ֶε���д���ݣ�

	uint8_t * AddrConfirmDATA;//�ڻ���������ȷ��ƫ�������ڵ�ַ
	uint8_t * AddrPacket;//�ڻ������е�ǰ���ƫ�������ڵ�ַ����ʱû�ã�
	uint8_t * AddrWaitDATA;//�ڻ������д�ȷ��ƫ�������ڵ�ַ����ʱû�ã�

	unsigned long RTOs;//��������ʱ�ӣ�RTO����1�벿�֣���λ�룩
	unsigned long RTOus;//��������ʱ�ӣ�RTOС��1�벿�֣���λ΢�룩
	unsigned long RTOns;//��������ʱ�ӣ�RTOС��1�벿�֣���λ���룩

	int FlagInputBUF;//�Ƿ��յ�������д�뻺��

	int TimingState;//��ʱ״̬��0��ʱ��1��ʱ
	int ThreadState;//����״̬�����ڽ������̵Ŀ��ƣ�

	//CNF�����߳�����	
	pthread_t pthread_timer;
	pthread_t pthread_CNF;
	pthread_t pthread_FTP;
	pthread_t pthread_NetworkLayertoTransportLayer;
	pthread_t pthread_TransportLayertoNetworkLayer;

	//�����շ��ӿ���UDPͨ�ű���
	int SENDCNFClientACK;
	struct sockaddr_in SENDAddrToCNFClientACK;
	int RECVCNFClientSEG;
	struct sockaddr_in RECVAddrToCNFClientSEG;

	//�ڲ��̴߳�����UDPͨ�ű���
	int CountCNFClientTimingBegin_Triggering;
	int CountCNFClientTimingBegin_Waiting;
	int CountCNFClientTimingBegin_Triggered;

	int SENDCNFClientTimingBegin;
	int RECVCNFClientTimingBegin;
	struct sockaddr_in SENDAddrToCNFClientTimingBegin;
	struct sockaddr_in RECVAddrToCNFClientTimingBegin;

	int CountCNFClientTimingEnd_Triggering;
	int CountCNFClientTimingEnd_Waiting;
	int CountCNFClientTimingEnd_Triggered;

	int SENDCNFClientTimingEnd;
	int RECVCNFClientTimingEnd;
	struct sockaddr_in SENDAddrToCNFClientTimingEnd;
	struct sockaddr_in RECVAddrToCNFClientTimingEnd;

	int CountCNFClientTimingRespond_Triggering;
	int CountCNFClientTimingRespond_Waiting;
	int CountCNFClientTimingRespond_Triggered;

	int SENDCNFClientTimingRespond;
	int RECVCNFClientTimingRespond;
	struct sockaddr_in SENDAddrToCNFClientTimingRespond;
	struct sockaddr_in RECVAddrToCNFClientTimingRespond;

	int CountCNFClientBufReady_Triggering;
	int CountCNFClientBufReady_Waiting;
	int CountCNFClientBufReady_Triggered;

	int SENDCNFClientBufReady;
	int RECVCNFClientBufReady;
	struct sockaddr_in SENDAddrToCNFClientBufReady;
	struct sockaddr_in RECVAddrToCNFClientBufReady;

	int CountCNFClientMoreData_Triggering;
	int CountCNFClientMoreData_Waiting;
	int CountCNFClientMoreData_Triggered;

	int SENDCNFClientMoreData;
	int RECVCNFClientMoreData;
	struct sockaddr_in SENDAddrToCNFClientMoreData;
	struct sockaddr_in RECVAddrToCNFClientMoreData;

	int CountCNFClientWriteData_Triggering;
	int CountCNFClientWriteData_Waiting;
	int CountCNFClientWriteData_Triggered;

	int SENDCNFClientWriteData;
	int RECVCNFClientWriteData;
	struct sockaddr_in SENDAddrToCNFClientWriteData;
	struct sockaddr_in RECVAddrToCNFClientWriteData;

	int CountCNFClientFileClose_Triggering;
	int CountCNFClientFileClose_Waiting;
	int CountCNFClientFileClose_Triggered;

	int SENDCNFClientFileClose;
	int RECVCNFClientFileClose;
	struct sockaddr_in SENDAddrToCNFClientFileClose;
	struct sockaddr_in RECVAddrToCNFClientFileClose;

	int CountCNFClientStartOrder1_Triggering;
	int CountCNFClientStartOrder1_Waiting;
	int CountCNFClientStartOrder1_Triggered;

	int SENDCNFClientStartOrder1;
	int RECVCNFClientStartOrder1;
	struct sockaddr_in SENDAddrToCNFClientStartOrder1;
	struct sockaddr_in RECVAddrToCNFClientStartOrder1;

	int CountCNFClientStartOrder2_Triggering;
	int CountCNFClientStartOrder2_Waiting;
	int CountCNFClientStartOrder2_Triggered;

	int SENDCNFClientStartOrder2;
	int RECVCNFClientStartOrder2;
	struct sockaddr_in SENDAddrToCNFClientStartOrder2;
	struct sockaddr_in RECVAddrToCNFClientStartOrder2;

	int CountCNFClientStartOrder3_Triggering;
	int CountCNFClientStartOrder3_Waiting;
	int CountCNFClientStartOrder3_Triggered;

	int SENDCNFClientStartOrder3;
	int RECVCNFClientStartOrder3;
	struct sockaddr_in SENDAddrToCNFClientStartOrder3;
	struct sockaddr_in RECVAddrToCNFClientStartOrder3;

	int CountCNFClientStartOrder4_Triggering;
	int CountCNFClientStartOrder4_Waiting;
	int CountCNFClientStartOrder4_Triggered;

	int SENDCNFClientStartOrder4;
	int RECVCNFClientStartOrder4;
	struct sockaddr_in SENDAddrToCNFClientStartOrder4;
	struct sockaddr_in RECVAddrToCNFClientStartOrder4;
};
struct CNFCliPara CNFCliParaList[THREADNUM];

/*

	sendto(SENDCNFClientTimingBegin,"",0,0,(struct sockaddr *)&AddrToCNFClientTimingBegin,sizeof(AddrToCNFClientTimingBegin));
	recvfrom(RECVCNFClientTimingBegin,Trigger,20,0,(struct sockaddr *)&AddrToCNFClientTimingBegin,&SockAddrLength);

	sendto(SENDCNFClientTimingEnd,"",0,0,(struct sockaddr *)&AddrToCNFClientTimingEnd,sizeof(AddrToCNFClientTimingEnd));
	recvfrom(RECVCNFClientTimingEnd,Trigger,20,0,(struct sockaddr *)&AddrToCNFClientTimingEnd,&SockAddrLength);

	sendto(SENDCNFClientTimingRespond,"",0,0,(struct sockaddr *)&AddrToCNFClientTimingRespond,sizeof(AddrToCNFClientTimingRespond));
	recvfrom(RECVCNFClientTimingRespond,Trigger,20,0,(struct sockaddr *)&AddrToCNFClientTimingRespond,&SockAddrLength);

	sendto(SENDCNFClientBufReady,"",0,0,(struct sockaddr *)&AddrToCNFClientBufReady,sizeof(AddrToCNFClientBufReady));
	recvfrom(RECVCNFClientBufReady,Trigger,20,0,(struct sockaddr *)&AddrToCNFClientBufReady,&SockAddrLength);

	sendto(SENDCNFClientMoreData,"",0,0,(struct sockaddr *)&AddrToCNFClientMoreData,sizeof(AddrToCNFClientMoreData));
	recvfrom(RECVCNFClientMoreData,Trigger,20,0,(struct sockaddr *)&AddrToCNFClientMoreData,&SockAddrLength);

	sendto(SENDCNFClientWriteData,"",0,0,(struct sockaddr *)&AddrToCNFClientWriteData,sizeof(AddrToCNFClientWriteData));
	recvfrom(RECVCNFClientWriteData,Trigger,20,0,(struct sockaddr *)&AddrToCNFClientWriteData,&SockAddrLength);

	sendto(SENDCNFClientFileClose,"",0,0,(struct sockaddr *)&AddrToCNFClientFileClose,sizeof(AddrToCNFClientFileClose));
	recvfrom(RECVCNFClientFileClose,Trigger,20,0,(struct sockaddr *)&AddrToCNFClientFileClose,&SockAddrLength);

	sendto(SENDCNFClientStartOrder1,"",0,0,(struct sockaddr *)&AddrToCNFClientStartOrder1,sizeof(AddrToCNFClientStartOrder1));
	recvfrom(RECVCNFClientStartOrder1,Trigger,20,0,(struct sockaddr *)&AddrToCNFClientStartOrder1,&SockAddrLength);

	sendto(SENDCNFClientStartOrder2,"",0,0,(struct sockaddr *)&AddrToCNFClientStartOrder2,sizeof(AddrToCNFClientStartOrder2));
	recvfrom(RECVCNFClientStartOrder2,Trigger,20,0,(struct sockaddr *)&AddrToCNFClientStartOrder2,&SockAddrLength);

	sendto(SENDCNFClientStartOrder3,"",0,0,(struct sockaddr *)&AddrToCNFClientStartOrder3,sizeof(AddrToCNFClientStartOrder3));
	recvfrom(RECVCNFClientStartOrder3,Trigger,20,0,(struct sockaddr *)&AddrToCNFClientStartOrder3,&SockAddrLength);

	sendto(SENDCNFClientStartOrder4,"",0,0,(struct sockaddr *)&AddrToCNFClientStartOrder4,sizeof(AddrToCNFClientStartOrder4));
	recvfrom(RECVCNFClientStartOrder4,Trigger,20,0,(struct sockaddr *)&AddrToCNFClientStartOrder4,&SockAddrLength);

*/

/*******************************************************************************************************************************************
*******************************************��ʼ�������������********************************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�CNFClient_Parameterinit
* ����������CNFClientģ��ȫ�ֱ�������ֵ
* �����б�
* ���ؽ����
*****************************************/
void
CNFClient_Parameterinit
(
	int threadnum
);

/*******************************************************************************************************************************************
*******************************************Ӧ�ò�CNFClient��������ͨ��CLI���ƣ�**************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.���������   To.Ӧ�÷�����********************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�CNFClient_UDP_SendData
* �������������ϲ�ת��ƽ�淢��UDP��װ��DATA������/ͬʱ��ֹ��CNF�������񲢻��յ�������Դ
* �����б�
* ���ؽ����
*****************************************/
void
CNFClient_UDP_SendData
(
	int threadnum
);

/*****************************************
* �������ƣ�CNFClient_SEM_SendData
* �������������ϲ�ת��ƽ�淢��UDP��װ��DATA������/ͬʱ��ֹ��CNF�������񲢻��յ�������Դ
* �����б�
* ���ؽ����
*****************************************/
void
CNFClient_SEM_SendData
(
	int threadnum
);

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.Ӧ�÷����� To.���������**********************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
**********************************************************���߳����ɳ���********************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�CNFClient_thread_timer
* ����������Ϊ���߳��ṩ��ʱ�ź�
* �����б�
* ���ؽ����
*****************************************/
void *
CNFClient_thread_timer
(
	void * fd
);

/*****************************************
* �������ƣ�CNFClient_thread_CNF
* ��������������ͨ�� - CNF�ͻ���������������߳�
* �����б�
* ���ؽ����
*****************************************/
void *
CNFClient_thread_CNF
(
	void * fd
);

/*****************************************
* �������ƣ�CNFClient_thread_FTP
* ������������ȡ�ļ��̣߳�FTPǰ���������ȡ�ļ���ά�ַ��ͻ���ĸ���
* �����б�
* ���ؽ����
*****************************************/
void *
CNFClient_thread_FTP
(
	void * fd
);

/*****************************************
* �������ƣ�CNFClient_thread_NetworkLayertoTransportLayer
* ��������������ͨ�� - From.���������   To.Ӧ�÷�����
* �����б�
* ���ؽ����
*****************************************/
void *
CNFClient_thread_NetworkLayertoTransportLayer
(
	void * fd
);

/*****************************************
* �������ƣ�CNFClient_thread_TransportLayertoNetworkLayer
* ��������������ͨ�� - From.Ӧ�÷����� To.���������
* �����б�
* ���ؽ����
*****************************************/
void *
CNFClient_thread_TransportLayertoNetworkLayer
(
	void * fd
);

/*****************************************
* �������ƣ�CNFClient_main
* ����������CNFClientģ��������������ForwardingPlaneת��ƽ���̡߳�������ѭ��ִ�м�HTTP���������ܡ�

* �����б�
eg: ./sub d1sub1 d1sub1-eth1
�ڶ�����ʹ��char**argv�ķ�ʽ����������֪�ǲ����﷨����
* ���ؽ����

*****************************************/
int
CNFClient_main
(
	void * fd
);

/*****************************************
* �������ƣ�CNFClientStart_main
* ����������CNFClient������������CNFClient��TCP����ģ�飬���²�SubscriberЭ��ջģ��
* �����б�
�ڶ�����ʹ��char**argv�ķ�ʽ����������֪�ǲ����﷨����
* ���ؽ����
*****************************************/
int
CNFClientStart_main
(
	int argc,
	char argv[][30]
);

#endif
