/*******************************************************************************************************************************************
* �ļ�����physicalport.c
* �ļ�������973�ǻ�Эͬ����SARϵͳTestBed�׼�����������/��ʵ��������������
* ��������ģ���������������Ϊ������ͨ��ѧ��һ�������������豸���ҹ���ʵ���������������̼�����ѵר�á������������ڴ�֣�����������ļ�������������ѵ��ģ������Ϊ���˾����뾭��Ĳ������Դ�Ĵ�����ʽ��Ϊ���߱��������˽���һ��һ���ڣ��κθ��˲����������ת����չʾ����Դ���ݡ��κ��������ѧԱ����������������ֱ����Ҫ��������������Ҫ����֮Ȩ������������Щ��Դ�����漰ʵ������Ŀ���ܣ��ݲ���������ר���������������Ȩ�����ˣ��ʸ�����Դ����ʵ�����ڲ�һ��һ���ţ��緢���й������߱��˵�һƬ�������Ϊ�����߽���������ԭ���Խ���׷�顢��֤�����ߺ������Ȩ��������Դ�ķ���Ȩ�����߱������У���������ܽ���̽�͸�޳����׵��ȳ���Ϊ���ѧҵ���ౡ֮������ϣ�ԸѧԱ�������ߵ��Ͷ��ɹ���лл������
* ���ߣ�������
* ��ݣ�������ͨ��ѧ��һ�������������豸���ҹ���ʵ���� 2013��˶�������о���
* E-mail. hellozxwang@foxmail.com
* Mobile. 18811774990
* QQ    . 535667240
* Addr  . �����к�������ֱ���ⱱ����ͨ��ѧ��е¥D706��, 100044
*******************************************************************************************************************************************/
/*******************************************************************************************************************************************
*****����˵����1.������������֡�����MAC�㲢��·�ɲ��ϴ�SAR/CoLoR�����ݰ���2.��·�ɲ���մ��������ݰ�����װMAC�㲢����************************
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

#include"physicalport.h"

//Ethernet���������жϵ��ֶΣ���ֹ���̶��ײ�������Version/Type�ֶ�ΪIP����CoLoR����ͨ�ã�
Ether_VersionType tempEtherVersionType;

//CoLoRЭ�����������жϵ��ֶΣ���ֹ���̶��ײ�������Version/Type�ֶ�ΪGet����Data����Register����ͨ�ã�
Ether_CoLoR_VersionType tempCoLoRVersionType;

//CoLoRЭ�����������жϵ��ֶΣ���ֹ���̶��ײ�������Version/Type�ֶ�ΪGet����Data����Register����ͨ�ã�
CoLoR_VersionType tempCoLoRwithoutMAC;

//CoLoRЭ��Get���ײ���PID֮ǰ���ֶγ��ȹ̶������ڷ�װ
//��δ�õ�

//CoLoRЭ��Get���ײ���PID֮ǰ���ֶγ��ȿɱ䣬���ڽ���
Ether_CoLoR_get_parse tempGet;

//CoLoRЭ��Data���ײ���PID֮ǰ���ֶγ��ȹ̶������ڷ�װ
//��δ�õ�

//CoLoRЭ��Data���ײ���PID֮ǰ���ֶγ��ȿɱ䣬���ڽ���
Ether_CoLoR_data_parse tempData;

//CoLoRЭ��Register���ײ���PID֮ǰ��
CoLoR_register_parse tempRegister;

/*******************************************************************************************************************************************
*******************************************��ʼ�������������********************************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�PhysicalPort_Parameterinit
* ����������physicalportģ��ȫ�ֱ�������ֵ
* �����б�
* ���ؽ����
*****************************************/
void
PhysicalPort_Parameterinit()
{
	int i;
	for(i=0;i<48;i++)
		selfpacketdonotcatch[i] = 0;

	//��ʼ���������DATA���������
	for(i=0;i<10;i++)
	{
		datapkglist[i].flag_occupied = 0;
		datapkglist[i].pkglength = 0;
		datapkglist[i].CNFthreadnum = -1;
		memset(datapkglist[i].physicalport,0,30);
		memset(datapkglist[i].local_ip,0,16);
		memset(datapkglist[i].sid,0,SIDLEN);
		memset(datapkglist[i].offset,0,OFFLEN);
		memset(datapkglist[i].sbd,0,SBDLEN);
		memset(datapkglist[i].datapkg,0,MTU);
	}
	//��ʼ���շ������б�
	for(i=0;i<10;i++)
	{
		CacheList[i].flag_occupied = 0;
		CacheList[i].flag_ack = 0;
		CacheList[i].datapkglocation = -1;
		CacheList[i].cachetop = 0;
		memset(CacheList[i].physicalport,0,30);
		memset(CacheList[i].sid,0,SIDLEN);
		memset(CacheList[i].sbd,0,SBDLEN);
		memset(CacheList[i].offset,0,OFFLEN);
		memset(CacheList[i].cache,0,CACHESIZE);
	}
	//��ʼ��CNF�������
	for(i=0;i<10;i++)
	{
		CNFThreadList[i].flag_occupied = 0;
		CNFThreadList[i].flag_IO = -1;
		CNFThreadList[i].flag_close = 0;
		CNFThreadList[i].datapkglocation = -1;
		CNFThreadList[i].cachelocation = -1;
		CNFThreadList[i].cachelength = -1;
		memset(CNFThreadList[i].physicalport,0,30);
		memset(CNFThreadList[i].physicalportIP,0,16);
		memset(CNFThreadList[i].sid,0,SIDLEN);
		memset(CNFThreadList[i].sbd,0,SBDLEN);
		memset(CNFThreadList[i].offset,0,OFFLEN);

	}

	if(CacheList[0].flag_occupied == 0)//��һ�����δ��ռ�ã�˵����ת���ڵ�Ϊ�����ʼ���ڵ㣨�������ʼ����������һ�������У���һ�����һ���ᱻռ�ã���δ��ռ���ǲ����ܳ��ֵ����Σ�
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

	memset(local_mac,0,7);
	memset(dest_mac,0,7);
	memset(dest_ip,0,16);
	
	uint8_t bm1[7]={0xff,0xff,0xff,0xff,0xff,0xff,0x00};
	memcpy(broad_mac1,bm1,7);
	uint8_t bm2[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	memcpy(broad_mac2,bm2,7);
}

/*****************************************
* �������ƣ�PhysicalPort_Configure
* ����������PhysicalPortģ���ʼ������
* �����б�
* ���ؽ����
*****************************************/
int
PhysicalPort_Configure
(
	char * PhysicalPort,
	uint8_t * LocalMAC,
	char * local_ip
)
{
	FILE *fp;
	char ch=0;
	int file_i=0;

	//��ȡ������������ĩβ���
	int i;
	int numfound=0;
	char portnum[3]={0};
	for(i=29;i>=0;i--)
	{
		if(PhysicalPort[i] >= '0' && PhysicalPort[i] <= '9' && numfound == 0)
		{
			numfound=1;
			portnum[1]=PhysicalPort[i];
			continue;
		}
		if(PhysicalPort[i] >= '0' && PhysicalPort[i] <= '9' && numfound == 1)
		{
			numfound=2;
			portnum[0]=PhysicalPort[i];
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

	//��ȡ����MAC
	struct ifreq ifreq;
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(0 > sock)
	{
		printf("PhysicalPort MAC getting failed (socket failed).\n");
		exit(0);
	}
	strcpy(ifreq.ifr_name,PhysicalPort);
	if(0 > ioctl(sock,SIOCGIFHWADDR,&ifreq))
	{
		printf("PhysicalPort MAC getting failed (ioctl failed).\n");
		exit(0);
	}
	//��������MAC
	memcpy(LocalMAC,(uint8_t *)ifreq.ifr_hwaddr.sa_data,6);
	if(GLOBALTEST == 2)//�������ʵ��������ϵ���
	memcpy(LocalMAC,PPSMAC,6);

	//���ñ���IP
	strcpy(local_ip,dest_ip);
	for(i=0;i<16;i++)
	{
		if(local_ip[i] == 0)
		{
			strcpy(local_ip+i,portnum);
			break;
		}
	}

	//ȷ��������ý��
	printf("[Configure]local_ip  == %s\n",local_ip);
	printf("[Configure]dest_ip   == %s\n",dest_ip);

	return 1;
}

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.ת��ƽ�� To.��������**************************************************************
*******************************************************************************************************************************************/

/*****************************************

* �������ƣ�PhysicalPort_CoLoR_SeeType
* �����������ж��Ƿ�Ϊ��Ϊ����CoLoR��ͷ
* �����б�
* ���ؽ����
*****************************************/
int
PhysicalPort_CoLoR_SeeType
(
	const CoLoR_VersionType * pkg
)
{
	if (NULL == pkg)
	{
		return 0;
	}

	unsigned int type=0;

	if(pkg->version_type>=160 && pkg->version_type<=164)
		return 10;
	
	return 0;
}

/*****************************************
* �������ƣ�PhysicalPort_Ethernet_Sendpkg
* �������������������ڷ���MAC+CoLoR���ݰ�
* �����б�
* ���ؽ����
*****************************************/
int
PhysicalPort_Ethernet_Sendpkg
(
	char * PhysicalPort,
	int portnum,
	char * LocalMAC,
	char * dest_mac,
	char * ip,
	char * dest,
	char * message,
	unsigned int messagelength
)
{
	int i,j;
	char pkg[BUFSIZE]={0};
	struct hostent *host =NULL;
	struct sockaddr sa;
	int sockfd,len;
	uint8_t temp_ip[5];
	memset((char *)&pkg,0,sizeof(pkg));
	
	//���ethernet����
	memcpy(pkg,dest_mac,6);
	memcpy(pkg+6,LocalMAC,6);
	pkg[12] = (char)(htons(0x0800)%256);
	pkg[13] = (char)(htons(0x0800)/256);
	memcpy(pkg+14,message,messagelength);

	//ʵ��Ӧ��ʹ��PF_PACKET
	if((sockfd = socket(PF_PACKET/*PF_INET*/,SOCK_PACKET,htons(ETH_P_ALL)))==-1)
	{
		fprintf(stderr,"Socket Error:%s\n\a",strerror(errno));
		return 0;
	}
	
	memset(&sa,0,sizeof(sa));
	strcpy(sa.sa_data,PhysicalPort);
	
	selfpacketdonotcatch[portnum]=1;

	if(pysicalportDEVETESTIMPL <= 1)
	{
		printf("[%s] Ethernet pkg length == %d\n",PhysicalPort,messagelength+14);
		printf("[%s] Packet sent out to Physical Port.\n",PhysicalPort);//�����ʾ
		printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");
	}	

	len = sendto(sockfd,&pkg,messagelength+14,0,&sa,sizeof(sa));//���Ͱ���mac��
	close(sockfd);

	if(len != messagelength+14)//������ͳ�����ʵ�ʰ���ƥ�䣬����ʧ��
	{
		fprintf(stderr,"Sendto Error:%s\n\a",strerror(errno));
		return 0;
	}
	return 1;
}

/*****************************************
* �������ƣ�PhysicalPort_UDP_Sendpkg
* �������������������ڷ���MAC+IP+UDP+CoLoR���ݰ�
* �����б�
* ���ؽ����
ע�⣺����Registerû����ȷ��Ŀ��NID��Ϊ��ά���������չ�ԣ�Ҫ������CoLoR����ʽ����������
*****************************************/
int
PhysicalPort_UDP_Sendpkg
(
	char * PhysicalPort,
	char * packet,
	int packetlength
)
{
	if(pysicalportDEVETESTIMPL <= 1)
		printf("[%s] PhysicalPort_UDP_Sendpkg()\n",PhysicalPort);

	int i;
	
	CoLoR_VersionType *pkg = NULL;
	char destNID[NIDLEN];
	memset(destNID,0,NIDLEN);
	
	//��ȡ����ת��ƽ������ݰ���Ŀ��NID
	pkg = (CoLoR_VersionType * )packet;
	
	if(pkg->version_type == 160)//����ʾIP�汾��Ϊ10��������Ϊ0��ר��CoLoR-Get��  �ֶ�Ϊ1010 0000���İ�
	{
		if(pysicalportDEVETESTIMPL <= 1)
			printf("[%s] This is a GET pkt from forwarding plane.\n",PhysicalPort);

		memcpy(destNID,packet+16+pkg->sid_len+pkg->nid_len/2,pkg->nid_len/2);

		if(pysicalportDEVETESTIMPL <= 1)
			printf("[%s] destNID == %s\n",PhysicalPort,destNID);
	}
	if(pkg->version_type == 161)//����ʾIP�汾��Ϊ10��������Ϊ1��ר��CoLoR-Data�� �ֶ�Ϊ1010 0001���İ�
	{
		if(pysicalportDEVETESTIMPL <= 1)
			printf("[%s] This is a DATA pkt from forwarding plane.\n",PhysicalPort);

		memcpy(destNID,packet+16+pkg->sid_len+pkg->nid_len/2,pkg->nid_len/2);

		if(pysicalportDEVETESTIMPL <= 1)
			printf("[%s] destNID == %s\n",PhysicalPort,destNID);
	}
	if(pkg->version_type == 162)//����ʾIP�汾��Ϊ10��������Ϊ2��ר��CoLoR-Register�� �ֶ�Ϊ1010 0010���İ�
	{
		if(pysicalportDEVETESTIMPL <= 1)
			printf("[%s] This is a REGISTER pkt from forwarding plane.\n",PhysicalPort);

		memcpy(destNID,packet+16+((int)*(packet+1))/2,((int)*(packet+1))/2);

		if(pysicalportDEVETESTIMPL <= 1)
			printf("[%s] destNID == %s\n",PhysicalPort,destNID);
	}

	//����destNID����Ŀ��IP��ַ
	char ipaddr[16];
	memset(ipaddr,0,16);
	
	for(i=0;i<=ipnodelistcount;i++)
	{
		if(strncmp(destNID,ipnodelist[i].nid,NIDLEN) == 0)
		{
			strcpy(ipaddr,ipnodelist[i].ip);
			break;
		}
	}
	if(pysicalportDEVETESTIMPL <= 1)
		printf("[%s] ipaddr == %s\n",PhysicalPort,ipaddr);

	//��װ������MAC+IP+UDP+CoLoR����
	int udpsender;

	udpsender = socket(AF_INET,SOCK_DGRAM,0);

	struct sockaddr_in addrTo = {0};
	//bzero(&addrTo,sizeof(addrTo));
	
	addrTo.sin_family = AF_INET;
	addrTo.sin_port = htons(6000);
	addrTo.sin_addr.s_addr = inet_addr(ipaddr);

	sendto(udpsender,packet,packetlength,0,(struct sockaddr *)&addrTo,sizeof(addrTo));
	close(udpsender);

	if(pysicalportDEVETESTIMPL <= 1)
	{
		printf("[%s] Packet sent out to the dest IP.\n",PhysicalPort);//�����ʾ
		printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");
	}

	return 1;
}

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.�������� To.ת��ƽ��**************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�PhysicalPort_Ethernet_SetPromisc
* ����������������������ģʽ���Բ���
* �����б�
* ���ؽ����
*****************************************/
int
PhysicalPort_Ethernet_SetPromisc
(
	const char * pcIfName,
	int fd,
	int iFlags
)
{
	int iRet = -1;
	struct ifreq stIfr;
	
	//��ȡ�ӿ����Ա�־λ
	strcpy(stIfr.ifr_name,pcIfName);
	iRet = ioctl(fd,SIOCGIFFLAGS,&stIfr);
	if(0 > iRet)
	{
		perror("[Error]Get Interface Flags");   
		return -1;
	}
	
	if(0 == iFlags)
	{
		//ȡ������ģʽ
		stIfr.ifr_flags &= ~IFF_PROMISC;
	}
	else
	{
		//����Ϊ����ģʽ
		stIfr.ifr_flags |= IFF_PROMISC;
	}
	
	//���ýӿڱ�־
	iRet = ioctl(fd,SIOCSIFFLAGS,&stIfr);
	if(0 > iRet)
	{
		perror("[Error]Set Interface Flags");
		return -1;
	}
	
	return 0;
}

/*****************************************
* �������ƣ�PhysicalPort_Ethernet_InitSocket
* ��������������RAW�׽���
* �����б�
* ���ؽ����
*****************************************/
int
PhysicalPort_Ethernet_InitSocket
(
	char * PhysicalPort
)
{
	int iRet = -1;
	int fd = -1;
	struct ifreq stIf;
	struct sockaddr_ll stLocal = {0};
	
	//����SOCKET
	fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	if (0 > fd)
	{
		perror("[Error]Initinate L2 raw socket");
		return -1;
	}
	
	//��������ģʽ����
	PhysicalPort_Ethernet_SetPromisc(PhysicalPort,fd,1);
	
	//����SOCKETѡ��
	int rawlength = BUFSIZE;
	iRet = setsockopt(fd,SOL_SOCKET,SO_RCVBUF,&rawlength,sizeof(int));
	if (0 > iRet)
	{
		perror("[Error]Set socket option");
		close(fd);
		return -1;
	}
	
	//��ȡ���������ӿ�����
	strcpy(stIf.ifr_name,PhysicalPort);
	iRet = ioctl(fd,SIOCGIFINDEX,&stIf);
	if (0 > iRet)
	{
		perror("[Error]Ioctl operation");
		close(fd);
		return -1;
	}
	
	//����������
	stLocal.sll_family = PF_PACKET;
	stLocal.sll_ifindex = stIf.ifr_ifindex;
	stLocal.sll_protocol = htons(ETH_P_ALL);
	iRet = bind(fd,(struct sockaddr *)&stLocal,sizeof(stLocal));
	if (0 > iRet)
	{
		perror("[Error]Bind the interface");
		close(fd);
		return -1;
	}
	
	return fd;   
}

/*****************************************
* �������ƣ�PhysicalPort_UDP_InitSocket
* ��������������UDP�׽���
* �����б�
* ���ؽ����
*****************************************/
int
PhysicalPort_UDP_InitSocket
(
	char * PhysicalPort
)
{
	int iRet = -1;
	int fd = -1;
	struct sockaddr_in sin = {0};
	
	//����SOCKET
	fd = socket(AF_INET,SOCK_DGRAM,0);
	if (0 > fd)
	{
		perror("[Error]Initinate L4 UDP socket");
		return -1;
	}

	int i,j;
	char physicalportIP[16];
	//��������IP
	for(i=0;i<=ipnodelistcount;i++)
	{
		if(strncmp(ipnodelist[i].nid,NID,NIDLEN) == 0)
		{
			for(j=i;strncmp(ipnodelist[j].nid,NID,NIDLEN) == 0;j++)
			{
				if(strncmp(ipnodelist[j].pp,PhysicalPort,30) == 0)
				{
					strcpy(physicalportIP,ipnodelist[j].ip);
					break;
				}
			}
			break;
		}
	}

	if(pysicalportDEVETESTIMPL <= 1)
		printf("[%s] physicalportIP == %s\n",PhysicalPort,physicalportIP);
	
	//����������
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(physicalportIP);
	sin.sin_port = htons(6000);
	
	iRet = bind(fd,(struct sockaddr *)&sin,sizeof(sin));
	if (0 > iRet)
	{
		perror("[Error]Bind the interface");
		close(fd);
		return -1;
	}
	
	return fd; 
}

/*****************************************

* �������ƣ�PhysicalPort_Ethernet_StartCapture
* ����������RAW�׽��ֲ�����������֡
* �����б�
* ���ؽ����
*****************************************/
void
PhysicalPort_Ethernet_StartCapture
(
	const int fd,
	char * PhysicalPort,
	int portnum,
	uint8_t * LocalMAC,
	char * local_ip
)
{
	//���ջ�����
	int EnBUFlength = BUFSIZE;
	uint8_t EnBUF[BUFSIZE];

	int iRet = -1;
	socklen_t stFromLen = 0;

	int packetcount=1;
	
	int cancelrepetition=0;//����δ֪ԭ��ÿһ������������ʵ����ݰ����ᱻ�������Ρ����趨���ڵ�ÿ�����հ���һ����Ч������Ȩ��֮�ƣ�û�и������⡣
	printf("[%s] South interface listening start... (RAW Socket to Physical Port: %s)\n",PhysicalPort,PhysicalPort);
	//ѭ������
	while(1)
	{
		//��ս��ջ�����
		memset(EnBUF, 0, BUFSIZE);
		
		//��������֡
		iRet = recvfrom(fd, EnBUF, EnBUFlength, 0, NULL, &stFromLen);

		//���մ���
		if (0 > iRet)
		{
			continue;
		}

		//��һʱ�䶪���Լ����������ݰ�
		//����һ�����򵥴ֱ���ֱ�Ӷ��������ϴη�������ġ����������İ�����Ϊ�м���������Լ�������
		/*
		if(selfpacketdonotcatch[portnum] == 1)
		{
			if(pysicalportDEVETESTIMPL <= 1)
				printf("Drop the pkg sent by myself.\n");
			selfpacketdonotcatch[portnum] = 0;
			continue;
		}
		*/
		//�������������ж�ԴMAC��ַ�ķ��������Լ������İ�
		if(strncmp(EnBUF+6,LocalMAC,6) == 0)
		{
			if(pysicalportDEVETESTIMPL <= 1)
				printf("Drop the pkg sent by myself.\n");
			continue;
		}

		//����°�������ʾ
		if(pysicalportDEVETESTIMPL <= 1)
		{
			printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
			printf("[%s] Physical Port report for new RAW Packet / No. %d\n",PhysicalPort,packetcount++);
		}

		//��������֡���
		//printf("[Ethernet]New Packet Received. Noooo.%d:\n",packetcount++);

		//if(cancelrepetition == 0)
		if(1)
		{
			//��������֡
			PhysicalPort_Ethernet_ParseFrame(EnBUF,PhysicalPort,local_ip);
			cancelrepetition=1;
		}
		else
		{
			cancelrepetition=0;
		}
	}
}

/*****************************************
* �������ƣ�PhysicalPort_UDP_StartCapture
* ����������UDP�׽��ֲ�����������֡
* �����б�
* ���ؽ����
*****************************************/
void
PhysicalPort_UDP_StartCapture
(
	const int fd,
	char * PhysicalPort,
	int portnum,
	char * local_ip
)
{
	//���ջ�����
	int EnBUFlength = BUFSIZE;
	uint8_t EnBUF[BUFSIZE];

	int iRet = -1;
	int packetcount=1;

	int sockaddr_len = sizeof(struct sockaddr_in);

	printf("[%s] South interface listening start... (UDP Socket to Physical Port: %s)\n",PhysicalPort,PhysicalPort);
	//ѭ������
	while(1)
	{
		//��ս��ջ�����
		//memset(EnBUF, 0, BUFSIZE);
		bzero(EnBUF,BUFSIZE);
		
		//��������֡
		iRet = recvfrom(fd, EnBUF+14, EnBUFlength-14, 0, NULL, &sockaddr_len);

		//���մ���
		if (0 > iRet)
		{
			continue;
		}

		//����°�������ʾ
		if(pysicalportDEVETESTIMPL <= 1)
		{
			printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
			printf("[%s] Physical Port report for new UDP Packet / No. %d\n",PhysicalPort,packetcount++);
		}

		//��������֡���
		//printf("[Ethernet]New Packet Received. Noooo.%d:\n",packetcount++);

		//���ethernet����
		//memcpy(EnBUF,dest_mac,6);
		//memcpy(EnBUF+6,local_mac,6);
		EnBUF[12] = (char)(htons(0x0800)%256);
		EnBUF[13] = (char)(htons(0x0800)/256);

		//��������֡�����յ�UDP-CoLOR��ԭ������MAC�㣬�ٷ�һ����
		PhysicalPort_Ethernet_ParseFrame(EnBUF,PhysicalPort,local_ip);
	}
}

/*****************************************
* �������ƣ�PhysicalPort_Ethernet_ParseFrame
* ��������������֡��������������������MAC��
* �����б�
* ���ؽ����
*****************************************/
int
PhysicalPort_Ethernet_ParseFrame
(
	uint8_t * EnBUF,
	char * PhysicalPort,
	char * local_ip
)
{
	static int packetcount=1;
	//��鱾��mac��IP��ַ
	/*
	memset ( local_mac,0,sizeof ( local_mac ) );
	memset ( local_ip,0,sizeof ( local_ip ) );
	memset ( dest_ip,0,sizeof ( dest_ip ) );
	
	if ( GetLocalMac ( PhysicalPort,local_mac,local_ip ) ==-1 )
		return ( -1 );
	*/
	
	int iEther = 0;
	int iRet = -1;
	
	/*
	struct <A name=ether_header < a>ether_header {
	u_char ether_dhost[ETHER_ADDR_LEN];
	u_char ether_shost[ETHER_ADDR_LEN];
	u_short ether_type;
	};
	*/
	
	Ether_VersionType *pEtherVT = NULL;
	//struct Ether_CoLoR_VersionType *pCoLoRVT = NULL;
	
	//���յ���ԭʼ��������ֵΪ��̫��ͷ
	pEtherVT = (Ether_VersionType*)EnBUF;
	
	//�ж���̫������֡����
	iEther = PhysicalPort_Ethernet_SeeType(pEtherVT);

	if(iEther == 10)//�յ�ָ�򱾶˿�MAC��IP���������Э��ΪCoLoR
	{
		if(pysicalportDEVETESTIMPL <= -1)
		{
			int dd=0;
			char c;
			printf("\n");
			while(dd<68)
			{
				c = *(EnBUF+dd);
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

			//pCoLoRVT = (struct Ether_CoLoR_VersionType*)EnBUF;
			//pkglength = pCoLoRVT->total_len;
		}
		if(pysicalportDEVETESTIMPL <= 1)
			printf("[%s] New CoLoR packet received. No.%d:\n",PhysicalPort,packetcount++);

		//����CoLoR��
		iRet = PhysicalPort_CoLoR_Parsepacket(EnBUF,PhysicalPort,local_ip);
	}
	else if(iEther == 4)//�յ�ָ�򱾶˿�MAC��IP���������Э��ΪIPv4
	{
		if(pysicalportDEVETESTIMPL <= 1)
			printf("[%s] New IPv4 packet received. No.%d\n",PhysicalPort,packetcount++);
	}
	else if(iEther == 6)//�յ�ָ�򱾶˿�MAC��IP���������Э��ΪIPv6
	{
		if(pysicalportDEVETESTIMPL <= 1)
			printf("[%s] New IPv6 packet received. No.%d\n",PhysicalPort,packetcount++);
	}
	else if(iEther == -10)//�յ��Ĳ���ָ�򱾶˿�MAC��IP���������Э��ΪCoLoR
	{
		if(pysicalportDEVETESTIMPL <= -1)
		{
			int dd=0;
			char c;
			printf("\n");
			while(dd<68)
			{
				c = *(EnBUF+dd);
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

			//iRet = CoLoR_Parsepacket(EnBUF);
		}
		if(pysicalportDEVETESTIMPL <= 1)
			printf("[%s] Strange CoLoR packet received. No.%d\n",PhysicalPort,packetcount++);
	}
	else if(iEther == -4)//�յ��Ĳ���ָ�򱾶˿�MAC��IP���������Э��ΪIPv4
	{
		if(pysicalportDEVETESTIMPL <= 1)
			printf("[%s] Strange IPv4 packet received. No.%d\n",PhysicalPort,packetcount++);
	}
	else if(iEther == -6)//�յ��Ĳ���ָ�򱾶˿�MAC��IP���������Э��ΪIPv6
	{
		if(pysicalportDEVETESTIMPL <= 1)
			printf("[%s] Strange IPv6 packet received. No.%d\n",PhysicalPort,packetcount++);
	}
	else
	{
		if(pysicalportDEVETESTIMPL <= 1)
			printf("[%s] Strange packet received. Protocol unknown. No.%d\n",PhysicalPort,packetcount++);
	}
	return iRet;
}

/*****************************************
* �������ƣ�PhysicalPort_Ethernet_SeeType
* �����������ж��Ƿ�Ϊ��Ϊ������̫����ͷ
* �����б�
* ���ؽ����
*****************************************/
int
PhysicalPort_Ethernet_SeeType
(
	const Ether_VersionType * pkg
)
{
	if (NULL == pkg)
	{
		return 0;
	}
	//����Ϊ�жϣ�ȷ�������������ݰ������Լ������ģ����Ա����������Զ˽��в��Ե�ʱ���뽫pysicalportGLOBALTEST�ж�ֵ��Ϊ0ֵ
	/*
	struct <A name=ether_header < a>ether_header {
	u_char ether_dhost[ETHER_ADDR_LEN];
	u_char ether_shost[ETHER_ADDR_LEN];
	u_short ether_type;
	};
	*/

	/*���mac��ַ��ƥ�����
	int qq=0;

	for(qq=0;qq<6;qq++)
	{
		printf("pkg->ether_dhost[%d] == %d\n",qq,pkg->ether_dhost[qq]);
		printf("local_mac[%d]        == %d\n",qq,local_mac[qq]);
	}
	if((strncmp((uint8_t*)pkg->ether_dhost,(uint8_t*)local_mac ,6)==0) || 
			    (strncmp((uint8_t*)pkg->ether_dhost,(uint8_t*)broad_mac1,6)==0) || 
			    (strncmp((uint8_t*)pkg->ether_dhost,(uint8_t*)broad_mac2,6)==0))
	{
		printf("pre-mac same!\n");
	}
	*/
	unsigned int type=0;
	if(pysicalportGLOBALTEST == 0)
	{
		if(((strncmp((uint8_t*)pkg->ether_dhost,(uint8_t*)local_mac ,6)==0) || 1 ||
		    (strncmp((uint8_t*)pkg->ether_dhost,(uint8_t*)broad_mac1,6)==0) || 
		    (strncmp((uint8_t*)pkg->ether_dhost,(uint8_t*)broad_mac2,6)==0)) && 
		   (pkg->ether_type == 8))//0x0800��ô��ȡ�������8�Ƿ������µģ���Ϊֻ�ܶ���8
		{
			type = pkg->version_type/16;
			return type;
		}
		else if(pkg->ether_type == 8)
		{
			type = pkg->version_type/16;
			return -type;
		}
	}
	else if(pysicalportGLOBALTEST >= 1)
	{
		if(pkg->ether_type == 8)
		{
			type = pkg->version_type/16;
			return type;
		}
	}
	
	return 0;
}

/*****************************************
* �������ƣ�PhysicalPort_CoLoR_Parsepacket
* ��������������CoLoR��ͷ
* �����б�
* ���ؽ����
*****************************************/
int
PhysicalPort_CoLoR_Parsepacket
(
	const char * pkg,
	char * PhysicalPort,
	char * local_ip
)
{
	int i;
	
	char* p;
	p = (char*)pkg;
	
	if (NULL == pkg)
	{
		return -1;
	}
	
	//����CoLoR�������ֶ�����λ�û�ȡ��������ֵ
	Ether_CoLoR_VersionType * pCoLoRVT = NULL;
	pCoLoRVT = (Ether_CoLoR_VersionType *)pkg;
	unsigned int pkglength;

	if(pysicalportDEVETESTIMPL <= 1)
		printf("[%s] pCoLoRVT->version_type=%d\n",PhysicalPort,pCoLoRVT->version_type);

	//ǿ���޸İ����ԣ�����У�����������ڱ��������
	/*
	if(CNFTCP == 0)
	{
		pCoLoRVT->options_static = 32;
	}
	else
	{
		pCoLoRVT->options_static = 0;
	}
	*/

	//��ȡ������
	if(pCoLoRVT->version_type == 161 && pCoLoRVT->options_static % 64 / 32 == 1)//����յ��İ���CNF����DATA��
	{
		pkglength = pCoLoRVT->total_len;
		PhysicalPort_CNF_NewClientThread(pkg,pkglength,PhysicalPort,local_ip);
		return 0;
	}
	else if(pCoLoRVT->version_type == 163 || pCoLoRVT->version_type == 164)//����յ��İ���CNF-SEQ/ACK��
	{
		pkglength = pCoLoRVT->total_len;
		PhysicalPort_CNF_SendMessage(pkg,pkglength,pCoLoRVT->version_type,PhysicalPort,local_ip);
		return 0;
	}
	else if((pCoLoRVT->version_type == 160 || pCoLoRVT->version_type == 161) && pCoLoRVT->options_static % 64 / 32 == 0)//����յ��İ��Ƿǻ���GET/DATA��
	{
		pkglength = pCoLoRVT->total_len;

		if(pysicalportDEVETESTIMPL <= 1)
			printf("[%s] pkglength == %d\n",PhysicalPort,pkglength);

		//��װ�����ϲ㷢����Ϣ
		if(INTRACOMMUNICATION == 0)
			PhysicalPort_UDP_SendMessage(pkg,pkglength,PhysicalPort,local_ip);
		else if(INTRACOMMUNICATION == 1)
			PhysicalPort_SEM_SendMessage(pkg,pkglength,PhysicalPort,local_ip);
	}
	else if(pCoLoRVT->version_type == 162)//����յ��İ���ע���
	{
		CoLoR_register_parse * pRegisterlen = NULL;
		pRegisterlen = (CoLoR_register_parse *)(pkg+14);
		pkglength = pRegisterlen->total_len;

		if(pysicalportDEVETESTIMPL <= 1)
			printf("[%s] pkglength == %d\n",PhysicalPort,pkglength);

		//��װ�����ϲ㷢����Ϣ
		if(INTRACOMMUNICATION == 0)
			PhysicalPort_UDP_SendMessage(pkg,pkglength,PhysicalPort,local_ip);
		else if(INTRACOMMUNICATION == 1)
			PhysicalPort_SEM_SendMessage(pkg,pkglength,PhysicalPort,local_ip);
	}
	else//�հ��İ汾�����ֶδ����޷��ж�����
	{
		printf("Error: PhysicalPort_CoLoR_Parsepacket() - Recv a bad packet!\n");
		exit(0);
	}
	
	return 0;
}

/*****************************************
* �������ƣ�PhysicalPort_CNF_NewClientThread
* ���������������µ�CNF-Client����ʵ��
* �����б�
* ���ؽ����
*****************************************/
void
PhysicalPort_CNF_NewClientThread
(
	const uint8_t * originalbuf,
	const unsigned int pkglength,
	char * PhysicalPort,
	char * local_ip
)
{
		//����ʱ��ڵ㣨΢�룩		
		gettimeofday(&utime,NULL);
		printf("<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<\n");
		printf("==================Time==================\nClient Connection Start   %ld us\n========================================\n",1000000 * utime.tv_sec + utime.tv_usec);

	int i;
	//����DATA��
	Ether_CoLoR_data * datapkg = NULL;
	datapkg = (Ether_CoLoR_data *)originalbuf;
	
	//���л���DATA��
	for(i=0;i<10;i++)
	{
		if(datapkglist[i].flag_occupied == 0)
		{
			datapkglist[i].flag_occupied = 1;
			datapkglist[i].pkglength = pkglength;
			memcpy(datapkglist[i].physicalport,PhysicalPort,30);
			memcpy(datapkglist[i].local_ip,local_ip,16);
			memcpy(datapkglist[i].sid,datapkg->sid,SIDLEN);
			memcpy(datapkglist[i].offset,datapkg->offset,OFFLEN);
			memcpy(datapkglist[i].datapkg,originalbuf,pkglength);
			break;
		}
	}
	//����ж�
	if(i >= 10)
	{
		printf("[PhysicalPort - CNF Client Init]datapkglist Over Loaded!\n");
		exit(0);
	}

	int DATAPKGLocation = i;
	
	//����ȡ���õ�CNF�����
	for(i=0;i<10;i++)
	{
		if(CacheList[i].flag_occupied == 0)
		{
			CacheList[i].flag_occupied = 1;
			CacheList[i].datapkglocation = DATAPKGLocation;
			memcpy(CacheList[i].physicalport,PhysicalPort,30);
			memcpy(CacheList[i].sid,datapkg->sid,SIDLEN);
			memcpy(CacheList[i].offset,datapkg->offset,OFFLEN);
			break;
		}
	}
	//����ж�
	if(i >= 10)
	{
		printf("[PhysicalPort - CNF Client Init]CacheList Over Loaded!\n");
		exit(0);
	}
	
	int CacheLocation = i;
	
	//����ȡ���õ�CNF�������
	for(i=0;i<10;i++)
	{
		if(CNFThreadList[i].flag_occupied == 0)
		{
			CNFThreadList[i].flag_occupied = 1;
			CNFThreadList[i].flag_IO = 0;
			CNFThreadList[i].datapkglocation = DATAPKGLocation;
			CNFThreadList[i].cachelocation = CacheLocation;
			memcpy(CNFThreadList[i].physicalport,PhysicalPort,30);
			memcpy(CNFThreadList[i].physicalportIP,local_ip,16);
			memcpy(CNFThreadList[i].sid,datapkg->sid,SIDLEN);
			memcpy(CNFThreadList[i].offset,datapkg->offset,OFFLEN);
			break;
		}
	}
	//����ж�
	if(i >= 10)
	{
		printf("[PhysicalPort - CNF Client Init]CNFThreadList Over Loaded!\n");
		exit(0);
	}

	int CNFThreadNum = i;
	
	datapkglist[DATAPKGLocation].CNFthreadnum = CNFThreadNum;
	
	//������CNF-Client����ʵ��
	struct cnfcliinput cnfinput;

	cnfinput.threadnum = CNFThreadNum;
	cnfinput.cachelocation = CacheLocation;
	cnfinput.cachelength = CACHESIZE;
	memcpy(cnfinput.physicalportIP,local_ip,16);
	memcpy(cnfinput.sid,datapkg->sid,SIDLEN);
	memcpy(cnfinput.offset,datapkg->offset,OFFLEN);
	
	pthread_t pthread_cnfclient;
	
	//�������߳�
	if(pthread_create(&pthread_cnfclient, NULL, CNFClient_main, (void *)&cnfinput)!=0)
	{
		perror("Creation of CNF Client main thread failed.");
	}
}

/*****************************************
* �������ƣ�PhysicalPort_CNF_NewServerThread
* ���������������µ�CNF-Server����ʵ��
* �����б�
* ���ؽ����
*****************************************/
void
PhysicalPort_CNF_NewServerThread
(
	const uint8_t * originalbuf,
	const unsigned int pkglength,
	char * PhysicalPort,
	char * local_ip
)
{
		//����ʱ��ڵ㣨΢�룩		
		gettimeofday(&utime,NULL);
		printf(">->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->->\n");
		printf("==================Time==================\nServer Connection Start   %ld us\n========================================\n",1000000 * utime.tv_sec + utime.tv_usec);

	int i;
	//����DATA��
	CoLoR_data * datapkg = NULL;
	datapkg = (CoLoR_data *)originalbuf;
	
	//����ȡ��Ҫ��ȡ���ݵ�CNF�����
	if(DEVETESTIMPL <= 6)
	{
		printf("[Init]datapkg->sid    = %s\n",datapkg->sid);
		printf("[Init]datapkg->offset = %s\n",datapkg->offset);
	}
	
	for(i=0;i<10;i++)
	{
		if(DEVETESTIMPL <= 6)
		{
			printf("[Init]CacheList[%d].sid    = %s\n",i,CacheList[i].sid);
			printf("[Init]CacheList[%d].offset = %s\n",i,CacheList[i].offset);
		}
		
		if(strncmp(CacheList[i].sid,datapkg->sid,SIDLEN)==0 && strncmp(CacheList[i].offset,datapkg->offset,OFFLEN)==0)
		{
			break;
		}
	}
	//����ж�
	if(i >= 10)
	{
		printf("[PhysicalPort - CNF Server Init]CacheList Over Loaded!\n");
		exit(0);
	}

	int CacheLocation = i;
	
	//����ȡ���õ�CNF�������
	for(i=0;i<10;i++)
	{
		if(CNFThreadList[i].flag_occupied == 0)
		{
			CNFThreadList[i].flag_occupied = 1;
			CNFThreadList[i].flag_IO = 1;
			CNFThreadList[i].datapkglocation = -1;
			CNFThreadList[i].cachelocation = CacheLocation;
			memcpy(CNFThreadList[i].physicalport,PhysicalPort,30);
			memcpy(CNFThreadList[i].physicalportIP,local_ip,16);
			memcpy(CNFThreadList[i].sid,datapkg->sid,SIDLEN);
			memcpy(CNFThreadList[i].offset,datapkg->offset,OFFLEN);

			if(DEVETESTIMPL <= 6)
			{
				printf("==O==ThreadNum = %d\n",i);
				printf("flag_occupied  = %d\n",CNFThreadList[i].flag_occupied);
				printf("flag_IO        = %d\n",CNFThreadList[i].flag_IO);
				printf("datapkglocation= %d\n",CNFThreadList[i].datapkglocation);
				printf("cachelocation  = %d\n",CNFThreadList[i].cachelocation);
				printf("physicalport   = %s\n",CNFThreadList[i].physicalport);
				printf("physicalportIP = %s\n",CNFThreadList[i].physicalportIP);
				printf("sid            = %s\n",CNFThreadList[i].sid);
				printf("offset         = %s\n",CNFThreadList[i].offset);
				printf("===================\n");
				printf("PhysicalPort   = %s\n",PhysicalPort);
				printf("datapkg->sid   = %s\n",datapkg->sid);
				printf("datapkg->offset= %s\n",datapkg->offset);
				printf("===================\n");
			}

			break;
		}
	}
	//����ж�
	if(i >= 10)
	{
		printf("[PhysicalPort - CNF Server Init]CNFThreadList Over Loaded!\n");
		exit(0);
	}

	int CNFThreadNum = i;
	
	//CNF�̴߳�������
	struct cnfserinput cnfinput;
	//������CNF-Server����ʵ��
	cnfinput.threadnum = CNFThreadNum;
	cnfinput.cachelocation = CacheLocation;
	cnfinput.cachelength = CACHESIZE;
	memcpy(cnfinput.physicalportIP,local_ip,16);
	memcpy(cnfinput.sid,datapkg->sid,SIDLEN);
	memcpy(cnfinput.offset,datapkg->offset,OFFLEN);
	
	if(DEVETESTIMPL <= 6)
	{
		printf("[NEW]local_ip = %s\n",local_ip);
		printf("[NEW]cnfinput.threadnum = %d\n",cnfinput.threadnum);
		printf("[NEW]CacheLocation = %d\n",CacheLocation);
	}
	
	pthread_t pthread_cnfserver;
	
	//�������߳�
	if(pthread_create(&pthread_cnfserver, NULL, CNFServer_main, (void *)&cnfinput)!=0)
	{
		perror("Creation of CNF Client main thread failed.");
	}
	sem_wait(&CNFThreadList[CNFThreadNum].StartParaTransEnd);
}

/*****************************************
* �������ƣ�PhysicalPort_CNF_SendMessage
* ������������CNF����ʵ������CNF������
* �����б�
* ���ؽ����
*****************************************/
void
PhysicalPort_CNF_SendMessage
(
	const uint8_t * originalbuf,
	const unsigned int pkglength,
	int versiontype,
	char * PhysicalPort,
	char * local_ip
)
{
	int i;
	int ThreadNum;
	//����CNF��
	int flag_occupied;
	int flag_IO;
	int datapkglocation;
	int cachelocation;
	unsigned long cachelength;
	uint8_t physicalport[30];
	uint8_t sid[SIDLEN];
	uint8_t sbd[SBDLEN];
	uint8_t offset[OFFLEN];
	if(versiontype == 163)//���CNF��ΪSEQ��
	{
		//����SEG��
		Ether_CoLoR_seg * segpkg = NULL;
		segpkg = (Ether_CoLoR_seg *)originalbuf;
		
		//����ƥ���ֶ�
		memcpy(sid,segpkg->sid,SIDLEN);
		memcpy(offset,segpkg->offset,OFFLEN);
		
		//����ƥ�������б���ȡĿ��˿ںŵ�ƫ����
		for(i=0;i<10;i++)
		{
			if(CNFThreadList[i].flag_occupied == 1
			&& CNFThreadList[i].flag_IO == 0
			&& strncmp(CNFThreadList[i].physicalport,PhysicalPort,30) == 0
			&& strncmp(CNFThreadList[i].sid,sid,SIDLEN) == 0
			&& strncmp(CNFThreadList[i].offset,offset,OFFLEN) == 0)
			{
				break;
			}
		}
//ȱ������ж�
		ThreadNum = i;
	}
	else if(versiontype == 164)//���CNF��ΪACK��
	{
		//����ACK��
		Ether_CoLoR_ack * ackpkg = NULL;
		ackpkg = (Ether_CoLoR_ack *)originalbuf;
		
		//����ƥ���ֶ�
		memcpy(sid,ackpkg->sid,SIDLEN);
		memcpy(offset,ackpkg->offset,OFFLEN);
		
		//����ƥ�������б���ȡĿ��˿ںŵ�ƫ����
		for(i=0;i<10;i++)
		{
			if(CNFThreadList[i].flag_occupied == 1
			&& CNFThreadList[i].flag_IO == 1
			&& strncmp(CNFThreadList[i].physicalport,PhysicalPort,30) == 0
			&& strncmp(CNFThreadList[i].sid,sid,SIDLEN) == 0
			&& strncmp(CNFThreadList[i].offset,offset,OFFLEN) == 0)
			{
				break;
			}
		}
//ȱ������ж�
		ThreadNum = i;
	}
	/*
	printf("==S==ThreadNum = %d\n",0);
	printf("flag_occupied  = %d\n",CNFThreadList[0].flag_occupied);
	printf("flag_IO        = %d\n",CNFThreadList[0].flag_IO);
	printf("datapkglocation= %d\n",CNFThreadList[0].datapkglocation);
	printf("cachelocation  = %d\n",CNFThreadList[0].cachelocation);
	printf("physicalport   = %s\n",CNFThreadList[0].physicalport);
	printf("sid            = %s\n",CNFThreadList[0].sid);
	printf("offset         = %s\n",CNFThreadList[0].offset);
	printf("===================\n");
	printf("PhysicalPort   = %s\n",PhysicalPort);
	printf("sid            = %s\n",sid);
	printf("offset         = %s\n",offset);
	printf("===================\n");
	*/

	//����ƥ�������б���ȡĿ��IP�Ͷ˿ں�

	//����CNF��Ϣ
	int socket_sender;
	socket_sender=socket(AF_INET,SOCK_DGRAM,0);
	
	struct sockaddr_in addrTo;
	bzero(&addrTo,sizeof(addrTo));
	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons(PhysicalportInterPORT + ThreadNum * CNFPORTRANGE);
	addrTo.sin_addr.s_addr=inet_addr(LOOPIP);

	sendto(socket_sender,originalbuf+14,pkglength,0,(struct sockaddr *)&addrTo,sizeof(addrTo));
	close(socket_sender);
}

/*****************************************
* �������ƣ�PhysicalPort_UDP_SendMessage
* �������������ϲ�ת��ƽ�淢��UDP��װ�����ݰ�����
* �����б�
* ���ؽ����
*****************************************/
void
PhysicalPort_UDP_SendMessage
(
	const uint8_t * originalbuf,
	const unsigned int pkglength,
	char * PhysicalPort,
	char * local_ip
)
{
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
	memcpy(pkg+3,originalbuf+14,pkglength);
	
	struct sockaddr_in addrTo;
	bzero(&addrTo,sizeof(addrTo));
	addrTo.sin_family=AF_INET;
	addrTo.sin_port=htons(PhysicalportInterPORT);
	addrTo.sin_addr.s_addr=inet_addr(dest_ip);
	
	if(pysicalportDEVETESTIMPL <= 1)
	{
		//printf("[%s] UDP Message has been sent out:%s\n",PhysicalPort,pkg);
		printf("[%s] UDP Message has been sent out.\n",PhysicalPort);
		printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
	}

	sendto(socket_sender,pkg,pkglength+3,0,(struct sockaddr *)&addrTo,sizeof(addrTo));
	close(socket_sender);
}

/*****************************************
* �������ƣ�PhysicalPort_SEM_SendMessage
* �������������ϲ�ת��ƽ�淢��SEM��װ�����ݰ�����
* �����б�
* ���ؽ����
*****************************************/
void
PhysicalPort_SEM_SendMessage
(
	const uint8_t * originalbuf,
	const unsigned int pkglength,
	char * PhysicalPort,
	char * local_ip
)
{
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

	//���ݰ�����ת��������У��˴���Ҫ�߳�����
	pthread_mutex_lock(&lockPkgBUF);
	int LocalPkgBUFHead;
	if(PkgBUF[PkgBUFHead].flag_occupied == 0)
	{
		LocalPkgBUFHead = PkgBUFHead;
		PkgBUFHead++;
		if(PkgBUFHead >=10)
			PkgBUFHead -= 10;
		
		PkgBUF[LocalPkgBUFHead].flag_occupied = 1;
		PkgBUF[LocalPkgBUFHead].pkglength = pkglength;
		strcpy(PkgBUF[LocalPkgBUFHead].sourcepp,PhysicalPort);
		memcpy(PkgBUF[LocalPkgBUFHead].pkg,originalbuf+14,pkglength);
		
		//����������ź���
		sem_post(&semPacketRECV);

		if(pysicalportDEVETESTIMPL <= 1)
		{
			//printf("[%s] SEM Message has been sent to NetworkLayer:%s\n",PhysicalPort,originalbuf+14);
			printf("[%s] SEM Message has been sent to NetworkLayer.\n",PhysicalPort);
			printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
		}
	}
	else
	{
		if(pysicalportDEVETESTIMPL <= 1)
		{
			//printf("[%s] SEM Message failed to be sent to NetworkLayer:%s\n",PhysicalPort,originalbuf+14);
			printf("[%s] SEM Message failed to be sent to NetworkLayer.\n",PhysicalPort);
			printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
		}
	}
	pthread_mutex_unlock(&lockPkgBUF);
}

/*******************************************************************************************************************************************
**********************************************************���߳����ɳ���********************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�PhysicalPort_thread_send
* ��������������ͨ�� - From.ת��ƽ�� To.��������
* �����б�
* ���ؽ����
*****************************************/
void *
PhysicalPort_thread_send
(
	void * fd
)
{
	//�����β�
	struct pplip * pi = (struct pplip *)fd;
	char * PhysicalPort = pi->pp;
	uint8_t * LocalMAC = pi->lmac;
	char * local_ip = pi->ip;

	int i;
	
	//��ȡ���������ڱ��
	int portnum = -1;
	for(i = 30;i>=0;i--)
	{
		if(strncmp(PhysicalPort+i,"eth",3) == 0)
		{
			if(PhysicalPort[i+4] > '0' && PhysicalPort[i+4] < '9')
			{
				portnum = ((int)PhysicalPort[i+3] - 48)*10 + ((int)PhysicalPort[i+4] - 48);
			}
			else
				portnum = (int)PhysicalPort[i+3] - 48;
			break;
		}
	}
	printf("[%s] PhysicalPort_thread_send() - portnum == %d\n",PhysicalPort,portnum);

	//���ͻ�����
	int ExBUFlength = BUFSIZE;
	uint8_t ExBUF[BUFSIZE];

	static int packetcount=1;
	
	int socket_FORPreceiver;
	socket_FORPreceiver = socket(AF_INET,SOCK_DGRAM,0);
	
	struct sockaddr_in sin;
	int sockaddr_len=sizeof(sin);
	bzero(&sin,sockaddr_len);
	sin.sin_family=AF_INET;
	//sin.sin_addr.s_addr=htonl(INADDR_ANY);
	sin.sin_addr.s_addr=inet_addr(local_ip);
	sin.sin_port=htons(PhysicalportInterPORT);
	
	bind(socket_FORPreceiver,(struct sockaddr *)&sin,sockaddr_len);
	
	printf("[%s] North interface listening start... (UDP Socket to Forwarding Plane: %s)\n",PhysicalPort,local_ip);


	int iCoLoR = 0;
	int iRet = -1;
	
	int LocalPkgBUFSearch=0;
	int LocalPkgBUFTail;
	while(1)
	{
		memset(ExBUF,0,BUFSIZE);

		if(INTRACOMMUNICATION == 0)
			recvfrom(socket_FORPreceiver,ExBUF,sizeof(ExBUF),0,(struct sockaddr *)&sin,&sockaddr_len);//printf("1\n");
		else if(INTRACOMMUNICATION == 1)
		{
			sem_wait(&semPacketSEND[portnum]);
			pthread_mutex_lock(&lockPkgBUF);
			for(;;)
			{
				if(PkgBUF[LocalPkgBUFSearch++].destppnum == portnum)
				{
					LocalPkgBUFTail = LocalPkgBUFSearch-1;

					ExBUFlength = PkgBUF[LocalPkgBUFTail].pkglength;
					memcpy(ExBUF,"FOR",3);
					memcpy(ExBUF+3,PkgBUF[LocalPkgBUFTail].pkg,ExBUFlength);

					//���ת��������ռ��
					memset(PkgBUF[LocalPkgBUFTail].sourcepp,0,30);
					memset(PkgBUF[LocalPkgBUFTail].pkg,0,MTU);
					PkgBUF[LocalPkgBUFTail].destppnum = 0;
					PkgBUF[LocalPkgBUFTail].pkglength = 0;
					PkgBUF[LocalPkgBUFTail].flag_occupied = 0;

					if(LocalPkgBUFSearch>=10)
						LocalPkgBUFSearch -= 10;
					break;
				}

				if(LocalPkgBUFSearch>=10)
					LocalPkgBUFSearch -= 10;
			}
			pthread_mutex_unlock(&lockPkgBUF);
		}

		if(pysicalportDEVETESTIMPL <= 1)
		{
			printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n");
			//printf("[%s] Got a UDP message:%s\n",PhysicalPort,ExBUF);
			printf("[%s] Got a UDP message.\n",PhysicalPort);
		}

		if(strncmp(ExBUF,"FOR",3) == 0)//�յ������ϼ�ת��ƽ���ת����
		{//printf("2\n");
			iCoLoR = 0;
			iRet = -1;
	
			CoLoR_VersionType * pCoLoRVT = NULL;
			pCoLoRVT = (CoLoR_VersionType *)(ExBUF+3);
			
			//�ж�CoLoR����֡����
			iCoLoR = PhysicalPort_CoLoR_SeeType(pCoLoRVT);

			if(iCoLoR == 10)//�յ�ָ�򱾶˿�MAC��IP���������Э��ΪCoLoR
			{//printf("3\n");
				//��ת���İ�������CNFЭ���DATA����������CNF-Server����ʵ��
				if(pCoLoRVT->version_type == 161 && pCoLoRVT->options_static % 64 / 32 == 1)
				{//printf("4\n");
					PhysicalPort_CNF_NewServerThread(ExBUF+3,pCoLoRVT->total_len,PhysicalPort,local_ip);
				}
				
				if(pysicalportDEVETESTIMPL <= 1)
					printf("[%s] A CoLoR packet will be sent. No.%d:\n",PhysicalPort,packetcount++);

				if(pysicalportDEVETESTIMPL <= 1)
					printf("[%s] pCoLoRVT->version_type=%d\n",PhysicalPort,pCoLoRVT->version_type);

				//�������ݰ�������Ϣ
				if(pCoLoRVT->version_type == 162)
				{
					CoLoR_register_parse *pRegisterlen = NULL;
					pRegisterlen = (CoLoR_register_parse*)(ExBUF+3);
					ExBUFlength = pRegisterlen->total_len;
				}
				else
				{
					ExBUFlength = pCoLoRVT->total_len;
				}

				if(pysicalportDEVETESTIMPL <= 1)
					printf("[%s] ExBUFlength == %d\n",PhysicalPort,ExBUFlength);

				//���������ڷ������ݰ�
				if(pysicalportPROTOCOL == 0)
					PhysicalPort_Ethernet_Sendpkg(PhysicalPort,portnum,LocalMAC,dest_mac,local_ip,dest_ip,ExBUF+3,ExBUFlength);
				else if(pysicalportPROTOCOL == 1)
					PhysicalPort_UDP_Sendpkg(PhysicalPort,ExBUF+3,ExBUFlength);
			}
			else
			{
				if(pysicalportDEVETESTIMPL <= 1)
					printf("[%s] A Strange forwarding UDP message received. Protocol unknown. No.%d\n",PhysicalPort,packetcount++);
			}
		}
		else
		{
			if(pysicalportDEVETESTIMPL <= 1)
				printf("[%s] A Strange UDP message received. Not forwarding. No.%d\n",PhysicalPort,packetcount++);
		}
	}
	//�ر��߳�
	close(socket_FORPreceiver);
	//exit(0);
	free(fd);
	pthread_exit(NULL);
}

/*****************************************
* �������ƣ�PhysicalPort_thread_recv
* ��������������ͨ�� - From.�������� To.ת��ƽ��
* �����б�
* ���ؽ����
*****************************************/
void *
PhysicalPort_thread_recv
(
	void * fd
)
{
	//�����β�
	struct pplip * pi = (struct pplip *)fd;
	char * PhysicalPort = pi->pp;
	uint8_t * LocalMAC = pi->lmac;
	char * local_ip = pi->ip;

	int i;

	//��ȡ���������ڱ��
	int portnum = -1;
	for(i = 30;i>=0;i--)
	{
		if(strncmp(PhysicalPort+i,"eth",3) == 0)
		{
			if(PhysicalPort[i+4] > '0' && PhysicalPort[i+4] < '9')
			{
				portnum = ((int)PhysicalPort[i+3] - 48)*10 + ((int)PhysicalPort[i+4] - 48);
			}
			else
				portnum = (int)PhysicalPort[i+3] - 48;
			break;
		}
	}
	printf("[%s] PhysicalPort_thread_send() - portnum == %d\n",PhysicalPort,portnum);

	int socket_recv = -1;
	if(pysicalportPROTOCOL == 0)
	{
		//��ʼ��SOCKET
		socket_recv = PhysicalPort_Ethernet_InitSocket(PhysicalPort);
		if(socket_recv < 0)
		{
			printf("[%s] Failed to Initiate Socket! Existing...\n",PhysicalPort);
			exit(0);
		}
	
		//�������ݰ�����ѭ����
		PhysicalPort_Ethernet_StartCapture(socket_recv,PhysicalPort,portnum,LocalMAC,local_ip);
	}
	else if(pysicalportPROTOCOL == 1)
	{
		//��ʼ��SOCKET
		socket_recv = PhysicalPort_UDP_InitSocket(PhysicalPort);
		if(socket_recv < 0)
		{
			printf("[%s] Failed to Initiate Socket! Existing...\n",PhysicalPort);
			exit(0);
		}

		PhysicalPort_UDP_StartCapture(socket_recv,PhysicalPort,portnum,local_ip);
	}
	
	//�ر�SOCKET
	close(socket_recv);
}

/*****************************************
* �������ƣ�thread_physicalport
* ����������PhysicalPortģ��������
* �����б�
* ���ؽ����
*****************************************/
//int main(int argc, char *argv[])
//int thread_physicalport(char * SouthboundInterface, char * NorthboundInterface)
void *
thread_physicalport
(
	void * argv
)
{
	//ȫ�ֱ�������ֵ
	PhysicalPort_Parameterinit();

	int i,j,k,l;

	struct physicalportinput * ppinput;
	ppinput = (struct physicalportinput *)argv;
	//printf("[RECV]ppinput.pp == %s\n",ppinput->pp);

	char PhysicalPort[30]={0};
	
	strcpy(PhysicalPort,ppinput->pp);
	strcpy(dest_ip,     ppinput->ip);

	//��ȡ�����ļ���
	if(pysicalportDEVETESTIMPL <= 0)
	{
		char sysfile[15] = "/proc/self/exe";
		char selfname[256];
		memset(selfname,0,256);
		int selfnamelen=256;
		if (-1 != readlink(sysfile,selfname,selfnamelen))
		{
			printf("[%s] Self Name == %s\n",PhysicalPort,selfname);
		}
	}

	FILE *fp;
	char ch=0;
	int file_i=0;
	//��ȡ�����ļ�
	//�ļ�1����ȡIP�ڵ�������IP��ַ��Ӧ��ϵ�ļ�
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
			return 0;
		}
		IPNODEbuf[--file_i] = '\0';
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
	printf("[FILEipnodelist]\n%s\n",IPNODEbuf);
	//printf("[input]SwitchDevice_local_ip  == %s\n",SwitchDevice_local_ip);
	
	//���ñ���
	int configsuccess = 0;
	char local_ip[16]={0};
	uint8_t LocalMAC[6]={0};
	configsuccess = PhysicalPort_Configure(PhysicalPort,LocalMAC,local_ip);
	if(configsuccess == -1)
	{
		printf("[%s] Failed to Read Configure File! Existing...\n",PhysicalPort);
		exit(0);
	}
	if(configsuccess == 0)
	{
		printf("[%s] No Required Information in Configure File! Existing...\n",PhysicalPort);
		exit(0);
	}
	//���MAC��ַ��ȡ���
	/*
	printf("LocalMAC = %x:%x:%x:%x:%x:%x\n",
	LocalMAC[0],
	LocalMAC[1],
	LocalMAC[2],
	LocalMAC[3],
	LocalMAC[4],
	LocalMAC[5]);
	*/

	//�����߳���������
	struct pplip pi;
	strcpy(pi.pp,PhysicalPort);
	strcpy(pi.lmac,LocalMAC);
	strcpy(pi.ip,local_ip);

	//�������з������߳�
	pthread_t pthread_send;
	if(pthread_create(&pthread_send, NULL, PhysicalPort_thread_send, (void *)&pi)!=0)
	{
		perror("Creation of send thread failed.");
	}

	//�������н������߳�
	pthread_t pthread_recv;
	if(pthread_create(&pthread_recv, NULL, PhysicalPort_thread_recv, (void *)&pi)!=0)
	{
		perror("Creation of recv thread failed.");
	}
	
	//��ѭ�����������ṩʵ�ʹ���
	while(1)
	{
		sleep(10000);
	}
	
	return 0;
}
