#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define PORT 6336
#define BUF_SIZE 1024
#define MAX_ADDR_LEN 16
#define ARRAY_SIZE 50
// #define TIMER_ALARM 5


char ip_tab[ARRAY_SIZE][MAX_ADDR_LEN];
size_t index = 0;

int main(int argc , char* argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage :./exe  chaine\n");
        exit(EXIT_FAILURE);
    }

    int sock;
    int bcenable = 1;
    struct sockaddr_in addr;
    int n;

    switch (fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:    
            sock = socket(AF_INET, SOCK_DGRAM, 0);
            if (sock == -1) {
                perror("socket");
                exit(EXIT_FAILURE);
            }
            if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &bcenable, sizeof(int)) == -1) {
                perror("setsockopt");
                return EXIT_FAILURE;
            }
            addr.sin_family = AF_INET;
            addr.sin_port = htons(PORT);
            n = inet_pton(AF_INET, "255.255.255.255", &addr.sin_addr.s_addr);
            if (n ==0 ){
                perror("inet_pton");
                exit(EXIT_FAILURE);
            }
            
            while (1) {
                 printf("[+] envoie de la chaine : %s\n",argv[1]);
                if (sendto(sock, argv[1], strlen(argv[1]) + 1, 0, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
                    perror("sendto");
                    exit(EXIT_FAILURE);
                }
                sleep(1);
            }
            break;
        
    default:
            sock = socket(AF_INET, SOCK_DGRAM, 0);
            if (sock == -1) {
                perror("socket");
                exit(EXIT_FAILURE);
            }
            addr.sin_family = AF_INET;
            addr.sin_port = htons(PORT);
            n = inet_pton(AF_INET, "0.0.0.0", &addr.sin_addr.s_addr);
            if (n ==0 ){
                perror("inet_pton");
                exit(EXIT_FAILURE);
            }

            if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
                perror("bind");
                exit(EXIT_FAILURE);
            }
            char buffer[BUF_SIZE];
            char chaine[BUF_SIZE];

            buffer[BUF_SIZE-1]=0;
            chaine[BUF_SIZE -1]=0;
            strncpy(chaine, "salut",6);
            struct sockaddr_in addr_from;
            socklen_t add_len = sizeof(addr_from);

        while (1) {
        memset(buffer, 0, BUF_SIZE); 
        ssize_t recv_bytes = recvfrom(sock, buffer, BUF_SIZE, 0, (struct sockaddr*)&addr_from, &add_len);
        if (recv_bytes == (ssize_t)-1) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        } else if (recv_bytes > 0) {
            printf("Message reçu du client : %s\n", buffer);
            // Comparaison du message reçu avec argv[1]
            if (strcmp(buffer,chaine) == 0) {
                printf("C'est le bon message\n");
                char ip_pointe[MAX_ADDR_LEN];
                inet_ntop(AF_INET, &addr_from.sin_addr.s_addr, ip_pointe, sizeof(ip_pointe));
                
               
                bool is_in = false;
                for (size_t i = 0; i < index; i++) {
                    if (strcmp(ip_tab[i], ip_pointe) == 0) {
                        is_in = true;
                        break;
                    }
                }
                
                if (!is_in) {
                    strncpy(ip_tab[index], ip_pointe, MAX_ADDR_LEN);
                    index += 1;
                }
            } else {
                printf("Ce n'est pas le bon message\n");
            }
        }
    }
    
    break;
        
    }

    return EXIT_SUCCESS;
}
