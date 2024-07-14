#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <IP> <port> [message]\n", argv[0]);
        return 1;
    }

    char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    char *message = NULL;

    if (argc > 3) {
        message = argv[3];
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket error");
        return 1;
    }

    char *timeout_str = getenv("CONN_TIMEOUT");
    int timeout_usec = timeout_str ? atoi(timeout_str) * 1000 : 1000000;  // default to 1 second

    struct timeval timeout;
    timeout.tv_sec = timeout_usec / 1000000;
    timeout.tv_usec = timeout_usec % 1000000;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        perror("setsockopt failed");
        return 1;
    }
    if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0) {
        perror("setsockopt failed");
        return 1;
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(server_port);

    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("connect error");
        return 1;
    }

    if (message != NULL) {
        if (write(sock, message, strlen(message)) < 0) {
            perror("write error");
            return 1;
        }

        char buffer[MAX_BUFFER_SIZE];
        int str_len;
        while ((str_len = read(sock, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[str_len] = 0;
            printf("%s", buffer);
        }
        if (str_len < 0) {
            perror("read error");
            return 1;
        }
        printf("\n");
    }

    close(sock);
    return 0;
}
