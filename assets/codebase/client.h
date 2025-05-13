#ifndef CLIENT_H_
#define CLIENT_H_

// Contains all of the information needed to create to connect to the server and
// send it a message.
typedef struct Config {
    char *port;
    char *host;
    uint8_t *payload;
    uint32_t payload_size;
    char *hw_id;
} Config;

/**
 * Creates a TCP socket and connects it to the specified host and port. It returns the socket file
 * descriptor.
 *
 * Config *config: A filled out Config struct. This function uses the host and port to connect to
 * the server.
 */
int client_connect(const Config *config);

/**
 * Using the socket and Config struct, send the homework ID and payload. The homework ID should be
 * sent first, followed by the payload.
 *
 * int sockfd: The socket file descriptor returned by the client_connect function.
 * Config *config: A filled out Config struct. This function uses the hw_id, payload, and
 * payload_size fields to send the data to the server.
 */
void client_send_image(int sockfd, const Config *config);

/**
 * Using the socket receive a response. This function just prints out the response from the server.
 *
 * int sockfd: The socket file descriptor to receive a response from.
 */
void client_receive_response(int sockfd);

/**
 * Close the socket.
 *
 * int sockfd: The socket file descriptor to be closed.
 */
void client_close(int sockfd);

#endif
