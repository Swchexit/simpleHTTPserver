//
// Created by Sheng Wei Chen on 2022/10/28.
//
#include "myhttpd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/stat.h>
// #include <sys/sendfile.h>
#include <time.h>


void initServer(unsigned int port, server_t *myServer){
    fprintf(stderr, "Starting server on port %u...\n", port);
    myServer->port = port;
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(myServer->port);

    if((myServer->listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket() failed.\n");
        exit(1);
    }
    int option = 1;
    if(setsockopt(myServer->listenfd, SOL_SOCKET, SO_REUSEADDR, (void*)&option, sizeof(option)) < 0){
        perror("setsockopt() failed.\n");
        exit(1);
    }
    if (bind(myServer->listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind() failed.\n");
        exit(1);
    }
    if (listen(myServer->listenfd, 512) != 0) {
        perror("listen() failed.\n");
        exit(1);
    }

    fprintf(stderr, "The server has been started\n");
}

void serveHTTP(server_t *myServer){
    struct sockaddr_in clientAddr;  // for accept()
    int clientfd;
    char recvBuff[1024];
    char *method, *uri, *http_ver;
    while(1){
        socklen_t clientLen;
        clientLen = sizeof(clientAddr);
        if ((clientfd = accept(myServer->listenfd, (struct sockaddr*)&clientAddr, &clientLen)) < 0) {
            if (errno == EINTR || errno == EAGAIN) {
                fprintf(stderr, "accept(): try again\n");
                continue; // try again
            }
            else{
                perror("accept failed\n");
                exit(1);
            }
        }

        fprintf(stderr, "----New request from %s, fd = %d----\n", inet_ntoa(clientAddr.sin_addr), clientfd);

        ssize_t n;
        n = read(clientfd, recvBuff, sizeof(recvBuff)-1);
        if(n < 0){
            // READ ERROR!
            fprintf(stderr, "ERROR: Read from client failed!\n");
        }
        else if(n == 0){    // Connection closed unexpectedly
            fprintf(stderr, "ERROR: Nothing is received from client!\n");
        }
        else{
            recvBuff[n] = 0;
            method = strtok(recvBuff,  " \t\r\n");
            uri = strtok(NULL, " \t");
            http_ver = strtok(NULL, " \t\r\n");
            if(method == NULL || uri == NULL || http_ver == NULL){ // bad request!
                fprintf(stderr, "400 Bad Request(Not valid http request)!\n");
                sendHeaders(clientfd, "400 Bad Request", 0, NULL);
                goto CONTINUE;
            }

            fprintf(stderr, "method: %s, uri: %s, version: %s\n", method, uri, http_ver);
            if(strcmp(uri, "/") != 0){
                fprintf(stderr, "404 Not Found!(uri problem)\n");
                // sendHeaders(clientfd, "404 Not Found", 0, NULL);
                sendPage(clientfd, "404.html");
                goto CONTINUE;
            }
            if(strcmp(method, "GET") != 0){
                fprintf(stderr, "501 Not Implemented! (Method not supported)\n");
                sendHeaders(clientfd, "501 Not Implemented", 0, NULL);
                goto CONTINUE;
            }

            // SEND "index.html" TO CLIENT
            sendPage(clientfd, "index.html");

            // The rest of request header(along with message body) is omitted
        }

CONTINUE:
        fprintf(stderr, "----Closing connection----\n\n");
        shutdown(clientfd, SHUT_RDWR);
        close(clientfd);
    }
}

void sendHeaders(int fd, char *status, int bodyLen, char *content_type) {
    time_t when;
    char timebuf[128];
    char sendbuf[1024];

    sprintf(sendbuf, "HTTP/1.1 %s\r\n", status);
    write(fd, sendbuf, strlen(sendbuf));

    write(fd, "Server: WEI's server/1.0\r\n", strlen("Server: WEI's server/1.0\r\n"));

    when = time(NULL);
    strftime(timebuf, sizeof(timebuf), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&when));
    sprintf(sendbuf, "Date: %s\r\n", timebuf);
    write(fd, sendbuf, strlen(sendbuf));

    if (bodyLen >= 0){
        sprintf(sendbuf, "Content-Type: %s\r\n", content_type);
        write(fd, sendbuf, strlen(sendbuf));
        sprintf(sendbuf, "Content-Length: %d\r\n", bodyLen);
        write(fd, sendbuf, strlen(sendbuf));
    }

    write(fd, "Connection: close\r\n", strlen("Connection: close\r\n"));
    write(fd, "\r\n", strlen("\r\n"));
}

void sendPage(int fd, char* filename){
    struct stat st;
    if(stat(filename, &st) < 0){   // The file does not exist(or error). Should send meaningful reply.
        fprintf(stderr, "stat() failed\n");
        sendHeaders(fd, "418 I'm a teapot", 0, NULL);
        return;
    }
    int size = S_ISREG(st.st_mode) ? (int)st.st_size : -1;
    if(size < 0){       // The file is not regular file, and should not be sent as http reply
        fprintf(stderr, "File is not send-able\n");
        sendHeaders(fd, "418 I'm a teapot", 0, NULL);
        return;
    }
    sendHeaders(fd, "200 OK", size, "text/html");   // FIXME: content type needn't be fixed

    /*
    int read_fd = open(filename, O_RDONLY);
    sendfile(fd, read_fd, NULL, size);
    close(read_fd);
    */
    char sendbuf[1024];
    FILE *fp = fopen(filename, "r");
    if(!fp){
        fprintf(stderr, "fopen() failed\n");
        sendHeaders(fd, "500 Internal Server Error", 0, NULL);
        return;
    }
    while (fgets(sendbuf, sizeof(sendbuf), fp)){
        if(write(fd, sendbuf, strlen(sendbuf)) < 0)
            break;
    }
    fclose(fp);
}
