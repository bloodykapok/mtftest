/*******************************************************************************************************************************************
* �ļ�����cnfserver.c
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
gcc cnfserver.c -o cnfserver -lpthread
4�����У����漰ԭʼ�׽��ֵ�ʹ�ã���rootȨ�ޣ�
sudo ./cnfserver
*/

#include"cnfserver.h"

/*******************************************************************************************************************************************
*************************************�궨����������************ȫ�ֱ�������******************************************************************
*******************************************************************************************************************************************/

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
)
{
	CNFSerParaList[threadnum].LostPacketCount=0;
	CNFSerParaList[threadnum].InputCount=0;
	CNFSerParaList[threadnum].TotalWindowLength=0;

	//�������򿪿ͻ��˽����ļ�
	CNFSerParaList[threadnum].LogFilePoint = open(LOGFilePath,O_RDWR | O_CREAT | O_APPEND | O_TRUNC);
	/*
	if(CNFSerParaList[threadnum].LogFilePoint == -1)
	{
		printf("[Deadly Error] The Log File \"%s\" opening failed.\n",FTPClientPath);
		exit(0);
	}
	*/

	//��ʼ�������û�������
	/*
	int i;	
	for(i=0;i<CACHENUM;i++)
	{
		memset(CacheList[i].cache,'A',CACHESIZE);
		CacheList[i].cachetop = 0;
	}
	*/


	SockAddrLength = sizeof(struct sockaddr_in);

	//SEG����socket
	bzero(&CNFSerParaList[threadnum].SENDAddrToCNFServerSEG,SockAddrLength);
	CNFSerParaList[threadnum].SENDAddrToCNFServerSEG.sin_family=AF_INET;
	CNFSerParaList[threadnum].SENDAddrToCNFServerSEG.sin_addr.s_addr=inet_addr(CNFThreadList[threadnum].physicalportIP);
	CNFSerParaList[threadnum].SENDAddrToCNFServerSEG.sin_port=htons(PhysicalportInterPORT);

	CNFSerParaList[threadnum].SENDCNFServerSEG = socket(AF_INET,SOCK_DGRAM,0);

	//ACK����socket
	bzero(&CNFSerParaList[threadnum].RECVAddrToCNFServerACK,SockAddrLength);
	CNFSerParaList[threadnum].RECVAddrToCNFServerACK.sin_family=AF_INET;
	CNFSerParaList[threadnum].RECVAddrToCNFServerACK.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFSerParaList[threadnum].RECVAddrToCNFServerACK.sin_port=htons(PhysicalportInterPORT + threadnum * CNFPORTRANGE);

	CNFSerParaList[threadnum].RECVCNFServerACK = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFSerParaList[threadnum].RECVCNFServerACK,(struct sockaddr *)&CNFSerParaList[threadnum].RECVAddrToCNFServerACK,SockAddrLength);

	//���ڴ��俪ʼ
	CNFSerParaList[threadnum].CountCNFServerWindowBegin_Triggering = 1;
	CNFSerParaList[threadnum].CountCNFServerWindowBegin_Waiting = 1;
	CNFSerParaList[threadnum].CountCNFServerWindowBegin_Triggered = 1;
/*
	printf("[%d] WindowBegin triggering...\n",CNFSerParaList[threadnum].CountCNFServerWindowBegin_Triggering);
	CNFSerParaList[threadnum].CountCNFServerWindowBegin_Triggering++;
	printf("[%d] WindowBegin waiting...\n",CNFSerParaList[threadnum].CountCNFServerWindowBegin_Waiting);
	CNFSerParaList[threadnum].CountCNFServerWindowBegin_Waiting++;
	printf("[%d] WindowBegin triggered!\n",CNFSerParaList[threadnum].CountCNFServerWindowBegin_Triggered);
	CNFSerParaList[threadnum].CountCNFServerWindowBegin_Triggered++;
*/
	bzero(&CNFSerParaList[threadnum].SENDAddrToCNFServerWindowBegin,SockAddrLength);
	CNFSerParaList[threadnum].SENDAddrToCNFServerWindowBegin.sin_family=AF_INET;
	CNFSerParaList[threadnum].SENDAddrToCNFServerWindowBegin.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFSerParaList[threadnum].SENDAddrToCNFServerWindowBegin.sin_port=htons(CNFServerWindowBegin + threadnum * CNFPORTRANGE);

	bzero(&CNFSerParaList[threadnum].RECVAddrToCNFServerWindowBegin,SockAddrLength);
	CNFSerParaList[threadnum].RECVAddrToCNFServerWindowBegin.sin_family=AF_INET;
	CNFSerParaList[threadnum].RECVAddrToCNFServerWindowBegin.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFSerParaList[threadnum].RECVAddrToCNFServerWindowBegin.sin_port=htons(CNFServerWindowBegin + threadnum * CNFPORTRANGE);

	CNFSerParaList[threadnum].SENDCNFServerWindowBegin = socket(AF_INET,SOCK_DGRAM,0);
	CNFSerParaList[threadnum].RECVCNFServerWindowBegin = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFSerParaList[threadnum].RECVCNFServerWindowBegin,(struct sockaddr *)&CNFSerParaList[threadnum].RECVAddrToCNFServerWindowBegin,SockAddrLength);

	//���ڴ�������ͨ��������ʱ
	CNFSerParaList[threadnum].CountCNFServerWindowTimingEnd_Triggering = 1;
	CNFSerParaList[threadnum].CountCNFServerWindowTimingEnd_Waiting = 1;
	CNFSerParaList[threadnum].CountCNFServerWindowTimingEnd_Triggered = 1;
/*
	printf("[%d] WindowTimingEnd triggering...\n",CNFSerParaList[threadnum].CountCNFServerWindowTimingEnd_Triggering);
	CNFSerParaList[threadnum].CountCNFServerWindowTimingEnd_Triggering++;
	printf("[%d] WindowTimingEnd waiting...\n",CNFSerParaList[threadnum].CountCNFServerWindowTimingEnd_Waiting);
	CNFSerParaList[threadnum].CountCNFServerWindowTimingEnd_Waiting++;
	printf("[%d] WindowTimingEnd triggered!\n",CNFSerParaList[threadnum].CountCNFServerWindowTimingEnd_Triggered);
	CNFSerParaList[threadnum].CountCNFServerWindowTimingEnd_Triggered++;
*/
	bzero(&CNFSerParaList[threadnum].SENDAddrToCNFServerWindowTimingEnd,SockAddrLength);
	CNFSerParaList[threadnum].SENDAddrToCNFServerWindowTimingEnd.sin_family=AF_INET;
	CNFSerParaList[threadnum].SENDAddrToCNFServerWindowTimingEnd.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFSerParaList[threadnum].SENDAddrToCNFServerWindowTimingEnd.sin_port=htons(CNFServerWindowTimingEnd + threadnum * CNFPORTRANGE);

	bzero(&CNFSerParaList[threadnum].RECVAddrToCNFServerWindowTimingEnd,SockAddrLength);
	CNFSerParaList[threadnum].RECVAddrToCNFServerWindowTimingEnd.sin_family=AF_INET;
	CNFSerParaList[threadnum].RECVAddrToCNFServerWindowTimingEnd.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFSerParaList[threadnum].RECVAddrToCNFServerWindowTimingEnd.sin_port=htons(CNFServerWindowTimingEnd + threadnum * CNFPORTRANGE);

	CNFSerParaList[threadnum].SENDCNFServerWindowTimingEnd = socket(AF_INET,SOCK_DGRAM,0);
	CNFSerParaList[threadnum].RECVCNFServerWindowTimingEnd = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFSerParaList[threadnum].RECVCNFServerWindowTimingEnd,(struct sockaddr *)&CNFSerParaList[threadnum].RECVAddrToCNFServerWindowTimingEnd,SockAddrLength);

	//���ڴ�������ͨ���������
	CNFSerParaList[threadnum].CountCNFServerWindowRespond_Triggering = 1;
	CNFSerParaList[threadnum].CountCNFServerWindowRespond_Waiting = 1;
	CNFSerParaList[threadnum].CountCNFServerWindowRespond_Triggered = 1;
/*
	printf("[%d] WindowRespond triggering...\n",CNFSerParaList[threadnum].CountCNFServerWindowRespond_Triggering);
	CNFSerParaList[threadnum].CountCNFServerWindowRespond_Triggering++;
	printf("[%d] WindowRespond waiting...\n",CNFSerParaList[threadnum].CountCNFServerWindowRespond_Waiting);
	CNFSerParaList[threadnum].CountCNFServerWindowRespond_Waiting++;
	printf("[%d] WindowRespond triggered!\n",CNFSerParaList[threadnum].CountCNFServerWindowRespond_Triggered);
	CNFSerParaList[threadnum].CountCNFServerWindowRespond_Triggered++;
*/
	bzero(&CNFSerParaList[threadnum].SENDAddrToCNFServerWindowRespond,SockAddrLength);
	CNFSerParaList[threadnum].SENDAddrToCNFServerWindowRespond.sin_family=AF_INET;
	CNFSerParaList[threadnum].SENDAddrToCNFServerWindowRespond.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFSerParaList[threadnum].SENDAddrToCNFServerWindowRespond.sin_port=htons(CNFServerWindowRespond + threadnum * CNFPORTRANGE);

	bzero(&CNFSerParaList[threadnum].RECVAddrToCNFServerWindowRespond,SockAddrLength);
	CNFSerParaList[threadnum].RECVAddrToCNFServerWindowRespond.sin_family=AF_INET;
	CNFSerParaList[threadnum].RECVAddrToCNFServerWindowRespond.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFSerParaList[threadnum].RECVAddrToCNFServerWindowRespond.sin_port=htons(CNFServerWindowRespond + threadnum * CNFPORTRANGE);

	CNFSerParaList[threadnum].SENDCNFServerWindowRespond = socket(AF_INET,SOCK_DGRAM,0);
	CNFSerParaList[threadnum].RECVCNFServerWindowRespond = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFSerParaList[threadnum].RECVCNFServerWindowRespond,(struct sockaddr *)&CNFSerParaList[threadnum].RECVAddrToCNFServerWindowRespond,SockAddrLength);

	//���ͻ�������
	CNFSerParaList[threadnum].CountCNFServerBufReady_Triggering = 1;
	CNFSerParaList[threadnum].CountCNFServerBufReady_Waiting = 1;
	CNFSerParaList[threadnum].CountCNFServerBufReady_Triggered = 1;
/*
	printf("[%d] BufReady triggering...\n",CNFSerParaList[threadnum].CountCNFServerBufReady_Triggering);
	CNFSerParaList[threadnum].CountCNFServerBufReady_Triggering++;
	printf("[%d] BufReady waiting...\n",CNFSerParaList[threadnum].CountCNFServerBufReady_Waiting);
	CNFSerParaList[threadnum].CountCNFServerBufReady_Waiting++;
	printf("[%d] BufReady triggered!\n",CNFSerParaList[threadnum].CountCNFServerBufReady_Triggered);
	CNFSerParaList[threadnum].CountCNFServerBufReady_Triggered++;
*/
	bzero(&CNFSerParaList[threadnum].SENDAddrToCNFServerBufReady,SockAddrLength);
	CNFSerParaList[threadnum].SENDAddrToCNFServerBufReady.sin_family=AF_INET;
	CNFSerParaList[threadnum].SENDAddrToCNFServerBufReady.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFSerParaList[threadnum].SENDAddrToCNFServerBufReady.sin_port=htons(CNFServerBufReady + threadnum * CNFPORTRANGE);

	bzero(&CNFSerParaList[threadnum].RECVAddrToCNFServerBufReady,SockAddrLength);
	CNFSerParaList[threadnum].RECVAddrToCNFServerBufReady.sin_family=AF_INET;
	CNFSerParaList[threadnum].RECVAddrToCNFServerBufReady.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFSerParaList[threadnum].RECVAddrToCNFServerBufReady.sin_port=htons(CNFServerBufReady + threadnum * CNFPORTRANGE);

	CNFSerParaList[threadnum].SENDCNFServerBufReady = socket(AF_INET,SOCK_DGRAM,0);
	CNFSerParaList[threadnum].RECVCNFServerBufReady = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFSerParaList[threadnum].RECVCNFServerBufReady,(struct sockaddr *)&CNFSerParaList[threadnum].RECVAddrToCNFServerBufReady,SockAddrLength);

	//��Ӧ��������
	CNFSerParaList[threadnum].CountCNFServerMoreData_Triggering = 1;
	CNFSerParaList[threadnum].CountCNFServerMoreData_Waiting = 1;
	CNFSerParaList[threadnum].CountCNFServerMoreData_Triggered = 1;
/*
	printf("[%d] MoreData triggering...\n",CNFSerParaList[threadnum].CountCNFServerMoreData_Triggering);
	CNFSerParaList[threadnum].CountCNFServerMoreData_Triggering++;
	printf("[%d] MoreData waiting...\n",CNFSerParaList[threadnum].CountCNFServerMoreData_Waiting);
	CNFSerParaList[threadnum].CountCNFServerMoreData_Waiting++;
	printf("[%d] MoreData triggered!\n",CNFSerParaList[threadnum].CountCNFServerMoreData_Triggered);
	CNFSerParaList[threadnum].CountCNFServerMoreData_Triggered++;
*/
	bzero(&CNFSerParaList[threadnum].SENDAddrToCNFServerMoreData,SockAddrLength);
	CNFSerParaList[threadnum].SENDAddrToCNFServerMoreData.sin_family=AF_INET;
	CNFSerParaList[threadnum].SENDAddrToCNFServerMoreData.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFSerParaList[threadnum].SENDAddrToCNFServerMoreData.sin_port=htons(CNFServerMoreData + threadnum * CNFPORTRANGE);

	bzero(&CNFSerParaList[threadnum].RECVAddrToCNFServerMoreData,SockAddrLength);
	CNFSerParaList[threadnum].RECVAddrToCNFServerMoreData.sin_family=AF_INET;
	CNFSerParaList[threadnum].RECVAddrToCNFServerMoreData.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFSerParaList[threadnum].RECVAddrToCNFServerMoreData.sin_port=htons(CNFServerMoreData + threadnum * CNFPORTRANGE);

	CNFSerParaList[threadnum].SENDCNFServerMoreData = socket(AF_INET,SOCK_DGRAM,0);
	CNFSerParaList[threadnum].RECVCNFServerMoreData = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFSerParaList[threadnum].RECVCNFServerMoreData,(struct sockaddr *)&CNFSerParaList[threadnum].RECVAddrToCNFServerMoreData,SockAddrLength);

	//�ļ���ȡ����
	CNFSerParaList[threadnum].CountCNFServerFileClose_Triggering = 1;
	CNFSerParaList[threadnum].CountCNFServerFileClose_Waiting = 1;
	CNFSerParaList[threadnum].CountCNFServerFileClose_Triggered = 1;
/*
	printf("[%d] FileClose triggering...\n",CNFSerParaList[threadnum].CountCNFServerFileClose_Triggering);
	CNFSerParaList[threadnum].CountCNFServerFileClose_Triggering++;
	printf("[%d] FileClose waiting...\n",CNFSerParaList[threadnum].CountCNFServerFileClose_Waiting);
	CNFSerParaList[threadnum].CountCNFServerFileClose_Waiting++;
	printf("[%d] FileClose triggered!\n",CNFSerParaList[threadnum].CountCNFServerFileClose_Triggered);
	CNFSerParaList[threadnum].CountCNFServerFileClose_Triggered++;
*/
	bzero(&CNFSerParaList[threadnum].SENDAddrToCNFServerFileClose,SockAddrLength);
	CNFSerParaList[threadnum].SENDAddrToCNFServerFileClose.sin_family=AF_INET;
	CNFSerParaList[threadnum].SENDAddrToCNFServerFileClose.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFSerParaList[threadnum].SENDAddrToCNFServerFileClose.sin_port=htons(CNFServerFileClose + threadnum * CNFPORTRANGE);

	bzero(&CNFSerParaList[threadnum].RECVAddrToCNFServerFileClose,SockAddrLength);
	CNFSerParaList[threadnum].RECVAddrToCNFServerFileClose.sin_family=AF_INET;
	CNFSerParaList[threadnum].RECVAddrToCNFServerFileClose.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFSerParaList[threadnum].RECVAddrToCNFServerFileClose.sin_port=htons(CNFServerFileClose + threadnum * CNFPORTRANGE);

	CNFSerParaList[threadnum].SENDCNFServerFileClose = socket(AF_INET,SOCK_DGRAM,0);
	CNFSerParaList[threadnum].RECVCNFServerFileClose = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFSerParaList[threadnum].RECVCNFServerFileClose,(struct sockaddr *)&CNFSerParaList[threadnum].RECVAddrToCNFServerFileClose,SockAddrLength);

	//����ͨ�������հ�
	CNFSerParaList[threadnum].CountCNFServerContinueRECV_Triggering = 1;
	CNFSerParaList[threadnum].CountCNFServerContinueRECV_Waiting = 1;
	CNFSerParaList[threadnum].CountCNFServerContinueRECV_Triggered = 1;
/*
	printf("[%d] ContinueRECV triggering...\n",CNFSerParaList[threadnum].CountCNFServerContinueRECV_Triggering);
	CNFSerParaList[threadnum].CountCNFServerContinueRECV_Triggering++;
	printf("[%d] ContinueRECV waiting...\n",CNFSerParaList[threadnum].CountCNFServerContinueRECV_Waiting);
	CNFSerParaList[threadnum].CountCNFServerContinueRECV_Waiting++;
	printf("[%d] ContinueRECV triggered!\n",CNFSerParaList[threadnum].CountCNFServerContinueRECV_Triggered);
	CNFSerParaList[threadnum].CountCNFServerContinueRECV_Triggered++;
*/
	bzero(&CNFSerParaList[threadnum].SENDAddrToCNFServerContinueRECV,SockAddrLength);
	CNFSerParaList[threadnum].SENDAddrToCNFServerContinueRECV.sin_family=AF_INET;
	CNFSerParaList[threadnum].SENDAddrToCNFServerContinueRECV.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFSerParaList[threadnum].SENDAddrToCNFServerContinueRECV.sin_port=htons(CNFServerContinueRECV + threadnum * CNFPORTRANGE);

	bzero(&CNFSerParaList[threadnum].RECVAddrToCNFServerContinueRECV,SockAddrLength);
	CNFSerParaList[threadnum].RECVAddrToCNFServerContinueRECV.sin_family=AF_INET;
	CNFSerParaList[threadnum].RECVAddrToCNFServerContinueRECV.sin_addr.s_addr=inet_addr(LOOPIP);
	CNFSerParaList[threadnum].RECVAddrToCNFServerContinueRECV.sin_port=htons(CNFServerContinueRECV + threadnum * CNFPORTRANGE);

	CNFSerParaList[threadnum].SENDCNFServerContinueRECV = socket(AF_INET,SOCK_DGRAM,0);
	CNFSerParaList[threadnum].RECVCNFServerContinueRECV = socket(AF_INET,SOCK_DGRAM,0);

	bind(CNFSerParaList[threadnum].RECVCNFServerContinueRECV,(struct sockaddr *)&CNFSerParaList[threadnum].RECVAddrToCNFServerContinueRECV,SockAddrLength);

}

/*****************************************
* �������ƣ�CNFServer_Parametershut
* ������������ֹ��CNF�������񲢻��յ�������Դ
* �����б�
* ���ؽ����
*****************************************/
void
CNFServer_Parametershut
(
	int threadnum
)
{
	//�ͷŸ�CNF���������ڲ�ͨ�ž��
	close(CNFSerParaList[threadnum].SENDCNFServerSEG);
	close(CNFSerParaList[threadnum].RECVCNFServerACK);
	close(CNFSerParaList[threadnum].SENDCNFServerWindowBegin);
	close(CNFSerParaList[threadnum].RECVCNFServerWindowBegin);
	close(CNFSerParaList[threadnum].SENDCNFServerWindowTimingEnd);
	close(CNFSerParaList[threadnum].RECVCNFServerWindowTimingEnd);
	close(CNFSerParaList[threadnum].SENDCNFServerWindowRespond);
	close(CNFSerParaList[threadnum].RECVCNFServerWindowRespond);
	close(CNFSerParaList[threadnum].SENDCNFServerBufReady);
	close(CNFSerParaList[threadnum].RECVCNFServerBufReady);
	close(CNFSerParaList[threadnum].SENDCNFServerMoreData);
	close(CNFSerParaList[threadnum].RECVCNFServerMoreData);
	close(CNFSerParaList[threadnum].SENDCNFServerFileClose);
	close(CNFSerParaList[threadnum].RECVCNFServerFileClose);
	close(CNFSerParaList[threadnum].SENDCNFServerContinueRECV);
	close(CNFSerParaList[threadnum].RECVCNFServerContinueRECV);

	//�ͷŸ�CNF�������б���
	//(������ʼʱ�г�ʼ������)
	
	//�ͷ�CacheList����λ��
	int CacheLocation = CNFThreadList[threadnum].cachelocation;
	memset(CacheList[CacheLocation].physicalport,0,30);
	memset(CacheList[CacheLocation].sid,0,SIDLEN);
	memset(CacheList[CacheLocation].sbd,0,SBDLEN);
	memset(CacheList[CacheLocation].offset,0,OFFLEN);
	memset(CacheList[CacheLocation].cache,0,CACHESIZE);
	CacheList[CacheLocation].cachetop = 0;
	CacheList[CacheLocation].datapkglocation = -1;
	CacheList[CacheLocation].flag_ack = 0;

	//�ͷ�CNF�������
	CNFThreadList[threadnum].flag_IO = -1;
	CNFThreadList[threadnum].datapkglocation = -1;
	CNFThreadList[threadnum].cachelocation = -1;
	memset(CNFThreadList[threadnum].physicalport,0,30);
	memset(CNFThreadList[threadnum].physicalportIP,0,16);
	memset(CNFThreadList[threadnum].sid,0,SIDLEN);
	memset(CNFThreadList[threadnum].offset,0,OFFLEN);
	
	//��������ռ��
	CacheList[CacheLocation].flag_occupied = 0;
	CNFThreadList[threadnum].flag_occupied = 0;
	
	if(cnfserDEVETESTIMPL <= 6)
		printf("threadnum %d released.\n",threadnum);
}

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
)
{
	//��CNFServerģ������̵߳��õĲ��������ӿ�
	struct cnfserinput * cnfinput;
	cnfinput = (struct cnfserinput *)fd;
	
	int threadnum = cnfinput->threadnum;
	
	if(cnfserDEVETESTIMPL <= 6)
		printf("CNF threadnum == %d (CNFServer_thread_timer)\n",threadnum);

	CNFSerParaList[threadnum].timer_XX = 0;
	
	while(1)
	{
		//sleep(10000);
		sleep(SLEEP);
		CNFSerParaList[threadnum].timer_XX++;
		
		//time(&GMT);//��ȡGMT����ֵ��GMT
		//UTC = localtime(&GMT);//GMT-UTCת��
		//printf("GMT(long)   %ld\n",GMT);//[GMT]���1970.1.1.00:00:00��������
		//printf("UTC(human)  %s",asctime(UTC));//[UTC]����ַ�������ɶ�ʱ��
	}

	sleep(10000);
	//�ر��߳�
	//exit(0);
	//free(fd);
	//pthread_exit(NULL);
}

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
)
{
	//��CNFServerģ������̵߳��õĲ��������ӿ�
	struct cnfserinput * cnfinput;
	cnfinput = (struct cnfserinput *)fd;
	
	int threadnum = cnfinput->threadnum;
	
	if(cnfserDEVETESTIMPL <= 6)
		printf("CNF threadnum == %d (CNFServer_thread_CNF)\n",threadnum);

	uint8_t PhysicalportIP[16];
	memcpy(PhysicalportIP,cnfinput->physicalportIP,16);

	int i,j;
	
	//��������SID����Ĵ���洢����
	uint8_t SID[SIDLEN];
	uint8_t OFFSET[OFFLEN];
	unsigned long offsetlong;
	uint8_t DATA[DATALEN];
	int SIDlen=0,DATAlen=0;
	
	uint8_t recvpkg[sizeof(CoLoR_ack)];
	CoLoR_ack * precvpkg;
	precvpkg = (CoLoR_ack *)recvpkg;

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
		memset(recvpkg,0,sizeof(CoLoR_ack));

		FoundSIDNum=-1;

		recvfrom(CNFSerParaList[threadnum].RECVCNFServerACK,recvpkg,sizeof(CoLoR_ack),0,(struct sockaddr *)&CNFSerParaList[threadnum].RECVAddrToCNFServerACK,&SockAddrLength);

		//��ȡSID��offset
		memcpy(SID,precvpkg->sid,SIDLEN);
		memcpy(OFFSET,precvpkg->offset,OFFLEN);
		SIDlen = 10;

		if(cnfserDEVETESTIMPL <= 4)
		{
			printf("[NetworkLayer to TransportLayer]SID = %s\n",SID);
			printf("[NetworkLayer to TransportLayer]OFF = %d %d %d %d\n",OFFSET[0],OFFSET[1],OFFSET[2],OFFSET[3]);
		}

		//����CNF��������нӿ�����
		
		//�ж�OFFSET��0���Ǵ���0������ж��յ���GET��Ϣ�� ������������Ϣ ���� ĳ���������ACK
		if(OFFSET[0] == 0 && OFFSET[1] == 0 && OFFSET[2] == 3 && OFFSET[3] == 232 && SingleThreadAlreadyStart == 0)//����������������ʱ������������Ϊ1��
		{
			if(cnfserDEVETESTIMPL <= 6)
				printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>A New Service Started.\n");

			SingleThreadAlreadyStart = 1;
			
			//��ʼ�������߳̿��Ʋ�����
			memset(CNFSerParaList[threadnum].SID,0,SIDLEN);
			memcpy(CNFSerParaList[threadnum].SID,SID,SIDlen);
			CNFSerParaList[threadnum].SIDLen = SIDlen;
	
			CNFSerParaList[threadnum].BUF = NULL;//���ͻ�������ͷ
			CNFSerParaList[threadnum].BUFEnd = NULL;//���ͻ�������β
			CNFSerParaList[threadnum].BUFFileEnd = NULL;//��ȡ�ļ�����ʱ�����������ļ�ĩβ���ڵ�ַ

			CNFSerParaList[threadnum].BUFRenewCount = 1;//���������´���
			CNFSerParaList[threadnum].BUFLength = CNFBUFSIZE;//���ͻ��峤��

			CNFSerParaList[threadnum].FileReadPoint = 0;//�ļ���ȡ����
			CNFSerParaList[threadnum].FileLength = 0;//�ļ��ܳ���

			CNFSerParaList[threadnum].PacketDataLength = PKTDATALEN;//�������ݶγ���
			CNFSerParaList[threadnum].LastPacketDataLength = PKTDATALEN;//����������������һ�����ݰ�ʵ�ʳ���

			CNFSerParaList[threadnum].WindowLength = 1;//���ڳ���
			CNFSerParaList[threadnum].WindowThreshold = CNFBUFSIZE;//��������ֵ

			CNFSerParaList[threadnum].OffsetWindowHead = 0;//�������ļ��д���ͷ����ָ��ƫ����
			CNFSerParaList[threadnum].OffsetWindowEnd = PKTDATALEN;//�������ļ��д���β����ָ��ƫ����
			CNFSerParaList[threadnum].OffsetConfirmACK = 0;//�������ļ�����ȷ��ƫ��������ƫ����
			CNFSerParaList[threadnum].OffsetRequestACK = 0;//�������ļ��нӵ������ƫ����
			CNFSerParaList[threadnum].OffsetPacket = PKTDATALEN;//�������ļ��е�ǰ���ƫ�������ش����ݰ�ƫ�����ֶε���д���ݣ�
			CNFSerParaList[threadnum].OffsetWaitACK = PKTDATALEN;//�������ļ��еȴ�ȷ���յ���ƫ��������һ�ڣ�

			CNFSerParaList[threadnum].AddrWindowHead = NULL;//�ڻ������д���ͷ�����ڵ�ַ
			CNFSerParaList[threadnum].AddrWindowEnd = NULL;//�ڻ������д���β�����ڵ�ַ
			CNFSerParaList[threadnum].AddrConfirmACK = NULL;//�ڻ���������ȷ��ƫ�������ڵ�ַ
			CNFSerParaList[threadnum].AddrRequestACK = NULL;//�ڻ������нӵ���������ڵ�ַ
			CNFSerParaList[threadnum].AddrPacket = NULL;//�ڻ������е�ǰ���ƫ�������ڵ�ַ
			CNFSerParaList[threadnum].AddrWaitACK = NULL;//�ڻ������д�ȷ��ƫ�������ڵ�ַ

			CNFSerParaList[threadnum].RTOs = 0;//��������ʱ�ӣ�RTO����1�벿�֣���λ�룩
			CNFSerParaList[threadnum].RTOus = 0;//��������ʱ�ӣ�RTOС��1�벿�֣���λ΢�룩
			CNFSerParaList[threadnum].RTOns = 0;//��������ʱ�ӣ�RTOС��1�벿�֣���λ���룩

			CNFSerParaList[threadnum].FlagTrebleAck = 0;//����������¼�յ���ͬƫ����ACK�ĸ����������ж�����ACK����״��

			CNFSerParaList[threadnum].WindowState = 0;//����״̬��1ACK�������µ�δ�ﴰ����Ҫ����Ͻ磻2���ڴ���ɹ���3����ACK��4��ʱ
			CNFSerParaList[threadnum].ThreadState = 0;//����״̬�����ڽ������̵Ŀ��ƣ���0δ��ɣ�1�ļ���ȡ��ϣ�2����¼����ϣ�3���ڷ�����ϣ�4����ȷ����ϣ�������ֹ��
			
			//����thread_FTP���߳�
			if(pthread_create(&CNFSerParaList[threadnum].pthread_FTP, NULL, CNFServer_thread_FTP, fd)!=0)
			{
				perror("Creation of FTP thread failed.");
			}
			
			//����thread_NetworkLayertoTransportLayer�������߳�
			if(pthread_create(&CNFSerParaList[threadnum].pthread_NetworkLayertoTransportLayer, NULL, CNFServer_thread_NetworkLayertoTransportLayer, fd)!=0)
			{
				perror("Creation of NetworkLayertoTransportLayer thread failed.");
			}
	
			//����thread_TransportLayertoNetworkLayer�������߳�
			if(pthread_create(&CNFSerParaList[threadnum].pthread_TransportLayertoNetworkLayer, NULL, CNFServer_thread_TransportLayertoNetworkLayer, fd)!=0)
			{
				perror("Creation of TransportLayertoNetworkLayer thread failed.");
			}
			
			//֪ͨӦ�ò����SID���ݲ����뷢�ͻ���
			//����ʱ�Ե��������򻯿�����
			if(cnfserDEVETESTIMPL <= 6)
			{
				printf("[RECV new]offsetlong = %ld\n",(unsigned long)1000);
				printf("[RECV new]OffsetRequestACK  = %ld\n",CNFSerParaList[threadnum].OffsetRequestACK);
			}
		}
		else//�������������ACK
		{
			//printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>There is an ACK for a Single Thread which Already Started.\n");

			offsetlong    = (unsigned long)OFFSET[0] * 256 * 256 * 256 + 
					(unsigned long)OFFSET[1] * 256 * 256 + 
					(unsigned long)OFFSET[2] * 256 + 
					(unsigned long)OFFSET[3];

			//ƥ��SID��������SID��Ӧ�������̱߳��
			if(strncmp(CNFSerParaList[threadnum].SID,SID,SIDLEN) == 0)
			{
				FoundSIDNum = 0;
			}
			if(FoundSIDNum == -1)
			{
				//printf("An unknown ACK Packet received, SID not found.\n");
				continue;
			}
			
			CNFSerParaList[threadnum].SIDLen = SIDlen;

			if(cnfserDEVETESTIMPL <= 1)
			{
				printf("[RECV]offsetlong       = %ld\n",offsetlong);
				printf("[LAST]OffsetRequestACK = %ld\n",CNFSerParaList[threadnum].OffsetRequestACK);
				printf("[NOW ]OffsetWindowEnd  = %ld\n",CNFSerParaList[threadnum].OffsetWindowEnd);
			}

			if(offsetlong > CNFSerParaList[threadnum].OffsetRequestACK)//�µ�����ƫ����������ȷ��ƫ����ֵ
			{//printf("New ACK\n");
				CNFSerParaList[threadnum].OffsetRequestACK = offsetlong;
				CNFSerParaList[threadnum].OffsetConfirmACK = CNFSerParaList[threadnum].OffsetRequestACK - CNFSerParaList[threadnum].PacketDataLength;
				
				CNFSerParaList[threadnum].FlagTrebleAck = 0;
				
				CNFSerParaList[threadnum].WindowState = 1;
				
				if(CNFSerParaList[threadnum].OffsetRequestACK == CNFSerParaList[threadnum].OffsetWindowEnd)//�յ�ACK�������ȴ����ڵ��Ͻ�
				{
					CNFSerParaList[threadnum].WindowState = 2;
				}
			}
			else if(offsetlong == CNFSerParaList[threadnum].OffsetRequestACK)//�յ��ظ�ƫ������ACK
			{//printf("Same ACK\n");printf("[RECV]offsetlong       = %ld\n",offsetlong);
				CNFSerParaList[threadnum].FlagTrebleAck++;
				
				if(CNFSerParaList[threadnum].FlagTrebleAck >= 3)//�ظ�ACK��3��
				{
					//printf("TrebleAck Reached!\n");
					CNFSerParaList[threadnum].WindowState = 3;
					CNFSerParaList[threadnum].FlagTrebleAck = 0;
					
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

			sendto(CNFSerParaList[threadnum].SENDCNFServerWindowTimingEnd,"WindowTimingEnd",20,0,(struct sockaddr *)&CNFSerParaList[threadnum].SENDAddrToCNFServerWindowTimingEnd,SockAddrLength);

			if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
				printf("[%d] WindowTimingEnd triggering...\n",CNFSerParaList[threadnum].CountCNFServerWindowTimingEnd_Triggering);
			CNFSerParaList[threadnum].CountCNFServerWindowTimingEnd_Triggering++;
			
			//����Ƿ�������������ﴰ���Ͻ������ACK���Ĵ���WindowTimingEnd�����ų�����ACK���²��ȴ�����ͨ�����ͼ����հ���ָ��

			if(CNFSerParaList[threadnum].WindowState != 1)
			{
				//�����ȴ�����ͨ�����ͼ����հ���ָ��
				if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
					printf("[%d] ContinueRECV waiting...\n",CNFSerParaList[threadnum].CountCNFServerContinueRECV_Waiting);
				CNFSerParaList[threadnum].CountCNFServerContinueRECV_Waiting++;
				
				if(cnfINTRACOMMUNICATION == 0)
				{
					recvfrom(CNFSerParaList[threadnum].RECVCNFServerContinueRECV,Trigger,20,0,(struct sockaddr *)&CNFSerParaList[threadnum].RECVAddrToCNFServerContinueRECV,&SockAddrLength);
				}
				else if(cnfINTRACOMMUNICATION == 1)
				{
					sem_wait(&CNFSerParaList[threadnum].semCNFServerContinueRECV);
				}


				if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
					printf("[%d] ContinueRECV triggered.\n",CNFSerParaList[threadnum].CountCNFServerContinueRECV_Triggered);
				CNFSerParaList[threadnum].CountCNFServerContinueRECV_Triggered++;
			}

		}
	}

	sleep(10000);
	//�ر��߳�
	//exit(0);
	//free(fd);
	//pthread_exit(NULL);	
}

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
)
{
	//��CNFServerģ������̵߳��õĲ��������ӿ�
	struct cnfserinput * cnfinput;
	cnfinput = (struct cnfserinput *)fd;
	
	int threadnum = cnfinput->threadnum;

	if(cnfserDEVETESTIMPL <= 6)
		printf("CNF threadnum == %d (CNFServer_thread_FTP)\n",threadnum);
	
	//��ȡ������
	//int ThreadNum = *((int *)fd);printf("[FTP]ThreadNum = %d\n",ThreadNum);
	
	//Ϊ�������뷢�ͻ����ڴ�
	CNFSerParaList[threadnum].BUF = (uint8_t *)malloc(CNFBUFSIZE);
	memset(CNFSerParaList[threadnum].BUF,0,CNFBUFSIZE);
	
	//���������Ŷ�Ӧ�Ĳ�����SIDѰ�Ҷ�Ӧ���ļ�
	
	
	//���ļ�����ȡ�������
	/*
	int fp;
	unsigned long filelen;
	
	fp = open(FTPServerPath,O_RDONLY);
	filelen = lseek(fp,0,SEEK_END);
	*/
	//������鿽���ļ��ܳ���
	unsigned long cachelen = CACHESIZE;
	//CNFSerParaList[threadnum].FileLength = filelen;
	CNFSerParaList[threadnum].FileLength = CACHESIZE;
	
	if(cnfserDEVETESTIMPL <= 6)
		printf("FileLength = %ld\n",CNFSerParaList[threadnum].FileLength);
	
	CNFSerParaList[threadnum].BUFEnd = CNFSerParaList[threadnum].BUF + CNFSerParaList[threadnum].BUFLength;

	//����ļ��ܳ���С�ڻ��������ȣ����С�ļ���
	int readlength = 0;
	if(CNFSerParaList[threadnum].FileLength <= CNFSerParaList[threadnum].BUFLength)
	{
		if(cnfserDEVETESTIMPL <= 6)
			printf("File is very small, smaller than the BUFFER.\n");

		readlength = CNFSerParaList[threadnum].FileLength;
		CNFSerParaList[threadnum].BUFFileEnd = CNFSerParaList[threadnum].BUF + CNFSerParaList[threadnum].FileLength;
		CNFSerParaList[threadnum].ThreadState = 1;
	}
	else
	{
		readlength = CNFBUFSIZE;
	}

	//��ȡ��һ������
	//lseek(fp,0,SEEK_SET);
	//read(fp,CNFSerParaList[threadnum].BUF,readlength);
	memcpy(CNFSerParaList[threadnum].BUF,CacheList[TESTCACHENO].cache,readlength);
	//��Ӧ���޸��ļ����Ʋ���
	CNFSerParaList[threadnum].BUFRenewCount = 0;
	CNFSerParaList[threadnum].FileReadPoint = CNFBUFSIZE;
	
	//printf("BUF = %s\n",CNFSerParaList[threadnum].BUF);
	
	//֪ͨ����㣬���ͻ����Ѿ�׼������
	if(cnfINTRACOMMUNICATION == 0)
	{
		sendto(CNFSerParaList[threadnum].SENDCNFServerBufReady,"BufReady",20,0,(struct sockaddr *)&CNFSerParaList[threadnum].SENDAddrToCNFServerBufReady,SockAddrLength);
	}
	else if(cnfINTRACOMMUNICATION == 1)
	{
		sem_post(&CNFSerParaList[threadnum].semCNFServerBufReady);
	}

	if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
		printf("[%d] BufReady triggering...\n",CNFSerParaList[threadnum].CountCNFServerBufReady_Triggering);

	CNFSerParaList[threadnum].CountCNFServerBufReady_Triggering++;
	
	if(cnfserDEVETESTIMPL <= 6)
		printf("DATA Reloading...(First for 1M size)\n");
	
		//����ʱ��ڵ㣨΢�룩		
		gettimeofday(&utime,NULL);
		printf("==================Time==================\nServer Transmission Start %ld us\n========================================\n",1000000 * utime.tv_sec + utime.tv_usec);

	//��ʼ����Ϊ����㹩Ӧ����
	while(CNFSerParaList[threadnum].ThreadState == 0)
	{
		//�����ȴ�����㴥���Թ�Ӧ�������ݵ�Ҫ��
		if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
			printf("[%d] MoreData waiting...\n",CNFSerParaList[threadnum].CountCNFServerMoreData_Waiting);
		CNFSerParaList[threadnum].CountCNFServerMoreData_Waiting++;

		if(cnfINTRACOMMUNICATION == 0)
		{
			recvfrom(CNFSerParaList[threadnum].RECVCNFServerMoreData,Trigger,20,0,(struct sockaddr *)&CNFSerParaList[threadnum].RECVAddrToCNFServerMoreData,&SockAddrLength);
		}
		else if(cnfINTRACOMMUNICATION == 1)
		{
			sem_wait(&CNFSerParaList[threadnum].semCNFServerMoreData);
		}


		if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
			printf("[%d] MoreData triggered.\n",CNFSerParaList[threadnum].CountCNFServerMoreData_Triggered);
		CNFSerParaList[threadnum].CountCNFServerMoreData_Triggered++;

		CNFSerParaList[threadnum].BUFRenewCount++;

		if(cnfserDEVETESTIMPL <= 6)
			printf("DATA Reloading...(Per BUFLEN/2 for once)\n");

		//һ�θ��·��ͻ�����һ������ݣ�ǰ��κͺ��ν������
		if(CNFSerParaList[threadnum].BUFRenewCount % 2 == 1)//ָʾ����ǰ���
		{
			//����ļ�ʣ�೤���Ƿ��㹻һ�β���
			if(CNFSerParaList[threadnum].FileReadPoint + CNFBUFSIZE/2 > cachelen)//����һ�β��䣬�����������ݣ�����¼β����ʶ
			{
				//lseek(fp,CNFSerParaList[threadnum].FileReadPoint,SEEK_SET);
				//read(fp,CNFSerParaList[threadnum].BUF,filelen - CNFSerParaList[threadnum].FileReadPoint);
				memcpy(CNFSerParaList[threadnum].BUF,
					CacheList[TESTCACHENO].cache + CNFSerParaList[threadnum].FileReadPoint,
					cachelen - CNFSerParaList[threadnum].FileReadPoint);
				
				CNFSerParaList[threadnum].BUFFileEnd = CNFSerParaList[threadnum].BUF + ( cachelen - CNFSerParaList[threadnum].FileReadPoint );
				CNFSerParaList[threadnum].ThreadState = 1;
				break;
			}
			else//�㹻һ�β��䣬�򲹳�
			{
				//lseek(fp,CNFSerParaList[threadnum].FileReadPoint,SEEK_SET);
				//read(fp,CNFSerParaList[threadnum].BUF,CNFBUFSIZE/2);
				memcpy(CNFSerParaList[threadnum].BUF,
					CacheList[TESTCACHENO].cache + CNFSerParaList[threadnum].FileReadPoint,
					CNFBUFSIZE/2);
			
				CNFSerParaList[threadnum].FileReadPoint += CNFBUFSIZE/2;
			}
		}
		else if(CNFSerParaList[threadnum].BUFRenewCount % 2 == 0)//ָʾ���º���
		{
			//����ļ�ʣ�೤���Ƿ��㹻һ�β���
			if(CNFSerParaList[threadnum].FileReadPoint + CNFBUFSIZE/2 > cachelen)//����һ�β��䣬�����������ݣ�����¼β����ʶ
			{
				//lseek(fp,CNFSerParaList[threadnum].FileReadPoint,SEEK_SET);
				//read(fp,CNFSerParaList[threadnum].BUF + CNFBUFSIZE/2,filelen - CNFSerParaList[threadnum].FileReadPoint);
				memcpy(CNFSerParaList[threadnum].BUF + CNFBUFSIZE/2,
					CacheList[TESTCACHENO].cache + CNFSerParaList[threadnum].FileReadPoint,
					cachelen - CNFSerParaList[threadnum].FileReadPoint);
				
				CNFSerParaList[threadnum].BUFFileEnd = CNFSerParaList[threadnum].BUF + CNFBUFSIZE/2 + ( cachelen - CNFSerParaList[threadnum].FileReadPoint );
				CNFSerParaList[threadnum].ThreadState = 1;
				break;
			}
			else//�㹻һ�β��䣬�򲹳�
			{
				//lseek(fp,CNFSerParaList[threadnum].FileReadPoint,SEEK_SET);
				//read(fp,CNFSerParaList[threadnum].BUF + CNFBUFSIZE/2,CNFBUFSIZE/2);
				memcpy(CNFSerParaList[threadnum].BUF + CNFBUFSIZE/2,
					CacheList[TESTCACHENO].cache + CNFSerParaList[threadnum].FileReadPoint,
					CNFBUFSIZE/2);
			
				CNFSerParaList[threadnum].FileReadPoint += CNFBUFSIZE/2;
			}
		}
		
		if(cnfserDEVETESTIMPL <= 6)
			printf("FILE Reloaded. Already loaded: %ld Byte\n",CNFSerParaList[threadnum].FileReadPoint);
	}

	//������ͨ��֪ͨ�ļ���ȡ����
	if(cnfINTRACOMMUNICATION == 0)
	{
		sendto(CNFSerParaList[threadnum].SENDCNFServerFileClose,"FileClose",20,0,(struct sockaddr *)&CNFSerParaList[threadnum].SENDAddrToCNFServerFileClose,SockAddrLength);
	}
	else if(cnfINTRACOMMUNICATION == 1)
	{
		sem_post(&CNFSerParaList[threadnum].semCNFServerFileClose);
	}

	if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
		printf("[%d] FileClose triggering...\n",CNFSerParaList[threadnum].CountCNFServerFileClose_Triggering);
	CNFSerParaList[threadnum].CountCNFServerFileClose_Triggering++;
	
	sleep(10000);
	//close(fp);
}

/*****************************************
* �������ƣ�CNFServer_thread_NetworkLayertoTransportLayer
* ��������������ͨ�� - From.������Ӧ��   To.Ӧ�÷�����������ACK�Ľ��պͼ�ʱ
* �����б�
* ���ؽ����
*****************************************/
void *
CNFServer_thread_NetworkLayertoTransportLayer
(
	void * fd
)
{
	//��CNFServerģ������̵߳��õĲ��������ӿ�
	struct cnfserinput * cnfinput;
	cnfinput = (struct cnfserinput *)fd;
	
	int threadnum = cnfinput->threadnum;
	
	if(cnfserDEVETESTIMPL <= 6)
		printf("CNF threadnum == %d (CNFServer_thread_NetworkLayertoTransportLayer)\n",threadnum);

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
		
		if(CNFSerParaList[threadnum].WindowState != 1)
		{
			//�����ȴ���ʱ����
			if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
				printf("[%d] WindowBegin waiting...\n",CNFSerParaList[threadnum].CountCNFServerWindowBegin_Waiting);
			CNFSerParaList[threadnum].CountCNFServerWindowBegin_Waiting++;

			if(cnfINTRACOMMUNICATION == 0)
			{
				recvfrom(CNFSerParaList[threadnum].RECVCNFServerWindowBegin,Trigger,20,0,(struct sockaddr *)&CNFSerParaList[threadnum].RECVAddrToCNFServerWindowBegin,&SockAddrLength);
			}
			else if(cnfINTRACOMMUNICATION == 1)
			{
				sem_wait(&CNFSerParaList[threadnum].semCNFServerWindowBegin);
			}

			if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
				printf("[%d] WindowBegin triggered!\n",CNFSerParaList[threadnum].CountCNFServerWindowBegin_Triggered);
			CNFSerParaList[threadnum].CountCNFServerWindowBegin_Triggered++;
		}
		
		//��ʱ�������ڴ�������ͨ��������ʱ��ʾ
		if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
			printf("[%d] WindowTimingEnd waiting...\n",CNFSerParaList[threadnum].CountCNFServerWindowTimingEnd_Waiting);
		CNFSerParaList[threadnum].CountCNFServerWindowTimingEnd_Waiting++;

		//����ʱ��UDP��Ϣ����
		//���ó�ʱ
		delaytime.tv_sec=CNFSerParaList[threadnum].RTOs;
		delaytime.tv_usec=CNFSerParaList[threadnum].RTOus;

		//��¼sockfd
		FD_ZERO(&readfds);
		FD_SET(CNFSerParaList[threadnum].RECVCNFServerWindowTimingEnd,&readfds);

		select(CNFSerParaList[threadnum].RECVCNFServerWindowTimingEnd+1,&readfds,NULL,NULL,&delaytime);
	
		//��ʱ����
		if(FD_ISSET(CNFSerParaList[threadnum].RECVCNFServerWindowTimingEnd,&readfds))
		{
			if(0 <= recvfrom(CNFSerParaList[threadnum].RECVCNFServerWindowTimingEnd,Trigger,20,0,(struct sockaddr *)&CNFSerParaList[threadnum].RECVAddrToCNFServerWindowTimingEnd,&SockAddrLength))
			{
				if(cnfserDEVETESTIMPL <= 0)
					printf("[TimingEndDelay]  %.3f ms\n",(double)(RTOUS - (int)delaytime.tv_usec)/(double)1000);
				judge = 1;
			}
		}
		else
		{
			if(cnfserDEVETESTIMPL <= 0)
				printf("[TimingEndDelay]  %.3f ms   (Timeout)\n",(double)(RTOUS - (int)delaytime.tv_usec)/(double)1000);
		}

		if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
			printf("[%d] WindowTimingEnd triggered!\n",CNFSerParaList[threadnum].CountCNFServerWindowTimingEnd_Triggered);
		CNFSerParaList[threadnum].CountCNFServerWindowTimingEnd_Triggered++;

		//��ʾ���
		if(judge > 0)//��ʱ���
		{
			//printf("On Time!\n");
			
			//��ʱ���յ��µ�ACK������������δ����ɹ������ڿ������ȴ�������������ö�ʱ��
			if(CNFSerParaList[threadnum].WindowState == 1)
			{
				continue;
			}
			//���������2���ڰ�ʱ����ɹ� 3����ACK������Ҫ���������ڴ��䴦��
		}
		else//��ʱ
		{
			//printf("Time Out!\n");
			CNFSerParaList[threadnum].WindowState = 4;
			//printf("WindowState = %d\n",CNFSerParaList[threadnum].WindowState);
		}

		//֪ͨ����ͨ�����������δ��ڴ����ѵó����
		if(cnfINTRACOMMUNICATION == 0)
		{
			sendto(CNFSerParaList[threadnum].SENDCNFServerWindowRespond,"WindowRespond",20,0,(struct sockaddr *)&CNFSerParaList[threadnum].SENDAddrToCNFServerWindowRespond,SockAddrLength);
		}
		else if(cnfINTRACOMMUNICATION == 1)
		{
			sem_post(&CNFSerParaList[threadnum].semCNFServerWindowRespond);
		}

		if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
			printf("[%d] WindowRespond triggering...\n",CNFSerParaList[threadnum].CountCNFServerWindowRespond_Triggering);
		CNFSerParaList[threadnum].CountCNFServerWindowRespond_Triggering++;
	}
	
	sleep(10000);
	//�ر��߳�
	//exit(0);
	//free(fd);
	//pthread_exit(NULL);
}

/*****************************************
* �������ƣ�CNFServer_thread_TransportLayertoNetworkLayer
* ��������������ͨ�� - From.Ӧ�÷����� To.������Ӧ�ˣ����𴰿ڵļ����뻬������ʱ����Ӧ����װ���ݲ����µݽ�
* �����б�
* ���ؽ����
*****************************************/
void *
CNFServer_thread_TransportLayertoNetworkLayer
(
	void * fd
)
{
	int i,j;

	//��CNFServerģ������̵߳��õĲ��������ӿ�
	struct cnfserinput * cnfinput;
	cnfinput = (struct cnfserinput *)fd;
	
	int threadnum = cnfinput->threadnum;
	
	if(cnfserDEVETESTIMPL <= 6)
		printf("CNF threadnum == %d (CNFServer_thread_TransportLayertoNetworkLayer)\n",threadnum);

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

	if(cnfserDEVETESTIMPL <= 6)
	{
		printf("cnfinput->physicalportIP == %s\n",cnfinput->physicalportIP);
		printf("PhysicalportInterPORT = %d\n",PhysicalportInterPORT);
	}

	//��ȡ������
	//int ThreadNum = *((int *)fd);printf("[DOWN]ThreadNum = %d\n",ThreadNum);
	
	//��������ȴ����ͻ���׼������

	if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
		printf("[%d] BufReady waiting...\n",CNFSerParaList[threadnum].CountCNFServerBufReady_Waiting);

	CNFSerParaList[threadnum].CountCNFServerBufReady_Waiting++;

	if(cnfINTRACOMMUNICATION == 0)
	{
		recvfrom(CNFSerParaList[threadnum].RECVCNFServerBufReady,Trigger,20,0,(struct sockaddr *)&CNFSerParaList[threadnum].RECVAddrToCNFServerBufReady,&SockAddrLength);
	}
	else if(cnfINTRACOMMUNICATION == 1)
	{
		sem_wait(&CNFSerParaList[threadnum].semCNFServerBufReady);
	}

	if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
		printf("[%d] BufReady triggered!\n",CNFSerParaList[threadnum].CountCNFServerBufReady_Triggered);

	CNFSerParaList[threadnum].CountCNFServerBufReady_Triggered++;
	
	//������ʼ���������ʼ�������˵�ʱ���Ѿ���ζ���յ�ƫ��ΪPKTDATALEN������

	CNFSerParaList[threadnum].BUFLength = CNFBUFSIZE;//���ͻ��峤��

	CNFSerParaList[threadnum].PacketDataLength = PKTDATALEN;//�������ݶγ���
	CNFSerParaList[threadnum].LastPacketDataLength = PKTDATALEN;//����������������һ�����ݰ�ʵ�ʳ���

	CNFSerParaList[threadnum].WindowLength = 2;//���ڳ���
	CNFSerParaList[threadnum].WindowThreshold = CNFBUFSIZE;//��������ֵ

	CNFSerParaList[threadnum].OffsetWindowHead = 0;//�������ļ��д���ͷ����ָ��ƫ����
	CNFSerParaList[threadnum].OffsetWindowEnd = PKTDATALEN + CNFSerParaList[threadnum].WindowLength * CNFSerParaList[threadnum].PacketDataLength;//�������ļ��д���β����ָ��ƫ����
	CNFSerParaList[threadnum].OffsetConfirmACK = 0;//�������ļ�����ȷ��ƫ��������ƫ����
	CNFSerParaList[threadnum].OffsetRequestACK = PKTDATALEN;//�������ļ��нӵ������ƫ����
	CNFSerParaList[threadnum].OffsetPacket = PKTDATALEN;//�������ļ��е�ǰ���ƫ�������ش����ݰ�ƫ�����ֶε���д���ݣ�
	CNFSerParaList[threadnum].OffsetWaitACK = PKTDATALEN;//�������ļ��еȴ�ȷ���յ���ƫ��������һ�ڣ�

	CNFSerParaList[threadnum].AddrWindowHead = CNFSerParaList[threadnum].BUF;//�ڻ������д���ͷ�����ڵ�ַ
	CNFSerParaList[threadnum].AddrWindowEnd = CNFSerParaList[threadnum].BUF + CNFSerParaList[threadnum].WindowLength * CNFSerParaList[threadnum].PacketDataLength;//�ڻ������д���β�����ڵ�ַ
	CNFSerParaList[threadnum].AddrConfirmACK = CNFSerParaList[threadnum].BUF;//�ڻ���������ȷ��ƫ�������ڵ�ַ
	CNFSerParaList[threadnum].AddrRequestACK = CNFSerParaList[threadnum].BUF;//�ڻ������нӵ���������ڵ�ַ
	CNFSerParaList[threadnum].AddrPacket = CNFSerParaList[threadnum].BUF;//�ڻ������е�ǰ���ƫ�������ڵ�ַ
	CNFSerParaList[threadnum].AddrWaitACK = CNFSerParaList[threadnum].BUF;//�ڻ������д�ȷ��ƫ�������ڵ�ַ

	CNFSerParaList[threadnum].RTOs = RTOS;//��������ʱ�ӣ�RTO����1�벿�֣���λ�룩
	CNFSerParaList[threadnum].RTOus = RTOUS;//��������ʱ�ӣ�RTOС��1�벿�֣���λ΢�룩
	CNFSerParaList[threadnum].RTOns = RTONS;//��������ʱ�ӣ�RTOС��1�벿�֣���λ���룩

	CNFSerParaList[threadnum].FlagTrebleAck = 0;//����������¼�յ���ͬƫ����ACK�ĸ����������ж�����ACK����״��

	CNFSerParaList[threadnum].WindowState = 0;//����״̬��1ACK�������µ�δ�ﴰ����Ҫ����Ͻ磻2���ڴ���ɹ���3����ACK��4��ʱ
	//CNFSerParaList[threadnum].ThreadState = 0;//����״̬��ʱ�Ѿ��ı䣨FTP�߳��ж�ȡС�ļ�ʱ�ᷢ���ı䣬�ʲ����ٴγ�ʼ����


	//������̱���

	uint8_t DATA[DATALEN];
	unsigned long offsetlong;
	uint8_t offset[OFFLEN];
	
	uint8_t sendpkg[sizeof(CoLoR_seg)+3];
	CoLoR_seg * psendpkg;
	psendpkg = (CoLoR_seg *)(sendpkg+3);

	int LocalPkgBUFHead;
	while(1)
	{
		CNFSerParaList[threadnum].FlagTrebleAck = 0;

		//����ļ��Ѿ���ȡ��ϣ������鷢�͹����Ƿ��Ѿ����ļ�ĩβ
		//printf("ThreadState == %d\n",CNFSerParaList[threadnum].ThreadState);
		//printf("[SET TAIL]CNFSerParaList[threadnum].AddrWindowEnd   = %d\n",CNFSerParaList[threadnum].AddrWindowEnd - CNFSerParaList[threadnum].BUF);
		//printf("ThreadState = %d\n",CNFSerParaList[threadnum].ThreadState);
		if(CNFSerParaList[threadnum].ThreadState >= 1)
		{
/*
			printf("[CHECK TAIL]OffsetConfirmACK == %ld\n",CNFSerParaList[threadnum].OffsetConfirmACK);
			printf("[CHECK TAIL]OffsetRequestACK == %ld\n",CNFSerParaList[threadnum].OffsetRequestACK);
			printf("[CHECK TAIL]OffsetPacket + WindowLength * PacketDataLength == %ld\n",CNFSerParaList[threadnum].OffsetPacket + CNFSerParaList[threadnum].WindowLength * CNFSerParaList[threadnum].PacketDataLength);
*/
			if( CNFSerParaList[threadnum].OffsetPacket - PKTDATALEN + CNFSerParaList[threadnum].WindowLength * CNFSerParaList[threadnum].PacketDataLength > CNFSerParaList[threadnum].FileLength )
			{
				CNFSerParaList[threadnum].ThreadState = 2;
				
				//�����ļ�ʣ�೤�����¼��㴰�ڴ�С
				CNFSerParaList[threadnum].WindowLength = ( CNFSerParaList[threadnum].FileLength - CNFSerParaList[threadnum].OffsetConfirmACK ) / CNFSerParaList[threadnum].PacketDataLength;

				if(cnfserDEVETESTIMPL <= 6)
					printf("WindowLength = %ld\n",CNFSerParaList[threadnum].WindowLength);

				if(( CNFSerParaList[threadnum].FileLength - CNFSerParaList[threadnum].OffsetConfirmACK ) % CNFSerParaList[threadnum].PacketDataLength != 0)
					CNFSerParaList[threadnum].WindowLength++;

				if(cnfserDEVETESTIMPL <= 6)
					printf("WindowLength = %ld\n",CNFSerParaList[threadnum].WindowLength);
				
				//������¼���õ��Ĵ��ڴ�СΪ0��˵���������ݾ��õ���ȷ�ϣ��ļ���û��ʣ�೤�ȣ��˳�
				if(CNFSerParaList[threadnum].WindowLength == 0)
				{
					CNFSerParaList[threadnum].ThreadState = 4;
					if(cnfserDEVETESTIMPL <= 6)
						printf("[END] File Transmission Succeed!!(By. WindowLength Zero)\n");
					break;
				}

				//���´���β��
				CNFSerParaList[threadnum].OffsetWindowEnd = CNFSerParaList[threadnum].OffsetWindowHead + CNFSerParaList[threadnum].WindowLength * CNFSerParaList[threadnum].PacketDataLength;
				CNFSerParaList[threadnum].AddrWindowEnd   = CNFSerParaList[threadnum].AddrWindowHead   + CNFSerParaList[threadnum].WindowLength * CNFSerParaList[threadnum].PacketDataLength;

				if(cnfserDEVETESTIMPL <= 6)
				{
					printf("[SET TAIL]CNFSerParaList[threadnum].OffsetWindowEnd = %ld\n",CNFSerParaList[threadnum].OffsetWindowEnd);
					printf("[SET TAIL]CNFSerParaList[threadnum].AddrWindowEnd   = %d\n",CNFSerParaList[threadnum].AddrWindowEnd - CNFSerParaList[threadnum].BUF);
				}
			}
		}
		
		//ѭ�����ʹ����ڵ����ݰ�


		//��¼���ڳ��ȱ仯����
		CNFSerParaList[threadnum].InputCount++;
		CNFSerParaList[threadnum].TotalWindowLength += CNFSerParaList[threadnum].WindowLength;
		//printf("[%d] ",CNFSerParaList[threadnum].InputCount);
		//printf("WindowLength = %ld\n",CNFSerParaList[threadnum].WindowLength);
		
/*
		itoa((int)CNFSerParaList[threadnum].WindowLength,CNFSerParaList[threadnum].NumStr,10);
		if(CNFSerParaList[threadnum].WindowLength<10)
			write(CNFSerParaList[threadnum].LogFilePoint,CNFSerParaList[threadnum].NumStr,1);
		else if(CNFSerParaList[threadnum].WindowLength<100)
			write(CNFSerParaList[threadnum].LogFilePoint,CNFSerParaList[threadnum].NumStr,2);
		else if(CNFSerParaList[threadnum].WindowLength<1000)
			write(CNFSerParaList[threadnum].LogFilePoint,CNFSerParaList[threadnum].NumStr,3);
		write(CNFSerParaList[threadnum].LogFilePoint," ",1);
*/

		for(i=0;i<CNFSerParaList[threadnum].WindowLength;i++)
		{
			if(cnfserDEVETESTIMPL <= 1)
				printf("WindowLength = %ld\n",CNFSerParaList[threadnum].WindowLength);

			memset(sendpkg,0,sizeof(CoLoR_seg)+3);
			memcpy(sendpkg,"FOR",3);

			//���CoLoR-SEQ����
			psendpkg->version_type = 163;//�汾4λ������4λ����Ϊ���ó�CoLoR_seq��
			psendpkg->ttl = 255;//����ʱ��
			psendpkg->total_len = sizeof(CoLoR_seg);//�ܳ���
	
			psendpkg->port_no = 1;//�˿ں�
			psendpkg->checksum = 0;//�����
	
			psendpkg->sid_len = SIDLEN;//SID����
			psendpkg->nid_len = NIDLEN;//NID����
			psendpkg->pid_n = PIDN;//PID����

			//�̶��ײ�ѡ��
			if(CNFON == 1)//CNF�趨
				psendpkg->options_static = 32;
			else if(CNFON == 0)//CNF�趨
				psendpkg->options_static = 0;
	
			psendpkg->signature_algorithm = 1;//ǩ���㷨
			psendpkg->if_hash_cache = 255;//�Ƿ��ϣ4λ���Ƿ񻺴�4λ
			psendpkg->options_dynamic = 0;//�ɱ��ײ�ѡ��
			
			//��ȡ��������
			if(i == CNFSerParaList[threadnum].WindowLength - 1 && CNFSerParaList[threadnum].ThreadState >= 2)//����Ѿ�¼�뵽������������������һ����
			{
				if(cnfserDEVETESTIMPL <= 6)
				{
					printf("The last Packet!!!!\n");
					printf("[LAST PACKET]AddrWindowEnd - BUF        = %d\n",CNFSerParaList[threadnum].AddrWindowEnd - CNFSerParaList[threadnum].BUF);
					printf("[LAST PACKET]AddrWindowEnd - AddrPacket = %d\n",CNFSerParaList[threadnum].AddrWindowEnd - CNFSerParaList[threadnum].AddrPacket);
				}

				memcpy(DATA,CNFSerParaList[threadnum].AddrPacket,CNFSerParaList[threadnum].AddrWindowEnd - CNFSerParaList[threadnum].AddrPacket);

				CNFSerParaList[threadnum].LastPacketDataLength = CNFSerParaList[threadnum].AddrWindowEnd - CNFSerParaList[threadnum].AddrPacket;
				
				//����offset����������
				offsetlong = CNFSerParaList[threadnum].OffsetPacket;
				for(j=OFFLEN-1;j>=0;j--)
				{
					offset[j] = (char)offsetlong%256;
					offsetlong/=256;
				}

				if(cnfserDEVETESTIMPL <= 6)
					printf("Sent Offset = %ld\n",CNFSerParaList[threadnum].OffsetPacket);
					//printf("Sent Offset = %d %d %d %d\n",(int)offset[0],(int)offset[1],(int)offset[2],(int)offset[3]);

				//��װSID��DATA��offset
				memcpy(psendpkg->sid,CNFSerParaList[threadnum].SID,SIDLEN);
				memcpy(psendpkg->data,DATA,CNFSerParaList[threadnum].AddrWindowEnd - CNFSerParaList[threadnum].AddrPacket);
				memcpy(psendpkg->offset,offset,OFFLEN);

				//���һ��ʱ�䷢����һ����
				usleep(FlowSlot);
				
				//����SEQ�����
				if(INTRACOMMUNICATION == 0)
				{
					sendto(CNFSerParaList[threadnum].SENDCNFServerSEG,sendpkg,sizeof(CoLoR_seg)+3,0,(struct sockaddr *)&CNFSerParaList[threadnum].SENDAddrToCNFServerSEG,SockAddrLength);
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
						//printf("[C]PkgBUFHead = %d\n",PkgBUFHead);
						//printf("[C]PkgBUF[%d].flag_occupied = %d\n",PkgBUFHead,PkgBUF[PkgBUFHead].flag_occupied);
		
						PkgBUF[LocalPkgBUFHead].flag_occupied = 1;
						PkgBUF[LocalPkgBUFHead].destppnum = portnum;
						PkgBUF[LocalPkgBUFHead].pkglength = sizeof(CoLoR_seg);
						strcpy(PkgBUF[LocalPkgBUFHead].sourcepp,CNFThreadList[threadnum].physicalport);
						memcpy(PkgBUF[LocalPkgBUFHead].pkg,sendpkg+3,sizeof(CoLoR_seg));
		
						//����������ź���
						sem_post(&semPacketSEND[portnum]);

						if(cnfserDEVETESTIMPL <= 1)
						{
							printf("[%s] SEQ Message has been sent\n",CNFThreadList[threadnum].physicalport);
							printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
						}
					}
					else
					{
						if(cnfserDEVETESTIMPL <= 1)
						{
							printf("[%s] SEQ Message failed to be sent\n",CNFThreadList[threadnum].physicalport);
							printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
						}
					}
					pthread_mutex_unlock(&lockPkgBUF);
				}

				if(cnfserDEVETESTIMPL <= 6)
					printf("[CNFServer]The last SEQ packet sent.\n");

				//����RTO��ʱ��
				if(i == 0)
				{
					if(cnfINTRACOMMUNICATION == 0)
					{
						sendto(CNFSerParaList[threadnum].SENDCNFServerWindowBegin,"WindowBegin",20,0,(struct sockaddr *)&CNFSerParaList[threadnum].SENDAddrToCNFServerWindowBegin,SockAddrLength);
					}
					else if(cnfINTRACOMMUNICATION == 1)
					{
						sem_post(&CNFSerParaList[threadnum].semCNFServerWindowBegin);
					}

					if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
						printf("[%d] WindowBegin triggering...\n",CNFSerParaList[threadnum].CountCNFServerWindowBegin_Triggering);
					CNFSerParaList[threadnum].CountCNFServerWindowBegin_Triggering++;
				}
			
				//�����仯
				//CNFSerParaList[threadnum].OffsetPacket += CNFSerParaList[threadnum].PacketDataLength;
				//CNFSerParaList[threadnum].AddrPacket   += CNFSerParaList[threadnum].PacketDataLength;
			
				//�������ػ�����
				if(CNFSerParaList[threadnum].AddrPacket >= CNFSerParaList[threadnum].BUFEnd)
				{
					CNFSerParaList[threadnum].AddrPacket = CNFSerParaList[threadnum].BUF + ( CNFSerParaList[threadnum].AddrPacket - CNFSerParaList[threadnum].BUFEnd );
				}

				CNFSerParaList[threadnum].OffsetWaitACK = CNFSerParaList[threadnum].OffsetPacket;
				CNFSerParaList[threadnum].AddrWaitACK   = CNFSerParaList[threadnum].AddrPacket;
				
				//����������ĩ�˲����ã�У���ۼӵ��ļ������Ƿ���ʵ���ļ��������
				if(CNFSerParaList[threadnum].OffsetWaitACK > CNFSerParaList[threadnum].FileLength + PKTDATALEN)
				{
					printf("Deadly Error! [ OffsetWaitACK > FileLength + PKTDATALEN ] In The End!\n");
					exit(0);
				}
				else
				{
					//printf("File Length Checking passed!.\n");
				}

				CNFSerParaList[threadnum].ThreadState = 3;//�޸�����״̬Ϊ��3���ڷ������
			}
			else//���͵����ݰ���δ��������������һ��
			{
/*
				printf("[NOT END]AddrWindowEnd - BUF        = %d\n",CNFSerParaList[threadnum].AddrWindowEnd - CNFSerParaList[threadnum].BUF);
				printf("[NOT END]BUFFileEnd    - AddrPacket = %d\n",CNFSerParaList[threadnum].BUFFileEnd - CNFSerParaList[threadnum].AddrPacket);
				printf("[NOT END]AddrWindowEnd - AddrPacket = %d\n",CNFSerParaList[threadnum].AddrWindowEnd - CNFSerParaList[threadnum].AddrPacket);
				printf("[NOT END]OffsetPacket               = %ld\n",CNFSerParaList[threadnum].OffsetPacket);
*/
				memcpy(DATA,CNFSerParaList[threadnum].AddrPacket,CNFSerParaList[threadnum].PacketDataLength);

				//printf("DATA = \n");
				int testnum;
				for(testnum=0;testnum<CNFSerParaList[threadnum].PacketDataLength;testnum++)
				{
					//printf("%c",DATA[testnum]);
				}
				//printf("\n");
				//����offset����������
				offsetlong = CNFSerParaList[threadnum].OffsetPacket;
				for(j=OFFLEN-1;j>=0;j--)
				{
					offset[j] = (char)offsetlong%256;
					offsetlong/=256;
				}

				if(cnfserDEVETESTIMPL <= 1)
					printf("Sent Offset = %ld\n",CNFSerParaList[threadnum].OffsetPacket);

				//printf("Sent Offset = %d %d %d %d\n",(int)offset[0],(int)offset[1],(int)offset[2],(int)offset[3]);

				//printf("OFFSET = %d %d %d %d\n",offset[0],offset[1],offset[2],offset[3]);
				//printf("PacketDataLength = %ld\n",CNFSerParaList[threadnum].PacketDataLength);
				//printf("SIDLen = %d\n",CNFSerParaList[threadnum].SIDLen);
				
				//��װSID��DATA��offset
				memcpy(psendpkg->sid,CNFSerParaList[threadnum].SID,SIDLEN);
				memcpy(psendpkg->data,DATA,CNFSerParaList[threadnum].PacketDataLength);
				memcpy(psendpkg->offset,offset,OFFLEN);

				//���һ��ʱ�䷢����һ����
				usleep(FlowSlot);

				//����SEQ�����
				if(INTRACOMMUNICATION == 0)
				{
					sendto(CNFSerParaList[threadnum].SENDCNFServerSEG,sendpkg,sizeof(CoLoR_seg)+3,0,(struct sockaddr *)&CNFSerParaList[threadnum].SENDAddrToCNFServerSEG,SockAddrLength);
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
						PkgBUF[LocalPkgBUFHead].pkglength = sizeof(CoLoR_seg);
						strcpy(PkgBUF[LocalPkgBUFHead].sourcepp,CNFThreadList[threadnum].physicalport);
						memcpy(PkgBUF[LocalPkgBUFHead].pkg,sendpkg+3,sizeof(CoLoR_seg));
		
						//����������ź���
						sem_post(&semPacketSEND[portnum]);

						if(cnfserDEVETESTIMPL <= 1)
						{
							printf("[%s] ACK Message has been sent\n",CNFThreadList[threadnum].physicalport);
							printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
						}
					}
					else
					{
						if(cnfserDEVETESTIMPL <= 1)
						{
							printf("[%s] ACK Message failed to be sent\n",CNFThreadList[threadnum].physicalport);
							printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
						}
					}
					pthread_mutex_unlock(&lockPkgBUF);
				}
				
				//����RTO��ʱ��
				if(i == 0)
				{
					if(cnfINTRACOMMUNICATION == 0)
					{
						sendto(CNFSerParaList[threadnum].SENDCNFServerWindowBegin,"WindowBegin",20,0,(struct sockaddr *)&CNFSerParaList[threadnum].SENDAddrToCNFServerWindowBegin,SockAddrLength);
					}
					else if(cnfINTRACOMMUNICATION == 1)
					{
						sem_post(&CNFSerParaList[threadnum].semCNFServerWindowBegin);
					}

					if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
						printf("[%d] WindowBegin triggering...\n",CNFSerParaList[threadnum].CountCNFServerWindowBegin_Triggering);
					CNFSerParaList[threadnum].CountCNFServerWindowBegin_Triggering++;
				}
			
				//�����仯
				CNFSerParaList[threadnum].OffsetPacket += CNFSerParaList[threadnum].PacketDataLength;
				CNFSerParaList[threadnum].AddrPacket   += CNFSerParaList[threadnum].PacketDataLength;
			
				//�������ػ�����
				if(CNFSerParaList[threadnum].AddrPacket >= CNFSerParaList[threadnum].BUFEnd)
				{
					CNFSerParaList[threadnum].AddrPacket = CNFSerParaList[threadnum].BUF + ( CNFSerParaList[threadnum].AddrPacket - CNFSerParaList[threadnum].BUFEnd );
				}

				CNFSerParaList[threadnum].OffsetWaitACK = CNFSerParaList[threadnum].OffsetPacket;
				CNFSerParaList[threadnum].AddrWaitACK   = CNFSerParaList[threadnum].AddrPacket;
/*
				printf("[PARA CHANGE]OffsetPacket     = %ld\n",CNFSerParaList[threadnum].OffsetPacket);
				printf("[PARA CHANGE]AddrPacket - BUF = %d\n",CNFSerParaList[threadnum].AddrPacket - CNFSerParaList[threadnum].BUF);
				printf("[PARA CHANGE]OffsetWaitACK    = %ld\n",CNFSerParaList[threadnum].OffsetWaitACK);
				printf("[PARA CHANGE]AddrWaitACK- BUF = %d\n",CNFSerParaList[threadnum].AddrWaitACK - CNFSerParaList[threadnum].BUF);
*/
			}
		}
		
		//�����������ȴ�����ͨ���ź�
		if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
			printf("[%d] WindowRespond waiting...\n",CNFSerParaList[threadnum].CountCNFServerWindowRespond_Waiting);
		CNFSerParaList[threadnum].CountCNFServerWindowRespond_Waiting++;

		if(cnfINTRACOMMUNICATION == 0)
		{
			recvfrom(CNFSerParaList[threadnum].RECVCNFServerWindowRespond,Trigger,20,0,(struct sockaddr *)&CNFSerParaList[threadnum].RECVAddrToCNFServerWindowRespond,&SockAddrLength);
		}
		else if(cnfINTRACOMMUNICATION == 1)
		{
			sem_wait(&CNFSerParaList[threadnum].semCNFServerWindowRespond);
		}

		if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
			printf("[%d] WindowRespond triggered!\n",CNFSerParaList[threadnum].CountCNFServerWindowRespond_Triggered);
		CNFSerParaList[threadnum].CountCNFServerWindowRespond_Triggered++;
		
		//��������������
		
		//���ڴ�С�����ڻ���������ֵ����ز������㣬���ݲ�����CNFSerParaList[threadnum].WindowState��CNFSerParaList[threadnum].OffsetRequestACK
		//printf("WindowState = %d\n",CNFSerParaList[threadnum].WindowState);

		if(CNFSerParaList[threadnum].WindowState == 1)//ACK�������µ�δ�ﴰ����Ҫ����Ͻ�
		{
			//printf("After WindowRespond, WindowState == 1. Maybe not normal. Check the code.\n");
			CNFSerParaList[threadnum].WindowState = 0;
		}
		else if(CNFSerParaList[threadnum].WindowState == 2)//��������˳���������
		{
			CNFSerParaList[threadnum].WindowState = 0;
			
			//����������ɹ�
			if(CNFSerParaList[threadnum].ThreadState >= 3)
			{
				CNFSerParaList[threadnum].ThreadState = 4;
				if(cnfserDEVETESTIMPL <= 6)
					printf("[END] File Transmission Succeed!!(By. Rightpoint)\n");
				break;
			}

			if(CNFSerParaList[threadnum].WindowLength < CNFSerParaList[threadnum].BUFLength / 2 -1)//���ⴰ�ڳ��ȴ��ڵ������ݴ���Ļ����ܳ�
			{
				if(CNFSerParaList[threadnum].WindowLength * 2 <= CNFSerParaList[threadnum].WindowThreshold)//�������׶�
					CNFSerParaList[threadnum].WindowLength *= 2;
				else//ӵ������׶�
					CNFSerParaList[threadnum].WindowLength ++;
			}

			//���ⴰ�ڳ��ȳ�����������������
			/*
			if(CNFSerParaList[threadnum].WindowLength > CNFSerParaList[threadnum].BUFLength / CNFSerParaList[threadnum].PacketDataLength / 2)
			{
				CNFSerParaList[threadnum].WindowLength = CNFSerParaList[threadnum].BUFLength / CNFSerParaList[threadnum].PacketDataLength / 2;
			}
			*/
			//���ⴰ�ڳ��ȳ�����󴰿ڳ���
			if(CNFSerParaList[threadnum].WindowLength > MAXWndLen)
			{
				CNFSerParaList[threadnum].WindowLength = MAXWndLen;
			}

			//���´���ͷ��
			CNFSerParaList[threadnum].OffsetWindowHead = CNFSerParaList[threadnum].OffsetWindowEnd;
			CNFSerParaList[threadnum].AddrWindowHead   = CNFSerParaList[threadnum].AddrWindowEnd;
			//���´���β��
			CNFSerParaList[threadnum].OffsetWindowEnd += CNFSerParaList[threadnum].WindowLength * CNFSerParaList[threadnum].PacketDataLength;
			CNFSerParaList[threadnum].AddrWindowEnd   += CNFSerParaList[threadnum].WindowLength * CNFSerParaList[threadnum].PacketDataLength;
/*
			printf("[RENEW WND HEAD1]AddrWindowHead - BUF = %d\n",CNFSerParaList[threadnum].AddrWindowHead - CNFSerParaList[threadnum].BUF);
			printf("[RENEW WND TAIL1]OffsetWindowEnd     = %ld\n",CNFSerParaList[threadnum].OffsetWindowEnd);
			printf("[RENEW WND TAIL1]AddrWindowEnd - BUF = %d\n",CNFSerParaList[threadnum].AddrWindowEnd - CNFSerParaList[threadnum].BUF);
*/
			//�������ػ�����
			if(CNFSerParaList[threadnum].AddrWindowEnd >= CNFSerParaList[threadnum].BUFEnd)
			{
				CNFSerParaList[threadnum].AddrWindowEnd = CNFSerParaList[threadnum].BUF + ( CNFSerParaList[threadnum].AddrWindowEnd - CNFSerParaList[threadnum].BUFEnd );
			}
		}
/*
		else if(CNFSerParaList[threadnum].WindowState == 3)//����ACK����ȶ�����
		{printf("Treble ACK!\n");
			CNFSerParaList[threadnum].WindowState = 0;
			
			//����״̬����
			if(CNFSerParaList[threadnum].ThreadState >= 2)
			{
				CNFSerParaList[threadnum].ThreadState = 1;
			}

			CNFSerParaList[threadnum].WindowThreshold = CNFSerParaList[threadnum].WindowLength / 2;//������ֵ����Ϊ��ǰ���ڴ�С��һ��
			
			CNFSerParaList[threadnum].WindowLength = CNFSerParaList[threadnum].WindowThreshold;//�Ѵ���ֵ����Ϊ����ֵ
			
			if(CNFSerParaList[threadnum].WindowThreshold == 0)
				CNFSerParaList[threadnum].WindowThreshold = 1;
			if(CNFSerParaList[threadnum].WindowLength == 0)
				CNFSerParaList[threadnum].WindowLength = 1;
			
			printf("CNFSerParaList[threadnum].WindowThreshold = %ld\n",CNFSerParaList[threadnum].WindowThreshold);

			//���´���ͷ��
			CNFSerParaList[threadnum].OffsetWindowHead = CNFSerParaList[threadnum].OffsetRequestACK;
			CNFSerParaList[threadnum].AddrWindowHead   = CNFSerParaList[threadnum].AddrWaitACK - ( CNFSerParaList[threadnum].OffsetWaitACK - CNFSerParaList[threadnum].OffsetRequestACK );

			//�������ػ�����
			if(CNFSerParaList[threadnum].AddrWindowHead < CNFSerParaList[threadnum].BUF)
			{
				CNFSerParaList[threadnum].AddrWindowHead = CNFSerParaList[threadnum].BUFEnd - ( CNFSerParaList[threadnum].BUF - CNFSerParaList[threadnum].AddrWindowHead );
			}

			//���´���β��
			CNFSerParaList[threadnum].OffsetWindowEnd = CNFSerParaList[threadnum].OffsetWindowHead + 
							 CNFSerParaList[threadnum].WindowLength * CNFSerParaList[threadnum].PacketDataLength;
			CNFSerParaList[threadnum].AddrWindowEnd   = CNFSerParaList[threadnum].AddrWindowHead   + 
							 CNFSerParaList[threadnum].WindowLength * CNFSerParaList[threadnum].PacketDataLength;

			//printf("[RENEW WND HEAD2]AddrWindowHead - BUF = %d\n",CNFSerParaList[threadnum].AddrWindowHead - CNFSerParaList[threadnum].BUF);
			//printf("[RENEW WND TAIL2]OffsetWindowEnd     = %ld\n",CNFSerParaList[threadnum].OffsetWindowEnd);
			//printf("[RENEW WND TAIL2]AddrWindowEnd - BUF = %d\n",CNFSerParaList[threadnum].AddrWindowEnd - CNFSerParaList[threadnum].BUF);

			//�������ػ�����
			if(CNFSerParaList[threadnum].AddrWindowEnd >= CNFSerParaList[threadnum].BUFEnd)
			{
				CNFSerParaList[threadnum].AddrWindowEnd = CNFSerParaList[threadnum].BUF + ( CNFSerParaList[threadnum].AddrWindowEnd - CNFSerParaList[threadnum].BUFEnd );
			}
		}
*/
		else if(CNFSerParaList[threadnum].WindowState == 3 || CNFSerParaList[threadnum].WindowState == 4)//����ACK��ʱ���ض�ӵ����
		{//printf("Time Out!\n");
			CNFSerParaList[threadnum].LostPacketCount++;//��¼��������
			
			CNFSerParaList[threadnum].WindowState = 0;
			
			//����������ɹ�
			if(CNFSerParaList[threadnum].ThreadState >= 3)
			{
				CNFSerParaList[threadnum].ThreadState = 4;

				if(cnfserDEVETESTIMPL <= 6)
					printf("[END] File Transmission Succeed!!(By. Timeouts)\n");

				break;
			}

			//����״̬����
			else if(CNFSerParaList[threadnum].ThreadState >= 2)
			{
				CNFSerParaList[threadnum].ThreadState = 1;
			}

			CNFSerParaList[threadnum].WindowThreshold = CNFSerParaList[threadnum].WindowLength / 2;//������ֵ����Ϊ��ǰ���ڴ�С��һ��
			
			CNFSerParaList[threadnum].WindowLength = 1;//�Ѵ��ڴ�С��������Ϊһ�����Ķ�

			if(CNFSerParaList[threadnum].WindowThreshold == 0)
				CNFSerParaList[threadnum].WindowThreshold = 1;

			//printf("CNFSerParaList[threadnum].WindowThreshold = %ld\n",CNFSerParaList[threadnum].WindowThreshold);
			
			//���´���ͷ��
			CNFSerParaList[threadnum].OffsetWindowHead = CNFSerParaList[threadnum].OffsetRequestACK;
			CNFSerParaList[threadnum].AddrWindowHead   = CNFSerParaList[threadnum].AddrWaitACK - ( CNFSerParaList[threadnum].OffsetWaitACK - CNFSerParaList[threadnum].OffsetRequestACK );

			//�������ػ�����
			if(CNFSerParaList[threadnum].AddrWindowHead < CNFSerParaList[threadnum].BUF)
			{
				CNFSerParaList[threadnum].AddrWindowHead = CNFSerParaList[threadnum].BUFEnd - ( CNFSerParaList[threadnum].BUF - CNFSerParaList[threadnum].AddrWindowHead );
			}

			//���´���β��
			CNFSerParaList[threadnum].OffsetWindowEnd = CNFSerParaList[threadnum].OffsetWindowHead + 
							 CNFSerParaList[threadnum].WindowLength * CNFSerParaList[threadnum].PacketDataLength;
			CNFSerParaList[threadnum].AddrWindowEnd   = CNFSerParaList[threadnum].AddrWindowHead   + 
							 CNFSerParaList[threadnum].WindowLength * CNFSerParaList[threadnum].PacketDataLength;
/*
			printf("[RENEW WND HEAD3]AddrWindowHead - BUF = %d\n",CNFSerParaList[threadnum].AddrWindowHead - CNFSerParaList[threadnum].BUF);
			printf("[RENEW WND TAIL3]OffsetWindowEnd     = %ld\n",CNFSerParaList[threadnum].OffsetWindowEnd);
			printf("[RENEW WND TAIL3]AddrWindowEnd - BUF = %d\n",CNFSerParaList[threadnum].AddrWindowEnd - CNFSerParaList[threadnum].BUF);
*/
			//�������ػ�����
			if(CNFSerParaList[threadnum].AddrWindowEnd >= CNFSerParaList[threadnum].BUFEnd)
			{
				CNFSerParaList[threadnum].AddrWindowEnd = CNFSerParaList[threadnum].BUF + ( CNFSerParaList[threadnum].AddrWindowEnd - CNFSerParaList[threadnum].BUFEnd );
			}
		}
		else//��������WindowState����״ֵ̬�쳣
		{
			//printf("[Deadly Error] (WindowState == %d)\n",CNFSerParaList[threadnum].WindowState);
			CNFSerParaList[threadnum].LostPacketCount++;
			continue;
			//exit(0);
		}

		CNFSerParaList[threadnum].OffsetPacket = CNFSerParaList[threadnum].OffsetRequestACK;
		CNFSerParaList[threadnum].AddrPacket   = CNFSerParaList[threadnum].AddrWindowHead;
		//printf("[PARA CHANGE]AddrPacket - BUF = %d\n",CNFSerParaList[threadnum].AddrPacket - CNFSerParaList[threadnum].BUF);
		
		//��Ӧ�ò�֪ͨ�Թ�Ӧ�������ݵ�Ҫ��
		if( CNFSerParaList[threadnum].OffsetRequestACK / ( CNFSerParaList[threadnum].BUFLength / 2 ) == CNFSerParaList[threadnum].BUFRenewCount + 1 )
		{
			if(cnfINTRACOMMUNICATION == 0)
			{
				sendto(CNFSerParaList[threadnum].SENDCNFServerMoreData,"MoreData",20,0,(struct sockaddr *)&CNFSerParaList[threadnum].SENDAddrToCNFServerMoreData,SockAddrLength);
			}
			else if(cnfINTRACOMMUNICATION == 1)
			{
				sem_post(&CNFSerParaList[threadnum].semCNFServerMoreData);
			}

			if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
				printf("[%d] MoreData triggering...\n",CNFSerParaList[threadnum].CountCNFServerMoreData_Triggering);
			CNFSerParaList[threadnum].CountCNFServerMoreData_Triggering++;
		}
		else if( CNFSerParaList[threadnum].OffsetRequestACK / ( CNFSerParaList[threadnum].BUFLength / 2 ) > CNFSerParaList[threadnum].BUFRenewCount + 1 )
		{
			if(cnfserDEVETESTIMPL <= 6)
				printf("[Deadly Error]This is not normal. BUFRenewCount has problems to solve.\n1. Maybe a MoreData message has been lost.\n2. WindowLength is larger than BUFLength/2.\n");
			
			//exit(0);
			//break;
		}

		//��������ͨ�������հ���ָ��
		if(cnfINTRACOMMUNICATION == 0)
		{
			sendto(CNFSerParaList[threadnum].SENDCNFServerContinueRECV,"ContinueRECV",20,0,(struct sockaddr *)&CNFSerParaList[threadnum].SENDAddrToCNFServerContinueRECV,SockAddrLength);
		}
		else if(cnfINTRACOMMUNICATION == 1)
		{
			sem_post(&CNFSerParaList[threadnum].semCNFServerContinueRECV);
		}
		
		if(cnfserDEVETESTIMPL <= 1 || CNFserverTRIGGERTEST == 1)
			printf("[%d] ContinueRECV triggering...\n",CNFSerParaList[threadnum].CountCNFServerContinueRECV_Triggering);
		CNFSerParaList[threadnum].CountCNFServerContinueRECV_Triggering++;

	}

	//���AIMD����ͳ�Ʋ���
	if(cnfserDEVETESTIMPL <= 7)
	{
		//����ʱ��ڵ㣨΢�룩		
		gettimeofday(&utime,NULL);
		printf("==================Time==================\nServer Transmission End   %ld us\n========================================\n",1000000 * utime.tv_sec + utime.tv_usec);

		printf("==============DATA Results==============\n");
		printf("Total Time             %ld seconds\n",CNFSerParaList[threadnum].timer_XX);
		printf("Transport Rate         %.3f Byte/s\n",(double)CACHESIZE / (double)CNFSerParaList[threadnum].timer_XX);
		printf("Average Window Length  %ld\n\n",CNFSerParaList[threadnum].TotalWindowLength / (unsigned long)CNFSerParaList[threadnum].InputCount);
		printf("LostPacketCount        %ld\n",CNFSerParaList[threadnum].LostPacketCount);
		printf("TotalPacketCount       %ld\n",CNFSerParaList[threadnum].FileLength / CNFSerParaList[threadnum].PacketDataLength + 1);
		printf("Lost Rate              %.3f%%\n",((double)CNFSerParaList[threadnum].LostPacketCount / ((double)CNFSerParaList[threadnum].FileLength / (double)CNFSerParaList[threadnum].PacketDataLength)) * (double)100);
		printf("========================================\n");
	}

	//�ر������߳�
	CNFThreadList[threadnum].flag_close = 1;

	int closethread1,closethread2,closethread3,closethread4;
	closethread1 = pthread_cancel(CNFSerParaList[threadnum].pthread_NetworkLayertoTransportLayer);
	closethread2 = pthread_cancel(CNFSerParaList[threadnum].pthread_FTP);
	closethread3 = pthread_cancel(CNFSerParaList[threadnum].pthread_CNF);
	closethread4 = pthread_cancel(CNFSerParaList[threadnum].pthread_timer);
	if(closethread1 == 0 && closethread2 == 0 && closethread3 == 0 && closethread4 == 0)
	{
		if(cnfserDEVETESTIMPL <= 7)
		{
			printf("Server Threads Close Success.\n");
			printf(">->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->\n");
		}
	}
	else
	{
		printf("Server Threads Close Failed.\n");
		printf("closethread1 = %d\n",closethread1);
		printf("closethread2 = %d\n",closethread2);
		printf("closethread3 = %d\n",closethread3);
		printf("closethread4 = %d\n",closethread4);
	}
	
	//ͬʱ��ֹ��CNF�������񲢻��յ�������Դ
	CNFServer_Parametershut(threadnum);

	//�������߳�
	pthread_exit(NULL);
	//exit(0);
	//free(fd);
}

/*****************************************
* �������ƣ�CNFServer_main
* ����������CNFServerģ�������������������̣߳������ṩʵ�ʹ���
* �����б��ڶ�����ʹ��char**argv�ķ�ʽ����������֪�ǲ����﷨����
* ���ؽ����
*****************************************/
//int main(int argc, char *argv[])
int
CNFServer_main
(
	void * fd
)
{
	//��CNFServerģ������̵߳��õĲ��������ӿ�
	struct cnfserinput * pcnfinput;
	pcnfinput = (struct cnfserinput *)fd;
	
	//��physicalport���÷����̴߳��δ��ڵȴ�ʱ���Ե�ʣ�ʵ����ʧ̫�죬�˿���������ʱ���ܵõ���ʱ���������������������ڵ�����threadnum��Ϊ0������£���ʱ������δ��룬�滻��ֱ�Ӵ�ȫ����������鿽���������
	
	int threadnum = pcnfinput->threadnum;

	struct cnfserinput cnfinput;
	cnfinput.threadnum = pcnfinput->threadnum;
	cnfinput.cachelocation = pcnfinput->cachelocation;
	cnfinput.cachelength = pcnfinput->cachelength;
	memcpy(cnfinput.physicalportIP,pcnfinput->physicalportIP,16);
	memcpy(cnfinput.sid,pcnfinput->sid,SIDLEN);
	memcpy(cnfinput.sbd,pcnfinput->sbd,SBDLEN);
	memcpy(cnfinput.offset,pcnfinput->offset,OFFLEN);

	//���������̵߳ĵȴ�
	sem_post(&CNFThreadList[threadnum].StartParaTransEnd);
	
/*
	int threadnum;
	int i;
	for(i=0;i<10;i++)
	{
		if(CNFThreadList[i].flag_occupied == 1 && CNFThreadList[i].flag_IO == 1 && CNFThreadList[i].datapkglocation == -1)
		{
			threadnum = i;
			break;
		}
	}
	
	struct cnfserinput cnfinput;
	cnfinput.threadnum = threadnum;
	cnfinput.cachelocation = CNFThreadList[threadnum].cachelocation;
	cnfinput.cachelength = CNFThreadList[threadnum].cachelength;
	memcpy(cnfinput.physicalportIP,CNFThreadList[threadnum].physicalportIP,16);
	memcpy(cnfinput.sid,CNFThreadList[threadnum].sid,SIDLEN);
	memcpy(cnfinput.sbd,CNFThreadList[threadnum].sbd,SBDLEN);
	memcpy(cnfinput.offset,CNFThreadList[threadnum].offset,OFFLEN);
*/
	if(cnfserDEVETESTIMPL <= 6)
	{
		printf("[CNFServer_main]#cnfinput.physicalportIP = %s\n",cnfinput.physicalportIP);
		printf("[CNFServer_main]CNF threadnum == %d (CNFServer_main)\n",threadnum);
	}
	if(cnfserDEVETESTIMPL <= 6)
	{
		printf("[CNFServer_main]==O==ThreadNum = %d\n",threadnum);
		printf("[CNFServer_main]flag_occupied  = %d\n",CNFThreadList[threadnum].flag_occupied);
		printf("[CNFServer_main]flag_IO        = %d\n",CNFThreadList[threadnum].flag_IO);
		printf("[CNFServer_main]datapkglocation= %d\n",CNFThreadList[threadnum].datapkglocation);
		printf("[CNFServer_main]cachelocation  = %d\n",CNFThreadList[threadnum].cachelocation);
		printf("[CNFServer_main]physicalport   = %s\n",CNFThreadList[threadnum].physicalport);
		printf("[CNFServer_main]physicalportIP = %s\n",CNFThreadList[threadnum].physicalportIP);
		printf("[CNFServer_main]sid            = %s\n",CNFThreadList[threadnum].sid);
		printf("[CNFServer_main]offset         = %s\n",CNFThreadList[threadnum].offset);
		printf("[CNFServer_main]===================\n");
	}

	CNFServer_Parameterinit(threadnum);
	
	//�������ͼ�ʱ�����߳�
	if(pthread_create(&CNFSerParaList[threadnum].pthread_timer, NULL, CNFServer_thread_timer, (void *)&cnfinput)!=0)
	{
		perror("Creation of timer thread failed.");
	}

	//����CNF�����߳�
	if(pthread_create(&CNFSerParaList[threadnum].pthread_CNF, NULL, CNFServer_thread_CNF, (void *)&cnfinput)!=0)
	{
		perror("Creation of CNF thread failed.");
	}
	
	//��ѭ�������������ε�����̵߳Ľ����նˣ�ȡ������ѭ�����ʹ���ε�ʵ�ʴ洢������ʧ�����¼��߳��޷���ȡ���õĲ���
	while (!CNFThreadList[threadnum].flag_close)
	{
		sleep(1);
	}
}

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
)
{
	int i;

	//����CNFServerģ�������߳�
	struct cnfserinput cnfinput;
	cnfinput.threadnum = TESTNO;
	memset(cnfinput.sid,0,SIDLEN);
	
	pthread_t pthread_cnfserver;
	
	//�������߳�
	if(pthread_create(&pthread_cnfserver, NULL, CNFServer_main, (void *)&cnfinput)!=0)
	{
		perror("Creation of CNF Server main thread failed.");
	}


	//��ѭ�������ͻ��˴���
	while (1)
	{
		sleep(10000);
	}
}
