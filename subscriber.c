/*******************************************************************************************************************************************
* �ļ�����subscriber.c
* �ļ�������973�ǻ�Эͬ����SARϵͳTestBed�׼�������������ˣ�Data Subscriber�����������֧��+GET����װ����+DATA�����ս���
* ��������ģ���������������Ϊ������ͨ��ѧ��һ�������������豸���ҹ���ʵ���������������̼�����ѵר�á������������ڴ�֣�����������ļ�������������ѵ��ģ������Ϊ���˾����뾭��Ĳ������Դ�Ĵ�����ʽ��Ϊ���߱��������˽���һ��һ���ڣ��κθ��˲����������ת����չʾ����Դ���ݡ��κ��������ѧԱ����������������ֱ����Ҫ��������������Ҫ����֮Ȩ������������Щ��Դ�����漰ʵ������Ŀ���ܣ��ݲ���������ר���������������Ȩ�����ˣ��ʸ�����Դ����ʵ�����ڲ�һ��һ���ţ��緢���й������߱��˵�һƬ�������Ϊ�����߽���������ԭ���Խ���׷�顢��֤�����ߺ������Ȩ��������Դ�ķ���Ȩ�����߱������У���������ܽ���̽�͸�޳����׵��ȳ���Ϊ���ѧҵ���ౡ֮������ϣ�ԸѧԱ�������ߵ��Ͷ��ɹ���лл������
* ���ߣ�������
* ��ݣ�������ͨ��ѧ��һ�������������豸���ҹ���ʵ���� 2013��˶�������о���
* E-mail. hellozxwang@foxmail.com
* Mobile. 18811774990
* QQ    . 535667240
* Addr  . �����к�������ֱ���ⱱ����ͨ��ѧ��е¥D706��, 100044
*******************************************************************************************************************************************/
/*******************************************************************************************************************************************
*****����˵����1.���������ڷ��Ͱ�������SID��GET���Ͱ�
**************2.�������������ڼ�����SAR/CoLoR�������ݰ���
**************3.�ӽ��յ���DATA������ȡSID��Ӧ��Data
**************4.������HTTPЭ��Խ��������ʵ�����ݵ�����ͻ���
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
gcc subscriber.c -o subscriber -lpthread
4�����У����漰ԭʼ�׽��ֵ�ʹ�ã���rootȨ�ޣ�
sudo ./subscriber
*/

#include"subscriber.h"

/*******************************************************************************************************************************************
*************************************�궨����������************ȫ�ֱ�������************����ʽ����*********************************************
*******************************************************************************************************************************************/

//����ӳ���
PPIP ppiplist[48];

//Ethernet�� / ����CoLoR�����жϵİ�ͷ����ֹ���̶��ײ�������Version/Type�ֶ�ΪGet����Data����Register����ͨ�ã�
Ether_CoLoR_VersionType tempVersionType;

//Ethernet�� / CoLoRЭ��Get���ײ���PID֮ǰ���ֶγ��ȹ̶������ڷ�װ

//Ethernet�� / CoLoRЭ��Get���ײ���PID֮ǰ���ֶγ��ȿɱ䣬���ڽ���
CoLoR_get_parse tempGet;

//Ethernet�� / CoLoRЭ��Data���ײ���PID֮ǰ���ֶγ��ȹ̶������ڷ�װ

//Ethernet�� / CoLoRЭ��Data���ײ���PID֮ǰ���ֶγ��ȿɱ䣬���ڽ���
CoLoR_data_parse tempData;

//Ethernet�� / CoLoRЭ��Register���ײ���PID֮ǰ��
CoLoR_register_parse tempRegister;

/*******************************************************************************************************************************************
*******************************************��ʼ�������������********************************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�Subscriber_Parameterinit
* ����������Subscriberģ��ȫ�ֱ�������ֵ
* �����б�
* ���ؽ����
*****************************************/
void
Subscriber_Parameterinit()
{
	memset(PhysicalPort,0,30);//�����˿�

	//����������Ϣ
	memset(SwitchDevice_local_ip,0,16);
	memcpy(SwitchDevice_local_ip,"127.1.1.1",9);//����ĸ�ֵ����Ч���ŵ����������¶˵�λ�þ���Ч��ͬ���ĳ�ʼ�������RM����������⣬���
	memset(Subscriber_dest_ip,0,16);

	//���ջ�����
	RecvBUFLength = 0;
	memset(RecvBUF,0,BUFSIZE);
	//���ͻ�����
	SendBUFLength = 0;
	memset(SendBUF,0,BUFSIZE);
	//�������б������ʼ��
	PkgBUFHead=0;
	PkgBUFTail=0;

	//ȫ�ֱ���
	//ר���ڸ����ַ�����ʽ�����м��ݴ����
	memset(temp,0,100);
	memset(tempdata,0,DATALEN);
	memset(tempSID,0,SIDLEN);
	//��֪��������������HTTP����������ú���֮�����ֵ����
	memset(SID,0,SIDLEN);

	Subscriber_selfpacketdonotcatch=0;

	memset(local_mac,0,7);
	memset(local_ip,0,5);
	memset(dest_ip,0,16);

	uint8_t bm1[7]={0xff,0xff,0xff,0xff,0xff,0xff,0x00};
	memcpy(broad_mac1,bm1,7);
	uint8_t bm2[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	memcpy(broad_mac2,bm2,7);

	memset(destmac,0,7);
	memset(localmac,0,7);

	//uint8_t sn[NIDLEN] = {'d','2','s','u','b','1',0,0,'d','2','r','m',0,0,0,0};
	//memcpy(sendnid,sn,NIDLEN);
	memset(sendnid,0,NIDLEN);
}

/*******************************************************************************************************************************************
*******************************************Ӧ�ò������**************************************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.������������ To.Ӧ�ò������*******************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�Subscriber_CoLoR_ParsePacket
* ����������CoLoR����������
* �����б�
* ���ؽ����
*****************************************/
int
Subscriber_CoLoR_ParsePacket
(
	const uint8_t * Packet,
	uint8_t * info
)
{	
	int PacketType = -1;
	int infolen = -1;
	
	CoLoR_VersionType * pkgvt = 0;
	CoLoR_get * pkgget = 0;
	CoLoR_data * pkgdata = 0;
	CoLoR_register * pkgregister = 0;
	
	//�ж�CoLoR���ݰ�����
	pkgvt = (CoLoR_VersionType *)Packet;

	PacketType = Subscriber_CoLoR_SeeType(pkgvt);
	
	if(PacketType == 0)//�յ�Get��
	{
		pkgget  = (CoLoR_get *)Packet;
		infolen = Subscriber_CoLoR_ParseGet(pkgget,info);
	}
	else if(PacketType == 1)//�յ�Data��
	{
		pkgdata  = (CoLoR_data *)Packet;
		infolen = Subscriber_CoLoR_ParseData(pkgdata,info);
		if(infolen <= 0)
			return -1;
	}
	else if(PacketType == 2)//�յ�Register��
	{
		pkgregister  = (CoLoR_register *)Packet;
		infolen = Subscriber_CoLoR_ParseRegister(pkgregister,info);
	}
	else//�����Ͳ�����CoLoRЭ��
	{
	}
	
	return infolen;
}

/*****************************************
* �������ƣ�Subscriber_CoLoR_SeeType
* �����������ж��Ƿ�Ϊ��Ϊ����CoLoR��ͷ
* �����б�
* ���ؽ����
*****************************************/
int
Subscriber_CoLoR_SeeType
(
	const CoLoR_VersionType * pkg
)
{
	int i;
	struct protoent *pstIpProto = NULL;
	
	if (NULL == pkg)
	{
		return -1;
	}
	
	//�汾��Э������
	//int version = pkg->version_type / 16;//ȡversion_type�ֶ�8λ����������ǰ��λ����ȡIP�汾��
	//int type    = pkg->version_type % 16;//ȡversion_type�ֶ�8λ���������ĺ���λ����ȡCoLoR�����ͺ�
	if(pkg->version_type == 160)//����ʾIP�汾��Ϊ10��������Ϊ0��ר��CoLoR-Get��  �ֶ�Ϊ1010 0000���İ�
	{
		return 0;
	}
	else if(pkg->version_type == 161)//����ʾIP�汾��Ϊ10��������Ϊ1��ר��CoLoR-Data�� �ֶ�Ϊ1010 0001���İ�
	{
		return 1;
	}
	else if(pkg->version_type == 162)//����ʾIP�汾��Ϊ10��������Ϊ2��ר��CoLoR-Register�� �ֶ�Ϊ1010 0010���İ�
	{
		return 2;
	}
	else
		printf("[ Recv WRONG ] version_type == %s\n",pkg->version_type);
	
	
	return -1;
}

/*****************************************
* �������ƣ�Subscriber_CoLoR_ParseGet
* ��������������CoLoR-Get��ͷ
* �����б�
* ���ؽ����
*****************************************/
int
Subscriber_CoLoR_ParseGet
(
	const CoLoR_get * pkg,
	uint8_t * info
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
	

	/*
	memcpy((uint8_t*)tempGet.ether_dhost,(uint8_t*)pkg->ether_dhost,6);
	memcpy((uint8_t*)tempGet.ether_shost,(uint8_t*)pkg->ether_shost,6);
	tempGet.ether_type = pkg->ether_type;
	*/
	
	tempGet.version_type = pkg->version_type;
	tempGet.ttl = pkg->ttl;
	tempGet.total_len = pkg->total_len;
	tempGet.port_no = pkg->port_no;
	tempGet.checksum = pkg->checksum;
	tempGet.sid_len = pkg->sid_len;
	tempGet.nid_len = pkg->nid_len;
	tempGet.pid_n = pkg->pid_n;
	tempGet.options_static = pkg->options_static;

	//p += sizeof(uint8_t) * (14 + 12);
	p += sizeof(uint8_t) * 12;

	memset(tempGet.offset,0,OFFLENst);
	memcpy(tempGet.offset,(uint8_t*)p,OFFLEN);

	p+= sizeof(uint8_t) * 16;
	
	tempGet.publickey_len = (uint16_t)(*p);
	tempGet.mtu = (uint16_t)(*(p+sizeof(uint16_t)));
	
	p += sizeof(uint16_t) * 2;
	
	//tempGet.sid = (uint8_t*)calloc(tempGet.sid_len,sizeof(uint8_t));
	memset(tempGet.sid,0,SIDLENst);
	memcpy(tempGet.sid,(uint8_t*)p,tempGet.sid_len);

	memcpy(info,tempGet.sid,tempGet.sid_len);
	
	p += sizeof(uint8_t) * tempGet.sid_len;
	
	//tempGet.nid = (uint8_t*)calloc(tempGet.nid_len,sizeof(uint8_t));
	memset(tempGet.nid,0,NIDLENst);
	memcpy(tempGet.nid,(uint8_t*)p,tempGet.nid_len);
	
	p += sizeof(uint8_t) * tempGet.nid_len;
	
	uint16_t data_len = tempGet.total_len - 16 - 16 - tempGet.sid_len - tempGet.nid_len - tempGet.pid_n*4 - tempGet.publickey_len;
	//tempGet.data = (uint8_t*)calloc(data_len+1/*+1��������Ϊ�������ر�Ԥ��*/,sizeof(uint8_t));
	memset(tempGet.data,0,DATALENst);
	memcpy(tempGet.data,(uint8_t*)p,data_len);
	*(tempGet.data+data_len) = '\0';//�����ΪʲôΨ��data����Ҫ�ر��ڳ�1byte��Ϊ�����������Խ���ȡ
	
	p += sizeof(uint8_t) * data_len;
	
	//tempGet.publickey = (uint8_t*)calloc(tempGet.publickey_len,sizeof(uint8_t));
	memset(tempGet.publickey,0,PUBKEYLENst);
	memcpy(tempGet.publickey,(uint8_t*)p,tempGet.publickey_len);

	if(DEVETESTIMPL <= 1)
		printf(">>>CoLoR-Get  Received.\n");
	if(DEVETESTIMPL <= 0)
	{
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
		printf("   |publickey_len  = %d\n",tempGet.publickey_len);
		printf("   |mtu            = %d\n",tempGet.mtu);
		printf("   |sid            = %s\n",tempGet.sid);
		printf("   |nid            = %s\n",tempGet.nid);
		//printf("   |data           = %s\n",tempGet.data);
		printf("   |publickey      = %s\n",tempGet.publickey);
		printf("   |=============================================|\n");
	}
	
	return tempGet.sid_len;
}

/*****************************************
* �������ƣ�Subscriber_CoLoR_ParseData
* ��������������CoLoR-Data��ͷ
* �����б�
* ���ؽ����
*****************************************/
int
Subscriber_CoLoR_ParseData
(
	const CoLoR_data * pkg,
	uint8_t * info
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
	
	/*
	memcpy((uint8_t*)tempData.ether_dhost,(uint8_t*)pkg->ether_dhost,6);
	memcpy((uint8_t*)tempData.ether_shost,(uint8_t*)pkg->ether_shost,6);
	tempData.ether_type = pkg->ether_type;
	*/

	tempData.version_type = pkg->version_type;
	tempData.ttl = pkg->ttl;
	tempData.total_len = pkg->total_len;
	tempData.port_no = pkg->port_no;
	tempData.checksum = pkg->checksum;
	tempData.sid_len = pkg->sid_len;
	tempData.nid_len = pkg->nid_len;
	tempData.pid_n = pkg->pid_n;
	tempData.options_static = pkg->options_static;
	
	//p += sizeof(uint8_t) * (14 + 12);
	p += sizeof(uint8_t) * 12;

	memset(tempData.offset,0,OFFLENst);
	memcpy(tempData.offset,(uint8_t*)p,OFFLEN);

	p+= sizeof(uint8_t) * 16;
	
	tempData.signature_algorithm = (uint8_t)(*p);
	tempData.if_hash_cache = (uint8_t)(*(p+sizeof(uint8_t)));
	tempData.options_dynamic = (uint16_t)(*(p+sizeof(uint16_t)));
	
	p += sizeof(uint32_t);

	//tempData.sid = (uint8_t*)calloc(tempData.sid_len,sizeof(uint8_t));
	memset(tempData.sid,0,SIDLENst);
	memcpy(tempData.sid,(uint8_t*)p,tempData.sid_len);
	
	p += sizeof(uint8_t) * tempData.sid_len;

	//tempData.nid = (uint8_t*)calloc(tempData.nid_len,sizeof(uint8_t));
	memset(tempData.nid,0,NIDLENst);
	memcpy(tempData.nid,(uint8_t*)p,tempData.nid_len);
	
	p += sizeof(uint8_t) * tempData.nid_len;
	
	int data_len_int = tempData.total_len - 16 - 16 - tempData.sid_len - tempData.nid_len - tempData.pid_n*4 - 16;

	//������ڲа���ǡ���ܳ����ֶλ����������ֶ���ֵ�����˴�����û��У�飬���п��ܳ���data_lenֵΪ��ֵ���������������Ҫ�ų���
	if(data_len_int <= 0 || data_len_int > DATALENst)
	{
		if(DEVETESTIMPL <= 2)
			printf("data_len == %d (break out)\n",data_len_int);
		return -1;
	}
	
	uint16_t data_len = data_len_int;

	//tempData.data = (uint8_t*)calloc(data_len+1/*+1��������Ϊ�������ر�Ԥ��*/,sizeof(uint8_t));
	memset(tempData.data,0,DATALENst);

	memcpy(tempData.data,(uint8_t*)p,data_len);

	*(tempData.data+data_len) = '\0';//�����ΪʲôΨ��data����Ҫ�ر��ڳ�1Byte��Ϊ�����������Խ���ȡ

	memcpy(info,tempData.data,data_len);
	
	p += sizeof(uint8_t) * data_len;

	for(i=0;i<16;i++)
	{
		tempData.data_signature[i] = (uint8_t)(*(p+sizeof(uint8_t)*i));
	}

	if(DEVETESTIMPL <= 1)
		printf(">>>CoLoR-Data Received.\n");//�����MAC���յ�CoLoR-Data����ʾ
	if(DEVETESTIMPL <= 0)
	{
		printf("   |====================Datapkg==================|\n");
		printf("   |version_type        = %d\n",tempData.version_type);
		printf("   |ttl                 = %d\n",tempData.ttl);
		printf("   |total_len           = %d\n",tempData.total_len);
		printf("   |port_no             = %d\n",tempData.port_no);
		printf("   |checksum            = %d\n",tempData.checksum);
		printf("   |sid_len             = %d\n",tempData.sid_len);
		printf("   |nid_len             = %d\n",tempData.nid_len);
		printf("   |pid_n               = %d\n",tempData.pid_n);
		printf("   |options_static      = %d\n",tempData.options_static);
		printf("   |signature_algorithm = %d\n",tempData.signature_algorithm);
		printf("   |if_hash_cache       = %d\n",tempData.if_hash_cache);
		printf("   |options_dynamic     = %d\n",tempData.options_dynamic);
		printf("   |sid                 = %s\n",tempData.sid);
		printf("   |nid                 = %s\n",tempData.nid);
		//printf("   |data                = %s\n",tempData.data);
		printf("   |data_signature      = %s\n",tempData.data_signature);
		printf("   |=============================================|\n");
	}
	
	return data_len;
}

/*****************************************
* �������ƣ�Subscriber_CoLoR_ParseRegister
* ��������������CoLoR-Register��ͷ
* �����б�
* ���ؽ����
*****************************************/
int
Subscriber_CoLoR_ParseRegister
(
	const CoLoR_register * pkg,
	uint8_t * info
)
{
	int i;
	
	if (NULL == pkg)
	{
		return -1;
	}
	//����Register�����ֶ�
	//�����Register����ʽ�������
	
	if(DEVETESTIMPL <= 1)
		printf (">>>CoLoR-Register from Somewhere. Type : %d\n",pkg->version_type%16);
	
	return 0;
}

/*****************************************
* �������ƣ�Subscriber_SendtoAPPLayer
* ������������Ӧ�ò������������Ӧ��Ϣ
* �����б�
* ���ؽ����
*****************************************/
int
Subscriber_SendtoAPPLayer
(
	char * DATA,
	int DATAlen
)
{
	int i;
	
	int socket_datasender;
	socket_datasender = socket(AF_INET,SOCK_DGRAM,0);
	
	uint8_t pkg[sizeof(Trans_data)];
	Trans_data * ppkg;
	ppkg = (Trans_data *)pkg;

	memset(pkg,0,sizeof(Trans_data));

	memcpy(ppkg->data,DATA,DATALEN);
	memcpy(ppkg->offset,tempData.offset,OFFLEN);

	//��װ��Ϣ
	/*
	memcpy(pkg,"GOT",3);
	memcpy(pkg+3,DATA,DATAlen);//����DATA��ʵ�ʳ��ȷ���
	//memcpy(pkg+3,DATA,DATALEN);//����DATA��ʵ�ʳ��ȣ����ձ�׼DATA�����ֶγ��ȷ���
	memcpy(pkg+3+DATAlen,"OFF",3);
	memcpy(pkg+3+DATAlen+3,tempData.offset,OFFLEN);
	*/
	
	//��װ�׽���
	struct sockaddr_in addrTo;
	bzero(&addrTo,sizeof(addrTo));
	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons(TRALtoAPPL);
	addrTo.sin_addr.s_addr=inet_addr(LOOPIP);
	
	sendto(socket_datasender,pkg,sizeof(Trans_data),0,(struct sockaddr *)&addrTo,sizeof(addrTo));
	close(socket_datasender);

	if(DEVETESTIMPL <= 1)
		printf("GOT pkg has been sent to the DATA server, pkg == %s\n",pkg);

	return 1;
}

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.Ӧ�ò������ To.������������*******************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�Subscriber_CoLoR_PacketGet
* ������������װ����mac���Get��
* �����б�
* ���ؽ����
*****************************************/
int
Subscriber_CoLoR_PacketGet
(
	uint8_t * pkg,
	uint8_t * SID,
	uint8_t * OFF
)
{
	CoLoR_get * getpkg;
	getpkg = (CoLoR_get *)pkg;
	
	//���CoLoR-Get����
	getpkg->version_type = 160;//�汾4λ������4λ����Ϊ���ó�CoLoR_get��
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
* �������ƣ�Subscriber_SendtoPhysicalPort
* �����������������������ڷ������ݰ�
* �����б�
* ���ؽ����
*****************************************/
int
Subscriber_SendtoPhysicalPort
(
	uint8_t * pkg
)
{
	int i;

	memset(DownPKG,0,PKGSIZE);
	
	//��װ���������������ڵ���Ϣ
	memcpy(DownPKG,"FOR",3);
	memcpy(DownPKG+3,pkg,sizeof(CoLoR_get));
	
	//Search for the Dest IP address (UDP)
	int portfound=0;
	for(i=0;i<=ppiplistcount;i++)
	{
		if(ppiplist[i].pp[0] != 0)
		{
			memcpy(Subscriber_dest_ip,ppiplist[i].ip,16);
			portfound=1;
			break;
		}
	}
	if(portfound==0)
	{
		printf("[%s] Do not found the proper port to forward message.\n",SwitchDevice_local_ip);
		return 0;
	}

	//Getting ready to send pkg to physical port
	int portnum;
	int LocalPkgBUFHead;
	if(INTRACOMMUNICATION == 0)
	{
		int socket_sender;
		socket_sender=socket(AF_INET,SOCK_DGRAM,0);

		struct sockaddr_in addrTo;
		bzero(&addrTo,sizeof(addrTo));
		addrTo.sin_family=AF_INET;
		addrTo.sin_port=htons(destPORT);
		addrTo.sin_addr.s_addr=inet_addr(Subscriber_dest_ip);
	
		if(DEVETESTIMPL <= 1)
			printf("[%s] Message has been sent out:%s\n",SwitchDevice_local_ip,DownPKG);

		sendto(socket_sender,DownPKG,sizeof(CoLoR_get)+3,0,(struct sockaddr *)&addrTo,sizeof(addrTo));
		close(socket_sender);
	}
	else if(INTRACOMMUNICATION == 1)
	{
		pthread_mutex_lock(&lockPkgBUF);
		for(i=15;i>=0;i--)
		{
			if(Subscriber_dest_ip[i] == '.')
			{
				portnum = Subscriber_dest_ip[i+2] - '0';

				if(PkgBUF[PkgBUFHead].flag_occupied == 0)
				{
					LocalPkgBUFHead = PkgBUFHead;
					PkgBUFHead++;
					if(PkgBUFHead >=10)
						PkgBUFHead -= 10;
		
					PkgBUF[LocalPkgBUFHead].flag_occupied = 1;
					PkgBUF[LocalPkgBUFHead].pkglength = sizeof(CoLoR_get);
					PkgBUF[LocalPkgBUFHead].destppnum = portnum;
					strcpy(PkgBUF[LocalPkgBUFHead].sourcepp,PhysicalPort);
					memcpy(PkgBUF[LocalPkgBUFHead].pkg,DownPKG+3,sizeof(CoLoR_get));

					if(pysicalportDEVETESTIMPL <= 1)
					{
						printf("[] SEM Message has been sent to NetworkLayer:\n");
						printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
					}
				}
				else
				{
					if(pysicalportDEVETESTIMPL <= 1)
					{
						printf("[] SEM Message failed to be sent to NetworkLayer:\n");
						printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
					}
				}

				sem_post(&semPacketSEND[portnum]);
				break;
			}
		}
		pthread_mutex_unlock(&lockPkgBUF);
	}

	return 1;
}

/*******************************************************************************************************************************************
**********************************************************���߳����ɳ���********************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�Subscriber_thread_timer
* ����������Ϊ���߳��ṩ��ʱ�ź�
* �����б�
* ���ؽ����
*****************************************/
void *
Subscriber_thread_timer
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
* �������ƣ�Subscriber_thread_PhysicalPorttoAPPLayer
* ��������������ͨ�� - From.������������ To.Ӧ�ò������
* �����б�
* ���ؽ����
*****************************************/
void *
Subscriber_thread_PhysicalPorttoAPPLayer
(
	void * fd
)
{
	int i;

	int socket_receiver;
	socket_receiver = socket(AF_INET,SOCK_DGRAM,0);

	struct sockaddr_in sin;
	int sockaddr_len=sizeof(struct sockaddr_in);
	bzero(&sin,sockaddr_len);
	sin.sin_family=AF_INET;
	//sin.sin_addr.s_addr=htonl(INADDR_ANY);
	sin.sin_addr.s_addr=inet_addr(SwitchDevice_local_ip);
	sin.sin_port=htons(localPORT);
	
	bind(socket_receiver,(struct sockaddr *)&sin,sockaddr_len);
	
	printf("UDP listening start...\n");

	int SIDDATAlen;
	uint8_t SIDDATA[100];
	memset(SIDDATA,0,100);

	while(1)
	{
		memset(RecvBUF,0,sizeof(RecvBUF));
		SIDDATAlen = -1;
		memset(SIDDATA,0,100);

		if(INTRACOMMUNICATION == 0)
			recvfrom(socket_receiver,RecvBUF,sizeof(RecvBUF),0,(struct sockaddr *)&sin,&sockaddr_len);
		else if(INTRACOMMUNICATION == 1)
		{
			sem_wait(&semPacketRECV);
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

/*���ϳ�����
			pthread_mutex_lock(&lockPkgBUF);
			for(;;)
			{
				if(PkgBUF[PkgBUFTail++].flag_occupied == 1)
				{
					LocalPkgBUFTail = PkgBUFTail-1;

					RecvBUFLength = PkgBUF[LocalPkgBUFTail].pkglength;
					memcpy(RecvBUF,"NET",3);
					memcpy(RecvBUF+3,PkgBUF[LocalPkgBUFTail].pkg,RecvBUFLength);
					//strcpy(sourcepp,PkgBUF[LocalPkgBUFTail].sourcepp);

					break;
				}
				
				if(PkgBUFTail>=10)
					PkgBUFTail -= 10;
			}
			pthread_mutex_unlock(&lockPkgBUF);
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
					//strcpy(sourcepp,PkgBUF[LocalPkgBUFTail].sourcepp);

					//���ת��������ռ��
					memset(PkgBUF[LocalPkgBUFTail].sourcepp,0,30);
					memset(PkgBUF[LocalPkgBUFTail].pkg,0,MTU);
					PkgBUF[LocalPkgBUFTail].destppnum = 0;
					PkgBUF[LocalPkgBUFTail].pkglength = 0;
					PkgBUF[LocalPkgBUFTail].flag_occupied = 0;

					//�ͷ�CacheList����λ��
					for(i=0;i<10;i++)
					{
						memset(CacheList[i].physicalport,0,30);
						memset(CacheList[i].sid,0,SIDLEN);
						memset(CacheList[i].sbd,0,SBDLEN);
						memset(CacheList[i].offset,0,OFFLEN);
						memset(CacheList[i].cache,0,CACHESIZE);
						CacheList[i].cachetop = 0;
						CacheList[i].datapkglocation = -1;
						CacheList[i].flag_ack = 0;
						CacheList[i].flag_occupied = 0;
					}

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

		//Main Forwarding Procession with Routing Table
		if(strncmp(RecvBUF,"NET",3) == 0)
		{
			//����CoLoR��
			SIDDATAlen = Subscriber_CoLoR_ParsePacket(RecvBUF+3,SIDDATA);
			//�����Ͳ�����CoLoRЭ�飬���������û��IP��������ζ���յ���CoLoR���ݰ�ԭ�����������
			if(SIDDATAlen <= 0)
				continue;

			//��Ӧ�ò������������Ӧ��Ϣ
			Subscriber_SendtoAPPLayer(SIDDATA,SIDDATAlen);
		}
	}

	//�ر��߳�
	close(socket_receiver);
	exit(0);
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�Subscriber_thread_APPLayertoPhysicalPort
* ��������������ͨ�� - From.Ӧ�ò������ To.������������
* �����б�
* ���ؽ����
*****************************************/
void *
Subscriber_thread_APPLayertoPhysicalPort
(
	void * fd
)
{
	int socket_sidreceiver;
	socket_sidreceiver = socket(AF_INET,SOCK_DGRAM,0);
	
	struct sockaddr_in sin;
	int sockaddr_len=sizeof(struct sockaddr_in);
	bzero(&sin,sockaddr_len);
	sin.sin_family=AF_INET;
	//sin.sin_addr.s_addr=htonl(INADDR_ANY);
	sin.sin_addr.s_addr=inet_addr(LOOPIP);
	sin.sin_port=htons(APPLtoTRAL);

	bind(socket_sidreceiver,(struct sockaddr *)&sin,sockaddr_len);

	uint8_t SID[SIDLEN];
	uint8_t OFF[OFFLEN];
	
	uint8_t recvpkg[sizeof(Trans_get)];
	Trans_get * precvpkg;
	precvpkg = (Trans_get *)recvpkg;
	uint8_t sendpkg[sizeof(CoLoR_get)];
	
	while(1)
	{
		memset(SID,0,SIDLEN);
		memset(OFF,0,OFFLEN);
		memset(recvpkg,0,sizeof(Trans_get));
		memset(sendpkg,0,sizeof(CoLoR_get));
		
		recvfrom(socket_sidreceiver,recvpkg,sizeof(Trans_get),0,(struct sockaddr *)&sin,&sockaddr_len);
		
		memcpy(SID,precvpkg->sid,SIDLEN);
		memcpy(OFF,precvpkg->offset,OFFLEN);

		//��װCoLoR-Get����
		Subscriber_CoLoR_PacketGet(sendpkg,SID,OFF);

		//��CoLoR-Get���ķ���������������
		Subscriber_SendtoPhysicalPort(sendpkg);
	}
	//�ر��߳�
	close(socket_sidreceiver);
	exit(0);
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�Subscriber_main
* ����������Subscriberģ��������������ForwardingPlaneת��ƽ���̡߳�������ѭ��ִ�м�HTTP���������ܡ�
* �����б�
eg: ./sub d1sub1 d1sub1-eth1
�ڶ�����ʹ��char**argv�ķ�ʽ����������֪�ǲ����﷨����
* ���ؽ����
*****************************************/
//int main(int argc,char *argv[])
//Subscriberģ��ֱ�ӵ���ģʽ
/*
int
Subscriber_main
(
	int argc,
	char argv[][30]
)
*/
//Subscriberģ���̵߳���ģʽ
void *
Subscriber_main
(
	void * fd
)
{
	int i,j,k,l;

	Subscriber_Parameterinit();

	//��Subscriberģ������̵߳��õĲ��������ӿ�
	struct subscriberinput * subinput;
	subinput = (struct subscriberinput *)fd;

	int argc = subinput->argc;
	char argv[50][30];
	memset(argv,0,1500);
	for(i=0;i<argc;i++)
	{
		strcpy(argv[i],subinput->argv[i]);
	}

	//��������
	printf("argc == %d\n",argc);
	for(i=0;i<argc;i++)
	{
		printf("argv[%d] == %s\n",i,argv[i]);
	}

	strcpy(NID,argv[1]);

	int ii;
	int numfound=0;
	char tempport[30];
	memset(tempport,0,30);
	char portnum[3]={0};
	for(i=2;i<argc;i++)
	{
		numfound=0;
		memset(tempport,0,30);
		memset(portnum,0,3);

		//��ȡ������������ĩβ���
		strcpy(tempport,argv[i]);

		for(ii=29;ii>=0;ii--)
		{
			if(tempport[ii] >= '0' && tempport[ii] <= '9' && numfound == 0)
			{
				numfound=1;
				portnum[1]=tempport[ii];
				continue;
			}
			if(tempport[ii] >= '0' && tempport[ii] <= '9' && numfound == 1)
			{
				numfound=2;
				portnum[0]=tempport[ii];
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
		strcpy(ppiplist[i-2].pp,tempport);
	
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
	if(GLOBALTEST <= 2)
	{
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

		//ȷ�������ڵ�GET������Ŀ�꣬������RM��NID
		memcpy(sendnid,NID,NIDLEN/2);
		strcpy(sendnid+NIDLEN/2,"d");
		strcpy(sendnid+NIDLEN/2+1,domainID);
		for(i=NIDLEN/2+1;i<NIDLEN;i++)
		{
			if(sendnid[i]==0)
			{
				strcpy(sendnid+i,"rm");
				break;
			}
		}
	}
	
	//�������ͼ�ʱ�����߳�
	pthread_t pthread_timer;
	if(pthread_create(&pthread_timer, NULL, Subscriber_thread_timer, NULL)!=0)
	{
		perror("Creation of timer thread failed.");
	}
	
	//����thread_PhysicalPorttoAPPLayer���߳�
	pthread_t pthread_PhysicalPorttoAPPLayer;
	if(pthread_create(&pthread_PhysicalPorttoAPPLayer, NULL, Subscriber_thread_PhysicalPorttoAPPLayer, NULL)!=0)
	{
		perror("Creation of PhysicalPorttoAPPLayer thread failed.");
	}
	
	//����thread_APPLayertoPhysicalPort���߳�
	pthread_t pthread_APPLayertoPhysicalPort;
	if(pthread_create(&pthread_APPLayertoPhysicalPort, NULL, Subscriber_thread_APPLayertoPhysicalPort, NULL)!=0)
	{
		perror("Creation of APPLayertoPhysicalPort thread failed.");
	}

	//����Ϊphysicalportģ�������ͨ�Ų�����������
	/*
	struct physicalportinput
	{
		char pp[30];
		char ip[16];
	};
	*/
	struct physicalportinput ppinput;
	memset(ppinput.pp,0,30);
	strcpy(ppinput.pp,argv[2]);
	strcpy(ppinput.ip,SwitchDevice_local_ip);

	pthread_t pthread_physicalport;
	//����physicalport���߳�
	if(pthread_create(&pthread_physicalport, NULL, thread_physicalport, (void *)&ppinput)!=0)
	{
		perror("Creation of physicalport thread failed.");
	}

	while(1)
	{
		sleep(10000);
	}

	printf("Subscriber_main exit success!\n");
	//return (EXIT_SUCCESS);
}
