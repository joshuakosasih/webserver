/**
 * 13514012 - Joshua Aditya Kosasih
 * 13514049 - Ade Surya Ramadhani
 */

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>

#define ISspace(x) isspace((int)(x))
char dir[20];

int get_line(int sock, char *buf, int size) {
	int i = 0;
	char c = '\0';
	int n;

	while ((i < size - 1) && (c != '\n')) {
		n = recv(sock, &c, 1, 0);		
		if (n > 0) {
			if (c == '\r') {
				n = recv(sock, &c, 1, MSG_PEEK);				
				if ((n > 0) && (c == '\n'))
					recv(sock, &c, 1, 0);
				else
					c = '\n';
			}
			buf[i] = c;
			i++;
		}
		else
			c = '\n';
	 }
	 buf[i] = '\0';
	 
	 return(i);
}

void server_accept(int client_sock) {
	//read message
	char buf[1024];
	char method[255];
	char url[255];
	char path[512];
	get_line(client_sock, buf, sizeof(buf));
	printf("%s", buf);
	
	//get method
	int i = 0, j = 0;
	while (!ISspace(buf[j]) && (i < sizeof(method) - 1)) {
		method[i] = buf[j];
		i++; j++;
	}
	method[i] = '\0';

	i = 0;
	while (ISspace(buf[j]) && (j < sizeof(buf)))
		j++;
	//get url
	while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf))) {
		url[i] = buf[j];
		i++; j++;
	}
	url[i] = '\0';
	printf("B: '%s'\n", buf);
	printf("M: '%s'\n", method);
	printf("U: '%s'\n", url);
	
	if (buf[0] != 0) {
	
		sprintf(path, "%s%s", dir, url);
		if (path[strlen(path) - 1] == '/')
			strcat(path, "index.html");
			
		printf("P: %s\n", path);
		
		FILE *resource = NULL;
		int numchars = 1;
		char filebuf[1024];
		char outbuf[1024];
		filebuf[0] = 'A'; filebuf[1] = '\0';
		while ((numchars > 0) && strcmp("\n", filebuf))
			numchars = get_line(client_sock, filebuf, sizeof(filebuf));
		
		resource = fopen(path, "r");
		if (resource == NULL) {
			printf("file not found\n");
			strcpy(buf, "HTTP/1.0 404 NOT FOUND\r\n");
			send(client_sock, outbuf, strlen(outbuf), 0);
			printf("404 sent\n");
			strcpy(outbuf, "JHTTPWebServer\r\n");
			send(client_sock, outbuf, strlen(outbuf), 0);
			sprintf(outbuf, "Content-Type: text/html\r\n");
			send(client_sock, outbuf, strlen(outbuf), 0);
			strcpy(outbuf, "\r\n");
			send(client_sock, outbuf, strlen(outbuf), 0);
			sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
			send(client_sock, outbuf, strlen(outbuf), 0);
			sprintf(buf, "<BODY><H1>404 - File Not Found\r\n");
			send(client_sock, outbuf, strlen(outbuf), 0);
			sprintf(buf, "</BODY></HTML>\r\n");
			send(client_sock, outbuf, strlen(outbuf), 0);
			printf("html sent\n");
		}
		else {
			printf("serving..\n");
			strcpy(buf, "HTTP/1.0 200 OK\r\n");		
			send(client_sock, outbuf, strlen(outbuf), 0);
			strcpy(outbuf, "JHTTPWebServer\r\n");
			send(client_sock, outbuf, strlen(outbuf), 0);
			sprintf(outbuf, "Content-Type: text/html\r\n");
			send(client_sock, outbuf, strlen(outbuf), 0);
			strcpy(outbuf, "\r\n");
			send(client_sock, outbuf, strlen(outbuf), 0);
			fgets(outbuf, sizeof(outbuf), resource);
			while (!feof(resource)) {
				send(client_sock, outbuf, strlen(outbuf), 0);
				fgets(outbuf, sizeof(outbuf), resource);
			}
			fclose(resource);
			printf("html sent\n");
		}
	}
	close(client_sock);
	printf("closed\n");
}

int main() {
	int pnum;
	printf("Enter valid port number: ");
	scanf("%d", &pnum);
	
	int server_sock = -1;
	int client_sock = -1;
	int port = pnum;
	
	struct sockaddr_in client_name, name;
	unsigned int client_name_len = sizeof(client_name);
	
	pthread_t newthread;
	
	server_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(server_sock, (struct sockaddr *)&name, sizeof(name));	
	listen(server_sock, 5);
	printf("http running on port %d\n", port);
		
	printf("Enter folder name to fetch (html) files: ");
	scanf("%s", dir);	
	
	while(1) {
		
		client_sock = accept(server_sock, (struct sockaddr *)&client_name, &client_name_len);
		
		if (client_sock == -1)
			printf("accept error\n");
		else
			printf("client accepted\n");
		
		pthread_create(&newthread , NULL, server_accept, client_sock);		
	}
	close(server_sock);
	
	return 0;
}
