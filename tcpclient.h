/*******************************************************************************************************************************************
* �ļ�����tcpclient.h
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

#include"transportlayerdefine.h"
#include"subscriber.h"

/*******************************************************************************************************************************************
*************************************�궨����������************ȫ�ֱ�������******************************************************************
*******************************************************************************************************************************************/
#ifndef _TCPCLI_
#define _TCPCLI_

//TCP�����߳̿��Ʋ�����
//Tip.�ź������������ܴ������ַ�ϣ��ڽṹ���У���������Ĵ洢�����ֽڶ���ķ�ʽ���Ż�������ַ������
struct TCPCliPara
{
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

	//�ڲ��̴߳�����UDPͨ�ű���
	int CountTCPClientTimingBegin_Triggering;
	int CountTCPClientTimingBegin_Waiting;
	int CountTCPClientTimingBegin_Triggered;

	int SENDTCPClientTimingBegin;
	int RECVTCPClientTimingBegin;
	struct sockaddr_in SENDAddrToTCPClientTimingBegin;
	struct sockaddr_in RECVAddrToTCPClientTimingBegin;

	int CountTCPClientTimingEnd_Triggering;
	int CountTCPClientTimingEnd_Waiting;
	int CountTCPClientTimingEnd_Triggered;

	int SENDTCPClientTimingEnd;
	int RECVTCPClientTimingEnd;
	struct sockaddr_in SENDAddrToTCPClientTimingEnd;
	struct sockaddr_in RECVAddrToTCPClientTimingEnd;

	int CountTCPClientTimingRespond_Triggering;
	int CountTCPClientTimingRespond_Waiting;
	int CountTCPClientTimingRespond_Triggered;

	int SENDTCPClientTimingRespond;
	int RECVTCPClientTimingRespond;
	struct sockaddr_in SENDAddrToTCPClientTimingRespond;
	struct sockaddr_in RECVAddrToTCPClientTimingRespond;

	int CountTCPClientBufReady_Triggering;
	int CountTCPClientBufReady_Waiting;
	int CountTCPClientBufReady_Triggered;

	int SENDTCPClientBufReady;
	int RECVTCPClientBufReady;
	struct sockaddr_in SENDAddrToTCPClientBufReady;
	struct sockaddr_in RECVAddrToTCPClientBufReady;

	int CountTCPClientMoreData_Triggering;
	int CountTCPClientMoreData_Waiting;
	int CountTCPClientMoreData_Triggered;

	int SENDTCPClientMoreData;
	int RECVTCPClientMoreData;
	struct sockaddr_in SENDAddrToTCPClientMoreData;
	struct sockaddr_in RECVAddrToTCPClientMoreData;

	int CountTCPClientWriteData_Triggering;
	int CountTCPClientWriteData_Waiting;
	int CountTCPClientWriteData_Triggered;

	int SENDTCPClientWriteData;
	int RECVTCPClientWriteData;
	struct sockaddr_in SENDAddrToTCPClientWriteData;
	struct sockaddr_in RECVAddrToTCPClientWriteData;

	int CountTCPClientFileClose_Triggering;
	int CountTCPClientFileClose_Waiting;
	int CountTCPClientFileClose_Triggered;

	int SENDTCPClientFileClose;
	int RECVTCPClientFileClose;
	struct sockaddr_in SENDAddrToTCPClientFileClose;
	struct sockaddr_in RECVAddrToTCPClientFileClose;

	int CountTCPClientStartOrder1_Triggering;
	int CountTCPClientStartOrder1_Waiting;
	int CountTCPClientStartOrder1_Triggered;

	int SENDTCPClientStartOrder1;
	int RECVTCPClientStartOrder1;
	struct sockaddr_in SENDAddrToTCPClientStartOrder1;
	struct sockaddr_in RECVAddrToTCPClientStartOrder1;

	int CountTCPClientStartOrder2_Triggering;
	int CountTCPClientStartOrder2_Waiting;
	int CountTCPClientStartOrder2_Triggered;

	int SENDTCPClientStartOrder2;
	int RECVTCPClientStartOrder2;
	struct sockaddr_in SENDAddrToTCPClientStartOrder2;
	struct sockaddr_in RECVAddrToTCPClientStartOrder2;

	int CountTCPClientStartOrder3_Triggering;
	int CountTCPClientStartOrder3_Waiting;
	int CountTCPClientStartOrder3_Triggered;

	int SENDTCPClientStartOrder3;
	int RECVTCPClientStartOrder3;
	struct sockaddr_in SENDAddrToTCPClientStartOrder3;
	struct sockaddr_in RECVAddrToTCPClientStartOrder3;

	int CountTCPClientStartOrder4_Triggering;
	int CountTCPClientStartOrder4_Waiting;
	int CountTCPClientStartOrder4_Triggered;

	int SENDTCPClientStartOrder4;
	int RECVTCPClientStartOrder4;
	struct sockaddr_in SENDAddrToTCPClientStartOrder4;
	struct sockaddr_in RECVAddrToTCPClientStartOrder4;
};
struct TCPCliPara TCPCliParaList;

/*

	sendto(SENDTCPClientTimingBegin,"",0,0,(struct sockaddr *)&AddrToTCPClientTimingBegin,sizeof(AddrToTCPClientTimingBegin));
	recvfrom(RECVTCPClientTimingBegin,Trigger,20,0,(struct sockaddr *)&AddrToTCPClientTimingBegin,&SockAddrLength);

	sendto(SENDTCPClientTimingEnd,"",0,0,(struct sockaddr *)&AddrToTCPClientTimingEnd,sizeof(AddrToTCPClientTimingEnd));
	recvfrom(RECVTCPClientTimingEnd,Trigger,20,0,(struct sockaddr *)&AddrToTCPClientTimingEnd,&SockAddrLength);

	sendto(SENDTCPClientTimingRespond,"",0,0,(struct sockaddr *)&AddrToTCPClientTimingRespond,sizeof(AddrToTCPClientTimingRespond));
	recvfrom(RECVTCPClientTimingRespond,Trigger,20,0,(struct sockaddr *)&AddrToTCPClientTimingRespond,&SockAddrLength);

	sendto(SENDTCPClientBufReady,"",0,0,(struct sockaddr *)&AddrToTCPClientBufReady,sizeof(AddrToTCPClientBufReady));
	recvfrom(RECVTCPClientBufReady,Trigger,20,0,(struct sockaddr *)&AddrToTCPClientBufReady,&SockAddrLength);

	sendto(SENDTCPClientMoreData,"",0,0,(struct sockaddr *)&AddrToTCPClientMoreData,sizeof(AddrToTCPClientMoreData));
	recvfrom(RECVTCPClientMoreData,Trigger,20,0,(struct sockaddr *)&AddrToTCPClientMoreData,&SockAddrLength);

	sendto(SENDTCPClientWriteData,"",0,0,(struct sockaddr *)&AddrToTCPClientWriteData,sizeof(AddrToTCPClientWriteData));
	recvfrom(RECVTCPClientWriteData,Trigger,20,0,(struct sockaddr *)&AddrToTCPClientWriteData,&SockAddrLength);

	sendto(SENDTCPClientFileClose,"",0,0,(struct sockaddr *)&AddrToTCPClientFileClose,sizeof(AddrToTCPClientFileClose));
	recvfrom(RECVTCPClientFileClose,Trigger,20,0,(struct sockaddr *)&AddrToTCPClientFileClose,&SockAddrLength);

	sendto(SENDTCPClientStartOrder1,"",0,0,(struct sockaddr *)&AddrToTCPClientStartOrder1,sizeof(AddrToTCPClientStartOrder1));
	recvfrom(RECVTCPClientStartOrder1,Trigger,20,0,(struct sockaddr *)&AddrToTCPClientStartOrder1,&SockAddrLength);

	sendto(SENDTCPClientStartOrder2,"",0,0,(struct sockaddr *)&AddrToTCPClientStartOrder2,sizeof(AddrToTCPClientStartOrder2));
	recvfrom(RECVTCPClientStartOrder2,Trigger,20,0,(struct sockaddr *)&AddrToTCPClientStartOrder2,&SockAddrLength);

	sendto(SENDTCPClientStartOrder3,"",0,0,(struct sockaddr *)&AddrToTCPClientStartOrder3,sizeof(AddrToTCPClientStartOrder3));
	recvfrom(RECVTCPClientStartOrder3,Trigger,20,0,(struct sockaddr *)&AddrToTCPClientStartOrder3,&SockAddrLength);

	sendto(SENDTCPClientStartOrder4,"",0,0,(struct sockaddr *)&AddrToTCPClientStartOrder4,sizeof(AddrToTCPClientStartOrder4));
	recvfrom(RECVTCPClientStartOrder4,Trigger,20,0,(struct sockaddr *)&AddrToTCPClientStartOrder4,&SockAddrLength);

*/


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
TCPClient_Parameterinit();

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
);

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
);

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
);

/*****************************************
* �������ƣ�TCPClient_thread_NetworkLayertoTransportLayer
* ��������������ͨ�� - From.���������   To.Ӧ�÷�����
* �����б�
* ���ؽ����
*****************************************/
void *
TCPClient_thread_NetworkLayertoTransportLayer
(
	void * fd
);

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
);

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
);

#endif
