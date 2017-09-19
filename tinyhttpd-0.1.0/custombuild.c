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

int main() {
	int server_sock = -1;
	int client_sock = -1;
	int port = 12345;
	
	struct sockaddr_in client_name, name;
	unsigned int client_name_len = sizeof(client_name);
	
	server_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(server_sock, (struct sockaddr *)&name, sizeof(name));	
	listen(server_sock, 5);
	printf("httpd running on port %d\n", port);
	
	client_sock = accept(server_sock, (struct sockaddr *)&client_name, &client_name_len);
	
	if (client_sock == -1)
		printf("accept error");
	else
		printf("accepted");
		
	//read message
	char buf[1024];
	char method[255];
	char url[255];
	char path[512];
	get_line(client_sock, buf, sizeof(buf));
	printf("%s", buf);
	
	
	int i = 0, j = 0;
	while (!ISspace(buf[j]) && (i < sizeof(method) - 1)) {
		method[i] = buf[j];
		i++; j++;
	}
	method[i] = '\0';

	i = 0;
	while (ISspace(buf[j]) && (j < sizeof(buf)))
		j++;
	while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf))) {
		url[i] = buf[j];
		i++; j++;
	}
	url[i] = '\0';
	printf("B: %s\n", buf);
	printf("M: %s\n", method);
	printf("U: %s\n", url);
	
	sprintf(path, "pages%s", url);
	if (path[strlen(path) - 1] == '/')
		strcat(path, "index.html");
		
	printf("P: %s\n", path);
	
	FILE *resource = NULL;
	int numchars = 1;
	char filebuf[1024];
	char outbuf[1024];
	filebuf[0] = 'A'; filebuf[1] = '\0';
	while ((numchars > 0) && strcmp("\n", filebuf))  /* read & discard headers */
		numchars = get_line(client_sock, filebuf, sizeof(filebuf));
	
	resource = fopen(path, "r");
	
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
	
	close(server_sock);
	return 0;
}
