#include <utils/sockets.h>

int crear_conexion(char *ip, char *puerto, t_log *logger)
{
    bool error = false;

    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(ip, puerto, &hints, &servinfo);

    int socket_fd;
    if ((socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
    {
        log_error_if_logger_not_null(logger, "Error al crear el socket con IP %s, PUERTO %s", ip, puerto);
        error = true;
    }
    else if (connect(socket_fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        log_error(logger, "Fallo la conexion con el socket con IP %s, PUERTO %s", ip, puerto);
        error = true;
    }

    freeaddrinfo(servinfo);

    return error ? -1 : socket_fd;
}

int enviar_por_socket(int socket, void *buffer_serializado, int bytes)
{
    return send(socket, buffer_serializado, bytes, 0);
}
