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
    bool error = false;

    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, puerto, &hints, &servinfo);

    // Creamos el socket de escucha del servidor
    int socket_servidor;
    if ((socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
    {
        log_error_if_logger_not_null(logger, "Error al crear el socket de escucha, con puerto %s", puerto);
        error = true;
    }
    else
    {
        int activado;
        setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

        // Asociamos el socket a un puerto
        if (bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen) != 0)
        {
            log_error_if_logger_not_null(logger, "Fallo el bind del socket de escucha");
            error = true;
        }
        else
        {
            // Escuchamos las conexiones entrantes
            if (listen(socket_servidor, SOMAXCONN) != 0)
            {
                log_error_if_logger_not_null(logger, "Fallo el listen del socket de escucha");
                error = true;
            }
            else
            {
                log_trace_if_logger_not_null(logger, "Listo para escuchar a mi cliente");
            }
        }
    }

    freeaddrinfo(servinfo);
    return error ? -1 : socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
    struct sockaddr_in direccion_cliente;
    unsigned int tamanio_direccion;
    int socket_cliente = accept(socket_servidor, (void *)&direccion_cliente, &tamanio_direccion);
    return socket_cliente;
}

int enviar_por_socket(int socket, void *buffer_serializado, int bytes)
{
    return send(socket, buffer_serializado, bytes, 0);
}

int enviar_string_por_socket(int socket, char *cadena)
{
    int bytes = strlen(cadena) + 1;
    char *alloc_cadena = malloc(bytes);
    memcpy(alloc_cadena, cadena, bytes);

    int result = enviar_por_socket(socket, cadena, bytes);

    free(alloc_cadena);
    return result;
}

int enviar_string_con_longitud_por_socket(int socket, char *cadena)
{
    uint32_t bytes = strlen(cadena) + 1;
    enviar_uint32_por_socket(socket, bytes);

    return enviar_string_por_socket(socket, cadena);
}

int enviar_uint32_por_socket(int socket, uint32_t numero)
{
    uint32_t *alloc_numero = malloc(sizeof(uint32_t));
    *alloc_numero = numero;

    int result = enviar_por_socket(socket, alloc_numero, sizeof(uint32_t));

    free(alloc_numero);
    return result;
}

int recibir_por_socket(int socket, void *buffer, int bytes)
{
    return recv(socket, buffer, bytes, MSG_WAITALL);
}

int recibir_string_con_longitud_por_socket(int socket, char **buffer)
{
    uint32_t tamanio;
    recibir_uint32_por_socket(socket, &tamanio);

    (*buffer) = malloc(tamanio);
    return recibir_por_socket(socket, (*buffer), tamanio);
}

int recibir_uint32_por_socket(int socket, uint32_t *numero)
{
    return recibir_por_socket(socket, numero, sizeof(uint32_t));
}

void liberar_conexion(int socket)
{
    close(socket);
}
