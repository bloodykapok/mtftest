/*******************************************************************************************************************************************
* �ļ�����tcpserver.h
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

#include"library.h"
#include"packet.h"
#include"basic.h"

#include"cnf.h"

/*******************************************************************************************************************************************
*************************************�궨����������************ȫ�ֱ�������******************************************************************
*******************************************************************************************************************************************/
#ifndef _CNFSER_
#define _CNFSER_

//���״̬�����Ƿ���Ҫ������׶����е����ݣ�������٣�
//�����׶� == 0
//���Խ׶� == 1
//�а��׶� == 2
//΢���׶� == 3
//���ο����׶� == 4
//����׶� == 5
//CNF���� == 6
//CNF������� == 7
#define cnfserDEVETESTIMPL cnfDEVETESTIMPL

//�Ƿ����AIMD���Ƶ�ÿһ��������ʾ
#define CNFserverTRIGGERTEST CNFTRIGGERTEST

#define CNFBUFSIZE       1000000    //CNF�������ͻ����С��1MB��

//CNF�����߳̿��Ʋ�����
//Tip.�ź������������ܴ������ַ�ϣ��ڽṹ���У���������Ĵ洢�����ֽڶ���ķ�ʽ���Ż�������ַ������
struct CNFSerPara
{
	//�ڲ��̴߳�����SEMͨ�ű���
	sem_t semCNFServerWindowBegin;
	//sem_t semCNFServerWindowTimingEnd;
	sem_t semCNFServerWindowRespond;
	sem_t semCNFServerBufReady;
	sem_t semCNFServerMoreData;
	sem_t semCNFServerFileClose;
	sem_t semCNFServerContinueRECV;
/*
	if(cnfINTRACOMMUNICATION == 0)
	{
		recvfrom();
	}
	else if(cnfINTRACOMMUNICATION == 1)
	{
		sem_wait(&CNFSerParaList[threadnum].semCNFClientTimingRespond);
	}
*/
/*
	if(cnfINTRACOMMUNICATION == 0)
	{
		sendto();
	}
	else if(cnfINTRACOMMUNICATION == 1)
	{
		sem_post(&CNFSerParaList[threadnum].semCNFClientWriteData);
	}
*/

	uint8_t SID[SIDLEN];//�����������SID
	int SIDLen;//SID��ʵ�ʳ���
	
	uint8_t * BUF;//���ͻ�������ͷ
	uint8_t * BUFEnd;//���ͻ�������β
	uint8_t * BUFFileEnd;//��ȡ�ļ�����ʱ�����������ļ�ĩβ���ڵ�ַ

	int BUFRenewCount;//���������´���
	unsigned long BUFLength;//���ͻ��峤��

	int LogFilePoint;//������־�ļ���ʶ
	unsigned long FileReadPoint;//�ļ���ȡ����
	unsigned long FileLength;//�ļ��ܳ���

	unsigned long PacketDataLength;//�������ݶγ���
	unsigned long LastPacketDataLength;//����������������һ�����ݰ�ʵ�ʳ���

	unsigned long WindowLength;//���ڳ���
	unsigned long WindowThreshold;//��������ֵ

	unsigned long OffsetWindowHead;//�������ļ��д���ͷ����ָ��ƫ����
	unsigned long OffsetWindowEnd;//�������ļ��д���β����ָ��ƫ����
	unsigned long OffsetConfirmACK;//�������ļ�����ȷ��ƫ��������ƫ����
	unsigned long OffsetRequestACK;//�������ļ��нӵ������ƫ����
	unsigned long OffsetPacket;//�������ļ��е�ǰ���ƫ�������ش����ݰ�ƫ�����ֶε���д���ݣ�
	unsigned long OffsetWaitACK;//�������ļ��еȴ�ȷ���յ���ƫ��������һ�ڣ�

	uint8_t * AddrWindowHead;//�ڻ������д���ͷ�����ڵ�ַ
	uint8_t * AddrWindowEnd;//�ڻ������д���β�����ڵ�ַ
	uint8_t * AddrConfirmACK;//�ڻ���������ȷ��ƫ�������ڵ�ַ����ʱû�ã�
	uint8_t * AddrRequestACK;//�ڻ������нӵ���������ڵ�ַ����ʱû�ã�
	uint8_t * AddrPacket;//�ڻ������е�ǰ���ƫ�������ڵ�ַ
	uint8_t * AddrWaitACK;//�ڻ������д�ȷ��ƫ�������ڵ�ַ

	unsigned long RTOs;//��������ʱ�ӣ�RTO����1�벿�֣���λ�룩
	unsigned long RTOus;//��������ʱ�ӣ�RTOС��1�벿�֣���λ΢�룩
	unsigned long RTOns;//��������ʱ�ӣ�RTOС��1�벿�֣���λ���룩

	int FlagTrebleAck;//����������¼�յ���ͬƫ����ACK�ĸ����������ж�����ACK����״��

	int WindowState;//����״̬��1ACK�������µ�δ�ﴰ����Ҫ����Ͻ磻2���ڴ���ɹ���3����ACK��4��ʱ
	int ThreadState;//����״̬�����ڽ������̵Ŀ��ƣ���0δ��ɣ�1�ļ���ȡ��ϣ�2����¼����ϣ�3���ڷ�����ϣ�4����ȷ����ϣ�������ֹ��

	char NumStr[100];//�ַ����棿

	//CNF�����߳�����
	pthread_t pthread_timer;
	pthread_t pthread_CNF;
	pthread_t pthread_NetworkLayertoTransportLayer;
	pthread_t pthread_TransportLayertoNetworkLayer;
	pthread_t pthread_FTP;
	
	//�����շ��ӿ���UDPͨ�ű���
	int SENDCNFServerSEG;
	struct sockaddr_in SENDAddrToCNFServerSEG;
	int RECVCNFServerACK;
	struct sockaddr_in RECVAddrToCNFServerACK;

	//�ڲ��̴߳�����UDPͨ�ű���
	int CountCNFServerWindowBegin_Triggering;
	int CountCNFServerWindowBegin_Waiting;
	int CountCNFServerWindowBegin_Triggered;

	int SENDCNFServerWindowBegin;
	int RECVCNFServerWindowBegin;
	struct sockaddr_in SENDAddrToCNFServerWindowBegin;
	struct sockaddr_in RECVAddrToCNFServerWindowBegin;

	int CountCNFServerWindowTimingEnd_Triggering;
	int CountCNFServerWindowTimingEnd_Waiting;
	int CountCNFServerWindowTimingEnd_Triggered;

	int SENDCNFServerWindowTimingEnd;
	int RECVCNFServerWindowTimingEnd;
	struct sockaddr_in SENDAddrToCNFServerWindowTimingEnd;
	struct sockaddr_in RECVAddrToCNFServerWindowTimingEnd;

	int CountCNFServerWindowRespond_Triggering;
	int CountCNFServerWindowRespond_Waiting;
	int CountCNFServerWindowRespond_Triggered;

	int SENDCNFServerWindowRespond;
	int RECVCNFServerWindowRespond;
	struct sockaddr_in SENDAddrToCNFServerWindowRespond;
	struct sockaddr_in RECVAddrToCNFServerWindowRespond;

	int CountCNFServerBufReady_Triggering;
	int CountCNFServerBufReady_Waiting;
	int CountCNFServerBufReady_Triggered;

	int SENDCNFServerBufReady;
	int RECVCNFServerBufReady;
	struct sockaddr_in SENDAddrToCNFServerBufReady;
	struct sockaddr_in RECVAddrToCNFServerBufReady;

	int CountCNFServerMoreData_Triggering;
	int CountCNFServerMoreData_Waiting;
	int CountCNFServerMoreData_Triggered;

	int SENDCNFServerMoreData;
	int RECVCNFServerMoreData;
	struct sockaddr_in SENDAddrToCNFServerMoreData;
	struct sockaddr_in RECVAddrToCNFServerMoreData;

	int CountCNFServerFileClose_Triggering;
	int CountCNFServerFileClose_Waiting;
	int CountCNFServerFileClose_Triggered;

	int SENDCNFServerFileClose;
	int RECVCNFServerFileClose;
	struct sockaddr_in SENDAddrToCNFServerFileClose;
	struct sockaddr_in RECVAddrToCNFServerFileClose;

	int CountCNFServerContinueRECV_Triggering;
	int CountCNFServerContinueRECV_Waiting;
	int CountCNFServerContinueRECV_Triggered;

	int SENDCNFServerContinueRECV;
	int RECVCNFServerContinueRECV;
	struct sockaddr_in SENDAddrToCNFServerContinueRECV;
	struct sockaddr_in RECVAddrToCNFServerContinueRECV;

	//ͳ����
	int InputCount;//����д�����
	unsigned long TotalWindowLength;//�ۼƴ����ܳ���
	unsigned long LostPacketCount;//�ܶ�������
	unsigned long timer_XX;//������ʱ������
};
struct CNFSerPara CNFSerParaList[THREADNUM];

/*******************************************************************************************************************************************
*******************************************��ʼ�������������********************************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�CNFServer_Parameterinit
* ����������CNFServerģ��ȫ�ֱ�������ֵ
* �����б�
* ���ؽ����
*****************************************/
void
CNFServer_Parameterinit
(
	int threadnum
);

/*****************************************
* �������ƣ�CNFServer_SendData
* ����������ͬʱ��ֹ��CNF�������񲢻��յ�������Դ
* �����б�
* ���ؽ����
*****************************************/
void
CNFServer_Parametershut
(
	int threadnum
);

/*******************************************************************************************************************************************
*******************************************Ӧ�ò�CNFServer��������ͨ��CLI���ƣ�******************************************************************
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
* �������ƣ�CNFServer_thread_timer
* ����������Ϊ���߳��ṩ��ʱ�ź�
* �����б�
* ���ؽ����
*****************************************/
void *
CNFServer_thread_timer
(
	void * fd
);

/*****************************************
* �������ƣ�CNFServer_thread_CNF
* ��������������ͨ�� - CNF�����������������߳�
* �����б�
* ���ؽ����
*****************************************/
void *
CNFServer_thread_CNF
(
	void * fd
);

/*****************************************
* �������ƣ�CNFServer_thread_FTP
* ������������ȡ�ļ��̣߳�FTPǰ���������ȡ�ļ���ά�ַ��ͻ���ĸ���
* �����б�
* ���ؽ����
*****************************************/
void *
CNFServer_thread_FTP
(
	void * fd
);

/*****************************************
* �������ƣ�CNFServer_thread_NetworkLayertoTransportLayer
* ��������������ͨ�� - From.������Ӧ��   To.Ӧ�÷�����
* �����б�
* ���ؽ����
*****************************************/
void *
CNFServer_thread_NetworkLayertoTransportLayer
(
	void * fd
);

/*****************************************
* �������ƣ�CNFServer_thread_TransportLayertoNetworkLayer
* ��������������ͨ�� - From.Ӧ�÷����� To.������Ӧ��
* �����б�
* ���ؽ����
*****************************************/
void *
CNFServer_thread_TransportLayertoNetworkLayer
(
	void * fd
);

/*****************************************
* �������ƣ�CNFServer_main
* ����������CNFServerģ�������������������̣߳������ṩʵ�ʹ���
* �����б��ڶ�����ʹ��char**argv�ķ�ʽ����������֪�ǲ����﷨����
* ���ؽ����
*****************************************/
int
CNFServer_main
(
	void * fd
);

/*****************************************
* �������ƣ�CNFServerStart_main
* ����������CNFServer������������CNFServer��TCP����ģ�飬���²�PublisherЭ��ջģ��
* �����б��ڶ�����ʹ��char**argv�ķ�ʽ����������֪�ǲ����﷨����
* ���ؽ����

*****************************************/
int
CNFServerStart_main
(
	int argc,
	char argv[][30]
);

#endif
