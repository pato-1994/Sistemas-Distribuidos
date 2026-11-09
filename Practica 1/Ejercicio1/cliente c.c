#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <IP_Servidor> <Puerto>\n", argv[0]);
        return 1;
    }

    char *ip = argv[1];
    int puerto = atoi(argv[2]);
    int sock;
    struct sockaddr_in servidor_addr;
    char buffer[1024] = {0};

    // Crear el socket TCP
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nError en la creación del socket\n");
        return -1;
    }

    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(puerto);

    // Convertir la dirección IP a formato binario
    if (inet_pton(AF_INET, ip, &servidor_addr.sin_addr) <= 0) {
        printf("\nDirección inválida\n");
        return -1;
    }

    // Conectar al servidor Java
    if (connect(sock, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) < 0) {
        printf("\nConexión Fallida\n");
        return -1;
    }

    // Enviar el mensaje inicial. El '\n' funciona como delimitador.
    char *mensaje = "Hola\n";
    send(sock, mensaje, strlen(mensaje), 0);
    printf("Mensaje enviado: Hola\n");

    // Leer la respuesta del servidor ("Hola que tal")
    int valread = read(sock, buffer, 1024);
    printf("Respuesta del servidor: %s\n", buffer);

    close(sock);
    return 0;
}