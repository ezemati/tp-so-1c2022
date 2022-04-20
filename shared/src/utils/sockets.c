#include <utils/sockets.h>

int crear_conexion(char *ip, char *puerto, t_log *logger)
{
    bool error = false;

    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo(ip, puerto, &hints, &servinfo);

    int socket_fd;
    if ((socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
    {
        log_error_if_logger_not_null(logger, "Error al crear el socket con IP %s, PUERTO %s", ip, puerto);
        error = true;
    }
    else if (connect(socket_fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        log_error_if_logger_not_null(logger, "Fallo la conexion con el socket con IP %s, PUERTO %s", ip, puerto);
        error = true;
    }

    freeaddrinfo(servinfo);

    return error ? -1 : socket_fd;
}

int iniciar_servidor(char *puerto, t_log *logger)
{
    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, puerto, &hints, &servinfo);

    // Creamos el socket de escucha del servidor
    int socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    // Asociamos el socket a un puerto
    bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);

    // Escuchamos las conexiones entrantes
    listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);
    log_trace_if_logger_not_null(logger, "Listo para escuchar a mi cliente");

    return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
    int socket_cliente = accept(socket_servidor, NULL, NULL);
    return socket_cliente;
}

int enviar_por_socket(int socket, void *buffer_serializado, int bytes)
{
    return send(socket, buffer_serializado, bytes, 0);
}

int recibir_por_socket(int socket, void *buffer, int bytes)
{
    return recv(socket, buffer, bytes, MSG_WAITALL);
}

void liberar_conexion(int socket)
{
    close(socket);
}
