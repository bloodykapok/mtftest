/*******************************************************************************************************************************************
* �ļ�����publisher.c
* �ļ�������973�ǻ�Эͬ����SARϵͳTestBed�׼��������ݷ����ˣ�Data Publisher������GET�����ս���+����SID��Dataƥ��+DATA����װ����
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
FILEsidnameָ�洢SID��Dataƥ���ϵ���ļ���Ĭ���ļ���cache.log��·����Ҫ���иó������Ա���о�����������ļ�ʵ�ʴ��ڵ�λ�ö��Ϻž�����
PhysicalPortָCoLoRЭ�鷢��Get���ͽ���Data���������˿ڣ�ע��������Ĭ�����߶˿������Ƿ�Ϊeth0����Fedora20ϵͳ�е�Ĭ������Ϊem1����ע��ʶ��
2��ϵͳ����
��Fedoraϵͳ������Ҫʹ��ԭʼ�׽��ַ����Զ����ʽ�����ݰ�����ر�Fedora�ķ���ǽ�����
sudo systemctl stop firewalld.service
��Ubuntuϵͳ�������κβ���
3����������
gcc publisher.c -o publisher -lpthread
4�����У����漰ԭʼ�׽��ֵ�ʹ�ã���rootȨ�ޣ�
sudo ./publisher
*/

#include"publisher.h"

/*******************************************************************************************************************************************
*************************************�궨����������************ȫ�ֱ�������************����ʽ����*********************************************
*******************************************************************************************************************************************/

//����ӳ���
PPIP ppiplist[48];

//Ethernet�� / ����CoLoR�����жϵİ�ͷ����ֹ���̶��ײ�������Version/Type�ֶ�ΪGet����Data����Register����ͨ�ã�
Ether_CoLoR_VersionType tempVersionType;

//Ethernet�� / CoLoRЭ��Get���ײ���PID֮ǰ���ֶγ��ȹ̶������ڷ�װ

//Ethernet�� / CoLoRЭ��Get���ײ���PID֮ǰ���ֶγ��ȿɱ䣬���ڽ���
Ether_CoLoR_get_parse tempGet;

//Ethernet�� / CoLoRЭ��Data���ײ���PID֮ǰ���ֶγ��ȹ̶������ڷ�װ

//Ethernet�� / CoLoRЭ��Data���ײ���PID֮ǰ���ֶγ��ȿɱ䣬���ڽ���
Ether_CoLoR_data_parse tempData;

//Ethernet�� / CoLoRЭ��Register���ײ���PID֮ǰ��
Ether_CoLoR_register tempRegister;

/*******************************************************************************************************************************************
*******************************************��ʼ�������������********************************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�Publisher_Parameterinit
* ����������Publisherģ��ȫ�ֱ�������ֵ
* �����б�
* ���ؽ����
*****************************************/
void
Publisher_Parameterinit()
{
	//�����˿�
	memset(PhysicalPort,0,30);

	//����������Ϣ
	memset(SwitchDevice_local_ip,0,16);
	memcpy(SwitchDevice_local_ip,"127.1.1.1",9);//����ĸ�ֵ����Ч���ŵ����������¶˵�λ�þ���Ч��ͬ���ĳ�ʼ�������RM����������⣬���
	memset(Publisher_dest_ip,0,16);

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
	Publisher_selfpacketdonotcatch=0;

	memset(tempsid,0,SIDLEN);
	memset(tempPIDs,0,120);

	memset(local_mac,0,7);
	memset(local_ip,0,5);
	memset(dest_ip,0,16);

	uint8_t bm1[7]={0xff,0xff,0xff,0xff,0xff,0xff,0x00};
	memcpy(broad_mac1,bm1,7);
	uint8_t bm2[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	memcpy(broad_mac2,bm2,7);

	memset(destmac,0,7);
	memset(localmac,0,7);

	//PID����
	memset(pidlist,0,3000);
	pidlistcount=-1;
}

/*****************************************
* �������ƣ�Publisher_configure_SIDfilepath
* �����������Զ����ýڵ�SIDע�������ļ��Ķ�ȡ·��
* �����б�
* ���ؽ����
*****************************************/
void
Publisher_configure_SIDfilepath
(
	char * SIDfilepath,
	char * domainID,
	char * deviceNA,
	char * deviceID
)
{
	int i;
	int zeropoint;

	strcpy(SIDfilepath,"./973domain/domain");

	for(i=0;i<FILEnameL;i++)
	{
		if(SIDfilepath[i]==0)
		{
			zeropoint=i;
			break;
		}
	}

	strcpy(SIDfilepath+zeropoint,domainID);

	for(i=0;i<FILEnameL;i++)
	{
		if(SIDfilepath[i]==0)
		{
			zeropoint=i;
			break;
		}
	}

	strcpy(SIDfilepath+zeropoint,"/");
	strcpy(SIDfilepath+zeropoint+1,deviceNA);

	for(i=0;i<FILEnameL;i++)
	{
		if(SIDfilepath[i]==0)
		{
			zeropoint=i;
			break;
		}
	}

	strcpy(SIDfilepath+zeropoint,deviceID);

	for(i=0;i<FILEnameL;i++)
	{
		if(SIDfilepath[i]==0)
		{
			zeropoint=i;
			break;
		}
	}

	strcpy(SIDfilepath+zeropoint,"/sid");

	printf("SIDfilepath == %s\n",SIDfilepath);
}

/*****************************************
* �������ƣ�Publisher_configure_PIDfilepath

* �����������Զ����ýڵ�PIDͨ��������ļ��Ķ�ȡ·��
* �����б�
* ���ؽ����
*****************************************/
void
Publisher_configure_PIDfilepath
(
	char * PIDfilepath,
	char * domainID,
	char * deviceNA,
	char * deviceID
)
{
	int i;
	int zeropoint;

	strcpy(PIDfilepath,"./973domain/domain");

	for(i=0;i<FILEnameL;i++)
	{
		if(PIDfilepath[i]==0)
		{
			zeropoint=i;
			break;
		}
	}

	strcpy(PIDfilepath+zeropoint,domainID);

	for(i=0;i<FILEnameL;i++)
	{
		if(PIDfilepath[i]==0)
		{
			zeropoint=i;
			break;
		}
	}

	strcpy(PIDfilepath+zeropoint,"/pid");

	printf("PIDfilepath == %s\n",PIDfilepath);
}

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.������������   To.Ӧ�÷�����*******************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�Publisher_CoLoR_ParsePacket
* ����������CoLoR����������
* �����б�
* ���ؽ����
*****************************************/
int
Publisher_CoLoR_ParsePacket
(
	const uint8_t * Packet,
	uint8_t * info
)
{	
	int PacketType = -1;
	int infolen = -1;
	
	CoLoR_VersionType * pkgvt = NULL;
	CoLoR_get * pkgget = NULL;
	CoLoR_data * pkgdata = NULL;
	CoLoR_register * pkgregister = NULL;
	
	//�ж�CoLoR���ݰ�����
	pkgvt = (CoLoR_VersionType *)Packet;

	PacketType = Publisher_CoLoR_SeeType(pkgvt);
	
	if(PacketType == 0)//�յ�Get��
	{
		pkgget  = (CoLoR_get *)Packet;
		infolen = Publisher_CoLoR_ParseGet(pkgget,info);
		if(infolen <= 0)
			return -1;
	}
	else if(PacketType == 1)//�յ�Data��
	{
		pkgdata  = (CoLoR_data *)Packet;
		infolen = Publisher_CoLoR_ParseData(pkgdata,info);
	}
	else if(PacketType == 2)//�յ�Register��
	{
		pkgregister  = (CoLoR_register *)Packet;
		infolen = Publisher_CoLoR_ParseRegister(pkgregister,info);
	}
	else//�����Ͳ�����CoLoRЭ��
	{
	}
	
	return infolen;
}

/*****************************************
* �������ƣ�Publisher_CoLoR_SeeType
* �����������ж��Ƿ�Ϊ��Ϊ����CoLoR��ͷ
* �����б�
* ���ؽ����
*****************************************/
int
Publisher_CoLoR_SeeType
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
* �������ƣ�Publisher_CoLoR_ParseGet
* ��������������CoLoR-Get��ͷ
* �����б�
* ���ؽ����
*****************************************/
int
Publisher_CoLoR_ParseGet
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
	
	int data_len_int = tempGet.total_len - 16 - 16 - tempGet.sid_len - tempGet.nid_len - tempGet.pid_n*4 - tempGet.publickey_len;

	//������ڲа���ǡ���ܳ����ֶλ����������ֶ���ֵ�����˴�����û��У�飬���п��ܳ���data_lenֵΪ��ֵ���������������Ҫ�ų���
	if(data_len_int <= 0 || data_len_int > DATALENst)
	{
		if(DEVETESTIMPL <= 2)
			printf("data_len == %d (break out)\n",data_len_int);
		return -1;
	}

	uint16_t data_len = data_len_int;
	//tempGet.data = (uint8_t*)calloc(data_len+1,sizeof(uint8_t));
	memset(tempGet.data,0,DATALENst);
	memcpy(tempGet.data,(uint8_t*)p,data_len);
	*(tempGet.data+data_len) = 0;//�����ΪʲôΨ��data����Ҫ�ر��ڳ�1byte��Ϊ�����������Խ���ȡ
	
	p += sizeof(uint8_t) * data_len;
	
	//tempGet.publickey = (uint8_t*)calloc(tempGet.publickey_len,sizeof(uint8_t));
	memset(tempGet.publickey,0,PUBKEYLENst);
	memcpy(tempGet.publickey,(uint8_t*)p,tempGet.publickey_len);

	p += sizeof(uint8_t) * tempGet.publickey_len;
	
	//tempPIDs = (uint8_t*)calloc(tempGet.pid_n*4,sizeof(uint8_t));
	memset(tempPIDs,0,120);
	memcpy(tempPIDs,(uint8_t*)p,tempGet.pid_n*4);
	
	if(DEVETESTIMPL <= 1)
		printf(">>>CoLoR-Get  Received.\n");
	if(DEVETESTIMPL <= 0)
	{
		printf("   |====================Getpkg===================|\n");
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
		for(i=0;i<tempGet.pid_n;i++)
		{
		printf("   |PID%d          = %s\n",i+1,tempPIDs+i*4);
		}
		printf("   |=============================================|\n");
	}

	memcpy(tempsid, tempGet.sid, tempGet.sid_len);

	
	return tempGet.sid_len;
}

/*****************************************
* �������ƣ�Publisher_CoLoR_ParseData
* ��������������CoLoR-Data��ͷ
* �����б�
* ���ؽ����
*****************************************/
int
Publisher_CoLoR_ParseData
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
	
	uint16_t data_len = tempData.total_len - sizeof(uint8_t)*(16+16+16) - tempData.sid_len - tempData.nid_len - tempData.pid_n*sizeof(uint8_t)*4;
	//tempData.data = (uint8_t*)calloc(data_len+1/*+1��������Ϊ�������ر�Ԥ��*/,sizeof(uint8_t));
	memset(tempData.data,0,DATALENst);
	memcpy(tempData.data,(uint8_t*)p,data_len);
	*(tempData.data+data_len) = 0;//�����ΪʲôΨ��data����Ҫ�ر��ڳ�1Byte��Ϊ�����������Խ���ȡ

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
		printf("   |version_type = %d\n",tempData.version_type);
		printf("   |ttl = %d\n",tempData.ttl);
		printf("   |total_len = %d\n",tempData.total_len);
		printf("   |port_no = %d\n",tempData.port_no);
		printf("   |checksum = %d\n",tempData.checksum);
		printf("   |sid_len = %d\n",tempData.sid_len);
		printf("   |nid_len = %d\n",tempData.nid_len);
		printf("   |pid_n = %d\n",tempData.pid_n);
		printf("   |options_static = %d\n",tempData.options_static);
		printf("   |signature_algorithm = %d\n",tempData.signature_algorithm);
		printf("   |if_hash_cache = %d\n",tempData.if_hash_cache);
		printf("   |options_dynamic = %d\n",tempData.options_dynamic);
		printf("   |sid = %s\n",tempData.sid);
		printf("   |nid = %s\n",tempData.nid);
		//printf("   |data = %s\n",tempData.data);
		printf("   |data_signature = %s\n",tempData.data_signature);
		printf("   |=============================================|\n");
	}
	
	return data_len;
}

/*****************************************
* �������ƣ�Publisher_CoLoR_ParseRegister
* ��������������CoLoR-Register��ͷ
* �����б�
* ���ؽ����
*****************************************/
int
Publisher_CoLoR_ParseRegister
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

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.Ӧ�÷����� To.������������*********************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�Publisher_CoLoR_PacketData
* ������������������SID���Ҷ�ӦDATA����װCoLoR-Data���ݰ���Ŀǰ�����յ�Get���󷵻�Data���Ĺ��̣�
* �����б�
* ���ؽ����
*****************************************/
int
Publisher_CoLoR_PacketData
(
	uint8_t * SID,
	uint8_t * DATA,
	uint8_t * offset,
	uint8_t * pkgdata
)
{
	int i,j;
	CoLoR_data * pkg = (CoLoR_data *)pkgdata;
	struct hostent *host =NULL;
	struct sockaddr sa;
	int sockfd,len;
	uint8_t temp_ip[5];

	uint8_t pkg_pids[PKGSIZE];
	//pkg_pids = (uint8_t*)calloc(16 +16 + SIDLEN + NIDLEN + tempGet.pid_n*4 + DATALEN + 16,sizeof(uint8_t));
	
	/*
	int SIDlen=0,DATAlen=0;
	for(i=0;i<SIDLEN;i++)
	{
		if(SID[i] == 0)
			break;
	}
	SIDlen = i;
	i=0;
	for(i=0;i<DATALEN;i++)
	{
		if(DATA[i] == 0)
			break;
	}
	DATAlen = i;
	i=0;
	*/
	
	//���CoLoR-data����
	pkg->version_type = 161;//�汾4λ������4λ����Ϊ���ó�CoLoR_Data��
	pkg->ttl = 255;//����ʱ��
	pkg->total_len = 16 + 16 + SIDLEN + NIDLEN + tempGet.pid_n*4 + DATALEN + 16;//�ܳ���
	
	pkg->port_no = 1;//�˿ں�
	pkg->checksum = 0;//�����
	
	pkg->sid_len = SIDLEN;//SID����
	pkg->nid_len = NIDLEN;//NID����
	pkg->pid_n = tempGet.pid_n;//PID����
	//�̶��ײ�ѡ��
	if(CNFON == 1)//CNF�趨
		pkg->options_static = 32;
	else if(CNFON == 0)//CNF�趨
		pkg->options_static = 0;
	
	
	memset(pkg->offset,0,OFFLEN);
	memcpy(pkg->offset,offset,OFFLEN);
	
	pkg->signature_algorithm = 1;//ǩ���㷨
	pkg->if_hash_cache = 255;//�Ƿ��ϣ4λ���Ƿ񻺴�4λ
	pkg->options_dynamic = 0;//�ɱ��ײ�ѡ��
	
	//����SID
	memcpy(pkg->sid, SID, SIDLEN);

	//����NID
	//char nid[NIDLEN] = {'I',' ','a','m',' ','t','h','e',' ','h','o','s','t','~','~','!'};
	int pidnidfound=0;

	static long flagerror=0;
	if(flagerror>2000000000)
		flagerror = 0;

	char destNID[NIDLEN/2];
	if(tempGet.pid_n != 0)
	{
		uint8_t judgedPID[4];
		memcpy(judgedPID,tempPIDs+(tempGet.pid_n-1)*4,4);

		i=0;
		pidnidfound=0;

		
		//��pidlist����ԭ����յ�bugû�жԲߣ�ֻ���Ƚ��Ͱ�pidlist�����������һ������
		while(pidnidfound==0)
		{
			if(strncmp(pidlist2[i].pid,judgedPID,4) == 0)
			{
				strcpy(destNID,pidlist2[i].nid);
				pidnidfound=1;
			}
			i++;
			if(i==pidlistcount2+1)
			{
				break;
			}
		}
		


		if(pidnidfound == 0)
		{
			flagerror++;
			/*
			printf("===============================================================\n");
			printf("Do not found a proper NID for the PID. This is not normal.\n");
			printf("flagerror          = %ld\n",flagerror);
			printf("tempGet.pid_n      = %d\n",tempGet.pid_n);
			printf("tempPIDs+(tempGet.pid_n-1)*4 = %s\n",tempPIDs+(tempGet.pid_n-1)*4);
			printf("judgedPID                    = %s\n",judgedPID);
			
			printf("pidlistcount = %d\n",pidlistcount2);
			
			i=0;
			while(1)
			{
				printf("pidlist[i].pid = %s\n",pidlist2[i].pid);
				printf("pidlist[i].nid = %s\n",pidlist2[i].nid);
				i++;
				if(i==pidlistcount2+1)
				{
					break;
				}
			}
			*/
		}
		else
		{
		}
	}
	else
	{
		pidnidfound=1;
		memcpy(destNID,tempGet.nid,NIDLEN/2);
	}
	if(pidnidfound == 1)
	{
		memcpy(pkg->nid,tempGet.nid,NIDLEN/2);//NID
		memcpy(pkg->nid+NIDLEN/2,destNID,NIDLEN/2);//NID
	}
	else
	{
		if(DEVETESTIMPL <= 1)
			printf("Search for dest NID failed.\n");
	}
	
	if(DEVETESTIMPL <= 1)
	{
		printf("localNID == %s\n",pkg->nid);
		printf("NIDLEN/2 == %d\n",NIDLEN/2);
		printf("destNID  == %s\n",destNID);
		printf("pkg->total_len == %d\n",pkg->total_len);
	}
	
	//ת���ڴ�ռ䣬����PID
	memcpy(pkg_pids, (uint8_t*)pkg, pkg->total_len);
	memcpy(pkg_pids+pkg->total_len-16-DATALEN-tempGet.pid_n*4, (uint8_t*)tempPIDs, tempGet.pid_n*4);
	
	//����DATA
	memcpy(pkg_pids+pkg->total_len-16-DATALEN, (uint8_t*)DATA, DATALEN);
	
	//����Data Signature
	char data_signature[16] = {'I',' ','a','m',' ','t','h','e',' ','s','i','g','~','~','~','!'};
	memcpy(pkg_pids+pkg->total_len-16, data_signature, 16);
	
	memcpy(pkgdata,pkg_pids,pkg->total_len);
	
	return 1;
}

/*****************************************
* �������ƣ�Publisher_SendtoPhysicalPort
* �����������������������ڷ������ݰ�
* �����б�
* ���ؽ����
*****************************************/
int
Publisher_SendtoPhysicalPort
(
	uint8_t * pkg
)
{
	int i;

	memset(DownPKG,0,PKGSIZE);
	
	//��װ���������������ڵ���Ϣ
	memcpy(DownPKG,"FOR",3);
	memcpy(DownPKG+3,pkg,sizeof(CoLoR_data));
	
	//Search for the Dest IP address (UDP)
	int portfound=0;
	for(i=0;i<=ppiplistcount;i++)
	{
		if(ppiplist[i].pp[0] != 0)
		{
			memcpy(Publisher_dest_ip,ppiplist[i].ip,16);
			portfound=1;
			break;
		}
	}
	if(portfound==0)
	{
		printf("[%s] Do not found the proper port to forward message.\n",SwitchDevice_local_ip);
		return 0;
	}

	//ΪCNF���ٳ�ʼ����Ļ������ݿ�
	if(CNFON == 1 && CacheList[0].flag_occupied == 0)//��һ�����δ��ռ�ã�˵����ת���ڵ�Ϊ�����ʼ���ڵ㣨�������ʼ����������һ�������У���һ�����һ���ᱻռ�ã���δ��ռ���ǲ����ܳ��ֵ����Σ�
	{
		CacheList[0].flag_occupied = 1;
		CacheList[0].flag_ack = 0;
		CacheList[0].datapkglocation = 0;
		CacheList[0].cachetop = CACHESIZE;
		memcpy(CacheList[0].physicalport,"d1pub1-eth0",30);
		memcpy(CacheList[0].sid,"wangzhaoxu",SIDLEN);
		memset(CacheList[0].sbd,0,SBDLEN);
		memset(CacheList[0].offset,0,OFFLEN);
		memset(CacheList[0].cache,'A',CACHESIZE);
	}

	//Getting ready to send pkg to physical port
	int portnum;
	int LocalPkgBUFHead;
	if(INTRACOMMUNICATION == 0)
	{
		int socket_sender;
		socket_sender = socket(AF_INET,SOCK_DGRAM,0);

		struct sockaddr_in addrTo;
		bzero(&addrTo,sizeof(addrTo));
		addrTo.sin_family=AF_INET;
		addrTo.sin_port=htons(destPORT);
		addrTo.sin_addr.s_addr=inet_addr(Publisher_dest_ip);
	
		if(DEVETESTIMPL <= 1)
			printf("[%s] Message has been sent out:%s\n",SwitchDevice_local_ip,DownPKG);

		sendto(socket_sender,DownPKG,sizeof(CoLoR_data)+3,0,(struct sockaddr *)&addrTo,sizeof(addrTo));
		close(socket_sender);
	}
	else if(INTRACOMMUNICATION == 1)
	{
		pthread_mutex_lock(&lockPkgBUF);
		for(i=15;i>=0;i--)
		{
			if(Publisher_dest_ip[i] == '.')
			{
				portnum = Publisher_dest_ip[i+2] - '0';

				if(PkgBUF[PkgBUFHead].flag_occupied == 0)
				{
					LocalPkgBUFHead = PkgBUFHead;
					PkgBUFHead++;
					if(PkgBUFHead >=10)
						PkgBUFHead -= 10;
		
					PkgBUF[LocalPkgBUFHead].flag_occupied = 1;
					PkgBUF[LocalPkgBUFHead].pkglength = sizeof(CoLoR_data);
					PkgBUF[LocalPkgBUFHead].destppnum = portnum;
					strcpy(PkgBUF[LocalPkgBUFHead].sourcepp,PhysicalPort);
					memcpy(PkgBUF[LocalPkgBUFHead].pkg,DownPKG+3,sizeof(CoLoR_data));

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
* �������ƣ�Publisher_thread_PhysicalPorttoAPPLayer
* ��������������ͨ�� - From.������������   To.Ӧ�÷�����
* �����б�
* ���ؽ����
*****************************************/
void *
Publisher_thread_PhysicalPorttoAPPLayer
(
	void * fd
)
{
	int i;
	
	//����socket��������·���
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
	
	//����socket�����ʹ�����
	int socket_sidsender;
	socket_sidsender = socket(AF_INET,SOCK_DGRAM,0);

	struct sockaddr_in addrTo;
	bzero(&addrTo,sizeof(addrTo));
	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons(NETLtoTRAL);
	addrTo.sin_addr.s_addr=inet_addr(LOOPIP);

	printf("UDP listening start...\n");

	int SIDlen;
	uint8_t SID[SIDLEN];
	uint8_t OFF[OFFLEN];

	uint8_t sendpkg[sizeof(CoLoR_get)];
	Trans_data * psendpkg;
	psendpkg = (Trans_data *)sendpkg;

	while(1)
	{
		memset(RecvBUF,0,sizeof(RecvBUF));

		SIDlen = 0;
		memset(SID,0,SIDLEN);
		memset(sendpkg,0,sizeof(CoLoR_get));

		//������·���
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

					if(PkgBUFTail>=10)
						PkgBUFTail -= 10;

					break;
				}
				
				if(PkgBUFTail>=10)
					PkgBUFTail -= 10;
			}
			pthread_mutex_unlock(&lockPkgBUF);
		}
		
		if(DEVETESTIMPL <= 1)
			printf("Publisher: A new UDP message received.\n");

		//Main Forwarding Procession with Routing Table
		if(strncmp(RecvBUF,"NET",3) == 0)
		{
			//����CoLoR��
			SIDlen = Publisher_CoLoR_ParsePacket(RecvBUF+3,SID);
			//�����Ͳ�����CoLoRЭ�飬���������û��IP��������ζ���յ���CoLoR���ݰ�ԭ�����������
			if(SIDlen <= 0)
				continue;

			//��װ��·�����SID,offset
			memcpy(psendpkg->sid,SID,SIDLEN);
			memcpy(psendpkg->offset,tempGet.offset,OFFLEN);

			//���ʹ�����
			sendto(socket_sidsender,sendpkg,sizeof(CoLoR_get),0,(struct sockaddr *)&addrTo,sizeof(addrTo));
			
			if(DEVETESTIMPL <= 1)
				printf("   Starting to search for the SID: %s\n",SID);
		}
	}
	close(socket_receiver);
	close(socket_sidsender);

	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�Publisher_thread_APPLayertoPhysicalPort
* ��������������ͨ�� - From.Ӧ�÷����� To.������������
* �����б�
* ���ؽ����
*****************************************/
void *
Publisher_thread_APPLayertoPhysicalPort
(
	void * fd
)
{
	int i,j;

	//�����ϲ㷢����DATA��Ӧ
	int socket_datareceiver;
	socket_datareceiver = socket(AF_INET,SOCK_DGRAM,0);

	struct sockaddr_in sin;
	int sockaddr_len=sizeof(struct sockaddr_in);
	bzero(&sin,sockaddr_len);
	sin.sin_family=AF_INET;
	//sin.sin_addr.s_addr=htonl(INADDR_ANY);
	sin.sin_addr.s_addr=inet_addr(LOOPIP);
	sin.sin_port=htons(TRALtoNETL);

	bind(socket_datareceiver,(struct sockaddr *)&sin,sockaddr_len);
	
	uint8_t datapkg[sizeof(CoLoR_data)+120];
	
	uint8_t SID[SIDLEN];
	uint8_t DATA[DATALEN];
	uint8_t offset[OFFLEN];

	uint8_t recvpkg[sizeof(Trans_data)];
	Trans_data * precvpkg;
	precvpkg = (Trans_data *)recvpkg;

	while(1)
	{
		memset(DATA,0,DATALEN);
		memset(offset,0,OFFLEN);
		
		memset(recvpkg,0,sizeof(Trans_data));
		memset(datapkg,0,sizeof(CoLoR_data)+120);

		recvfrom(socket_datareceiver,recvpkg,sizeof(Trans_data),0,(struct sockaddr *)&sin,&sockaddr_len);
		
		if(DEVETESTIMPL <= 1)
			printf("Publisher: A north to south UDP message received.\n");
		memcpy(DATA,precvpkg->data,DATALEN);
		memcpy(SID,precvpkg->sid,SIDLEN);
		memcpy(offset,precvpkg->offset,OFFLEN);
		
		//��װCoLoR-Data����
		Publisher_CoLoR_PacketData(SID,DATA,offset,datapkg);

		//��CoLoR-Data���ķ���������������
		Publisher_SendtoPhysicalPort(datapkg);

		/*
		i=0;
		j=0;
		//��ȡ����Ӧ�ò��������DATA��Ӧ��Ϣ
		if(strncmp(message,"GOT",3) == 0)
		{
			i+=2;
			for(j=0,i++;j<DATALEN;j++,i++)
			{
				if(1)
				{
					DATA[j] = message[i];
				}
				else
					break;
			}
			DATA[j] = 0;
			//printf("DATA = %s\n",DATA);

			i+=2;
			for(j=0;;j++)
			{
				i++;
				if(!(message[i]=='O' && message[i+1]=='F' && message[i+2]=='F'))
				{
					SID[j] = message[i];
				}
				else
					break;
			}
			SID[j] = 0;

			i+=2;
			for(j=0;j<OFFLEN;j++)
			{
				i++;
				offset[j] = message[i];

			}
			offset[j] = 0;
			
			//û���������յ�SID�Ͷ�Ӧ��DATA
			//if(DATA[0] == 0 || SID[0] == 0)
				//continue;
			
			//��װCoLoR-Data����
			Publisher_CoLoR_PacketData(SID,DATA,offset,datapkg);

			//��CoLoR-Data���ķ���������������
			Publisher_SendtoPhysicalPort(datapkg);
		}
		*/
	}
	close(socket_datareceiver);

	//�ر��߳�
	exit(0);
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�Publisher_main
* ����������Publisherģ�������������������̣߳������ṩʵ�ʹ���
* �����б��ڶ�����ʹ��char**argv�ķ�ʽ����������֪�ǲ����﷨����
* ���ؽ����
*****************************************/
//int main(int argc, char *argv[])
//Publisherģ��ֱ�ӵ���ģʽ
/*
int
Publisher_main
(
	int argc,
	char argv[][30]
)
*/
//Publisherģ���̵߳���ģʽ
void *
Publisher_main
(
	void * fd
)
{
	int i,j,k,l;

	Publisher_Parameterinit();

	//��Publisherģ������̵߳��õĲ��������ӿ�
	struct publisherinput * pubinput;
	pubinput = (struct publisherinput *)fd;

	int argc = pubinput->argc;
	char argv[50][30];
	memset(argv,0,1500);
	for(i=0;i<argc;i++)
	{
		strcpy(argv[i],pubinput->argv[i]);
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
		memset(FILEsidname,0,FILEnameL);
		memset(FILEpidname,0,FILEnameL);
		Publisher_configure_SIDfilepath(FILEsidname,domainID,deviceNA,deviceID);
		Publisher_configure_PIDfilepath(FILEpidname,domainID,deviceNA,deviceID);
	}

	FILE *fp;
	char ch=0;
	int file_i=0;
	
	//�ļ�1����ȡPIDע���ļ�
	char PIDbuf[PIDLENTH];
	char PIDcmd[PIDLENTH/32][32];

	if((fp=fopen(FILEpidname,"r"))==NULL)
	{
		printf("cannot open PID file!\n");
		exit(0);
	}

	file_i = 0;
	while ((ch=fgetc(fp))!=EOF)
		PIDbuf[file_i++]=ch;

	if(file_i == 0)
	{
		printf("FILEpid is empty!\n");
		fclose(fp);
		exit(0);
	}
	PIDbuf[--file_i] = 0;
	file_i = 0;

	fclose(fp);

	//Ϊ�����������
	i=0;j=0;k=0;l=0;
	while(1)
	{
		if(PIDbuf[i] == 10 || PIDbuf[i] == 0)
		{
			l=0;
			while(j<i)
			{
				PIDcmd[k][l++] = PIDbuf[j++];
			}
			PIDcmd[k][l] = 0;

			j++;
			k++;
		}
		if(PIDbuf[i] == 0)
		{
			break;
		}
		i++;
	}

	//i,j==totallength; k==numofcmds;
	//���н�����������
	int higherupsno=0;
	int PIDdidno=0;
	int PIDpidno=0;
	int PIDnidno=0;
	for(i=0,j=0;i<k;j=0,i++)
	{
		if(strncmp("higherups",PIDcmd[i],9) == 0)
		{
			continue;
		}
		else
		{
			pidlistcount++;
		
			//DID input
			for(PIDdidno=0;PIDcmd[i][j] != ' ' && PIDcmd[i][j] != 0;PIDdidno++,j++)
				pidlist[pidlistcount].did[PIDdidno] = PIDcmd[i][j];
			pidlist[pidlistcount].did[PIDdidno] = 0;

			j++;

			//PID input
			for(PIDpidno=0;PIDcmd[i][j] != ' ' && PIDcmd[i][j] != 0;PIDpidno++,j++)
				pidlist[pidlistcount].pid[PIDpidno] = PIDcmd[i][j];
			pidlist[pidlistcount].pid[PIDpidno] = 0;
		
			j++;

			//NID input
			for(PIDnidno=0;PIDcmd[i][j] != ' ' && PIDcmd[i][j] != 0;PIDnidno++,j++)
				pidlist[pidlistcount].nid[PIDnidno] = PIDcmd[i][j];
			pidlist[pidlistcount].nid[PIDnidno] = 0;
			
			continue;
		}
	}
	
	printf("[FILEpid]\n%s\n",PIDbuf);
	
	i=0;
	pidlistcount2 = pidlistcount;
	while(1)
	{
		strcpy(pidlist2[i].did,pidlist[i].did);
		strcpy(pidlist2[i].pid,pidlist[i].pid);
		strcpy(pidlist2[i].nid,pidlist[i].nid);
		i++;
		if(i == pidlistcount2 + 1)
			break;
	}

	//�ļ�2����ȡSID�����ļ�
	char SIDbuf[SIDLENTH];
	char SIDcmd[SIDLENTH/32][32];
	int sidlistcount=0;

	if((fp=fopen(FILEsidname,"r"))==NULL)
	{
		printf("cannot open SID file!\n");
		exit(0);
	}

	file_i = 0;
	while ((ch=fgetc(fp))!=EOF)
		SIDbuf[file_i++]=ch;

	if(file_i == 0)
	{
		printf("FILEpid is empty!\n");
		fclose(fp);
		exit(0);
	}
	SIDbuf[--file_i] = 0;
	file_i = 0;

	fclose(fp);

	//Ϊ�����������
	i=0;j=0;k=0;l=0;
	while(1)
	{
		if(SIDbuf[i] == 10 || SIDbuf[i] == 0)
		{
			l=0;
			while(j<i)
			{
				SIDcmd[k][l++] = SIDbuf[j++];
			}
			SIDcmd[k][l] = 0;

			j++;
			k++;
		}
		if(SIDbuf[i] == 0)
		{

			break;
		}
		i++;
	}

	//i,j==totallength; k==numofcmds;
	//���н�����������
	int SIDno=0;
	for(i=0,j=0;i<k;j=0,i++)
	{
		sidlistcount++;
		
		//SID shaping
		for(SIDno=0;SIDcmd[i][j] != ' ' && SIDcmd[i][j] != 0;SIDno++,j++)
			continue;
		bzero(SIDcmd[i]+j,32-j-1);
	}

	//�ļ�3����ȡIP�ڵ�������IP��ַ��Ӧ��ϵ�ļ�
	char FILEIPNODEname[100];
	strcpy(FILEIPNODEname,FILEIPNODE);

	char IPNODEbuf[FILEIPNODELENTH];
	char IPNODEcmd[FILEIPNODELENTH/50][50];

	if((fp=fopen(FILEIPNODEname,"r"))==NULL)
	{
		printf("Cannot open routingtable file!\n");
	}
	else
	{
		file_i = 0;
		while ((ch=fgetc(fp))!=EOF)
			IPNODEbuf[file_i++]=ch;
		if(file_i == 0)
		{
			fclose(fp);
			exit(0);
		}
		IPNODEbuf[--file_i] = 0;
		file_i = 0;
		fclose(fp);

		//Ϊ�����������
		i=0;j=0;k=0;l=0;
		while(1)
		{
			if(IPNODEbuf[i] == 10 || IPNODEbuf[i] == 0)
			{
				l=0;
				while(j<i)
				{
					IPNODEcmd[k][l++] = IPNODEbuf[j++];
				}
				IPNODEcmd[k][l] = 0;

				j++;
				k++;
			}
			if(IPNODEbuf[i] == 0)
			{
				break;
			}
			i++;
		}

		//i,j==totallength; k==numofcmds;
		//���н�����������
		int IPNODEnid=0;
		int IPNODEpp=0;
		int IPNODEip=0;
		for(i=0,j=0;i<k;j=0,i++)
		{
			ipnodelistcount++;
		
			//NID input
			for(IPNODEnid=0;IPNODEcmd[i][j] != ' ' && IPNODEcmd[i][j] != 0;IPNODEnid++,j++)
				ipnodelist[ipnodelistcount].nid[IPNODEnid] = IPNODEcmd[i][j];
			ipnodelist[ipnodelistcount].nid[IPNODEnid] = 0;
printf("ipnodelist[ipnodelistcount].nid == %s\n",ipnodelist[ipnodelistcount].nid);
		
			j++;
		
			//physical port input
			for(IPNODEpp=0;IPNODEcmd[i][j] != ' ' && IPNODEcmd[i][j] != 0;IPNODEpp++,j++)
				ipnodelist[ipnodelistcount].pp[IPNODEpp] = IPNODEcmd[i][j];
			ipnodelist[ipnodelistcount].pp[IPNODEpp] = 0;
printf("ipnodelist[ipnodelistcount].pp  == %s\n",ipnodelist[ipnodelistcount].pp);

			j++;
		
			//IP input
			for(IPNODEip=0;IPNODEcmd[i][j] != ' ' && IPNODEcmd[i][j] != 0;IPNODEip++,j++)
				ipnodelist[ipnodelistcount].ip[IPNODEip] = IPNODEcmd[i][j];
			ipnodelist[ipnodelistcount].ip[IPNODEip] = 0;
printf("ipnodelist[ipnodelistcount].ip  == %s\n",ipnodelist[ipnodelistcount].ip);
		}
	}
	
	//ȷ��������ý��
	printf("[FILEsid]\n%s\n",SIDbuf);
	printf("[FILEipnodelist]\n%s\n",IPNODEbuf);

	printf("Get packet listening...\n");



	//����thread_PhysicalPorttoAPPLayer���߳�
	pthread_t pthread_PhysicalPorttoAPPLayer;
	if(pthread_create(&pthread_PhysicalPorttoAPPLayer, NULL, Publisher_thread_PhysicalPorttoAPPLayer, NULL)!=0)
	{
		perror("Creation of recv thread failed.");
	}
	
	//����thread_APPLayertoPhysicalPort���߳�
	pthread_t pthread_APPLayertoPhysicalPort;
	if(pthread_create(&pthread_APPLayertoPhysicalPort, NULL, Publisher_thread_APPLayertoPhysicalPort, NULL)!=0)
	{
		perror("Creation of send thread failed.");
	}

	//����Ϊphysicalportģ�������ͨ�Ų�����������
	struct physicalportinput
	{
		char pp[30];
		char ip[16];
	}ppinput;

	pthread_t pthread_physicalport;

	memset(ppinput.pp,0,30);
	strcpy(ppinput.pp,argv[2]);
	strcpy(ppinput.ip,SwitchDevice_local_ip);

	//����physicalport���߳�
	if(pthread_create(&pthread_physicalport, NULL, thread_physicalport, (void *)&ppinput)!=0)
	{
		perror("Creation of physicalport thread failed.");
	}

	while(1)
	{
		sleep(10000);
	}

	/*
	int iRet = -1;
	int fd   = -1;
	
	if(PROTOCOL == 0)
	{printf("PROTOCOL == 0\n");
		//��ʼ��SOCKET
		fd = Publisher_Ethernet_InitSocket();
		if(0 > fd)
		{
			return -1;
		}
		//��RM����ע���
		struct hostent *host =NULL;
		struct sockaddr sa;
		int sockfd,len;
		uint8_t temp_ip[5];
		int sidnum;

		uint8_t* pkgregister;
		pkgregister = (uint8_t*)calloc(14+16+NIDLEN+SIDLEN*sidlistcount+PUBKEYLEN+SIGNATURELEN,sizeof(uint8_t));
		bzero(pkgregister,14+16+NIDLEN+SIDLEN*sidlistcount+PUBKEYLEN+SIGNATURELEN);
	
		Ether_CoLoR_register * pkg=0;
		pkg = (Ether_CoLoR_register *)pkgregister;
	
		//���ethernet����
		memcpy((char*)pkg->ether_dhost,(char*)destmac,6);
		memcpy((char*)pkg->ether_shost,(char*)localmac,6);
		//pkg.ether_type = htons ( ETHERTYPE_ARP );
		pkg->ether_type=htons(0x0800);
	
		//���CoLoR-data����
		pkg->version_type = 162;//�汾4λ������4λ����Ϊ���ó�CoLoR_Register��
		pkg->nid_len = NIDLEN;//NID����
		pkg->sid_n = sidlistcount;//SID����
		pkg->sid_len = SIDLEN;//SID����

		pkg->Public_key_len = PUBKEYLEN;
		pkg->signature_algorithm = 1;//ǩ���㷨
		pkg->options_static = 0;//�̶��ײ�ѡ��

		pkg->checksum = 0;//�����
		pkg->Sequence_number = 0;//���к�

		pkg->Sequence_number_ack = 0;//���к�_ack
		pkg->total_len = 16+NIDLEN+SIDLEN*sidlistcount+PUBKEYLEN+SIGNATURELEN;//�ܳ���
	
		memcpy(pkg->nid,"d1pub1",NIDLEN/2);
		bzero(pkg->nid+6,NIDLEN/2-6);
		memcpy(pkg->nid+NIDLEN/2,"d1rm",NIDLEN/2);
		bzero(pkg->nid+NIDLEN/2+4,NIDLEN/2-4);
	
		for(i=0;i<sidlistcount;i++)
			memcpy(pkg->nid+NIDLEN+i*SIDLEN,SIDcmd[i],SIDLEN);//SID input
	
		//ʵ��Ӧ��ʹ��PF_PACKET
		if((sockfd=socket(PF_PACKET,SOCK_PACKET,htons(ETH_P_ALL)))==-1)
		{
			fprintf(stderr,"Socket Error:%s\n\a",strerror(errno));
			return(0);
		}
	
		memset(&sa,0,sizeof(sa));
		strcpy(sa.sa_data,PhysicalPort);
	
		len=sendto(sockfd,pkg,14+pkg->total_len,0,&sa,sizeof(sa));//����Data����mac��㲥
		printf(">>>CoLoR-Register to  RM.\n");//�������register����ʾ
		if(len!=14+pkg->total_len)//������ͳ�����ʵ�ʰ���ƥ�䣬����ʧ��
		{
			fprintf(stderr,"Sendto Error:%s\n\a",strerror(errno));
			close(sockfd);
			return (0);
		}
	
		close(sockfd);
		free(pkgregister);
	
		//�������ݰ�����ѭ����
		Publisher_Ethernet_StartCapture(fd);
	}
	else if(PROTOCOL == 1)
	{printf("PROTOCOL == 1\n");
		//��ʼ��SOCKET
		fd = Publisher_UDP_InitSocket();
		if(0 > fd)
		{
			return -1;
		}

		//�������ݰ�����ѭ����
		Publisher_UDP_StartCapture(fd);
	}

	//�ر�SOCKET
	close(fd);
	*/
	
	exit(0);
}
