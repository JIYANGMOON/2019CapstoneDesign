#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <string.h>

#define BUF_SIZE 1024
#define OPSZ 4
#define SERVER_IPADDR "192.168.1.31"
#define SERVER_PORTNO 1111



void error_handling(char *message);
int create_server(char *ip_addr, int port_no);
char get_direction(char di);

int server_sock, client_sock;


int create_server(char *ip_addr, int port_no)
{
	printf("create called\n");
	struct sockaddr_in server_addr;
	int val = 1;
	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0)
	{
		error_handling("socket() error!");
		return -1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip_addr);
	server_addr.sin_port = htons(port_no);

	if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
	{
		error_handling("bind() error!");
		return -1;
	}

	if (listen(server_sock, 10) != 0)
	{
		error_handling("listen() error!");
		return -1;
	}
	printf("create_server complete\n");
	return server_sock;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char *argv[])
{
	char buffer;
	char ch, result, dir, recv_dir;
	char ip_addr[20] = SERVER_IPADDR;
	int port_no = SERVER_PORTNO;

	struct sockaddr_in sockaddr;
	socklen_t client_addr_sz;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	printf("before create_server\n");

	server_sock = create_server(ip_addr, port_no);

	int len = sizeof(sockaddr);
	printf("before accept");
	client_sock = accept(server_sock, (struct sockaddr *)&sockaddr, (socklen_t *)&len);
	if (client_sock < 0) {
		error_handling("accept() error!");

	}
	printf("accept complete");
	while (1)
	{
		char buf[BUF_SIZE] = "";
		read(client_sock, &buf, 1);

		//printf("read complete\n");
		//for debug
		printf("%s\n", buf);
		result = get_direction(*buf);
		write(client_sock, &result, sizeof(result));
	}
	return 0;
}

char get_direction(char di)
{
	char result;
	switch (di)
	{

	case 'w':
		result = 'g';

		break;
	case 'a':
		result = 'l';
	
		break;
	case 's':
		result = 'b';

		break;
	case 'd':
		result = 'r';
	
		break;

	default:
		printf("Wrong!\n");
		break;
	}
	return result;
}
