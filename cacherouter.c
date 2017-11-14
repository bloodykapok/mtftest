/*******************************************************************************************************************************************
* �ļ�����cacherouter.c
* �ļ�������973�ǻ�Эͬ����SARϵͳTestBed�׼��������ڻ���·������Cache Router������ת��ƽ��+·�ɻ�ȡ
* ��������ģ���������������Ϊ������ͨ��ѧ��һ�������������豸���ҹ���ʵ���������������̼�����ѵר�á������������ڴ�֣�����������ļ�������������ѵ��ģ������Ϊ���˾����뾭��Ĳ������Դ�Ĵ�����ʽ��Ϊ���߱��������˽���һ��һ���ڣ��κθ��˲����������ת����չʾ����Դ���ݡ��κ��������ѧԱ����������������ֱ����Ҫ��������������Ҫ����֮Ȩ������������Щ��Դ�����漰ʵ������Ŀ���ܣ��ݲ���������ר���������������Ȩ�����ˣ��ʸ�����Դ����ʵ�����ڲ�һ��һ���ţ��緢���й������߱��˵�һƬ�������Ϊ�����߽���������ԭ���Խ���׷�顢��֤�����ߺ������Ȩ��������Դ�ķ���Ȩ�����߱������У���������ܽ���̽�͸�޳����׵��ȳ���Ϊ���ѧҵ���ౡ֮������ϣ�ԸѧԱ�������ߵ��Ͷ��ɹ���лл������
* ���ߣ�������
* ��ݣ�������ͨ��ѧ��һ�������������豸���ҹ���ʵ���� 2013��˶�������о���
* E-mail. hellozxwang@foxmail.com
* Mobile. 18811774990
* QQ    . 535667240
* Addr  . �����к�������ֱ���ⱱ����ͨ��ѧ��е¥D706��, 100044
*******************************************************************************************************************************************/
/*******************************************************************************************************************************************
*****����˵����1.�������������������ϴ���SAR/CoLoR�������ݰ���
**************2.��ȡ·�ɱ�����NID����ת����Ŀ������
**************3.�������´���ѡ����������������
*******************************************************************************************************************************************/
/*
�������ò��裺
1���궨���޸�

2��ϵͳ����
��Fedoraϵͳ������Ҫʹ��ԭʼ�׽��ַ����Զ����ʽ�����ݰ�����ر�Fedora�ķ���ǽ�����
sudo systemctl stop firewalld.service
��Ubuntuϵͳ�������κβ���
3����������
make
4������
���漰ԭʼ�׽��ֵ�ʹ�ã���rootȨ��
sudo ./xxx��xxx�����ϲ���ñ��ļ��ĳ��������
*/

#include"cacherouter.h"

//����˿����������ڲ�ͨ��IP��ӳ�����
PPIP ppiplist[49];//����ӳ���

//·�ɱ���
RoutingTableEvent routingtable[100];//����·�ɱ�

//CoLoRЭ�����������жϵ��ֶΣ���ֹ���̶��ײ�������Version/Type�ֶ�ΪGet����Data����Register����ͨ�ã�
CoLoR_VersionType tempCoLoRwithoutMAC;

//CoLoRЭ��Get���ײ���PID֮ǰ���ֶγ��ȿɱ䣬���ڽ���
CoLoR_get_parse tempGet;

//CoLoRЭ��Data���ײ���PID֮ǰ���ֶγ��ȿɱ䣬���ڽ���
CoLoR_data_parse tempData;

//CoLoRЭ��Register���ײ���PID֮ǰ��
CoLoR_register_parse tempRegister;

/*******************************************************************************************************************************************
*******************************************��ʼ�������������********************************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�CacheRouter_Parameterinit
* ����������CacheRouterģ��ȫ�ֱ�������ֵ
* �����б�
* ���ؽ����
*****************************************/
void
CacheRouter_Parameterinit()
{
	memset(SwitchDevice_local_ip,0,16);
	memcpy(SwitchDevice_local_ip,"127.1.1.1",9);
	memset(CacheRouter_dest_ip,0,16);

	memset(ppiplist,0,26*49);
	ppiplistcount=-1;

	memset(routingtable,0,50*100);
	routingtablecount=-1;

	//�����ļ�·����ʼ��
	strcpy(FILERTname,FILERT);

	//���й�����ص���ת���жϱ���
	memset(tempNID,0,NIDLENst);

	//���ջ�����
	RecvBUFLength = 0;
	memset(RecvBUF,0,BUFSIZE);
	//���ͻ�����
	SendBUFLength = 0;
	memset(SendBUF,0,BUFSIZE);

	//�������б������ʼ��
	PkgBUFHead=0;
	PkgBUFTail=0;

	CacheRouter_PlaneOccupy = 0; //CacheRouterģ��ת��ƽ��ռ�ñ�ʶ
	CacheRouter_UpOccupy = 0;    //CacheRouterģ������ͨ��ռ�ñ�ʶ
	CacheRouter_DownOccupy = 0;  //CacheRouterģ������ͨ��ռ�ñ�ʶ
}

/*******************************************************************************************************************************************
*******************************************ת��ƽ�� - From.��Դ������������ To.Ŀ��������������***********************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�CacheRouter_CoLoR_ParseGet
* ��������������CoLoR-Get��ͷ
* �����б�
* ���ؽ����
*****************************************/
static int
CacheRouter_CoLoR_ParseGet
(
	const CoLoR_get_parse * pkg
)
{
	int i;
	
	char* p;
	p = (char*)pkg;
	
	if (NULL == pkg)
	{
		return -1;
	}
	
	//����Get�����ֶ�
	//free(tempGet.sid);
	//free(tempGet.nid);
	//free(tempGet.data);
	//free(tempGet.publickey);

	//tempGet.version_type = pkg->version_type;
	//tempGet.ttl = pkg->ttl;
	tempGet.total_len = pkg->total_len;

	//tempGet.port_no = pkg->port_no;
	//tempGet.checksum = pkg->checksum;

	tempGet.sid_len = pkg->sid_len;
	tempGet.nid_len = pkg->nid_len;
	tempGet.pid_n = pkg->pid_n;
	//tempGet.options_static = pkg->options_static;

	//p += sizeof(uint8_t) * (14 + 12);
	p += sizeof(uint8_t) * (12+16);

	tempGet.publickey_len = (uint16_t)(*p);
	//tempGet.mtu = (uint16_t)(*(p+sizeof(uint16_t)));

	p += sizeof(uint16_t) * 2;

	//tempGet.sid = (uint8_t*)calloc(tempGet.sid_len,sizeof(uint8_t));
	//memcpy((uint8_t*)tempGet.sid,(uint8_t*)p,tempGet.sid_len);

	p += sizeof(uint8_t) * tempGet.sid_len;

	//tempGet.nid = (uint8_t*)calloc(tempGet.nid_len,sizeof(uint8_t));
	//memcpy((uint8_t*)tempGet.nid,(uint8_t*)p,tempGet.nid_len);

	p += sizeof(uint8_t) * (tempGet.nid_len/2);

	//tempNID = (uint8_t*)calloc(tempGet.nid_len/2,sizeof(uint8_t));
	memset(tempNID,0,NIDLENst);
	memcpy(tempNID,(uint8_t*)p,tempGet.nid_len/2);

	//p += sizeof(uint8_t) * tempGet.nid_len;
	
	int data_len_int = tempGet.total_len - 16 - 16 - tempGet.sid_len - tempGet.nid_len - tempGet.pid_n*4 - tempGet.publickey_len;

	//������ڲа���ǡ���ܳ����ֶλ����������ֶ���ֵ�����˴�����û��У�飬���п��ܳ���data_lenֵΪ��ֵ���������������Ҫ�ų���
	if(data_len_int <= 0 || data_len_int > DATALENst)
	{
		if(netDEVETESTIMPL <= 2)
			printf("data_len == %d (break out)\n",data_len_int);
		return -1;
	}

	//uint16_t data_len = tempGet.total_len - 16 - 16 - tempGet.sid_len - tempGet.nid_len - tempGet.pid_n*4 - tempGet.publickey_len;
	//tempGet.data = (uint8_t*)calloc(data_len+1/*+1��������Ϊ�������ر�Ԥ��*/,sizeof(uint8_t));
	//memcpy((uint8_t*)tempGet.data,(uint8_t*)p,data_len);
	//*(tempGet.data+data_len) = '\0';//�����ΪʲôΨ��data����Ҫ�ر��ڳ�1byte��Ϊ�����������Խ���ȡ

	//p += sizeof(uint8_t) * data_len;

	//tempGet.publickey = (uint8_t*)calloc(tempGet.publickey_len,sizeof(uint8_t));
	//memcpy((uint8_t*)tempGet.publickey,(uint8_t*)p,tempGet.publickey_len);
	//memcpy(tempsid, tempGet.sid, tempGet.sid_len/*SID����*/);

	//�������
	if(0)
	{
		p = (char*)pkg;
		
		printf(">>>CoLoR-Get  Received.\n");

		printf("   |====================GetPkg===================|\n");
		printf("   |version_type   = %d\n",tempGet.version_type);
		printf("   |ttl            = %d\n",tempGet.ttl);
		printf("   |total_len      = %d\n",tempGet.total_len);

		printf("   |port_no        = %d\n",tempGet.port_no);
		printf("   |checksum       = %d\n",tempGet.checksum);

		printf("   |sid_len        = %d\n",tempGet.sid_len);
		printf("   |nid_len        = %d\n",tempGet.nid_len);
		printf("   |pid_n          = %d\n",tempGet.pid_n);
		printf("   |options_static = %d\n",tempGet.options_static);

		//p += sizeof(uint8_t) * (14 + 12);
		p += sizeof(uint8_t) * (12+16);

		//printf("   |publickey_len = %d\n",tempGet.publickey_len);
		//printf("   |mtu = %d\n",tempGet.mtu);

		p += sizeof(uint16_t) * 2;

		//printf("   |sid = %s\n",tempGet.sid);

		p += sizeof(uint8_t) * tempGet.sid_len;

		//printf("   |nid = %s\n",tempGet.nid);

		p += sizeof(uint8_t) * (tempGet.nid_len/2);

		//printf("   |nid = %s\n",tempNID);

		//p += sizeof(uint8_t) * tempGet.nid_len;

		//printf("   |data = %s\n",tempGet.data);

		//p += sizeof(uint8_t) * data_len;

		//printf("   |publickey = %s\n",tempGet.publickey);

		printf("   |=============================================|\n");
	}

	return 1;
}

/*****************************************
* �������ƣ�CacheRouter_CoLoR_ParseData
* ��������������CoLoR-Data��ͷ
* �����б�
* ���ؽ����
*****************************************/
static int
CacheRouter_CoLoR_ParseData
(
	const CoLoR_data_parse * pkg
)
{
	int i;
	
	char* p;
	p = (char*)pkg;
	
	if (NULL == pkg)

	{
		return -1;
	}
	
	//����Data�����ֶ�
	//free(tempData.sid);
	//free(tempData.nid);
	//free(tempData.data);
	
	//tempData.version_type = pkg->version_type;
	//tempData.ttl = pkg->ttl;
	tempData.total_len = pkg->total_len;

	//tempData.port_no = pkg->port_no;
	//tempData.checksum = pkg->checksum;

	tempData.sid_len = pkg->sid_len;
	tempData.nid_len = pkg->nid_len;
	tempData.pid_n = pkg->pid_n;
	//tempData.options_static = pkg->options_static;

	//p += sizeof(uint8_t) * (14 + 12);
	p += sizeof(uint8_t) * (12+16);

	//tempData.signature_algorithm = (uint8_t)(*p);
	//tempData.if_hash_cache = (uint8_t)(*(p+sizeof(uint8_t)));
	//tempData.options_dynamic = (uint16_t)(*(p+sizeof(uint16_t)));

	p += sizeof(uint32_t);

	//tempData.sid = (uint8_t*)calloc(tempData.sid_len,sizeof(uint8_t));
	//memcpy((uint8_t*)tempData.sid,(uint8_t*)p,tempData.sid_len);

	p += sizeof(uint8_t) * tempData.sid_len;

	//tempData.nid = (uint8_t*)calloc(tempData.nid_len,sizeof(uint8_t));
	//memcpy((uint8_t*)tempData.nid,(uint8_t*)p,tempData.nid_len);

	p += sizeof(uint8_t) * (tempData.nid_len/2);

	//tempNID = (uint8_t*)calloc(tempData.nid_len/2,sizeof(uint8_t));
	memset(tempNID,0,NIDLENst);
	memcpy(tempNID,(uint8_t*)p,tempData.nid_len/2);

	//p += sizeof(uint8_t) * tempData.nid_len;
	
	int data_len_int = tempData.total_len - 16 - 16 - tempData.sid_len - tempData.nid_len - tempData.pid_n*4 - 16;

	//������ڲа���ǡ���ܳ����ֶλ����������ֶ���ֵ�����˴�����û��У�飬���п��ܳ���data_lenֵΪ��ֵ���������������Ҫ�ų���
	if(data_len_int <= 0 || data_len_int > DATALENst)
	{
		if(netDEVETESTIMPL <= 2)
			printf("data_len == %d (break out)\n",data_len_int);
		return -1;
	}

	//uint16_t data_len = tempData.total_len - sizeof(uint8_t)*(16+16+16) - tempData.sid_len - tempData.nid_len - tempData.pid_n*sizeof(uint8_t)*4;
	//tempData.data = (uint8_t*)calloc(data_len+1/*+1��������Ϊ�������ر�Ԥ��*/,sizeof(uint8_t));
	//memcpy((uint8_t*)tempData.data,(uint8_t*)p,data_len);
	//*(tempData.data+data_len) = '\0';//�����ΪʲôΨ��data����Ҫ�ر��ڳ�1Byte��Ϊ�����������Խ���ȡ

	//p += sizeof(uint8_t) * data_len;

/*
	for(i=0;i<16;i++)
	{
		tempData.data_signature[i] = (uint8_t)(*(p+sizeof(uint8_t)*i));

	}
*/

	//�������
	if(0)
	{
		p = (char*)pkg;

		printf (">>>CoLoR-Data Received.\n");

		printf ("   |====================Datapkg==================|\n");

		printf("   |version_type   = %d\n",tempData.version_type);
		printf("   |ttl            = %d\n",tempData.ttl);
		printf("   |total_len      = %d\n",tempData.total_len);

		printf("   |port_no        = %d\n",tempData.port_no);
		printf("   |checksum       = %d\n",tempData.checksum);

		printf("   |sid_len        = %d\n",tempData.sid_len);
		printf("   |nid_len        = %d\n",tempData.nid_len);
		printf("   |pid_n          = %d\n",tempData.pid_n);
		printf("   |options_static = %d\n",tempData.options_static);

		//p += sizeof(uint8_t) * (14 + 12);
		p += sizeof(uint8_t) * (12+16);

		//printf("   |signature_algorithm = %d\n",tempData.signature_algorithm);
		//printf("   |if_hash_cache = %d\n",tempData.if_hash_cache);
		//printf("   |options_dynamic = %d\n",tempData.options_dynamic);

		p += sizeof(uint32_t);

		//printf("   |sid = %s\n",tempData.sid);

		p += sizeof(uint8_t) * tempData.sid_len;

		//printf("   |nid = %s\n",tempData.nid);

		p += sizeof(uint8_t) * (tempData.nid_len/2);

		//printf("   |nid = %s\n",tempNID);

		//p += sizeof(uint8_t) * tempData.nid_len;

		//printf("   |data = %s\n",tempData.data);

		//p += sizeof(uint8_t) * data_len;

		//printf("   |data_signature = %s\n",tempData.data_signature);
		printf("   |=============================================|\n");
	}
	
	return 1;
}

/*****************************************
* �������ƣ�CacheRouter_CoLoR_ParseRegister
* ��������������CoLoR-Register��ͷ
* �����б�
* ���ؽ����
*****************************************/
static int
CacheRouter_CoLoR_ParseRegister
(
	const CoLoR_register_parse * pkg
)
{
	int i;
	
	char* p;
	p = (char*)pkg;
	
	if (NULL == pkg)
	{
		return -1;
	}
	
	//����Register�����ֶ�
	tempRegister.total_len = pkg->total_len;

	tempRegister.nid_len = pkg->nid_len;

	p += sizeof(uint8_t) * (16+tempRegister.nid_len/2);

	//tempNID = (uint8_t*)calloc(tempRegister.nid_len/2,sizeof(uint8_t));
	memset(tempNID,0,NIDLENst);
	memcpy(tempNID,(uint8_t*)p,tempRegister.nid_len/2);
	
	return 0;
}

/*****************************************
* �������ƣ�CacheRouter_FindDest
* ����������Ѱ�����ݰ��е�NID�ֶΣ��ж���ת������
* �����б�
* ���ؽ����
*****************************************/
int
CacheRouter_FindDest
(
	const CoLoR_VersionType * pkg
)
{
	int infolen = -1;
	
	if(pkg->version_type == 160)//����ʾIP�汾��Ϊ10��������Ϊ0��ר��CoLoR-Get��  �ֶ�Ϊ1010 0000���İ�
	{
		if(netDEVETESTIMPL <= 1)
			printf("[%s] This is a GET pkt.\n",SwitchDevice_local_ip);

		infolen = CacheRouter_CoLoR_ParseGet((CoLoR_get_parse*)pkg);
		if(infolen <= 0)
			return -1;
		
		RecvBUFLength = tempGet.total_len;
	}
	else if(pkg->version_type == 161)//����ʾIP�汾��Ϊ10��������Ϊ1��ר��CoLoR-Data�� �ֶ�Ϊ1010 0001���İ�
	{
		if(netDEVETESTIMPL <= 1)
			printf("[%s] This is a DATA pkt.\n",SwitchDevice_local_ip);

		infolen = CacheRouter_CoLoR_ParseData((CoLoR_data_parse*)pkg);
		if(infolen <= 0)
			return -1;

		RecvBUFLength = tempData.total_len;
	}
	else if(pkg->version_type == 162)//����ʾIP�汾��Ϊ10��������Ϊ2��ר��CoLoR-Register�� �ֶ�Ϊ1010 0010���İ�
	{
		if(netDEVETESTIMPL <= 1)
			printf("[%s] This is a REGISTER pkt.\n",SwitchDevice_local_ip);

		infolen = CacheRouter_CoLoR_ParseRegister((CoLoR_register_parse*)pkg);
		RecvBUFLength = tempRegister.total_len;
	}
	else
		printf("[%s] [ Recv WRONG ] version_type == %d\n",SwitchDevice_local_ip,pkg->version_type);

	if(netDEVETESTIMPL <= 1)
		printf("[%s] [ Find ] NID == %s\n",SwitchDevice_local_ip,tempNID);

	return infolen;
}

/*******************************************************************************************************************************************
**********************************************************���߳����ɳ���********************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�CacheRouter_thread_recv
* ��������������������������PhysicalPortģ������ݰ�
* �����б�
* ���ؽ����
*****************************************/
void *
CacheRouter_thread_recv
(
	void * fd
)
{
	char sourceip[16]={0};
	char sourcepp[30]={0};
	char destpp[30]={0};

	int sourcefound=0;
	int destfound=0;
	int portfound=0;

	int i,j;
	
	int socket_receiver;
	socket_receiver=socket(AF_INET,SOCK_DGRAM,0);

	struct sockaddr_in sin;
	int sockaddr_len=sizeof(struct sockaddr_in);
	bzero(&sin,sockaddr_len);
	sin.sin_family=AF_INET;
	//sin.sin_addr.s_addr=htonl(INADDR_ANY);
	sin.sin_addr.s_addr=inet_addr(SwitchDevice_local_ip);
	sin.sin_port=htons(localPORT);
	
	bind(socket_receiver,(struct sockaddr *)&sin,sockaddr_len);
	
	printf("UDP listening start...\n");

	while(1)
	{
		RecvBUFLength = 0;
		memset(RecvBUF,0,BUFSIZE);

		if(INTRACOMMUNICATION == 0)
			recvfrom(socket_receiver,RecvBUF,sizeof(RecvBUF),0,(struct sockaddr *)&sin,&sockaddr_len);
		else if(INTRACOMMUNICATION == 1)
		{
			sem_wait(&semPacketRECV);
			//printf("[Forwarding Plane]semPacketRECV triggered.\n");
/*
//������ת���������
typedef struct _PkgCacheUnit PkgCacheUnit;
struct _PkgCacheUnit
{
	int flag_occupied;
	int pkglength;
	uint8_t sourcepp[30];
	uint8_t pkg[MTU];
}PkgBUF[10];
int PkgBUFHead;
int PkgBUFTail;
*/
			pthread_mutex_lock(&lockPkgBUF);
			for(;;)
			{
				if(PkgBUF[PkgBUFTail++].flag_occupied == 1)
				{
					LocalPkgBUFTail = PkgBUFTail-1;

					RecvBUFLength = PkgBUF[LocalPkgBUFTail].pkglength;
					memcpy(RecvBUF,"NET",3);
					memcpy(RecvBUF+3,PkgBUF[LocalPkgBUFTail].pkg,RecvBUFLength);
					strcpy(sourcepp,PkgBUF[LocalPkgBUFTail].sourcepp);

					if(PkgBUFTail>=10)
						PkgBUFTail -= 10;
					break;
				}
				
				if(PkgBUFTail>=10)
					PkgBUFTail -= 10;
			}
			//printf("[Forwarding Plane]pkg found.\n");
			pthread_mutex_unlock(&lockPkgBUF);
		}


		//usleep(7500);
		//��һʱ����CacheRouterģ��ת��ƽ�����Ƿ����������ݰ�ռ�ã�����У�ֱ�Ӷ��������û�У�����ռ��
		if(CacheRouter_PlaneOccupy == 1)
		{
			if(netDEVETESTIMPL <= 0)
				printf("CacheRouter ForwardingPlane Occupied\n");
			continue;
		}
		else
		{
			CacheRouter_PlaneOccupy = 1;
		}

		//Main Forwarding Procession with Routing Table
		if(strncmp(RecvBUF,"NET",3) == 0)
		{
			if(SEETIME >=1)
			{
				gettimeofday(&starttime,0);
				timefornow = 0;
			}
			
			if(netDEVETESTIMPL <= 1)
			{
				printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
				//printf("[%s] Got a NET message:%s\n",SwitchDevice_local_ip,RecvBUF);
				printf("[%s] Got a NET message.\n",SwitchDevice_local_ip);
			}
			
			bzero(sourceip,16);
			bzero(sourcepp,30);
			bzero(destpp,30);

			strcpy(sourceip,(const char*)inet_ntoa(sin.sin_addr));
			
			if(netDEVETESTIMPL <= 1)
				printf("[%s] [Source] IP == %s\n",SwitchDevice_local_ip,sourceip);
			
			/*
			//Search for the Source Physical Port
			sourcefound=0;
			for(j=0;j<=ppiplistcount;j++)

			{
				if(strncmp(sourceip,ppiplist[j].ip,16) == 0)

				{
					memcpy(sourcepp,ppiplist[j].pp,16);

					sourcefound=1;
					break;

				}
			}
			if(sourcefound==0)
			{

				printf("Do not found the proper source port to match the routingtable.\n");

				//��CacheRouterģ��ת��ƽ����ռ��
				CacheRouter_PlaneOccupy = 0;
				if(netDEVETESTIMPL <= 0)
					printf("[%s] CacheRouter_PlaneOccupy == %d, released.\n",SwitchDevice_local_ip,CacheRouter_PlaneOccupy);

				continue;

			}
			printf("[Source] PhysicalPort == %s\n",sourcepp);
			*/

			if(SEETIME >=1)
			{
				gettimeofday(&forwardingtime,0);
				timefornow = 1000000*(forwardingtime.tv_sec - starttime.tv_sec) + forwardingtime.tv_usec - starttime.tv_usec;
				printf("%lf microsecond: initial parameter.\n",timefornow);
			}
			
			//Find the dest value in the "NET" packet
			int JudgeByte = -1;
			JudgeByte = CacheRouter_FindDest((CoLoR_VersionType*)(RecvBUF+3));

			//�յ���CoLoR������ݰ���а�
			if(JudgeByte <= 0)
			{
				//��CacheRouterģ��ת��ƽ����ռ��
				CacheRouter_PlaneOccupy = 0;
				if(netDEVETESTIMPL <= 0)
					printf("[%s] CacheRouter_PlaneOccupy == %d, released.(Bad Pakcet)\n",SwitchDevice_local_ip,CacheRouter_PlaneOccupy);

				continue;
			}
			
			if(SEETIME >=1)
			{
				gettimeofday(&forwardingtime,0);
				timefornow = 1000000*(forwardingtime.tv_sec - starttime.tv_sec) + forwardingtime.tv_usec - starttime.tv_usec;
				printf("%lf microsecond: Find the dest value in the packet.\n",timefornow);
			}

			//Search for the Location of Routing Table Event
			destfound=0;
			i=0;
			while(destfound==0)
			{
				if(strncmp(routingtable[i++].dest,tempNID,8) == 0)
				{
					destfound=1;
					i--;
				}
				if(i==routingtablecount+1)
				{
					break;
				}
			}
			memcpy(destpp,routingtable[i].port,30);

			if(SEETIME >=1)
			{
				gettimeofday(&forwardingtime,0);
				timefornow = 1000000*(forwardingtime.tv_sec - starttime.tv_sec) + forwardingtime.tv_usec - starttime.tv_usec;
				printf("%lf microsecond: Search for the Location of Routing Table Event.\n",timefornow);
			}

			if(netDEVETESTIMPL <= 1)
				printf("[%s] [ Dest ] PhysicalPort == %s\n",SwitchDevice_local_ip,destpp);
			
			//Search for the Dest IP address (UDP)
			if(destfound==1)
			{
				portfound=0;
				for(j=0;j<=ppiplistcount;j++)
				{
					if(strncmp(routingtable[i].port,ppiplist[j].pp,30) == 0)
					{
						memcpy(CacheRouter_dest_ip,ppiplist[j].ip,16);
						portfound=1;

						//�����ջ��������ݰ����������ͻ�����
						SendBUFLength = RecvBUFLength;
						memcpy(SendBUF,RecvBUF,BUFSIZE);
						
						if(INTRACOMMUNICATION == 0)
						{
							//����socket
							//�׽ӿ�������
							int socket_NETUtoNETD;
							socket_NETUtoNETD = socket(AF_INET,SOCK_DGRAM,0);

							struct sockaddr_in addrNtoS;
							bzero(&addrNtoS,sizeof(addrNtoS));
							addrNtoS.sin_family=AF_INET;
							addrNtoS.sin_port=htons(NETUtoNETD);
							//unsigned long IPto = 2130706433;//�ػ���ַ���� == 2130706433
							//addrNtoS.sin_addr.s_addr=htonl(IPto);//htonl�������ֽ���ת��Ϊ�����ֽ���
							addrNtoS.sin_addr.s_addr=inet_addr(LOOPIP);//htonl�������ֽ���ת��Ϊ�����ֽ���
						
							//�������򴥷�ָ��
							sendto(socket_NETUtoNETD,"NTOS",4,0,(struct sockaddr *)&addrNtoS,sizeof(addrNtoS));
							close(socket_NETUtoNETD);
						}
						else if(INTRACOMMUNICATION == 1)
						{
							sem_post(&semNTOS);
						}
						
						if(netDEVETESTIMPL <= 6)
							printf("[%s] NTOS sent.\n",SwitchDevice_local_ip);
						
						break;
					}
				}
				if(portfound==0)
				{
					if(netDEVETESTIMPL <= 2)
						printf("[%s] Do not found the proper port to forward message.\n",SwitchDevice_local_ip);

					//��CacheRouterģ��ת��ƽ����ռ��
					CacheRouter_PlaneOccupy = 0;
					if(netDEVETESTIMPL <= 0)
						printf("[%s] CacheRouter_PlaneOccupy == %d, released.(Do Not Found Port)\n",SwitchDevice_local_ip,CacheRouter_PlaneOccupy);

					continue;
				}
			}
			else
			{
				if(netDEVETESTIMPL <= 2)
					printf("[%s] Do not found the proper routing table event to forward message.\n",SwitchDevice_local_ip);

				//��CacheRouterģ��ת��ƽ����ռ��
				CacheRouter_PlaneOccupy = 0;
				if(netDEVETESTIMPL <= 0)
					printf("[%s] CacheRouter_PlaneOccupy == %d, released.(Do Not Found Routing)\n",SwitchDevice_local_ip,CacheRouter_PlaneOccupy);

				continue;//Print the content of message or not? "continue" for "No"
			}

			if(netDEVETESTIMPL <= 1)
				printf("[%s] [ Dest ] IP == %s\n",SwitchDevice_local_ip,CacheRouter_dest_ip);

			if(SEETIME >=1)
			{
				gettimeofday(&forwardingtime,0);
				timefornow = 1000000*(forwardingtime.tv_sec - starttime.tv_sec) + forwardingtime.tv_usec - starttime.tv_usec;
				printf("%lf microsecond: Search for the Dest IP address (UDP) and set ON-Signal for SEND thread.\n",timefornow);
			}
		}

		if(netDEVETESTIMPL <= -1)
		{
			printf("The Content of Packet NTOS:\n");
			int dd=0;
			char c;
			printf("\n");
			while(dd<100)
			{
				c = *(RecvBUF+3+dd);
				printf("%d",c/128);

				c%=128;
				printf("%d",c/64);

				c%=64;
				printf("%d",c/32);

				c%=32;
				printf("%d",c/16);

				c%=16;
				printf("%d",c/8);

				c%=8;
				printf("%d",c/4);

				c%=4;
				printf("%d",c/2);

				c%=2;
				printf("%d",c);

				printf(" ");
				dd++;
			}
			printf("\n\n");
		}
	}

	//�ر��߳�
	//exit(0);
	close(socket_receiver);
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�CacheRouter_thread_send
* ��������������������PhysicalPortģ�鷢�����ݰ�
* �����б�
* ���ؽ����
*****************************************/
void *
CacheRouter_thread_send
(
	void * fd
)
{
	int i;
	
	int socket_recvNtoSSignal;
	socket_recvNtoSSignal = socket(AF_INET,SOCK_DGRAM,0);

	struct sockaddr_in addrSignal;
	int sockaddr_len=sizeof(struct sockaddr_in);
	bzero(&addrSignal,sockaddr_len);
	addrSignal.sin_family=AF_INET;
	//addrSignal.sin_addr.s_addr=htonl(INADDR_ANY);
	addrSignal.sin_addr.s_addr=inet_addr(LOOPIP);
	addrSignal.sin_port=htons(NETUtoNETD);

	bind(socket_recvNtoSSignal,(struct sockaddr *)&addrSignal,sockaddr_len);
	
	char signal[10];

	char sendbuf[BUFSIZE];
	
	int socket_sender;
	socket_sender=socket(AF_INET,SOCK_DGRAM,0);
	
	int portnum;
	while(1)
	{
		memset(signal,0,10);
		memset(sendbuf,0,BUFSIZE);
		
		if(INTRACOMMUNICATION == 0)
			recvfrom(socket_recvNtoSSignal,signal,sizeof(signal),0,(struct sockaddr *)&addrSignal,&sockaddr_len);
		else if(INTRACOMMUNICATION == 1)
		{
			sem_wait(&semNTOS);
			memcpy(signal,"NTOS",4);
		}

		if(strncmp(signal,"NTOS",4) != 0)
		{
			//��CacheRouterģ��ת��ƽ����ռ��
			CacheRouter_PlaneOccupy = 0;
			if(netDEVETESTIMPL <= 6)
				printf("[%s] CacheRouter_PlaneOccupy == %d, released.(NOT NTOS)\n",SwitchDevice_local_ip,CacheRouter_PlaneOccupy);

			continue;
		}

		if(SEETIME >=1)
		{
			gettimeofday(&forwardingtime,0);
			timefornow = 1000000*(forwardingtime.tv_sec - starttime.tv_sec) + forwardingtime.tv_usec - starttime.tv_usec;
			printf("%lf microsecond: Prepare socket for SEND thread.\n",timefornow);
		}

		if(INTRACOMMUNICATION == 0)
		{
			struct sockaddr_in addrFrom;
			bzero(&addrFrom,sizeof(addrFrom));
			addrFrom.sin_family=AF_INET;
			//addrFrom.sin_addr.s_addr=htonl(INADDR_ANY);
			addrFrom.sin_addr.s_addr=inet_addr(SwitchDevice_local_ip);
			addrFrom.sin_port=htons(0);

			bind(socket_sender,(struct sockaddr *)&addrFrom,sizeof(addrFrom));

			memcpy(sendbuf,"FOR",3);
			memcpy(sendbuf+3,SendBUF+3,SendBUFLength);
		
			struct sockaddr_in addrTo;
			bzero(&addrTo,sizeof(addrTo));
			addrTo.sin_family=AF_INET;
			addrTo.sin_port=htons(destPORT);
			addrTo.sin_addr.s_addr=inet_addr(CacheRouter_dest_ip);
		
			if(SEETIME >=1)
			{
				gettimeofday(&forwardingtime,0);
				timefornow = 1000000*(forwardingtime.tv_sec - starttime.tv_sec) + forwardingtime.tv_usec - starttime.tv_usec;
				printf("%lf microsecond: Preparation of socket succeed.\n",timefornow);
			}

			if(netDEVETESTIMPL <= 1)
			{
				//printf("[%s] Message has been sent out:%s\n",SwitchDevice_local_ip,sendbuf);
				printf("[%s] Message has been sent out.\n",SwitchDevice_local_ip);
			}
		
			sendto(socket_sender,sendbuf,SendBUFLength+3,0,(struct sockaddr *)&addrTo,sizeof(addrTo));
		}
		else if(INTRACOMMUNICATION == 1)
		{
			for(i=15;i>=0;i--)
			{
				if(CacheRouter_dest_ip[i] == '.')
				{
					portnum = CacheRouter_dest_ip[i+2] - '0';
					PkgBUF[LocalPkgBUFTail].destppnum = portnum;
					sem_post(&semPacketSEND[portnum]);
					break;
				}
			}
		}

		//��CacheRouterģ��ת��ƽ����ռ��
		CacheRouter_PlaneOccupy = 0;
		if(netDEVETESTIMPL <= 6)
			printf("[%s] CacheRouter_PlaneOccupy == %d, released.(NTOS recv)\n",SwitchDevice_local_ip,CacheRouter_PlaneOccupy);
	}

	//�ر��߳�
	//exit(0);
	close(socket_sender);
	free(fd);
	pthread_exit(NULL);
}


/*****************************************
* �������ƣ�CacheRouter_main
* ����������CacheRouterģ�������������������̣߳������ṩʵ�ʹ���
* �����б�
* ���ؽ����
*****************************************/
//int main(int argc,char *argv[])
int
CacheRouter_main
(
	int argc,char argv[][30]
)
{
	CacheRouter_Parameterinit();
	
	int i,j,k,l;
	
	printf("argc == %d\n",argc);
	for(i=0;i<argc;i++)
	{
		printf("argv[%d] == %s\n",i,argv[i]);
	}

	strcpy(NID,argv[1]);
	
	int ii;
	int numfound=0;
	char port[30];
	memset(port,0,30);
	char portnum[3]={0};
	for(i=2;i<argc;i++)
	{
		numfound=0;
		memset(port,0,30);
		memset(portnum,0,3);

		//��ȡ������������ĩβ���
		strcpy(port,argv[i]);

		for(ii=29;ii>=0;ii--)
		{
			if(port[ii] >= '0' && port[ii] <= '9' && numfound == 0)
			{
				numfound=1;
				portnum[1]=port[ii];
				continue;
			}
			if(port[ii] >= '0' && port[ii] <= '9' && numfound == 1)
			{
				numfound=2;
				portnum[0]=port[ii];
			}
			else if(numfound == 1)
			{
				portnum[0] = portnum[1];
				portnum[1]=0;
				break;
			}
			if(numfound==2)
				break;
		}

		//��������˿����������ڲ�ͨ��IP��ӳ�����
		strcpy(ppiplist[i-2].pp,port);
	
		strcpy(ppiplist[i-2].ip,SwitchDevice_local_ip);
		for(ii=0;ii<16;ii++)
		{
			if(ppiplist[i-2].ip[ii] == 0)
			{
				strcpy(ppiplist[i-2].ip+ii,portnum);
				break;
			}
		}
		ppiplistcount++;
	}

	//ȷ�������ļ�·��
	if(GLOBALTEST >= 1)
	{
		memset(FILERTname,0,FILEnameL);

		memset(domainID,0,10);
		memset(deviceNA,0,30);
		memset(deviceID,0,30);

		strcpy(domainID,NID);
		strcpy(deviceNA,NID);
		strcpy(deviceID,NID);

		int sliceflag=0;

		//��ȡ����
		for(i=0;i<30;i++)
		{
			if(NID[i]==0)
				break;
			if(((NID[i] >= 'A' && NID[i] <= 'Z') || (NID[i] >= 'a' && NID[i] <= 'z')) && sliceflag == 0)
			{
				domainID[i]=0;
				deviceNA[i]=0;
				deviceID[i]=0;
printf("NID[%d] == %c, domainID[%d] == %c, deviceNA[%d] == %c, deviceID[%d] == %c\n",i,NID[i],i,domainID[i],i,deviceNA[i],i,deviceID[i]);
				continue;
			}
			else if(NID[i] >= '0' && NID[i] <= '9' && sliceflag == 0)
			{
				sliceflag=1;
				deviceNA[i]=0;
				deviceID[i]=0;
printf("NID[%d] == %c, domainID[%d] == %c, deviceNA[%d] == %c, deviceID[%d] == %c\n",i,NID[i],i,domainID[i],i,deviceNA[i],i,deviceID[i]);
				continue;
			}
			else if(sliceflag == 0)
			{
				printf("Input wrong!\n");
				exit(0);
			}

			if(NID[i] >= '0' && NID[i] <= '9' && sliceflag == 1)
			{
				deviceNA[i]=0;
				deviceID[i]=0;
printf("NID[%d] == %c, domainID[%d] == %c, deviceNA[%d] == %c, deviceID[%d] == %c\n",i,NID[i],i,domainID[i],i,deviceNA[i],i,deviceID[i]);
				continue;
			}
			else if(((NID[i] >= 'A' && NID[i] <= 'Z') || (NID[i] >= 'a' && NID[i] <= 'z')) && sliceflag == 1)
			{
				sliceflag=2;
				memset(domainID+i,0,10-i);
				deviceID[i]=0;
printf("NID[%d] == %c, domainID[%d] == %c, deviceNA[%d] == %c, deviceID[%d] == %c\n",i,NID[i],i,domainID[i],i,deviceNA[i],i,deviceID[i]);
				continue;
			}
			else if(sliceflag == 1)
			{
				printf("Input wrong!\n");
				exit(0);
			}

			if(((NID[i] >= 'A' && NID[i] <= 'Z') || (NID[i] >= 'a' && NID[i] <= 'z')) && sliceflag == 2)
			{
				deviceID[i]=0;
printf("NID[%d] == %c, domainID[%d] == %c, deviceNA[%d] == %c, deviceID[%d] == %c\n",i,NID[i],i,domainID[i],i,deviceNA[i],i,deviceID[i]);
				continue;
			}
			else if(NID[i] >= '0' && NID[i] <= '9' && sliceflag == 2)
			{
				sliceflag=3;
				memset(deviceNA+i,0,30-i);
printf("NID[%d] == %c, domainID[%d] == %c, deviceNA[%d] == %c, deviceID[%d] == %c\n",i,NID[i],i,domainID[i],i,deviceNA[i],i,deviceID[i]);
				continue;
			}
			else if(sliceflag == 2)
			{
				memset(deviceID,0,30);
			}
			if((NID[i] < '0' || NID[i] > '9') && sliceflag == 3)
			{
				memset(deviceID+i,0,30-i);
				break;
			}
		}

		int founddomainID=0;
		int founddeviceNA=0;
		int founddeviceID=0;

		//����ǰ��
		for(i=0;i<10;i++)
		{
			if(domainID[i]!=0 && founddomainID==0)
			{
				founddomainID=1;
				for(j=0;j<10;j++)
					domainID[j] = domainID[j+i];
				break;
			}
		}
		for(i=0;i<30;i++)
		{
			if(deviceNA[i]!=0 && founddeviceNA==0)
			{
				founddeviceNA=1;
				for(j=0;j<30;j++)
					deviceNA[j] = deviceNA[j+i];
				break;
			}
		}
		for(i=0;i<30;i++)
		{
			if(deviceID[i]!=0 && founddeviceID==0)
			{
				founddeviceID=1;
				for(j=0;j<30;j++)
					deviceID[j] = deviceID[j+i];
				break;
			}
		}

		printf("domainID == %s\n",domainID);
		printf("deviceNA == %s\n",deviceNA);
		printf("deviceID == %s\n",deviceID);

		//�����ļ�·��
		memset(FILERTname,0,FILEnameL);
		configure_RTfilepath(FILERTname,domainID,deviceNA,deviceID);
	}

	FILE *fp;
	char ch=0;
	int file_i=0;
	
	//�ļ�1����ȡ·�ɱ������ļ�
	char RTbuf[RTLENTH];
	char RTcmd[RTLENTH/70][70];

	if((fp=fopen(FILERTname,"r"))==NULL)
	{
		printf("Cannot open routingtable file!\n");
	}
	else
	{
		file_i = 0;
		while ((ch=fgetc(fp))!=EOF)
			RTbuf[file_i++]=ch;
		if(file_i == 0)
		{
			fclose(fp);
			return 0;
		}
		RTbuf[--file_i] = '\0';
		file_i = 0;
		fclose(fp);

		//Ϊ�����������
		i=0;j=0;k=0;l=0;
		while(1)
		{
			if(RTbuf[i] == 10 || RTbuf[i] == 0)
			{
				l=0;
				while(j<i)
				{
					RTcmd[k][l++] = RTbuf[j++];
				}
				RTcmd[k][l] = 0;

				j++;
				k++;
			}
			if(RTbuf[i] == 0)
			{
				break;
			}
			i++;
		}

		//i,j==totallength; k==numofcmds;
		//���н�����������
		//uint8_t sdest_mac[18]={0};
		//int dMACno=0;
		int RTdest=0;
		int RTport=0;
		for(i=0,j=0;i<k;j=0,i++)
		{
			routingtablecount++;
		
			//dest flag input
			for(RTdest=0;RTcmd[i][j] != ' ' && RTcmd[i][j] != 0;RTdest++,j++)
				routingtable[routingtablecount].dest[RTdest] = RTcmd[i][j];
			routingtable[routingtablecount].dest[RTdest] = 0;
		
			j++;
		
			//combined physical port input
			for(RTport=0;RTcmd[i][j] != ' ' && RTcmd[i][j] != 0;RTport++,j++)
				routingtable[routingtablecount].port[RTport] = RTcmd[i][j];
			routingtable[routingtablecount].port[RTport] = 0;
		}
	}
	
	//ȷ��������ý��
	printf("[FILEroutingtable]\n%s\n",RTbuf);
	printf("[input]SwitchDevice_local_ip  == %s\n",SwitchDevice_local_ip);
	
	
	//����recv���߳�
	pthread_t pthread_recv;
	if(pthread_create(&pthread_recv, NULL, CacheRouter_thread_recv, NULL)!=0)
	{
		perror("Creation of recv thread failed.");
	}
	
	//����send���߳�
	pthread_t pthread_send;
	if(pthread_create(&pthread_send, NULL, CacheRouter_thread_send, NULL)!=0)
	{
		perror("Creation of send thread failed.");
	}

	//����Ϊphysicalportģ�������ͨ�Ų�����������
	struct physicalportinput
	{
		char pp[30];
		char ip[16];
	}ppinput[100];

	pthread_t pthread_physicalport[100];

	for(i=2;i<argc;i++)
	{
		memset(ppinput[i].pp,0,30);
		strcpy(ppinput[i].pp,argv[i]);
		strcpy(ppinput[i].ip,SwitchDevice_local_ip);

		//printf("[SEND]ppinput[%d].pp == %s\n",i,ppinput[i].pp);
		
		//����physicalport���߳�
		if(pthread_create(&pthread_physicalport[i], NULL, thread_physicalport, (void *)&ppinput[i])!=0)
		{
			perror("Creation of physicalport thread failed.");
		}
	}
	
	//ά�����̵߳����У�û��ʵ�ʹ���
	while(1)
	{
		//�����ܴ�ؼ�СCPUռ��
		sleep(10000);
	}
	
	exit(0);
	return (EXIT_SUCCESS);
}


