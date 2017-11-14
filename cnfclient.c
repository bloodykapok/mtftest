/*******************************************************************************************************************************************
* �ļ�����cnfclient.c
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

#include"cnfclient.h"

/*******************************************************************************************************************************************
*************************************�궨����������************ȫ�ֱ�������******************************************************************
*******************************************************************************************************************************************/

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
)
{
	//
	int i;
	for(i=0;i<CACHENUM;i++)
	{
		CacheList[i].cachetop = 0;
	}
	

	SockAddrLength = sizeof(struct sockaddr_in);

	//ACK����socket
	bzero(&CNFCliParaList[threadnum].SENDAddrToCNFClientACK,SockAddrLength);
	CNFCliParaList[threadnum].SENDAddrToCNFClientACK.sin_family=AF_INET;
	CNFCliParaList[threadnum].SENDAddrToCNFClientACK.sin_addr.s_addr=inet_addr(CNFThreadList[threadnum].physicalportIP);
	CNFCliParaList[threadnum].SENDAddrToCNFClientACK.sin_port=htons(PhysicalportInterPORT);

	CNFCliParaList[threadnum].SENDCNFClientACK = socket(AF_INET,SOCK_DGRAM,0);

	//SEG����socket
	bzero(&CNFCliParaList[threadnum].RECVAddrToCNFClientSEG,SockAddrLength);
	CNFCliParaList[threadnum].RECVAddrToCNFClientSEG.sin_family=AF_INET;
	CNFCliParaList[threadnum].RECVAddrToCNFClientSEG.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].RECVAddrToCNFClientSEG.sin_port=htons(PhysicalportInterPORT + threadnum * CNFPORTRANGE);

	CNFCliParaList[threadnum].RECVCNFClientSEG = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFCliParaList[threadnum].RECVCNFClientSEG,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientSEG,SockAddrLength);
	
	//��ʱ��ʼ
	CNFCliParaList[threadnum].CountCNFClientTimingBegin_Triggering = 1;
	CNFCliParaList[threadnum].CountCNFClientTimingBegin_Waiting = 1;
	CNFCliParaList[threadnum].CountCNFClientTimingBegin_Triggered = 1;

	bzero(&CNFCliParaList[threadnum].SENDAddrToCNFClientTimingBegin,SockAddrLength);
	CNFCliParaList[threadnum].SENDAddrToCNFClientTimingBegin.sin_family=AF_INET;
	CNFCliParaList[threadnum].SENDAddrToCNFClientTimingBegin.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].SENDAddrToCNFClientTimingBegin.sin_port=htons(CNFClientTimingBegin + threadnum * CNFPORTRANGE);

	bzero(&CNFCliParaList[threadnum].RECVAddrToCNFClientTimingBegin,SockAddrLength);
	CNFCliParaList[threadnum].RECVAddrToCNFClientTimingBegin.sin_family=AF_INET;
	CNFCliParaList[threadnum].RECVAddrToCNFClientTimingBegin.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].RECVAddrToCNFClientTimingBegin.sin_port=htons(CNFClientTimingBegin + threadnum * CNFPORTRANGE);

	CNFCliParaList[threadnum].SENDCNFClientTimingBegin = socket(AF_INET,SOCK_DGRAM,0);
	CNFCliParaList[threadnum].RECVCNFClientTimingBegin = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFCliParaList[threadnum].RECVCNFClientTimingBegin,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientTimingBegin,SockAddrLength);

	//��ʱ����
	CNFCliParaList[threadnum].CountCNFClientTimingEnd_Triggering = 1;
	CNFCliParaList[threadnum].CountCNFClientTimingEnd_Waiting = 1;
	CNFCliParaList[threadnum].CountCNFClientTimingEnd_Triggered = 1;

	bzero(&CNFCliParaList[threadnum].SENDAddrToCNFClientTimingEnd,SockAddrLength);
	CNFCliParaList[threadnum].SENDAddrToCNFClientTimingEnd.sin_family=AF_INET;
	CNFCliParaList[threadnum].SENDAddrToCNFClientTimingEnd.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].SENDAddrToCNFClientTimingEnd.sin_port=htons(CNFClientTimingEnd + threadnum * CNFPORTRANGE);

	bzero(&CNFCliParaList[threadnum].RECVAddrToCNFClientTimingEnd,SockAddrLength);
	CNFCliParaList[threadnum].RECVAddrToCNFClientTimingEnd.sin_family=AF_INET;
	CNFCliParaList[threadnum].RECVAddrToCNFClientTimingEnd.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].RECVAddrToCNFClientTimingEnd.sin_port=htons(CNFClientTimingEnd + threadnum * CNFPORTRANGE);

	CNFCliParaList[threadnum].SENDCNFClientTimingEnd = socket(AF_INET,SOCK_DGRAM,0);
	CNFCliParaList[threadnum].RECVCNFClientTimingEnd = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFCliParaList[threadnum].RECVCNFClientTimingEnd,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientTimingEnd,SockAddrLength);

	//������ʱ���
	CNFCliParaList[threadnum].CountCNFClientTimingRespond_Triggering = 1;
	CNFCliParaList[threadnum].CountCNFClientTimingRespond_Waiting = 1;
	CNFCliParaList[threadnum].CountCNFClientTimingRespond_Triggered = 1;

	bzero(&CNFCliParaList[threadnum].SENDAddrToCNFClientTimingRespond,SockAddrLength);
	CNFCliParaList[threadnum].SENDAddrToCNFClientTimingRespond.sin_family=AF_INET;
	CNFCliParaList[threadnum].SENDAddrToCNFClientTimingRespond.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].SENDAddrToCNFClientTimingRespond.sin_port=htons(CNFClientTimingRespond + threadnum * CNFPORTRANGE);

	bzero(&CNFCliParaList[threadnum].RECVAddrToCNFClientTimingRespond,SockAddrLength);
	CNFCliParaList[threadnum].RECVAddrToCNFClientTimingRespond.sin_family=AF_INET;
	CNFCliParaList[threadnum].RECVAddrToCNFClientTimingRespond.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].RECVAddrToCNFClientTimingRespond.sin_port=htons(CNFClientTimingRespond + threadnum * CNFPORTRANGE);

	CNFCliParaList[threadnum].SENDCNFClientTimingRespond = socket(AF_INET,SOCK_DGRAM,0);
	CNFCliParaList[threadnum].RECVCNFClientTimingRespond = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFCliParaList[threadnum].RECVCNFClientTimingRespond,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientTimingRespond,SockAddrLength);

	//���ջ�������
	CNFCliParaList[threadnum].CountCNFClientBufReady_Triggering = 1;
	CNFCliParaList[threadnum].CountCNFClientBufReady_Waiting = 1;
	CNFCliParaList[threadnum].CountCNFClientBufReady_Triggered = 1;

	bzero(&CNFCliParaList[threadnum].SENDAddrToCNFClientBufReady,SockAddrLength);
	CNFCliParaList[threadnum].SENDAddrToCNFClientBufReady.sin_family=AF_INET;
	CNFCliParaList[threadnum].SENDAddrToCNFClientBufReady.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].SENDAddrToCNFClientBufReady.sin_port=htons(CNFClientBufReady + threadnum * CNFPORTRANGE);

	bzero(&CNFCliParaList[threadnum].RECVAddrToCNFClientBufReady,SockAddrLength);
	CNFCliParaList[threadnum].RECVAddrToCNFClientBufReady.sin_family=AF_INET;
	CNFCliParaList[threadnum].RECVAddrToCNFClientBufReady.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].RECVAddrToCNFClientBufReady.sin_port=htons(CNFClientBufReady + threadnum * CNFPORTRANGE);

	CNFCliParaList[threadnum].SENDCNFClientBufReady = socket(AF_INET,SOCK_DGRAM,0);
	CNFCliParaList[threadnum].RECVCNFClientBufReady = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFCliParaList[threadnum].RECVCNFClientBufReady,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientBufReady,SockAddrLength);

	//���պ�������
	CNFCliParaList[threadnum].CountCNFClientMoreData_Triggering = 1;
	CNFCliParaList[threadnum].CountCNFClientMoreData_Waiting = 1;
	CNFCliParaList[threadnum].CountCNFClientMoreData_Triggered = 1;
	
	bzero(&CNFCliParaList[threadnum].SENDAddrToCNFClientMoreData,SockAddrLength);
	CNFCliParaList[threadnum].SENDAddrToCNFClientMoreData.sin_family=AF_INET;
	CNFCliParaList[threadnum].SENDAddrToCNFClientMoreData.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].SENDAddrToCNFClientMoreData.sin_port=htons(CNFClientMoreData + threadnum * CNFPORTRANGE);

	bzero(&CNFCliParaList[threadnum].RECVAddrToCNFClientMoreData,SockAddrLength);
	CNFCliParaList[threadnum].RECVAddrToCNFClientMoreData.sin_family=AF_INET;
	CNFCliParaList[threadnum].RECVAddrToCNFClientMoreData.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].RECVAddrToCNFClientMoreData.sin_port=htons(CNFClientMoreData + threadnum * CNFPORTRANGE);

	CNFCliParaList[threadnum].SENDCNFClientMoreData = socket(AF_INET,SOCK_DGRAM,0);
	CNFCliParaList[threadnum].RECVCNFClientMoreData = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFCliParaList[threadnum].RECVCNFClientMoreData,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientMoreData,SockAddrLength);

	//֪ͨ������д�뻺��
	CNFCliParaList[threadnum].CountCNFClientWriteData_Triggering = 1;
	CNFCliParaList[threadnum].CountCNFClientWriteData_Waiting = 1;
	CNFCliParaList[threadnum].CountCNFClientWriteData_Triggered = 1;

	bzero(&CNFCliParaList[threadnum].SENDAddrToCNFClientWriteData,SockAddrLength);
	CNFCliParaList[threadnum].SENDAddrToCNFClientWriteData.sin_family=AF_INET;
	CNFCliParaList[threadnum].SENDAddrToCNFClientWriteData.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].SENDAddrToCNFClientWriteData.sin_port=htons(CNFClientWriteData + threadnum * CNFPORTRANGE);

	bzero(&CNFCliParaList[threadnum].RECVAddrToCNFClientWriteData,SockAddrLength);
	CNFCliParaList[threadnum].RECVAddrToCNFClientWriteData.sin_family=AF_INET;
	CNFCliParaList[threadnum].RECVAddrToCNFClientWriteData.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].RECVAddrToCNFClientWriteData.sin_port=htons(CNFClientWriteData + threadnum * CNFPORTRANGE);

	CNFCliParaList[threadnum].SENDCNFClientWriteData = socket(AF_INET,SOCK_DGRAM,0);
	CNFCliParaList[threadnum].RECVCNFClientWriteData = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFCliParaList[threadnum].RECVCNFClientWriteData,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientWriteData,SockAddrLength);

	//�ļ�д�����
	CNFCliParaList[threadnum].CountCNFClientFileClose_Triggering = 1;
	CNFCliParaList[threadnum].CountCNFClientFileClose_Waiting = 1;
	CNFCliParaList[threadnum].CountCNFClientFileClose_Triggered = 1;

	bzero(&CNFCliParaList[threadnum].SENDAddrToCNFClientFileClose,SockAddrLength);
	CNFCliParaList[threadnum].SENDAddrToCNFClientFileClose.sin_family=AF_INET;
	CNFCliParaList[threadnum].SENDAddrToCNFClientFileClose.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].SENDAddrToCNFClientFileClose.sin_port=htons(CNFClientFileClose + threadnum * CNFPORTRANGE);

	bzero(&CNFCliParaList[threadnum].RECVAddrToCNFClientFileClose,SockAddrLength);
	CNFCliParaList[threadnum].RECVAddrToCNFClientFileClose.sin_family=AF_INET;
	CNFCliParaList[threadnum].RECVAddrToCNFClientFileClose.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].RECVAddrToCNFClientFileClose.sin_port=htons(CNFClientFileClose + threadnum * CNFPORTRANGE);

	CNFCliParaList[threadnum].SENDCNFClientFileClose = socket(AF_INET,SOCK_DGRAM,0);
	CNFCliParaList[threadnum].RECVCNFClientFileClose = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFCliParaList[threadnum].RECVCNFClientFileClose,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientFileClose,SockAddrLength);

	//����˳��1
	CNFCliParaList[threadnum].CountCNFClientStartOrder1_Triggering = 1;
	CNFCliParaList[threadnum].CountCNFClientStartOrder1_Waiting = 1;
	CNFCliParaList[threadnum].CountCNFClientStartOrder1_Triggered = 1;
	
	bzero(&CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder1,SockAddrLength);
	CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder1.sin_family=AF_INET;
	CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder1.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder1.sin_port=htons(CNFClientStartOrder1 + threadnum * CNFPORTRANGE);

	bzero(&CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder1,SockAddrLength);
	CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder1.sin_family=AF_INET;
	CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder1.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder1.sin_port=htons(CNFClientStartOrder1 + threadnum * CNFPORTRANGE);

	CNFCliParaList[threadnum].SENDCNFClientStartOrder1 = socket(AF_INET,SOCK_DGRAM,0);
	CNFCliParaList[threadnum].RECVCNFClientStartOrder1 = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFCliParaList[threadnum].RECVCNFClientStartOrder1,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder1,SockAddrLength);

	//����˳��2
	CNFCliParaList[threadnum].CountCNFClientStartOrder2_Triggering = 1;
	CNFCliParaList[threadnum].CountCNFClientStartOrder2_Waiting = 1;
	CNFCliParaList[threadnum].CountCNFClientStartOrder2_Triggered = 1;
	
	bzero(&CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder2,SockAddrLength);
	CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder2.sin_family=AF_INET;
	CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder2.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder2.sin_port=htons(CNFClientStartOrder2 + threadnum * CNFPORTRANGE);

	bzero(&CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder2,SockAddrLength);
	CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder2.sin_family=AF_INET;
	CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder2.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder2.sin_port=htons(CNFClientStartOrder2 + threadnum * CNFPORTRANGE);

	CNFCliParaList[threadnum].SENDCNFClientStartOrder2 = socket(AF_INET,SOCK_DGRAM,0);
	CNFCliParaList[threadnum].RECVCNFClientStartOrder2 = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFCliParaList[threadnum].RECVCNFClientStartOrder2,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder2,SockAddrLength);

	//����˳��3
	CNFCliParaList[threadnum].CountCNFClientStartOrder3_Triggering = 1;
	CNFCliParaList[threadnum].CountCNFClientStartOrder3_Waiting = 1;
	CNFCliParaList[threadnum].CountCNFClientStartOrder3_Triggered = 1;
	
	bzero(&CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder3,SockAddrLength);
	CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder3.sin_family=AF_INET;
	CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder3.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder3.sin_port=htons(CNFClientStartOrder3 + threadnum * CNFPORTRANGE);

	bzero(&CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder3,SockAddrLength);
	CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder3.sin_family=AF_INET;
	CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder3.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder3.sin_port=htons(CNFClientStartOrder3 + threadnum * CNFPORTRANGE);

	CNFCliParaList[threadnum].SENDCNFClientStartOrder3 = socket(AF_INET,SOCK_DGRAM,0);
	CNFCliParaList[threadnum].RECVCNFClientStartOrder3 = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFCliParaList[threadnum].RECVCNFClientStartOrder3,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder3,SockAddrLength);

	//����˳��4
	CNFCliParaList[threadnum].CountCNFClientStartOrder4_Triggering = 1;
	CNFCliParaList[threadnum].CountCNFClientStartOrder4_Waiting = 1;
	CNFCliParaList[threadnum].CountCNFClientStartOrder4_Triggered = 1;
	
	bzero(&CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder4,SockAddrLength);
	CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder4.sin_family=AF_INET;
	CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder4.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder4.sin_port=htons(CNFClientStartOrder4 + threadnum * CNFPORTRANGE);

	bzero(&CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder4,SockAddrLength);
	CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder4.sin_family=AF_INET;
	CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder4.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder4.sin_port=htons(CNFClientStartOrder4 + threadnum * CNFPORTRANGE);

	CNFCliParaList[threadnum].SENDCNFClientStartOrder4 = socket(AF_INET,SOCK_DGRAM,0);
	CNFCliParaList[threadnum].RECVCNFClientStartOrder4 = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFCliParaList[threadnum].RECVCNFClientStartOrder4,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder4,SockAddrLength);

}

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
)
{
	//����CNF�������ȡDATA������λ��
	int DATALocation = CNFThreadList[threadnum].datapkglocation;
	
	//��DATA������λ����ȡ��ز���
	int pkglength = datapkglist[DATALocation].pkglength;
	uint8_t local_ip[16];
	memcpy(local_ip,datapkglist[DATALocation].local_ip,16);

	//UDP��������

	char pkg[BUFSIZE];
	
	int socket_sender;
	socket_sender=socket(AF_INET,SOCK_DGRAM,0);
	
	struct sockaddr_in addrFrom;
	bzero(&addrFrom,sizeof(addrFrom));
	addrFrom.sin_family=AF_INET;
	//addrFrom.sin_addr.s_addr=htonl(INADDR_ANY);
	addrFrom.sin_addr.s_addr=inet_addr(local_ip);
	addrFrom.sin_port=htons(0);

	int error = bind(socket_sender,(struct sockaddr *)&addrFrom,sizeof(addrFrom));
	if(error != 0)
	{
		printf("bind() error!\n");
	}

	memcpy(pkg,"NET",3);
	memcpy(pkg+3,datapkglist[DATALocation].datapkg+14,pkglength);
	
	struct sockaddr_in addrTo;
	bzero(&addrTo,sizeof(addrTo));
	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons(PhysicalportInterPORT);
	addrTo.sin_addr.s_addr=inet_addr("127.1.1.1");
	
	if(cnfcliDEVETESTIMPL <= 6)
	{
		//printf("[%s] UDP Message has been sent out:%s\n",datapkglist[DATALocation].physicalport,pkg);
		printf("[%s] UDP Message has been sent out.\n",datapkglist[DATALocation].physicalport);
		printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
	}

	sendto(socket_sender,pkg,pkglength+3,0,(struct sockaddr *)&addrTo,sizeof(addrTo));
	close(socket_sender);

	//�ͷŸ�CNF���������ڲ�ͨ�ž��
	close(CNFCliParaList[threadnum].SENDCNFClientACK);
	close(CNFCliParaList[threadnum].RECVCNFClientSEG);
	close(CNFCliParaList[threadnum].SENDCNFClientTimingBegin);
	close(CNFCliParaList[threadnum].RECVCNFClientTimingBegin);
	close(CNFCliParaList[threadnum].SENDCNFClientTimingEnd);
	close(CNFCliParaList[threadnum].RECVCNFClientTimingEnd);
	close(CNFCliParaList[threadnum].SENDCNFClientTimingRespond);
	close(CNFCliParaList[threadnum].RECVCNFClientTimingRespond);
	close(CNFCliParaList[threadnum].SENDCNFClientBufReady);
	close(CNFCliParaList[threadnum].RECVCNFClientBufReady);
	close(CNFCliParaList[threadnum].SENDCNFClientMoreData);
	close(CNFCliParaList[threadnum].RECVCNFClientMoreData);
	close(CNFCliParaList[threadnum].SENDCNFClientWriteData);
	close(CNFCliParaList[threadnum].RECVCNFClientWriteData);
	close(CNFCliParaList[threadnum].SENDCNFClientFileClose);
	close(CNFCliParaList[threadnum].RECVCNFClientFileClose);
	close(CNFCliParaList[threadnum].SENDCNFClientStartOrder1);
	close(CNFCliParaList[threadnum].RECVCNFClientStartOrder1);
	close(CNFCliParaList[threadnum].SENDCNFClientStartOrder2);
	close(CNFCliParaList[threadnum].RECVCNFClientStartOrder2);
	close(CNFCliParaList[threadnum].SENDCNFClientStartOrder3);
	close(CNFCliParaList[threadnum].RECVCNFClientStartOrder3);
	close(CNFCliParaList[threadnum].SENDCNFClientStartOrder4);
	close(CNFCliParaList[threadnum].RECVCNFClientStartOrder4);

	//�ͷŸ�CNF�������б���
	//��������ʼʱ�г�ʼ�����룩
	
	//�ͷ�DATA����λ��
	datapkglist[DATALocation].pkglength = 0;
	memset(datapkglist[DATALocation].physicalport,0,30);
	memset(datapkglist[DATALocation].local_ip,0,16);
	memset(datapkglist[DATALocation].sid,0,SIDLEN);
	memset(datapkglist[DATALocation].offset,0,OFFLEN);
	memset(datapkglist[DATALocation].datapkg,0,pkglength);
	
	//�ͷ�CNF�������
	CNFThreadList[threadnum].flag_IO = -1;
	CNFThreadList[threadnum].datapkglocation = -1;
	CNFThreadList[threadnum].cachelocation = -1;
	memset(CNFThreadList[threadnum].physicalport,0,30);
	memset(CNFThreadList[threadnum].physicalportIP,0,16);
	memset(CNFThreadList[threadnum].sid,0,SIDLEN);
	memset(CNFThreadList[threadnum].offset,0,OFFLEN);
	
	//��������ռ��
	datapkglist[DATALocation].flag_occupied = 0;
	CNFThreadList[threadnum].flag_occupied = 0;
	
	if(cnfcliDEVETESTIMPL <= 6)
		printf("threadnum %d released.\n",threadnum);
}

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
)
{
	//����CNF�������ȡDATA������λ��
	int DATALocation = CNFThreadList[threadnum].datapkglocation;
	
	//��DATA������λ����ȡ��ز���
	int pkglength = datapkglist[DATALocation].pkglength;
	uint8_t local_ip[16];
	memcpy(local_ip,datapkglist[DATALocation].local_ip,16);

	//���ת��������һ�а�����data(161)���������ACK(164)��������죬�ʴ�Ϊ�ٶ����е�����·��Ȩ��֮���������ܽ�����⣩
	int i;
	for(i=0;i<10;i++)
	{
		PkgBUF[i].flag_occupied = 0;
	}

	//���ݰ�����ת��������У��˴���Ҫ�߳�����
	//printf("[CNF Client Stopping] Locking...\n");
	pthread_mutex_lock(&lockPkgBUF);
	//printf("[CNF Client Stopping] Locked.\n");
	int LocalPkgBUFHead;
	if(PkgBUF[PkgBUFHead].flag_occupied == 0)
	{
		LocalPkgBUFHead = PkgBUFHead;
		PkgBUFHead++;
		if(PkgBUFHead >=10)
			PkgBUFHead -= 10;
		
		PkgBUF[LocalPkgBUFHead].flag_occupied = 1;
		PkgBUF[LocalPkgBUFHead].pkglength = pkglength;
		strcpy(PkgBUF[LocalPkgBUFHead].sourcepp,datapkglist[DATALocation].physicalport);
		memcpy(PkgBUF[LocalPkgBUFHead].pkg,datapkglist[DATALocation].datapkg+14,pkglength);
		
		//����������ź���
		sem_post(&semPacketRECV);

		if(cnfcliDEVETESTIMPL <= 6)
		{
			printf("[%s] SEM Message has been sent to NetworkLayer:\n",datapkglist[DATALocation].physicalport);
			printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
		}
	}
	else
	{
		if(cnfcliDEVETESTIMPL <= 6)
		{
			printf("[%s] SEM Message failed to be sent to NetworkLayer:\n",datapkglist[DATALocation].physicalport);
			printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
		}
	}

	//�ر���������

	//�ͷŸ�CNF���������ڲ�ͨ�ž��
	close(CNFCliParaList[threadnum].SENDCNFClientACK);
	close(CNFCliParaList[threadnum].RECVCNFClientSEG);
	close(CNFCliParaList[threadnum].SENDCNFClientTimingBegin);
	close(CNFCliParaList[threadnum].RECVCNFClientTimingBegin);
	close(CNFCliParaList[threadnum].SENDCNFClientTimingEnd);
	close(CNFCliParaList[threadnum].RECVCNFClientTimingEnd);
	close(CNFCliParaList[threadnum].SENDCNFClientTimingRespond);
	close(CNFCliParaList[threadnum].RECVCNFClientTimingRespond);
	close(CNFCliParaList[threadnum].SENDCNFClientBufReady);
	close(CNFCliParaList[threadnum].RECVCNFClientBufReady);
	close(CNFCliParaList[threadnum].SENDCNFClientMoreData);
	close(CNFCliParaList[threadnum].RECVCNFClientMoreData);
	close(CNFCliParaList[threadnum].SENDCNFClientWriteData);
	close(CNFCliParaList[threadnum].RECVCNFClientWriteData);
	close(CNFCliParaList[threadnum].SENDCNFClientFileClose);
	close(CNFCliParaList[threadnum].RECVCNFClientFileClose);
	close(CNFCliParaList[threadnum].SENDCNFClientStartOrder1);
	close(CNFCliParaList[threadnum].RECVCNFClientStartOrder1);
	close(CNFCliParaList[threadnum].SENDCNFClientStartOrder2);
	close(CNFCliParaList[threadnum].RECVCNFClientStartOrder2);
	close(CNFCliParaList[threadnum].SENDCNFClientStartOrder3);
	close(CNFCliParaList[threadnum].RECVCNFClientStartOrder3);
	close(CNFCliParaList[threadnum].SENDCNFClientStartOrder4);
	close(CNFCliParaList[threadnum].RECVCNFClientStartOrder4);

	//�ͷŸ�CNF�������б���
	//��������ʼʱ�г�ʼ�����룩
	
	//�ͷ�DATA����λ��
	datapkglist[DATALocation].pkglength = 0;
	memset(datapkglist[DATALocation].physicalport,0,30);
	memset(datapkglist[DATALocation].local_ip,0,16);
	memset(datapkglist[DATALocation].sid,0,SIDLEN);
	memset(datapkglist[DATALocation].offset,0,OFFLEN);
	memset(datapkglist[DATALocation].datapkg,0,pkglength);
	
	//�ͷ�CNF�������
	CNFThreadList[threadnum].flag_IO = -1;
	CNFThreadList[threadnum].datapkglocation = -1;
	CNFThreadList[threadnum].cachelocation = -1;
	memset(CNFThreadList[threadnum].physicalport,0,30);
	memset(CNFThreadList[threadnum].physicalportIP,0,16);
	memset(CNFThreadList[threadnum].sid,0,SIDLEN);
	memset(CNFThreadList[threadnum].offset,0,OFFLEN);
	
	//��������ռ��
	datapkglist[DATALocation].flag_occupied = 0;
	CNFThreadList[threadnum].flag_occupied = 0;
	
	if(cnfcliDEVETESTIMPL <= 6)
		printf("threadnum %d released.\n",threadnum);
	
	pthread_mutex_unlock(&lockPkgBUF);
	//printf("[CNF Client Stopping] Lock Released.\n");
}

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
)
{
	//��CNFClientģ������̵߳��õĲ��������ӿ�
	struct cnfcliinput * cnfinput;
	cnfinput = (struct cnfcliinput *)fd;
	
	int threadnum = cnfinput->threadnum;

	if(cnfcliDEVETESTIMPL <= 6)
		printf("CNF threadnum == %d (CNFClient_thread_timer)\n",threadnum);

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

	sleep(10000);
	//�ر��߳�
	//free(fd);
	//pthread_exit(NULL);
}

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
)
{
	//��CNFClientģ������̵߳��õĲ��������ӿ�
	struct cnfcliinput * cnfinput;
	cnfinput = (struct cnfcliinput *)fd;
	
	int threadnum = cnfinput->threadnum;
	
	if(cnfcliDEVETESTIMPL <= 6)
		printf("CNF threadnum == %d (CNFClient_thread_CNF)\n",threadnum);

	uint8_t PhysicalportIP[16];
	memcpy(PhysicalportIP,cnfinput->physicalportIP,16);

	//usleep(ProgramWaitingPeriod);//�ȴ�physicalport����������CNF�����У��ò���
	
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
	memset(CNFCliParaList[threadnum].SID,0,SIDLEN);//�����������SID
	CNFCliParaList[threadnum].SIDLen = 0;//SID��ʵ�ʳ���

	CNFCliParaList[threadnum].BUF = NULL;//���ͻ�������ͷ
	CNFCliParaList[threadnum].BUFEnd = NULL;//���ͻ�������β
	CNFCliParaList[threadnum].BUFFileEnd = NULL;//��ȡ�ļ�����ʱ�����������ļ�ĩβ���ڵ�ַ

	CNFCliParaList[threadnum].BUFRenewCount = 0;//���������´���
	CNFCliParaList[threadnum].BUFLength = CNFBUFSIZE;//���ͻ��峤��

	CNFCliParaList[threadnum].FileWritePoint = 0;//�ļ�д�����
	CNFCliParaList[threadnum].FileLength = 0;//�ļ��ܳ���

	CNFCliParaList[threadnum].PacketDataLength = PKTDATALEN;//�������ݶγ���
	CNFCliParaList[threadnum].LastPacketDataLength = 0;//����������������һ�����ݰ�ʵ�ʳ���

	CNFCliParaList[threadnum].OffsetConfirmDATA = 0;//�������ļ���ȷ���յ����ֽڶ���
	CNFCliParaList[threadnum].OffsetPacket = 0;//�������ļ��е�ǰ����ֽ���
	CNFCliParaList[threadnum].OffsetWaitDATA = PKTDATALEN;//�������ļ��еȴ�ȷ���յ����ֽ���������ACK���ݰ�ƫ�����ֶε���д���ݣ�

	CNFCliParaList[threadnum].AddrConfirmDATA = NULL;//�ڻ���������ȷ��ƫ�������ڵ�ַ
	CNFCliParaList[threadnum].AddrPacket = NULL;//�ڻ������е�ǰ���ƫ�������ڵ�ַ
	CNFCliParaList[threadnum].AddrWaitDATA = NULL;//�ڻ������д�ȷ��ƫ�������ڵ�ַ

	CNFCliParaList[threadnum].RTOs = 0;//��������ʱ�ӣ�RTO����1�벿�֣���λ�룩
	CNFCliParaList[threadnum].RTOus = 0;//��������ʱ�ӣ�RTOС��1�벿�֣���λ΢�룩
	CNFCliParaList[threadnum].RTOns = 0;//��������ʱ�ӣ�RTOС��1�벿�֣���λ���룩

	CNFCliParaList[threadnum].FlagInputBUF = 0;//�Ƿ��յ�������д�뻺��

	CNFCliParaList[threadnum].TimingState = 0;//��ʱ״̬��0��ʱ��1��ʱ
	CNFCliParaList[threadnum].ThreadState = 0;//����״̬�����ڽ������̵Ŀ��ƣ�

	if(cnfcliDEVETESTIMPL <= 6)
	{
		printf("xxx waiting...\n");
		printf("xxx triggering...\n");
		printf("xxx triggered!\n");
	}

	//����FTP�߳̿�ʼ
	if(cnfINTRACOMMUNICATION == 0)
	{
		sendto(CNFCliParaList[threadnum].SENDCNFClientStartOrder1,"StartOrder1",20,0,(struct sockaddr *)&CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder1,SockAddrLength);
	}
	else if(cnfINTRACOMMUNICATION == 1)
	{
		sem_post(&CNFCliParaList[threadnum].semCNFClientStartOrder1);
	}
	
	if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
		printf("[%d] StartOrder1 triggering...\n",CNFCliParaList[threadnum].CountCNFClientStartOrder1_Triggering);
	CNFCliParaList[threadnum].CountCNFClientStartOrder1_Triggering++;

	//����CNF�����߳̿�ʼ
	if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
		printf("[%d] StartOrder3 waiting...\n",CNFCliParaList[threadnum].CountCNFClientStartOrder3_Waiting);
	CNFCliParaList[threadnum].CountCNFClientStartOrder3_Waiting++;
	
	if(cnfINTRACOMMUNICATION == 0)
	{
		recvfrom(CNFCliParaList[threadnum].RECVCNFClientStartOrder3,Trigger,20,0,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder3,&SockAddrLength);
	}
	else if(cnfINTRACOMMUNICATION == 1)
	{
		sem_wait(&CNFCliParaList[threadnum].semCNFClientStartOrder3);
	}
	
	if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
		printf("[%d] StartOrder3 triggered!\n",CNFCliParaList[threadnum].CountCNFClientStartOrder3_Triggered);
	CNFCliParaList[threadnum].CountCNFClientStartOrder3_Triggered++;

	//���������߳̿�ʼ
	if(cnfINTRACOMMUNICATION == 0)
	{
		sendto(CNFCliParaList[threadnum].SENDCNFClientStartOrder4,"StartOrder4",20,0,(struct sockaddr *)&CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder4,SockAddrLength);
	}
	else if(cnfINTRACOMMUNICATION == 1)
	{
		sem_post(&CNFCliParaList[threadnum].semCNFClientStartOrder4);
	}
	
	if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
		printf("[%d] StartOrder4 triggering...\n",CNFCliParaList[threadnum].CountCNFClientStartOrder4_Triggering);
	CNFCliParaList[threadnum].CountCNFClientStartOrder4_Triggering++;

	//ѭ������������Դ�ɹ�������ı���׼��
	int flag_sidgot = 0;
	int counter_sidget = 0;

	if(cnfcliDEVETESTIMPL <= 1)
		printf("[Transport Layer] CNFClient responce listening start...\n");
	
	uint8_t recvpkg[sizeof(CoLoR_seg)];
	CoLoR_seg * precvpkg;
	precvpkg = (CoLoR_seg *)recvpkg;

	int FoundSIDNum = -1;
	
	//ѭ������������Դ�ɹ�������
	while(1)
	{
		i=0;
		j=0;

		//��ջ��壬Ϊ����������׼��
		DATAlen = 0;
		REQoffsetlong = 0;
		RESoffsetlong = 0;
		memset(DATA,0,DATALEN);
		memset(recvpkg,0,sizeof(CoLoR_seg));

		FoundSIDNum = -1;

		//����
		recvfrom(CNFCliParaList[threadnum].RECVCNFClientSEG,recvpkg,sizeof(CoLoR_seg),0,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientSEG,&SockAddrLength);
		
		if(cnfcliDEVETESTIMPL <= 1)
			printf("[Transport Layer] CNFClient responce listener received a new UDP message.\n");
		
		//��ȡDATA��offset
		memcpy(DATA,precvpkg->data,DATALEN);
		memcpy(RESoffset,precvpkg->offset,OFFLEN);
		DATAlen = DATALEN;

		//�յ������ݳ����ۻ�����Ԥ֪�ļ����ȣ��ݴ���ƣ����ǽ������ƣ����Գ����ж�������Ҫ���ڽ���������
		if((long)CNFCliParaList[threadnum].OffsetConfirmDATA - (long)DATALEN >= CACHESIZE)//ǿ��long��ת���Ǳ�������������������
		{
			if(cnfcliDEVETESTIMPL <= 6)
			{
				printf("Received File Length   = %ld\n",(long)CNFCliParaList[threadnum].OffsetConfirmDATA - (long)DATALEN);
				printf("File Length Ever known = %ld\n",(unsigned long)CACHESIZE);
				printf("[Deadly Error] Received File Length is Bigger Than the File Length Ever known.\n");
			}
			
			CNFCliParaList[threadnum].FlagInputBUF = 0;
			sendto(CNFCliParaList[threadnum].SENDCNFClientTimingEnd,"TimingEnd",20,0,(struct sockaddr *)&CNFCliParaList[threadnum].SENDAddrToCNFClientTimingEnd,SockAddrLength);

			if(cnfcliDEVETESTIMPL <= 6 || CNFTRIGGERTEST == 1)
				printf("[%d] TimingEnd triggering...\n",CNFCliParaList[threadnum].CountCNFClientTimingEnd_Triggering);

			CNFCliParaList[threadnum].CountCNFClientTimingEnd_Triggering++;
		}
	
		if(cnfcliDEVETESTIMPL <= 5)
		{
			printf("[DATAlen]\n%d\n",DATAlen);
			//printf("[DATA]\n%s\n",DATA);
			printf("[DATA-END]\n");
		}

		//����CNF��������нӿ����ã��ӿ���Ϣ��ʽGOTdataOFFoffset
		//�˴�ȱ�ٶ�������ȷ�Ե�У��
		
		//�ж�OFFSET�ĺϷ��ԣ�������Ҫ�������
		if(RESoffset[0] >= 0 && RESoffset[1] >= 0 && RESoffset[2] >= 0 && RESoffset[3] >= 0)
		{
			RESoffsetlong = (unsigned long)RESoffset[0] * 256 * 256 * 256 + 
					(unsigned long)RESoffset[1] * 256 * 256 + 
					(unsigned long)RESoffset[2] * 256 + 
					(unsigned long)RESoffset[3];
			
			//������SID��Ӧ�������̱߳��
			if(strncmp(CNFCliParaList[threadnum].SID,SID,SIDLEN) == 0)
			{
				FoundSIDNum = 0;
			}
			if(FoundSIDNum == -1)
			{
				printf("An unknown ACK Packet received, SID not found.\n");
				exit(0);
			}

			CNFCliParaList[threadnum].SIDLen = SIDlen;
			//if(RESoffsetlong >= 10000)exit(0);
			
			if(cnfcliDEVETESTIMPL <= 5)
			{
				printf("[RECV]RESoffsetlong  = %ld\n",RESoffsetlong);
				printf("[RECV]OffsetWaitDATA = %ld\n",CNFCliParaList[threadnum].OffsetWaitDATA);
			}
			
			if(RESoffsetlong == CNFCliParaList[threadnum].OffsetWaitDATA)//�յ���ǰ�������ƫ������д����ջ��壬����ȷ��ƫ����ֵ
			{
				//printf("New Data Input!\n");
				//�򻺳��������ո��յ��������ֶ�
				
				//�յ����ݶγ��ȴ��ڱ���CNF�涨�ı�׼�������ݶγ��ȣ�
				//��ζ���ļ���δ�������ļ���СǡΪPacketDataLength��������������⣩
				if(DATAlen == CNFCliParaList[threadnum].PacketDataLength)
				{
					//���յ������ݿ���������
					memcpy(CNFCliParaList[threadnum].AddrConfirmDATA,DATA,CNFCliParaList[threadnum].PacketDataLength);

					//�ƶ����ඨλ��ʶ
					CNFCliParaList[threadnum].OffsetConfirmDATA = RESoffsetlong;
					CNFCliParaList[threadnum].OffsetWaitDATA    = RESoffsetlong + DATAlen;
					CNFCliParaList[threadnum].AddrConfirmDATA  += CNFCliParaList[threadnum].PacketDataLength;

					//�������ػ�����
					if(CNFCliParaList[threadnum].AddrConfirmDATA >= CNFCliParaList[threadnum].BUFEnd)
					{
						CNFCliParaList[threadnum].AddrConfirmDATA = CNFCliParaList[threadnum].BUF + ( CNFCliParaList[threadnum].AddrConfirmDATA - CNFCliParaList[threadnum].BUFEnd );
					}
				}
				//�յ����ݶγ���С�ڱ���CNF�涨�ı�׼�������ݶγ��ȣ�
				//��ζ�������ļ������һ������Ӧ���������ļ����ɺͽ�������Ĵ���
				/*
				else if(DATAlen < CNFCliParaList[threadnum].PacketDataLength)
				{
					//���յ������ݿ���������
					memcpy(CNFCliParaList[threadnum].AddrConfirmDATA,DATA,DATAlen);
					
					//�ƶ����ඨλ��ʶ
					CNFCliParaList[threadnum].OffsetWaitDATA = RESoffsetlong + DATAlen;
					CNFCliParaList[threadnum].AddrConfirmDATA += CNFCliParaList[threadnum].PacketDataLength;

					//֪ͨ���ļ�д������
					sendto(CNFCliParaList[threadnum].SENDCNFClientWriteData,"WriteData",20,0,(struct sockaddr *)&CNFCliParaList[threadnum].SENDAddrToCNFClientWriteData,SockAddrLength);

					if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
						printf("[%d] WriteData triggering...\n",CNFCliParaList[threadnum].CountCNFClientWriteData_Triggering);
					CNFCliParaList[threadnum].CountCNFClientWriteData_Triggering++;
				}
				*/
				//�յ����ݶγ��Ȳ����ڱ���CNF�涨�ı�׼�������ݶγ��ȣ�
				//��������Э�������ƥ�䣬��Ҫ�޸�����
				else
				{
					printf("Deadly error!(DATAlen == %d > CNFCliParaList[threadnum].PacketDataLength)\n",DATAlen);
					exit(0);
				}

				CNFCliParaList[threadnum].FlagInputBUF = 1;
				
				//֪ͨ������ʱ
				sendto(CNFCliParaList[threadnum].SENDCNFClientTimingEnd,"TimingEnd",20,0,(struct sockaddr *)&CNFCliParaList[threadnum].SENDAddrToCNFClientTimingEnd,SockAddrLength);

				if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
					printf("[%d] TimingEnd triggering...\n",CNFCliParaList[threadnum].CountCNFClientTimingEnd_Triggering);
				CNFCliParaList[threadnum].CountCNFClientTimingEnd_Triggering++;
			}
			else if(RESoffsetlong != CNFCliParaList[threadnum].OffsetWaitDATA)//�յ��ǵ�ǰ�������ƫ�������ظ����͵�ǰ�������ƫ����
			{
				CNFCliParaList[threadnum].FlagInputBUF = 0;

				sendto(CNFCliParaList[threadnum].SENDCNFClientTimingEnd,"TimingEnd",20,0,(struct sockaddr *)&CNFCliParaList[threadnum].SENDAddrToCNFClientTimingEnd,SockAddrLength);

				if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
					printf("[%d] TimingEnd triggering...\n",CNFCliParaList[threadnum].CountCNFClientTimingEnd_Triggering);
				CNFCliParaList[threadnum].CountCNFClientTimingEnd_Triggering++;
			}
		}
		else//offsetֵ�Ƿ�
		{
			//����ACK�ش�
			CNFCliParaList[threadnum].FlagInputBUF = 0;

			sendto(CNFCliParaList[threadnum].SENDCNFClientTimingEnd,"TimingEnd",20,0,(struct sockaddr *)&CNFCliParaList[threadnum].SENDAddrToCNFClientTimingEnd,SockAddrLength);

			if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
				printf("[%d] TimingEnd triggering...\n",CNFCliParaList[threadnum].CountCNFClientTimingEnd_Triggering);
			CNFCliParaList[threadnum].CountCNFClientTimingEnd_Triggering++;
		}
	}

	sleep(10000);
	//�ر��߳�
	//exit(0);
	//free(fd);
	//pthread_exit(NULL);	
}

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
)
{
	//��CNFClientģ������̵߳��õĲ��������ӿ�
	struct cnfcliinput * cnfinput;
	cnfinput = (struct cnfcliinput *)fd;
	
	int threadnum = cnfinput->threadnum;

	if(cnfcliDEVETESTIMPL <= 6)
		printf("CNF threadnum     == %d (CNFClient_thread_FTP)\n",threadnum);

	int CacheLocation = CNFThreadList[cnfinput->threadnum].cachelocation;

	if(cnfcliDEVETESTIMPL <= 6)
		printf("CNF CacheLocation == %d (CNFClient_thread_FTP)\n",CacheLocation);

	//��ȡ������
	//int ThreadNum = *((int *)fd);printf("[FTP]ThreadNum = %d\n",ThreadNum);
	
	//����FTP�߳̿�ʼ
	if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
		printf("[%d] StartOrder1 waiting...\n",CNFCliParaList[threadnum].CountCNFClientStartOrder1_Waiting);
	CNFCliParaList[threadnum].CountCNFClientStartOrder1_Waiting++;

	if(cnfINTRACOMMUNICATION == 0)
	{
		recvfrom(CNFCliParaList[threadnum].RECVCNFClientStartOrder1,Trigger,20,0,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder1,&SockAddrLength);
	}
	else if(cnfINTRACOMMUNICATION == 1)
	{
		sem_wait(&CNFCliParaList[threadnum].semCNFClientStartOrder1);
	}

	if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
		printf("[%d] StartOrder1 triggered!\n",CNFCliParaList[threadnum].CountCNFClientStartOrder1_Triggered);
	CNFCliParaList[threadnum].CountCNFClientStartOrder1_Triggered++;

	//Ϊ����������ջ����ڴ�
	CNFCliParaList[threadnum].BUF = (uint8_t *)malloc(CNFBUFSIZE);
	memset(CNFCliParaList[threadnum].BUF,0,CNFBUFSIZE);
	
	//������ؿ��Ʋ���
	CNFCliParaList[threadnum].BUFEnd = CNFCliParaList[threadnum].BUF + CNFBUFSIZE;//���ͻ�������β

	CNFCliParaList[threadnum].BUFRenewCount = 0;//���������´���
	CNFCliParaList[threadnum].BUFLength = CNFBUFSIZE;//���ͻ��峤��

	CNFCliParaList[threadnum].FileWritePoint = 0;//�ļ�д�����
	CNFCliParaList[threadnum].FileLength = 0;//�ļ��ܳ���

	CNFCliParaList[threadnum].PacketDataLength = PKTDATALEN;//�������ݶγ���
	CNFCliParaList[threadnum].LastPacketDataLength = 0;//����������������һ�����ݰ�ʵ�ʳ���
	
	CNFCliParaList[threadnum].AddrConfirmDATA = CNFCliParaList[threadnum].BUF;//�ڻ���������ȷ��ƫ�������ڵ�ַ
	CNFCliParaList[threadnum].AddrPacket = CNFCliParaList[threadnum].BUF;//�ڻ������е�ǰ���ƫ�������ڵ�ַ
	CNFCliParaList[threadnum].AddrWaitDATA = CNFCliParaList[threadnum].BUF;//�ڻ������д�ȷ��ƫ�������ڵ�ַ

	//���������Ŷ�Ӧ�Ĳ�����SIDѰ�Ҷ�Ӧ���ļ�
	
	
	//���ļ�����ȡ�������
	/*
	int fp;
	unsigned long filelen = FILELEN;//debuglog.txt���ļ���С
	*/

	unsigned long cachelen = CACHESIZE;
	
	//�������򿪿ͻ��˽����ļ�
	/*
	fp = open(FTPClientPath,O_RDWR | O_CREAT | O_APPEND | O_TRUNC);
	if(fp == -1)
	{
		printf("[Deadly Error] The Client File \"%s\" opening failed.\n",FTPClientPath);
		exit(0);
	}
	*/
	
	//֪ͨ����㣬���ͻ����Ѿ�׼����������ʱ���ã�
	if(cnfINTRACOMMUNICATION == 0)
	{
		sendto(CNFCliParaList[threadnum].SENDCNFClientBufReady,"BufReady",20,0,(struct sockaddr *)&CNFCliParaList[threadnum].SENDAddrToCNFClientBufReady,SockAddrLength);
	}
	else if(cnfINTRACOMMUNICATION == 1)
	{
		sem_post(&CNFCliParaList[threadnum].semCNFClientBufReady);
	}

	if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
		printf("[%d] BufReady triggering...\n",CNFCliParaList[threadnum].CountCNFClientBufReady_Triggering);
	CNFCliParaList[threadnum].CountCNFClientBufReady_Triggering++;

	//�������м�ʱ�߳̿�ʼ
	//sendto(CNFCliParaList[threadnum].SENDCNFClientStartOrder2,"StartOrder2",20,0,(struct sockaddr *)&CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder2,SockAddrLength);

	//printf("[%d] StartOrder2 triggering...\n",CNFCliParaList[threadnum].CountCNFClientStartOrder2_Triggering);
	
	//����ʱ��ڵ㣨΢�룩		
	gettimeofday(&utime,NULL);
	printf("==================Time==================\nClient Transmission Start %ld us\n========================================\n",1000000 * utime.tv_sec + utime.tv_usec);

	//��ʼ����Ϊ����㹩Ӧ����
	while(1)
	{
		if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
			printf("[%d] WriteData waiting...\n",CNFCliParaList[threadnum].CountCNFClientWriteData_Waiting);
		CNFCliParaList[threadnum].CountCNFClientWriteData_Waiting++;

		//�����ȴ�����㴥�����ϴ����ݵ�Ҫ��
		if(cnfINTRACOMMUNICATION == 0)
		{
			recvfrom(CNFCliParaList[threadnum].RECVCNFClientWriteData,Trigger,20,0,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientWriteData,&SockAddrLength);
		}
		else if(cnfINTRACOMMUNICATION == 1)
		{
			sem_wait(&CNFCliParaList[threadnum].semCNFClientWriteData);
		}
		
		if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
			printf("[%d] WriteData triggered!\n",CNFCliParaList[threadnum].CountCNFClientWriteData_Triggered);
		CNFCliParaList[threadnum].CountCNFClientWriteData_Triggered++;

		CNFCliParaList[threadnum].BUFRenewCount++;
		
		//һ���ϴ����ջ�����һ������ݣ�ǰ��κͺ��ν����ϴ�
		if(CNFCliParaList[threadnum].BUFRenewCount % 2 == 1)//�ж��ϴ�ǰ���
		{
			//��黺��ʣ�೤���Ƿ��㹻һ���ϴ�
			if(CNFCliParaList[threadnum].FileWritePoint + CNFCliParaList[threadnum].BUFLength/2 > cachelen)//����һ���ϴ������¼β����ʶ���ϴ���������
			{
				//write(fp,CNFCliParaList[threadnum].BUF,filelen - CNFCliParaList[threadnum].FileWritePoint);
				memcpy(CacheList[CacheLocation].cache + CacheList[CacheLocation].cachetop,
					CNFCliParaList[threadnum].BUF,
					cachelen - CNFCliParaList[threadnum].FileWritePoint);
				CacheList[CacheLocation].cachetop += cachelen - CNFCliParaList[threadnum].FileWritePoint;
				
				CNFCliParaList[threadnum].BUFFileEnd = CNFCliParaList[threadnum].BUF + ( cachelen - CNFCliParaList[threadnum].FileWritePoint );
				CNFCliParaList[threadnum].ThreadState = 1;
				break;
			}
			else//�㹻һ���ϴ������ϴ�
			{
				//write(fp,CNFCliParaList[threadnum].BUF,CNFCliParaList[threadnum].BUFLength/2);
				memcpy(CacheList[CacheLocation].cache + CacheList[CacheLocation].cachetop,
					CNFCliParaList[threadnum].BUF,
					CNFCliParaList[threadnum].BUFLength/2);
				CacheList[CacheLocation].cachetop += CNFCliParaList[threadnum].BUFLength/2;
				
				CNFCliParaList[threadnum].FileWritePoint += CNFCliParaList[threadnum].BUFLength/2;
				//printf("CNFCliParaList[threadnum].FileWritePoint = %ld\n",CNFCliParaList[threadnum].FileWritePoint);
			}
		}
		else if(CNFCliParaList[threadnum].BUFRenewCount % 2 == 0)//�ж��ϴ�����
		{
			//����ļ�ʣ�೤���Ƿ��㹻һ���ϴ�
			if(CNFCliParaList[threadnum].FileWritePoint + CNFCliParaList[threadnum].BUFLength/2 > cachelen)//����һ���ϴ������¼β����ʶ���ϴ���������
			{
				//write(fp,CNFCliParaList[threadnum].BUF + CNFCliParaList[threadnum].BUFLength/2,filelen - CNFCliParaList[threadnum].FileWritePoint);
				//write(fp,CNFCliParaList[threadnum].BUF + CNFCliParaList[threadnum].BUFLength/2,filelen - CNFCliParaList[threadnum].FileWritePoint);
				memcpy(CacheList[CacheLocation].cache + CacheList[CacheLocation].cachetop,
					CNFCliParaList[threadnum].BUF + CNFCliParaList[threadnum].BUFLength/2,
					cachelen - CNFCliParaList[threadnum].FileWritePoint);
				CacheList[CacheLocation].cachetop += cachelen - CNFCliParaList[threadnum].FileWritePoint;
				
				CNFCliParaList[threadnum].BUFFileEnd = CNFCliParaList[threadnum].BUF + CNFCliParaList[threadnum].BUFLength/2 + ( cachelen - CNFCliParaList[threadnum].FileWritePoint );
				CNFCliParaList[threadnum].ThreadState = 1;
				break;
			}
			else//�㹻һ���ϴ������ϴ�
			{
				//write(fp,CNFCliParaList[threadnum].BUF + CNFCliParaList[threadnum].BUFLength/2,CNFCliParaList[threadnum].BUFLength/2);
				memcpy(CacheList[CacheLocation].cache + CacheList[CacheLocation].cachetop,
					CNFCliParaList[threadnum].BUF + CNFCliParaList[threadnum].BUFLength/2,
					CNFCliParaList[threadnum].BUFLength/2);
				CacheList[CacheLocation].cachetop += CNFCliParaList[threadnum].BUFLength/2;
				
				CNFCliParaList[threadnum].FileWritePoint += CNFCliParaList[threadnum].BUFLength/2;
				//printf("CNFCliParaList[threadnum].FileWritePoint = %ld\n",CNFCliParaList[threadnum].FileWritePoint);
			}
		}

		if(cnfcliDEVETESTIMPL <= 6)
			printf("Data already received: %ld Byte\n",CNFCliParaList[threadnum].FileWritePoint);
	}

	//������ͨ��֪ͨ�ļ�д�����
	if(cnfINTRACOMMUNICATION == 0)
	{
		sendto(CNFCliParaList[threadnum].SENDCNFClientFileClose,"FileClose",20,0,(struct sockaddr *)&CNFCliParaList[threadnum].SENDAddrToCNFClientFileClose,SockAddrLength);
	}
	else if(cnfINTRACOMMUNICATION == 1)
	{
		sem_post(&CNFCliParaList[threadnum].semCNFClientFileClose);
	}

	if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
		printf("[%d] FileClose triggering...\n",CNFCliParaList[threadnum].CountCNFClientFileClose_Triggering);
	CNFCliParaList[threadnum].CountCNFClientFileClose_Triggering++;
	
	if(cnfcliDEVETESTIMPL <= 6)
		printf("[FTP]File Transport End.\n");

	if(cnfcliDEVETESTIMPL <= 7)
	{
/*
		printf("==========DATA Results==========\n");
		//�������������ܹ���������
		time(&GMT);//��ȡGMT����ֵ��GMT
		EndTime = GMT;
		UsedTime = EndTime - StartTime;
		printf("Total Time  %ld\n",UsedTime);//������������ܹ���������
*/
		//����ʱ��ڵ㣨΢�룩		
		gettimeofday(&utime,NULL);
		printf("==================Time==================\nClient Transmission End   %ld us\n========================================\n",1000000 * utime.tv_sec + utime.tv_usec);
/*
		//����ʱ��ڵ㣨�룩
		time(&GMT);//��ȡGMT����ֵ��GMT
		UTC = localtime(&GMT);//GMT-UTCת��
		printf("GMT(long)   %ld s\n",GMT);//[GMT]���1970.1.1.00:00:00��������
		printf("UTC(human)  %s\n",asctime(UTC));//[UTC]����ַ�������ɶ�ʱ��
		printf("================================\n");
*/
	}
	
	//printf("BUF = \n%s\n",CacheList[CacheLocation].cache);//����������ݿ�����
	
	CNFThreadList[threadnum].flag_close = 1;

	int closethread1,closethread2,closethread3,closethread4;
	closethread1 = pthread_cancel(CNFCliParaList[threadnum].pthread_NetworkLayertoTransportLayer);
	closethread2 = pthread_cancel(CNFCliParaList[threadnum].pthread_TransportLayertoNetworkLayer);
	closethread3 = pthread_cancel(CNFCliParaList[threadnum].pthread_CNF);
	closethread4 = pthread_cancel(CNFCliParaList[threadnum].pthread_timer);
	
	if(closethread1 == 0 && closethread2 == 0 && closethread3 == 0 && closethread4 == 0)
	{
		if(cnfcliDEVETESTIMPL <= 7)
		{
			printf("Client Threads Close Success.\n");
			printf("<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<\n");
		}
	}
	else
	{
		printf("Client Threads Close Failed.\n");
		printf("closethread1 = %d\n",closethread1);
		printf("closethread2 = %d\n",closethread2);
		printf("closethread3 = %d\n",closethread3);
		printf("closethread4 = %d\n",closethread4);
	}

	//CNF���ν׶δ�����ϣ��������DATA���ϴ�ת��ƽ�棬�Ա�����һ��ת��
	if(INTRACOMMUNICATION == 0)
		CNFClient_UDP_SendData(threadnum);
	else if(INTRACOMMUNICATION == 1)
		CNFClient_SEM_SendData(threadnum);
	
	//�������߳�
	pthread_exit(NULL);
	//exit(0);
	//close(fp);
}

/*****************************************
* �������ƣ�CNFClient_thread_NetworkLayertoTransportLayer
* ��������������ͨ�� - From.���������   To.Ӧ�÷�����  �ṩ��ʱ
* �����б�
* ���ؽ����
*****************************************/
void *
CNFClient_thread_NetworkLayertoTransportLayer
(
	void * fd
)
{
	//��CNFClientģ������̵߳��õĲ��������ӿ�
	struct cnfcliinput * cnfinput;
	cnfinput = (struct cnfcliinput *)fd;
	
	int threadnum = cnfinput->threadnum;
	
	if(cnfcliDEVETESTIMPL <= 6)
		printf("CNF threadnum == %d\n (CNFClient_thread_NetworkLayertoTransportLayer)\n",threadnum);

	//��ȡ������
	//int ThreadNum = *((int *)fd);printf("[UP]ThreadNum = %d\n",ThreadNum);

	//�������м�ʱ�߳̿�ʼ
	if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
		printf("[%d] BufReady waiting...\n",CNFCliParaList[threadnum].CountCNFClientBufReady_Waiting);
	CNFCliParaList[threadnum].CountCNFClientBufReady_Waiting++;
	
	if(cnfINTRACOMMUNICATION == 0)
	{
		recvfrom(CNFCliParaList[threadnum].RECVCNFClientBufReady,Trigger,20,0,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientBufReady,&SockAddrLength);
	}
	else if(cnfINTRACOMMUNICATION == 1)
	{
		sem_wait(&CNFCliParaList[threadnum].semCNFClientBufReady);
	}

	if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
		printf("[%d] BufReady triggered!\n",CNFCliParaList[threadnum].CountCNFClientBufReady_Triggered);
	CNFCliParaList[threadnum].CountCNFClientBufReady_Triggered++;
	
	//printf("[%d] StartOrder2 waiting...\n",CNFCliParaList[threadnum].CountCNFClientStartOrder2_Waiting);
	//CNFCliParaList[threadnum].CountCNFClientStartOrder2_Waiting++;
	
	//recvfrom(CNFCliParaList[threadnum].RECVCNFClientStartOrder2,Trigger,20,0,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder2,&SockAddrLength);
	
	//printf("[%d] StartOrder2 triggered!\n",CNFCliParaList[threadnum].CountCNFClientStartOrder2_Triggered);
	//CNFCliParaList[threadnum].CountCNFClientStartOrder2_Triggered++;
	
	//��ʱ����ز�����ʼ��
	CNFCliParaList[threadnum].RTOs = RTOS;//��������ʱ�ӣ�RTO����1�벿�֣���λ�룩
	CNFCliParaList[threadnum].RTOus = RTOUS;//��������ʱ�ӣ�RTOС��1�벿�֣���λ΢�룩
	CNFCliParaList[threadnum].RTOns = RTONS;//��������ʱ�ӣ�RTOС��1�벿�֣���λ���룩
	time_t GMT;//���ʱ�׼ʱ�䣬ʵ����time_t�ṹ(typedef long time_t;)
	struct timespec delaytime;

	//����CNF�����߳̿�ʼ
	if(cnfINTRACOMMUNICATION == 0)
	{
		sendto(CNFCliParaList[threadnum].SENDCNFClientStartOrder3,"StartOrder3",20,0,(struct sockaddr *)&CNFCliParaList[threadnum].SENDAddrToCNFClientStartOrder3,SockAddrLength);
	}
	else if(cnfINTRACOMMUNICATION == 1)
	{
		sem_post(&CNFCliParaList[threadnum].semCNFClientStartOrder3);
	}

	if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
		printf("[%d] StartOrder3 triggering...\n",CNFCliParaList[threadnum].CountCNFClientStartOrder3_Triggering);
	CNFCliParaList[threadnum].CountCNFClientStartOrder3_Triggering++;

	while(1)
	{
		//��������
		CNFCliParaList[threadnum].TimingState = 0;

		if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
			printf("[%d] TimingBegin waiting...\n",CNFCliParaList[threadnum].CountCNFClientTimingBegin_Waiting);
		CNFCliParaList[threadnum].CountCNFClientTimingBegin_Waiting++;
		
		//�����ȴ���ʱ����
		if(cnfINTRACOMMUNICATION == 0)
		{
			recvfrom(CNFCliParaList[threadnum].RECVCNFClientTimingBegin,Trigger,20,0,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientTimingBegin,&SockAddrLength);
		}
		else if(cnfINTRACOMMUNICATION == 1)
		{
			sem_wait(&CNFCliParaList[threadnum].semCNFClientTimingBegin);
		}

		if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
			printf("[%d] TimingBegin triggered!\n",CNFCliParaList[threadnum].CountCNFClientTimingBegin_Triggered);
		CNFCliParaList[threadnum].CountCNFClientTimingBegin_Triggered++;
		
		//��ʱ�������ڴ�������ͨ��������ʱ��ʾ
		if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
			printf("[%d] TimingEnd waiting...\n",CNFCliParaList[threadnum].CountCNFClientTimingEnd_Waiting);
		CNFCliParaList[threadnum].CountCNFClientTimingEnd_Waiting++;

		//����ʱ��UDP��Ϣ����
		//���ó�ʱ
		struct timeval delaytime;
		delaytime.tv_sec=CNFCliParaList[threadnum].RTOs;
		delaytime.tv_usec=CNFCliParaList[threadnum].RTOus;

		//��¼sockfd
		fd_set readfds;//�Ѹ��ú��������ⲿ����ָ��
		FD_ZERO(&readfds);
		FD_SET(CNFCliParaList[threadnum].RECVCNFClientTimingEnd,&readfds);

		select(CNFCliParaList[threadnum].RECVCNFClientTimingEnd+1,&readfds,NULL,NULL,&delaytime);
		
		//��ʱ����
		if(FD_ISSET(CNFCliParaList[threadnum].RECVCNFClientTimingEnd,&readfds))
		{
			if(0 <= recvfrom(CNFCliParaList[threadnum].RECVCNFClientTimingEnd,Trigger,20,0,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientTimingEnd,&SockAddrLength))
			{
				if(cnfcliDEVETESTIMPL <= 0)
					printf("[TimingEndDelay]  %.3f ms\n",(double)(CNFCliParaList[threadnum].RTOus - (int)delaytime.tv_usec)/(double)1000);
			}
		}
		else
		{
			if(cnfcliDEVETESTIMPL <= 0)
				printf("[TimingEndDelay]  %.3f ms   (Timeout)\n",(double)(CNFCliParaList[threadnum].RTOus - (int)delaytime.tv_usec)/(double)1000);
		}

		if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
			printf("[%d] TimingEnd triggered!\n",CNFCliParaList[threadnum].CountCNFClientTimingEnd_Triggered);
		CNFCliParaList[threadnum].CountCNFClientTimingEnd_Triggered++;

		//֪ͨ����ͨ�����������η�����ʱ�ѵó����
		if(cnfINTRACOMMUNICATION == 0)
		{
			sendto(CNFCliParaList[threadnum].SENDCNFClientTimingRespond,"TimingRespond",20,0,(struct sockaddr *)&CNFCliParaList[threadnum].SENDAddrToCNFClientTimingRespond,SockAddrLength);
		}
		else if(cnfINTRACOMMUNICATION == 1)
		{
			sem_post(&CNFCliParaList[threadnum].semCNFClientTimingRespond);
		}
		if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
			printf("[%d] TimingRespond triggering...\n",CNFCliParaList[threadnum].CountCNFClientTimingRespond_Triggering);
		CNFCliParaList[threadnum].CountCNFClientTimingRespond_Triggering++;
	}
	
	sleep(10000);
	//�ر��߳�
	//exit(0);
	//free(fd);
	//pthread_exit(NULL);
}

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
)
{
	int i;

	//��CNFClientģ������̵߳��õĲ��������ӿ�
	struct cnfcliinput * cnfinput;
	cnfinput = (struct cnfcliinput *)fd;
	
	int threadnum = cnfinput->threadnum;

	if(cnfcliDEVETESTIMPL <= 6)
		printf("CNF threadnum == %d (CNFClient_thread_TransportLayertoNetworkLayer)\n",threadnum);

	uint8_t PhysicalportIP[16];
	memcpy(PhysicalportIP,cnfinput->physicalportIP,16);
	
	int portnum;
	for(i=15;i>=0;i--)
	{
		if(CNFThreadList[threadnum].physicalportIP[i] == '.')
		{
			portnum = CNFThreadList[threadnum].physicalportIP[i+2] - '0';
			break;
		}
	}

	//��ȡ������
	//int ThreadNum = *((int *)fd);printf("[DOWN]ThreadNum = %d\n",ThreadNum);

	//���������߳̿�ʼ
	if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
		printf("[%d] StartOrder4 waiting...\n",CNFCliParaList[threadnum].CountCNFClientStartOrder4_Waiting);
	CNFCliParaList[threadnum].CountCNFClientStartOrder4_Waiting++;

	if(cnfINTRACOMMUNICATION == 0)
	{
		recvfrom(CNFCliParaList[threadnum].RECVCNFClientStartOrder4,Trigger,20,0,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientStartOrder4,&SockAddrLength);
	}
	else if(cnfINTRACOMMUNICATION == 1)
	{
		sem_wait(&CNFCliParaList[threadnum].semCNFClientStartOrder4);
	}

	if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
		printf("[%d] StartOrder4 triggered!\n",CNFCliParaList[threadnum].CountCNFClientStartOrder4_Triggered);
	CNFCliParaList[threadnum].CountCNFClientStartOrder4_Triggered++;

	//����������һ��SID�ļ�
	/*
	�ڴ˴����SIDת�Ӵ���
	����realPath��ΪSID
	����ӿڣ�SID
	�������CoLoRЭ��GET����װ����
	�����ʽ��1���ļ���ȡ�����ӳٽϴ󲢲����飩��2��socket���ͱ��ػػ���Ϣ
	*/
	
	//��ֵSID
	uint8_t SID[SIDLEN];
	memset(SID,0,SIDLEN);
	strcpy(SID,"wangzhaoxu");
	if(cnfcliDEVETESTIMPL <= 0)
		printf("\n[SID  ]  %s\n",SID);
	
	uint8_t offset[OFFLEN];
	/*
	unsigned long filelen = FILELEN;//���յ��ļ���С
	*/
	unsigned long cachelen = CACHESIZE;

	uint8_t sendpkg[sizeof(CoLoR_ack)+3];
	CoLoR_ack * psendpkg;
	psendpkg = (CoLoR_ack *)(sendpkg+3);

	int LocalPkgBUFHead;
	while(CNFCliParaList[threadnum].OffsetConfirmDATA <= cachelen)
	{
		memset(sendpkg,0,sizeof(CoLoR_ack)+3);
		memcpy(sendpkg,"FOR",3);

		if(CNFCliParaList[threadnum].TimingState == 0)//δ��ʱ
		{
			CNFCliParaList[threadnum].OffsetWaitDATA += 0;
		}
		else if(CNFCliParaList[threadnum].TimingState == 1)//��ʱ
		{
		}
		//printf("[Down]OffsetWaitDATA = %ld\n",CNFCliParaList[threadnum].OffsetWaitDATA);
		
		//�����ȷ�ϵ�ƫ����ֵ
		offset[3] = CNFCliParaList[threadnum].OffsetWaitDATA % 256;
		offset[2] = CNFCliParaList[threadnum].OffsetWaitDATA / 256 % 256;
		offset[1] = CNFCliParaList[threadnum].OffsetWaitDATA / 256 / 256 % 256;
		offset[0] = CNFCliParaList[threadnum].OffsetWaitDATA / 256 / 256 / 256 % 256;
		
		//��װ�����ACK��
		CNFClient_PacketACK(psendpkg,SID,offset);
		//strcpy(psendpkg->sid,SID);
		//memcpy(psendpkg->offset,offset,OFFLEN);

				//����ACK�����
				if(INTRACOMMUNICATION == 0)
				{
					sendto(CNFCliParaList[threadnum].SENDCNFClientACK,sendpkg,sizeof(CoLoR_ack)+3,0,(struct sockaddr *)&CNFCliParaList[threadnum].SENDAddrToCNFClientACK,SockAddrLength);
				}
				else if(INTRACOMMUNICATION == 1)
				{
					//���ݰ�����ת��������У��˴���Ҫ�߳�����
					pthread_mutex_lock(&lockPkgBUF);
					if(PkgBUF[PkgBUFHead].flag_occupied == 0)
					{
						LocalPkgBUFHead = PkgBUFHead;
						PkgBUFHead++;
						if(PkgBUFHead >=10)
							PkgBUFHead -= 10;
		
						PkgBUF[LocalPkgBUFHead].flag_occupied = 1;
						PkgBUF[LocalPkgBUFHead].destppnum = portnum;
						PkgBUF[LocalPkgBUFHead].pkglength = sizeof(CoLoR_ack);
						strcpy(PkgBUF[LocalPkgBUFHead].sourcepp,CNFThreadList[threadnum].physicalport);
						memcpy(PkgBUF[LocalPkgBUFHead].pkg,sendpkg+3,sizeof(CoLoR_ack));
		
						//����������ź���
						sem_post(&semPacketSEND[portnum]);

						if(cnfcliDEVETESTIMPL <= 1)
						{
							printf("[%s] SEQ Message has been sent\n",CNFThreadList[threadnum].physicalport);
							printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
						}
					}
					else
					{
						if(cnfcliDEVETESTIMPL <= 1)
						{
							printf("[%s] SEQ Message failed to be sent\n",CNFThreadList[threadnum].physicalport);
							printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
						}
					}
					pthread_mutex_unlock(&lockPkgBUF);
				}

		if(cnfcliDEVETESTIMPL <= 1)
			printf("[Transport Layer] The CNFClient asked for Data whitch match the SID: %s\n",SID);

		//����������ʱ
		if(cnfINTRACOMMUNICATION == 0)
		{
			sendto(CNFCliParaList[threadnum].SENDCNFClientTimingBegin,"TimingBegin",20,0,(struct sockaddr *)&CNFCliParaList[threadnum].SENDAddrToCNFClientTimingBegin,SockAddrLength);
		}
		else if(cnfINTRACOMMUNICATION == 1)
		{
			sem_post(&CNFCliParaList[threadnum].semCNFClientTimingBegin);
		}

		if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
			printf("[%d] TimingBegin triggering...\n",CNFCliParaList[threadnum].CountCNFClientTimingBegin_Triggering);
		CNFCliParaList[threadnum].CountCNFClientTimingBegin_Triggering++;

		if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
			printf("[%d] TimingRespond waiting...\n",CNFCliParaList[threadnum].CountCNFClientTimingRespond_Waiting);
		CNFCliParaList[threadnum].CountCNFClientTimingRespond_Waiting++;

		//�ȴ���������ACK
		if(cnfINTRACOMMUNICATION == 0)
		{
			recvfrom(CNFCliParaList[threadnum].RECVCNFClientTimingRespond,Trigger,20,0,(struct sockaddr *)&CNFCliParaList[threadnum].RECVAddrToCNFClientTimingRespond,&SockAddrLength);
		}
		else if(cnfINTRACOMMUNICATION == 1)
		{
			sem_wait(&CNFCliParaList[threadnum].semCNFClientTimingRespond);
		}
		if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
			printf("[%d] TimingRespond triggered!\n",CNFCliParaList[threadnum].CountCNFClientTimingRespond_Triggered);
		CNFCliParaList[threadnum].CountCNFClientTimingRespond_Triggered++;
		
		//�����жϣ��յ������ݳ����ۻ�����Ԥ֪�ļ����ȵ��߼����̷�Χ���߼����̷�Χ���ļ�����+�������ȣ�
		if(CNFCliParaList[threadnum].OffsetConfirmDATA >= cachelen)
		{
			if(cnfcliDEVETESTIMPL <= 6)
				printf("[END message]Received File Length is Bigger Than the File Length Ever known.\n");
			
			CNFCliParaList[threadnum].FlagInputBUF = 0;

			//֪ͨ���ļ�д������
			if(cnfINTRACOMMUNICATION == 0)
			{
				sendto(CNFCliParaList[threadnum].SENDCNFClientWriteData,"WriteData",20,0,(struct sockaddr *)&CNFCliParaList[threadnum].SENDAddrToCNFClientWriteData,SockAddrLength);
			}
			else if(cnfINTRACOMMUNICATION == 1)
			{
				sem_post(&CNFCliParaList[threadnum].semCNFClientWriteData);
			}
			if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
				printf("[%d] WriteData triggering...\n",CNFCliParaList[threadnum].CountCNFClientWriteData_Triggering);
			CNFCliParaList[threadnum].CountCNFClientWriteData_Triggering++;
		}
		//д���ļ��ж�
		else if(CNFCliParaList[threadnum].OffsetConfirmDATA / ( CNFCliParaList[threadnum].BUFLength / 2 ) >= CNFCliParaList[threadnum].BUFRenewCount + 1)
		{
			//֪ͨ���ļ�д������
			if(cnfINTRACOMMUNICATION == 0)
			{
				sendto(CNFCliParaList[threadnum].SENDCNFClientWriteData,"WriteData",20,0,(struct sockaddr *)&CNFCliParaList[threadnum].SENDAddrToCNFClientWriteData,SockAddrLength);
			}
			else if(cnfINTRACOMMUNICATION == 1)
			{
				sem_post(&CNFCliParaList[threadnum].semCNFClientWriteData);
			}

			if(cnfcliDEVETESTIMPL <= 1 || CNFclientTRIGGERTEST == 1)
				printf("[%d] WriteData triggering...\n",CNFCliParaList[threadnum].CountCNFClientWriteData_Triggering);
			CNFCliParaList[threadnum].CountCNFClientWriteData_Triggering++;
		}
	}
	
	if(cnfcliDEVETESTIMPL <= 6)
		printf("[Down]File transport end.\n");
	
	sleep(10000);
	//�ر��߳�
	//exit(0);
	//free(fd);
	//pthread_exit(NULL);
}

/*****************************************
* �������ƣ�CNFClient_PacketACK
* ������������װ����mac���Get��
* �����б�
* ���ؽ����
*****************************************/
int
CNFClient_PacketACK
(
	uint8_t * pkg,
	uint8_t * SID,
	uint8_t * OFF
)
{
	CoLoR_ack * getpkg;
	getpkg = (CoLoR_ack *)pkg;
	
	//���CoLoR-Get����
	getpkg->version_type = 164;//�汾4λ������4λ����Ϊ���ó�CoLoR_ack��
	getpkg->ttl = 255;//����ʱ��
	//getpkg->data_len = htons(4);
	getpkg->total_len = 16 + 16 + SIDLEN + NIDLEN + DATALEN + PUBKEYLEN + PIDN*4;//�ܳ���
	
	getpkg->port_no = 0;//�˿ں�
	getpkg->checksum = 0;//�����
	
	getpkg->sid_len = SIDLEN;//SID����
	getpkg->nid_len = NIDLEN;//NID����
	getpkg->pid_n = PIDN;//PID����
	getpkg->options_static = 0;//�̶��ײ�ѡ��

	memset(getpkg->offset,0,OFFLEN);
	memcpy(getpkg->offset,OFF,OFFLEN);
	
	memcpy(getpkg->sid, SID, SIDLEN);//SID

	char nid[NIDLEN] = {'d','1','s','u','b','1',0,0,'d','1','r','m',0,0,0,0};
	memcpy(getpkg->nid, sendnid, NIDLEN);//NID
	
	getpkg->publickey_len = PUBKEYLEN;
	getpkg->mtu = MTU;
	
	char data[DATALEN] = {'I',' ','a','m',' ','t','h','e',' ','d','a','t','a','~','~','~','~','~','~','!'};
	memcpy(getpkg->data, data, DATALEN);//Data
	
	char publickey[PUBKEYLEN] = {'I',' ','a','m',' ','t','h','e',' ','p','u','b','k','e','y','!'};
	memcpy(getpkg->publickey, publickey, PUBKEYLEN);//��Կ

	return 1;
}

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
)
{
	//printf("BUF = \n%s\n",CacheList[0].cache);//����������ݿ�����
	
	//��CNFClientģ������̵߳��õĲ��������ӿ�
	struct cnfcliinput * cnfinput;
	cnfinput = (struct cnfcliinput *)fd;
	
	int threadnum = cnfinput->threadnum;
	
	if(cnfcliDEVETESTIMPL <= 6)
		printf("CNF threadnum == %d (CNFClient_main)\n",threadnum);
	
	CNFClient_Parameterinit(threadnum);

	//�������ͼ�ʱ�����߳�
	if(pthread_create(&CNFCliParaList[threadnum].pthread_timer, NULL, CNFClient_thread_timer, fd)!=0)
	{
		perror("Creation of timer thread failed.");
	}
	
	//����CNF�����߳�
	if(pthread_create(&CNFCliParaList[threadnum].pthread_CNF, NULL, CNFClient_thread_CNF, fd)!=0)
	{
		perror("Creation of CNF thread failed.");
	}

	//����thread_FTP���߳�
	if(pthread_create(&CNFCliParaList[threadnum].pthread_FTP, NULL, CNFClient_thread_FTP, fd)!=0)
	{
		perror("Creation of FTP thread failed.");
	}
	
	//����thread_NetworkLayertoTransportLayer�������߳�
	if(pthread_create(&CNFCliParaList[threadnum].pthread_NetworkLayertoTransportLayer, NULL, CNFClient_thread_NetworkLayertoTransportLayer, fd)!=0)
	{
		perror("Creation of NetworkLayertoTransportLayer thread failed.");
	}
	
	//����thread_TransportLayertoNetworkLayer�������߳�
	if(pthread_create(&CNFCliParaList[threadnum].pthread_TransportLayertoNetworkLayer, NULL, CNFClient_thread_TransportLayertoNetworkLayer, fd)!=0)
	{
		perror("Creation of TransportLayertoNetworkLayer thread failed.");
	}

	//��ѭ�������ͻ��˴���
	while (!CNFThreadList[threadnum].flag_close)
	{
		sleep(1);
	}
}

/*****************************************
* �������ƣ�CNFClientStart_main
* ����������CNFClient������������CNFClient��TCP����ģ�飬���²�SubscriberЭ��ջģ��
* �����б�
eg: ./sub d1sub1 d1sub1-eth1
�ڶ�����ʹ��char**argv�ķ�ʽ����������֪�ǲ����﷨����
* ���ؽ����
*****************************************/
int
CNFClientStart_main
(
	int argc,
	char argv[][30]
)
{
	int i;

	for(i=0;i<CACHESIZE;i++)
		printf("%d",CacheList[TESTCACHENO].cache[i]);
	printf("\n");

	//����CNFClientģ�������߳�
	struct cnfcliinput cnfinput;
	cnfinput.threadnum = TESTNO;
	memset(cnfinput.sid,0,SIDLEN);
	
	pthread_t pthread_cnfclient;
	
	//�������߳�
	if(pthread_create(&pthread_cnfclient, NULL, CNFClient_main, (void *)&cnfinput)!=0)
	{
		perror("Creation of CNF Client main thread failed.");
	}

	//��ѭ�������ͻ��˴���
	while (1)
	{
		sleep(10000);
	}
}
