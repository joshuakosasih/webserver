all: httpd

httpd: httpd.c
	gcc -W -Wall -pthread -o main main.c -lev

clean:
	rm httpd
