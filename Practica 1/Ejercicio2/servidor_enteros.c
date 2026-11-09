#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <Puerto>\n", argv[0]);
        return 1;
    }

    int puerto = atoi(argv[1]);
    int servidor_fd, nuevo_socket;
    struct sockaddr_in direccion;
    int opt = 1;
    int addrlen = sizeof(direccion);
    char buffer[1024] = {0};

    // 1. Crear el socket TCP
    if ((servidor_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Fallo al crear socket");
        exit(EXIT_FAILURE);
    }

    // Permitir reutilizar la dirección y puerto rápidamente
    setsockopt(servidor_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY; // Escuchar en todas las interfaces de red
    direccion.sin_port = htons(puerto);

    // 2. Asociar el socket al puerto (bind)
    if (bind(servidor_fd, (struct sockaddr *)&direccion, sizeof(direccion)) < 0) {
        perror("Fallo en bind");
        exit(EXIT_FAILURE);
    }

    // 3. Poner el socket en modo de escucha
    if (listen(servidor_fd, 3) < 0) {
        perror("Fallo en listen");
        exit(EXIT_FAILURE);
    }

    printf("Servidor C escuchando en el puerto %d...\n", puerto);

    // 4. Aceptar la conexión del cliente Java
    if ((nuevo_socket = accept(servidor_fd, (struct sockaddr *)&direccion, (socklen_t*)&addrlen)) < 0) {
        perror("Fallo en accept");
        exit(EXIT_FAILURE);
    }
    printf("Cliente conectado.\n");

    // 5. Ciclo para procesar números hasta recibir 0 o desconexión
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_leidos = read(nuevo_socket, buffer, sizeof(buffer) - 1);
        if (bytes_leidos <= 0) {
            printf("Cliente desconectado.\n");
            break;
        }

        int numero = atoi(buffer);
        printf("Numero recibido: %d\n", numero);

        // Si el cliente envía 0, se termina la ejecución
        if (numero == 0) {
            printf("Recibido 0. Cerrando servidor...\n");
            break;
        }

        // Incrementar el número en uno
        int resultado = numero + 1;
        char respuesta[64];
        snprintf(respuesta, sizeof(respuesta), "%d\n", resultado);

        // Enviar respuesta al cliente Java
        send(nuevo_socket, respuesta, strlen(respuesta), 0);
        printf("Enviado al cliente: %d\n", resultado);
    }

    close(nuevo_socket);
    close(servidor_fd);
    return 0;
}