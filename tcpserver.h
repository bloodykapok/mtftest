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

#include"transportlayerdefine.h"
#include"publisher.h"

/*******************************************************************************************************************************************
*************************************�궨����������************ȫ�ֱ�������******************************************************************
*******************************************************************************************************************************************/
#ifndef _TCPSER_
#define _TCPSER_



//TCP�����߳̿��Ʋ�����
//Tip.�ź������������ܴ������ַ�ϣ��ڽṹ���У���������Ĵ洢�����ֽڶ���ķ�ʽ���Ż�������ַ������
struct TCPSerPara
{
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

	//�ڲ��̴߳�����UDPͨ�ű���
	int CountTCPServerWindowBegin_Triggering;
	int CountTCPServerWindowBegin_Waiting;
	int CountTCPServerWindowBegin_Triggered;

	int SENDTCPServerWindowBegin;
	int RECVTCPServerWindowBegin;
	struct sockaddr_in SENDAddrToTCPServerWindowBegin;
	struct sockaddr_in RECVAddrToTCPServerWindowBegin;

	int CountTCPServerWindowTimingEnd_Triggering;
	int CountTCPServerWindowTimingEnd_Waiting;
	int CountTCPServerWindowTimingEnd_Triggered;

	int SENDTCPServerWindowTimingEnd;
	int RECVTCPServerWindowTimingEnd;
	struct sockaddr_in SENDAddrToTCPServerWindowTimingEnd;
	struct sockaddr_in RECVAddrToTCPServerWindowTimingEnd;

	int CountTCPServerWindowRespond_Triggering;
	int CountTCPServerWindowRespond_Waiting;
	int CountTCPServerWindowRespond_Triggered;

	int SENDTCPServerWindowRespond;
	int RECVTCPServerWindowRespond;
	struct sockaddr_in SENDAddrToTCPServerWindowRespond;
	struct sockaddr_in RECVAddrToTCPServerWindowRespond;

	int CountTCPServerBufReady_Triggering;
	int CountTCPServerBufReady_Waiting;
	int CountTCPServerBufReady_Triggered;

	int SENDTCPServerBufReady;
	int RECVTCPServerBufReady;
	struct sockaddr_in SENDAddrToTCPServerBufReady;
	struct sockaddr_in RECVAddrToTCPServerBufReady;

	int CountTCPServerMoreData_Triggering;
	int CountTCPServerMoreData_Waiting;
	int CountTCPServerMoreData_Triggered;

	int SENDTCPServerMoreData;
	int RECVTCPServerMoreData;
	struct sockaddr_in SENDAddrToTCPServerMoreData;
	struct sockaddr_in RECVAddrToTCPServerMoreData;

	int CountTCPServerFileClose_Triggering;
	int CountTCPServerFileClose_Waiting;
	int CountTCPServerFileClose_Triggered;

	int SENDTCPServerFileClose;
	int RECVTCPServerFileClose;
	struct sockaddr_in SENDAddrToTCPServerFileClose;
	struct sockaddr_in RECVAddrToTCPServerFileClose;

	int CountTCPServerContinueRECV_Triggering;
	int CountTCPServerContinueRECV_Waiting;
	int CountTCPServerContinueRECV_Triggered;

	int SENDTCPServerContinueRECV;
	int RECVTCPServerContinueRECV;
	struct sockaddr_in SENDAddrToTCPServerContinueRECV;
	struct sockaddr_in RECVAddrToTCPServerContinueRECV;

	//ͳ����
	int InputCount;//����д�����
	unsigned long TotalWindowLength;//�ۼƴ����ܳ���
	unsigned long LostPacketCount;//�ܶ�������
	unsigned long timer_XX;//������ʱ������
};
struct TCPSerPara TCPSerParaList;

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
TCPServer_Parameterinit();

/*******************************************************************************************************************************************
*******************************************Ӧ�ò�TCPServer��������ͨ��CLI���ƣ�******************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.������Ӧ��   To.Ӧ�÷�����*********************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�TCPServer_FindData
* ������������ѯSID��Ӧ��DATA
* �����б�
* ���ؽ����
*****************************************/
int
TCPServer_FindData
(
	uint8_t * SID,
	uint8_t * DATA
);

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
);

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
);

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
);

/*****************************************
* �������ƣ�TCPServer_thread_NetworkLayertoTransportLayer
* ��������������ͨ�� - From.������Ӧ��   To.Ӧ�÷�����
* �����б�
* ���ؽ����
*****************************************/
void *
TCPServer_thread_NetworkLayertoTransportLayer
(
	void * fd
);

/*****************************************
* �������ƣ�TCPServer_thread_TransportLayertoNetworkLayer
* ��������������ͨ�� - From.Ӧ�÷����� To.������Ӧ��
* �����б�
* ���ؽ����
*****************************************/
void *
TCPServer_thread_TransportLayertoNetworkLayer
(
	void * fd
);

/*****************************************
* �������ƣ�TCPServer_main
* ����������TCPServerģ�������������������̣߳������ṩʵ�ʹ���
* �����б��ڶ�����ʹ��char**argv�ķ�ʽ����������֪�ǲ����﷨����
* ���ؽ����
*****************************************/
int
TCPServer_main
(
	int argc,
	char argv[][30]
);

#endif
