//
// Created by Sheng Wei Chen on 2022/10/28.
//

#ifndef INTROCN_PROJ_MYHTTPD_H
#define INTROCN_PROJ_MYHTTPD_H

typedef struct server_t{
    unsigned int port;
    int listenfd;
} server_t;

void initServer(unsigned int port, server_t *myServer);
void serveHTTP(server_t *myServer);
void sendHeaders(int fd, char *status, int bodyLen, char *content_type);
void sendPage(int fd, char* filename);
#endif //INTROCN_PROJ_MYHTTPD_H
