/*******************************************************************************************************************************************
* �ļ�����browserclient.c
* �ļ�������973�ǻ�Эͬ����SARϵͳTestBed�׼��������������Э��ת�Ӽ����϶����������������¶�������������ˣ�Data Subscriber��
* ��������ģ���������������Ϊ������ͨ��ѧ��һ�������������豸���ҹ���ʵ���������������̼�����ѵר�á������������ڴ�֣�����������ļ�������������ѵ��ģ������Ϊ���˾����뾭��Ĳ������Դ�Ĵ�����ʽ��Ϊ���߱��������˽���һ��һ���ڣ��κθ��˲����������ת����չʾ����Դ���ݡ��κ��������ѧԱ����������������ֱ����Ҫ��������������Ҫ����֮Ȩ������������Щ��Դ�����漰ʵ������Ŀ���ܣ��ݲ���������ר���������������Ȩ�����ˣ��ʸ�����Դ����ʵ�����ڲ�һ��һ���ţ��緢���й������߱��˵�һƬ�������Ϊ�����߽���������ԭ���Խ���׷�顢��֤�����ߺ������Ȩ��������Դ�ķ���Ȩ�����߱������У���������ܽ���̽�͸�޳����׵��ȳ���Ϊ���ѧҵ���ౡ֮������ϣ�ԸѧԱ�������ߵ��Ͷ��ɹ���лл������
* ���ߣ�������
* ��ݣ�������ͨ��ѧ��һ�������������豸���ҹ���ʵ���� 2013��˶�������о���
* E-mail. hellozxwang@foxmail.com
* Mobile. 18811774990
* QQ    . 535667240
* Addr  . �����к�������ֱ���ⱱ����ͨ��ѧ��е¥D706��, 100044
*******************************************************************************************************************************************/
/*******************************************************************************************************************************************
*****����˵����1.�䵱�϶����������ı���HTTP������
**************2.���¶���������ˣ�Data Subscriber���ṩ�������SID������DATA��������������HTTPЭ�鷵��
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
gcc browserclient.c -o browserclient -lpthread
4�����У����漰ԭʼ�׽��ֵ�ʹ�ã���rootȨ�ޣ�
sudo ./browserclient
*/

#include"browserclient.h"

/*******************************************************************************************************************************************
*************************************�궨����������************ȫ�ֱ�������******************************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************��ʼ�������������********************************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�BrowserClient_Parameterinit
* ����������BrowserClientģ��ȫ�ֱ�������ֵ
* �����б�
* ���ؽ����
*****************************************/
void
BrowserClient_Parameterinit()
{
	//HTTP_Server���������뺯��
	memset(buffer,0,MAXBUF+1);

	host       = 0;
	port       = 0;
	back       = 0;
	dirroot    = 0;
	logdir     = 0;
	daemon_y_n = 0;

	logfp      = 0;
}

/*******************************************************************************************************************************************
*******************************************Ӧ�ò㣨HTTP������������browserclient��ѭ�����úͿ��ƣ�***********************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�BrowserClient_dir_up
* ��������������dirpath��ָĿ¼����һ��Ŀ¼
* �����б�
* ���ؽ����
*****************************************/
char *
BrowserClient_dir_up
(
	char * dirpath
)
{
	static char Path[MAXPATH];
	int len;
	
	strcpy(Path, dirpath);
	len = strlen(Path);
	if (len > 1 && Path[len - 1] == '/')
		len--;
	while (Path[len - 1] != '/' && len > 1)
		len--;
	Path[len] = 0;
	return Path;
}


/*****************************************
* �������ƣ�BrowserClient_AllocateMemory
* ��������������ռ䲢��d��ָ�����ݸ���
* �����б�
* ���ؽ����
*****************************************/
void
BrowserClient_AllocateMemory
(
	char ** s,
	int l,
	char * d
)
{
	*s = malloc(l + 1);
	bzero(*s, l + 1);
	memcpy(*s, d, l);
}

/*****************************************
* �������ƣ�BrowserClient_GiveResponse
* ������������Path��ָ�����ݷ��͵�client_sockȥ
* �����б�
* ���ؽ����
���Path��һ��Ŀ¼�����г�Ŀ¼����
���Path��һ���ļ����������ļ�
*****************************************/
void
BrowserClient_GiveResponse
(
	FILE * client_sock,
	char * Path
)
{
	int i=0;
	int j=0;
	char str[100];
	memset(str,0,100);
	
	
	struct dirent *dirent;
	struct stat info;
	char Filename[MAXPATH];
	DIR *dir;
	int fd, len, ret;
	char *p, *realPath, *realFilename, *nport;
	
	//���ʵ�ʹ���Ŀ¼���ļ�
	len = strlen(dirroot) + strlen(Path) + 1;
	realPath = malloc(len + 1);
	bzero(realPath, len + 1);
	sprintf(realPath, "%s/%s", dirroot, Path);
	
	//���ʵ�ʹ����˿�
	len = strlen(port) + 1;
	nport = malloc(len + 1);
	bzero(nport, len + 1);
	sprintf(nport, ":%s", port);
	
	//����յ�������·��������
	if (stat(realPath, &info))
	{
		/*
		�ڴ˴����SIDת�Ӵ���
		����realPath��ΪSID
		����ӿڣ�SID
		�������CoLoRЭ��GET����װ����
		�����ʽ��1���ļ���ȡ�����ӳٽϴ󲢲����飩��2��socket���ͱ��ػػ���Ϣ
		*/
		//����socket
		//�׽ӿ�������
		int socket_sidsender;
		socket_sidsender = socket(AF_INET,SOCK_DGRAM,0);
		
		struct sockaddr_in addrTo;
		bzero(&addrTo,sizeof(addrTo));
		addrTo.sin_family=AF_INET;
		addrTo.sin_port=htons(APPLtoTRAL);
		//unsigned long IPto = 2130706433;//�ػ���ַ���� == 2130706433
		//addrTo.sin_addr.s_addr=htonl(IPto);//htonl�������ֽ���ת��Ϊ�����ֽ���
		addrTo.sin_addr.s_addr=inet_addr(LOOPIP);//htonl�������ֽ���ת��Ϊ�����ֽ���

		//����SID����
		sprintf(temp,"GET%s",SID);
		sendto(socket_sidsender,temp,sizeof(temp),0,(struct sockaddr *)&addrTo,sizeof(addrTo));
		close(socket_sidsender);

		if(DEVETESTIMPL <= 1)
			printf("   The HTTP Client asked for Data whitch match the SID: %s\n",SID);
		
		//��ѭ������������Դ�ɹ�������
		int socket_datareceiver;
		socket_datareceiver = socket(AF_INET,SOCK_DGRAM,0);

		struct sockaddr_in sin;
		int sockaddr_len=sizeof(struct sockaddr_in);
		bzero(&sin,sockaddr_len);
		sin.sin_family=AF_INET;
		//sin.sin_addr.s_addr=htonl(INADDR_ANY);
		sin.sin_addr.s_addr=inet_addr(LOOPIP);
		sin.sin_port=htons(TRALtoAPPL);


		bind(socket_datareceiver,(struct sockaddr *)&sin,sockaddr_len);
		
		int flag_sidgot = 0;
		int counter_sidget = 0;

		if(DEVETESTIMPL <= 1)
			printf("HTTP responce listening start.\n");
		
		char message[100];
		while(1)
		{
			memset(message,0,100);
			
			recvfrom(socket_datareceiver,message,sizeof(message),0,(struct sockaddr *)&sin,&sockaddr_len);

			if(DEVETESTIMPL <= 1)
				printf("HTTP responce listener receives a new UDP message.\n");
			
			//���ܵ�����ϢΪ ��GOT��
			if(strncmp(message,"GOT",3) == 0)
			{
				if(DEVETESTIMPL <= 1)
					printf("HTTP responce listener receives a new UDP GOT message.\n");

				flag_sidgot = 1;
				i=2;
				for(j=0;;j++)
				{
					if(message[++i] != 0)
					{
						str[j] = message[i];
					}
					else
						break;
				}
				str[j] = 0;
				break;
			}
			//printf("Data \"%s\" has been received and it will be sent to the HTTP Client immediately./n",str);
			/*
			sleep(1);
			counter_sidget++;
			if(counter_sidget >= MAX_GETSIDTIME)//�����ȡSID��Ӧ��Դ�����󳬹�MAX_GETSIDTIME��Ԥ��ʱ��û�лظ�������Ϊ����ʧ��
			{
			break;
			}
			*/
		}	
		close(socket_datareceiver);
		
		/*
		����ӿڣ�ֱ�ӽ�ȡ�ص��ļ�����DEFAULTDIR�������뿪ͷ�궨�壩��ָ��ĸ�Ŀ¼����������Ĵ����ȡ�ļ�
		������Դ��CoLoRЭ��ȡ�ص��ļ�
		����չ�ԣ������ļ���ȡ�����صĻ���ʵ��������˻���Ļ������ܣ�δ���������ڳ�������ӻ��������б���ϻ�����
		*/
		
		if(flag_sidgot == 1)//�����ȡ������Դ���Ѿ��洢�������ļ���ֱ����ʾ��ҳ����
		{
			fprintf(client_sock,
				"HTTP/1.1 200 OK\r\nServer:SONG\r\nConnection: close\r\nContent-Type:text/html; charset=UTF-8\r\n\r\n<html><head><title>%d - %s</title></head>"
				"<body><font size=+4>[NGI Lab] 973 Project Testing Server - coded by zxWang</font><br><hr width=\"100%%\"><br><center>"
				"<table border cols=3 width=\"100%%\">", errno,
				strerror(errno));
			fprintf(client_sock,
				"</table><font color=\"CC0000\" size=+2>Success: SID \"%s\" was recognized successfully! The Data is: \"%s\"</font></body></html>",
				SID,str);
			goto out;
		}
		else if(flag_sidgot == 0)
		{
			fprintf(client_sock,
				"HTTP/1.1 200 OK\r\nServer:SONG\r\nConnection: close\r\nContent-Type:text/html; charset=UTF-8\r\n\r\n<html><head><title>%d - %s</title></head>"
				"<body><font size=+4>[NGI Lab] 973 Project Testing Server - coded by zxWang</font><br><hr width=\"100%%\"><br><center>"
				"<table border cols=3 width=\"100%%\">", errno,
				strerror(errno));
			fprintf(client_sock,
				"</table><font color=\"CC0000\" size=+2>Error: \n%s %s</font></body></html>",
				Path, strerror(errno));
			goto out;
		}
	}
	//��������ļ����󣬼������ļ�
	if (S_ISREG(info.st_mode))
	{
		fd = open(realPath, O_RDONLY);
		len = lseek(fd, 0, SEEK_END);
		p = (char *) malloc(len + 1);
		bzero(p, len + 1);
		lseek(fd, 0, SEEK_SET);
		ret = read(fd, p, len);
		close(fd);
		fprintf(client_sock,
			"HTTP/1.1 200 OK\r\nServer:SONG\r\nConnection: keep-alive\r\nContent-type: application/*\r\nContent-Length:%d\r\n\r\n",
			len);
		fwrite(p, len, 1, client_sock);
		free(p);
	} 
	else if (S_ISDIR(info.st_mode))
	{
		//�������Ŀ¼����
		dir = opendir(realPath);
		fprintf(client_sock,
			"HTTP/1.1 200 OK\r\nServer:SONG\r\nConnection: close\r\nContent-Type:text/html; charset=UTF-8\r\n\r\n<html><head><title>%s</title></head>"
			"<body><font size=+4>[NGI Lab] 973 Project Testing Server - coded by zxWang-dir</font><br><hr width=\"100%%\"><br><center>"
			"<table border cols=3 width=\"100%%\">", Path);
		fprintf(client_sock,
			"<caption><font size=+3>Dir %s</font></caption>\n",
			Path);
		fprintf(client_sock,
			"<tr><td>Name</td><td>Size</td><td>Time</td></tr>\n");
		if (dir == 0)
		{
			fprintf(client_sock,
				"</table><font color=\"CC0000\" size=+2>%s</font></body></html>",
				strerror(errno));
			return;
		}
		//��ȡĿ¼�����������
		while ((dirent = readdir(dir)) != 0)
		{
			if (strcmp(Path, "/") == 0)
				sprintf(Filename, "/%s", dirent->d_name);
			else
				sprintf(Filename, "%s/%s", Path, dirent->d_name);
			fprintf(client_sock, "<tr>");
			len = strlen(dirroot) + strlen(Filename) + 1;
			realFilename = malloc(len + 1);
			bzero(realFilename, len + 1);
			sprintf(realFilename, "%s/%s", dirroot, Filename);
			if (stat(realFilename, &info) == 0)
			{
				if (strcmp(dirent->d_name, "..") == 0)
				{
					fprintf(client_sock,
						"<td><a href=\"http://%s%s%s\">(parent)</a></td>",
						host, atoi(port) == 80 ? "" : nport,
						BrowserClient_dir_up(Path));
				}
				else
				{
					fprintf(client_sock,
						"<td><a href=\"http://%s%s%s\">%s</a></td>",
						host, atoi(port) == 80 ? "" : nport, Filename,
						dirent->d_name);
				}
				if (S_ISDIR(info.st_mode))
					//fprintf(client_sock, "<td>Ŀ¼</td>");
                    fprintf(client_sock, "<td>Dir</td>");
                else if (S_ISREG(info.st_mode))
                    fprintf(client_sock, "<td>%d</td>", (int)info.st_size);
				else if (S_ISLNK(info.st_mode))
					//fprintf(client_sock, "<td>����</td>");
					fprintf(client_sock, "<td>Link</td>");
				else if (S_ISCHR(info.st_mode))
					//fprintf(client_sock, "<td>�ַ��豸</td>");
					fprintf(client_sock, "<td>Char Device</td>");
				else if (S_ISBLK(info.st_mode))
					//fprintf(client_sock, "<td>���豸</td>");
					fprintf(client_sock, "<td>Block Device</td>");
                else if (S_ISFIFO(info.st_mode))
					fprintf(client_sock, "<td>FIFO</td>");
				else if (S_ISSOCK(info.st_mode))
					fprintf(client_sock, "<td>Socket</td>");
				else
					//fprintf(client_sock, "<td>(δ֪)</td>");
					fprintf(client_sock, "<td>(Unknown)</td>");
				fprintf(client_sock, "<td>%s</td>", ctime(&info.st_ctime));
			}
			fprintf(client_sock, "</tr>\n");
			free(realFilename);
		}
        fprintf(client_sock, "</table></center></body></html>");
	}
	else
	{
		//�ȷǳ����ļ��ַ�Ŀ¼����ֹ����
		fprintf(client_sock,
			"HTTP/1.1 200 OK\r\nServer:SONG\r\nConnection: close\r\nContent-Type:text/html; charset=UTF-8\r\n<html><head><title>permission denied</title></head>"
			"<body><font size=+4>[NGI Lab] 973 Project Testing Server - coded by zxWang-wrong</font><br><hr width=\"100%%\"><br><center>"
			"<table border cols=3 width=\"100%%\">");
		fprintf(client_sock,
			"</table><font color=\"CC0000\" size=+2>Path '%s' denied</font></body></html& gt;",
			Path);
	}
out:
	free(realPath);
	free(nport);
}

/*****************************************
* �������ƣ�BrowserClient_getoption
* ��������������ȡ������Ĳ���
* �����б�
* ���ؽ����
*****************************************/
void
BrowserClient_getoption
(
	int argc,
	char argv[][30]
)
{
	int c, len;
	char *p = 0;
	
	opterr = 0;
	while (1)
	{
		int option_index = 0;
		static struct option long_options[] =
		{
			{"host", 1, 0, 0},
			{"port", 1, 0, 0},
			{"back", 1, 0, 0},
			{"dir", 1, 0, 0},
			{"log", 1, 0, 0},
			{"daemon", 0, 0, 0},
			{0, 0, 0, 0}
		};
		//������֧����һЩ������
		//--host   IP��ַ        ��  -H  IP��ַ
		//--port   �˿�          ��  -P  �˿�
		//--back   ��������      ��  -B  ��������
		//--dir    ��վ��Ŀ¼    ��  -D  ��վ��Ŀ¼
		//--log    ��־���·��  ��  -L  ��־���·��
		//--daemon ��̨ģʽ
		c = getopt_long(argc, (char **)argv, "H:P:B:D:L",
			long_options, &option_index);
		if (c == -1 || c == '?')
			break;
		
		if(optarg)
			len = strlen(optarg);
		else
			len = 0;
		
		if ((!c
			&& !(strcasecmp(long_options[option_index].name, "host")))
			|| c == 'H')
			p = host = malloc(len + 1);
		else if ((!c
			&& !(strcasecmp(long_options[option_index].name, "port")))
			|| c == 'P')
			p = port = malloc(len + 1);
		else if ((!c
			&& !(strcasecmp(long_options[option_index].name, "back")))
			|| c == 'B')
			p = back = malloc(len + 1);
		else if ((!c
			&& !(strcasecmp(long_options[option_index].name, "dir")))
			|| c == 'D')
			p = dirroot = malloc(len + 1);
		else if ((!c
			&& !(strcasecmp(long_options[option_index].name, "log")))
			|| c == 'L')
			p = logdir = malloc(len + 1);
		else if ((!c
			&& !(strcasecmp(long_options[option_index].name, "daemon"))))
		{
			daemon_y_n = 1;
			continue;
		}
		else
			break;
		bzero(p, len + 1);
		memcpy(p, optarg, len);
	}
}

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.���������   To.Ӧ�÷�����*********************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
*******************************************����ͨ�� - From.Ӧ�÷����� To.���������***********************************************************
*******************************************************************************************************************************************/

/*******************************************************************************************************************************************
**********************************************************���߳����ɳ���********************************************************************
*******************************************************************************************************************************************/

/*****************************************
* �������ƣ�BrowserClient_main
* ����������BrowserClientģ��������������ForwardingPlaneת��ƽ���̡߳�������ѭ��ִ�м�HTTP���������ܡ�
* �����б�
eg: ./sub d1sub1 d1sub1-eth1
�ڶ�����ʹ��char**argv�ķ�ʽ����������֪�ǲ����﷨����
* ���ؽ����
*****************************************/
int
BrowserClient_main
(
	int argc,
	char argv[][30]
)
{
	int i,j;

	//����ΪSubscriberģ�������ͨ�Ų�����������
	/*
	struct subscriberinput
	{
		int argc;
		char argv[50][30];
	};
	*/
	struct subscriberinput subinput;
	subinput.argc=0;
	memset(subinput.argv,0,1500);

	subinput.argc=argc;
	for(i=0;i<subinput.argc;i++)
	{
		strcpy(subinput.argv[i],argv[i]);
	}

	pthread_t pthread_subscriber;

	//����Subscriber���߳�
	if(pthread_create(&pthread_subscriber, NULL, Subscriber_main, (void *)&subinput)!=0)
	{
		perror("Creation of subscriber thread failed.");
	}

	//Subscriber_main(argc,(char (*)[30])argv);

	//��ѭ������HTTP����˴���

	struct sockaddr_in addr;
	struct sockaddr_in my_addr;
	my_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	int sock_fd, addrlen;
	
	//��ó������Ĳ������� IP ���˿ڡ�����������ҳ��Ŀ¼��Ŀ¼���λ�õ�
	//BrowserClient_getoption(argc, (char (*)[30])argv);
	
	if (!host)
	{
		addrlen = strlen(DEFAULTIP);
		BrowserClient_AllocateMemory(&host, addrlen,DEFAULTIP);
	}
	if (!port)
	{
		addrlen = strlen(DEFAULTPORT);
		BrowserClient_AllocateMemory(&port, addrlen, DEFAULTPORT);
	}
	if (!back)
	{
		addrlen = strlen(DEFAULTBACK);
		BrowserClient_AllocateMemory(&back, addrlen, DEFAULTBACK);
	}
	if (!dirroot)
	{
		addrlen = strlen(DEFAULTDIR);
		BrowserClient_AllocateMemory(&dirroot, addrlen, DEFAULTDIR);
	}
	if (!logdir)
	{
		addrlen = strlen(DEFAULTLOG);
		BrowserClient_AllocateMemory(&logdir, addrlen, DEFAULTLOG);
	}
	
	
	printf("host=%s \nport=%s \nback=%s \ndirroot=%s \nlogdir=%s \n%sthe background model(Process ID: %d)\n",
		host, port, back, dirroot, logdir, daemon_y_n?"This is ":"This is not ", getpid());
	
	//fork() ���δ��ں�̨����ģʽ��
	if (daemon_y_n)
	{
		if (fork())
			exit(0);
		if (fork())
			exit(0);
		close(0), close(1), close(2);
		logfp = fopen(logdir, "a+");
		if (!logfp)
			exit(0);
	}
	
	//�����ӽ����˳����������ʬ����
	signal(SIGCHLD, SIG_IGN);
	
	//���� socket
	/*
	PF_INET is for BSD
	AF_INET is for POSIX
	This location maybe the very place the bug which occured in Debian but not occured in Ubuntu lies.
	*/
	if ((sock_fd = socket(AF_INET/*ԭΪPF_INET*/, SOCK_STREAM, 0)) < 0)
	{
		if (!daemon_y_n)
		{
			prterrmsg("socket()");
		}
		else
		{
			wrterrmsg("socket()");
		}
	}
	
	//���ö˿ڿ�������
	addrlen = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &addrlen,
		sizeof(addrlen));
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(host);
	addrlen = sizeof(struct sockaddr_in);
	//�󶨵�ַ���˿ڵ���Ϣ
	if (bind(sock_fd, (struct sockaddr *) &addr, addrlen) < 0)
	{
		if (!daemon_y_n)
		{
			prterrmsg("bind()");
		}
		else
		{
			wrterrmsg("bind()");
		}
	}
	
	//��������
	if (listen(sock_fd, atoi(back)) < 0)
	{
		if (!daemon_y_n)
		{
			prterrmsg("listen()");
		}
		else
		{
			wrterrmsg("listen()");
		}
	}
	while (1)
	{
		int len;
		int new_fd;
		addrlen = sizeof(struct sockaddr_in);
		//��������������
		new_fd = accept(sock_fd, (struct sockaddr *) &addr, &addrlen);
		if (new_fd < 0) 
		{
			if (!daemon_y_n) 
			{
				prterrmsg("accept()");
			} 
			else
			{
				wrterrmsg("accept()");
			}
			break;
		}
		bzero(buffer, MAXBUF + 1);
		
		if(DEVETESTIMPL <= 1)
		{
			sprintf(buffer, ">>>Connection from: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
			if (!daemon_y_n) 
			{
				prtinfomsg(buffer);
			} 
			else 
			{
				wrtinfomsg(buffer);
			}
		}

		//����һ���ӽ���ȥ�������󣬵�ǰ���̼����ȴ��µ����ӵ���
		if (!fork()) 
		{
			bzero(buffer, MAXBUF + 1);
			if ((len = recv(new_fd, buffer, MAXBUF, 0)) > 0) 
			{
				FILE *ClientFP = fdopen(new_fd, "w");
				if (ClientFP == NULL) 
				{
					if (!daemon_y_n) 
					{
						prterrmsg("fdopen()");
					} 
					else 
					{
						prterrmsg("fdopen()");
					}
				} 
				else 
				{
					char Req[MAXPATH + 1] = "";
					sscanf(buffer, "GET %s HTTP", Req);

					bzero(buffer, MAXBUF + 1);
					
					//��ȡSID
					for(i=1,j=0;;i++)
					{
						if(Req[i]!='\0')
						{
							SID[j++]=Req[i];
						}
						else
						{
							SID[j] = '\0';
							break;
						}
					}

					if (strcmp(SID, "favicon.ico") == 0)
					{
						continue;
					}
					sprintf(buffer, "");
					if (!daemon_y_n) 
					{
						prtinfomsg(buffer);
					} 
					else
					{
						wrtinfomsg(buffer);
					}
					//�����û�����
					BrowserClient_GiveResponse(ClientFP, Req);
					
					fclose(ClientFP);
				}
			}
			//exit(0);
		}
		close(new_fd);
	}
	close(sock_fd);
}
