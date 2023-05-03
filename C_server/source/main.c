#include "myhttpd.h"

server_t myHttpServer;
int main() {
    int port = 6969;
    initServer(port, &myHttpServer);
    serveHTTP(&myHttpServer);

    return 0;
}
