#define _GNU_SOURCE

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

#include "common.h"
#include "http_proxy.h"
#include "queue.h"
#include "thread_pool.h"

#define LISTEN_PORT 8080  // Proxy listens on this port

int main(int argc, char* argv[]) {
    // Accept command-line arguments for the number of threads
    int num_threads = 2;    // provide a default number of threads
    if (argc == 2) {
        num_threads = atoi(argv[1]);
    }

    // TODO Initialize a queue and thread pool to handle requests

    pthread_mutex_t req_mutex;
    pthread_cond_t req_cond;

    pthread_mutex_init(&req_mutex, NULL);
    pthread_cond_init(&req_cond, NULL);

    queue_t *que = queue_init(&req_mutex, &req_cond);
    struct thread_pool *thread_pool = thread_pool_init(que, num_threads);



    // TODO Create socket for the proxy server to listen on the LISTEN_PORT

    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    ssize_t n;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Initialize socket structure */
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    portno = 5001;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    /* Now start listening for the clients, here process will
     * go in sleep mode and will wait for the incoming connection
     */

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(1);
    }
    printf("Connected to client\n");
    /* If connection is established then start receiving data from client */
    memset(buffer, 0, 256);
    n = read(newsockfd, buffer, 255);
    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }
    

    // TODO Add a signal handler to cleaning shutdown the server
    // when a SIGINT (ctrl-c) is received.


    // TODO Accept incoming client connections. Add a request_t to the queue to
    // handle the client's HTTP request. The client socket is the argument to be
    // passed to the handle_client_request_thread function.
    // Continue to accept client connections until SIGINT is received.


    // TODO perform any clean up before server shutdown

    return EXIT_SUCCESS;
}