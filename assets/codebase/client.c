#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "client.h"
#include "log.h"

int client_connect(const Config *config) {
    // Sets up structs for address info of specified hostname/port pair
    log_info("Connecting to server...");
    struct addrinfo hints, *result, *rp;
    int sockfd = -1;
    int status;

    // Sets the IP version to unspecified to be version neutral and the socket type to TCP
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;     // Either IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    // Checks to see if there is correct address info for hostname/port pair
    if ((status = getaddrinfo(config->host, config->port, &hints, &result)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // Loops through linked list pairs of info to establish a valid socket
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        // Socket incorrect
        if (sockfd < 0) {
            printf(strerror(ENOTCONN));
            continue;
        }

        // Checks to see if connection is valid
        if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            log_info("Connection established");
            break;
        } else {
            printf(strerror(ETIMEDOUT));
            close(sockfd);
        }
    }

    freeaddrinfo(result); // free the linked list
    if (sockfd == -1) {
        printf("could not establish connection with TCP server\n");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

void client_send_image(int sockfd, const Config *config) {
    // IMPLEMENT THIS FUNCTION
}

void client_receive_response(int sockfd) {
    uint8_t buf[101];
    int num_recv = recv(sockfd, buf, 100, 0);
    buf[num_recv] = '\0';

    printf("Response from server: %s\n", buf);
}

void client_close(int sockfd) {
    // IMPLEMENT THIS FUNCTION
}
