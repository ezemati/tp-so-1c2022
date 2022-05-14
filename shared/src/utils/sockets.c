#include <utils/sockets.h>

int crear_conexion(char *ip, uint32_t puerto, t_log *logger)
{
    bool error = false;

    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    char *strPuerto = int_to_string(puerto);
    getaddrinfo(ip, strPuerto, &hints, &servinfo);

    int socket_fd;
    if ((socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
    {
        log_error_if_logger_not_null(logger, "Error al crear el socket con IP %s, PUERTO %s", ip, strPuerto);
        error = true;
    }
    else if (connect(socket_fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
    {
        log_error_if_logger_not_null(logger, "Fallo la conexion con el socket con IP %s, PUERTO %s", ip, strPuerto);
        error = true;
    }

    freeaddrinfo(servinfo);
    free(strPuerto);

    return error ? -1 : socket_fd;
}

int iniciar_servidor(uint32_t puerto, t_log *logger)
{
    bool error = false;

    struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    char *strPuerto = int_to_string(puerto);
    getaddrinfo(NULL, strPuerto, &hints, &servinfo);

    // Creamos el socket de escucha del servidor
    int socket_servidor;
    if ((socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
    {
        log_error_if_logger_not_null(logger, "Error al crear el socket de escucha, con puerto %s", strPuerto);
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
    free(strPuerto);

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

int enviar_instruccion_por_socket(int socket, identificador_operacion operacion, void *buffer_serializado, int bytes)
{
    int tamanio_buffer = sizeof(identificador_operacion) // El identificador de la instruccion
                         + sizeof(uint32_t)              // Los bytes totales del buffer (para que el servidor sepa cuantos bytes recibir del socket)
                         + bytes;
    void *buffer = malloc(tamanio_buffer);

    int desplazamiento = 0;
    escribir_uint32(buffer, &desplazamiento, operacion);
    escribir_uint32(buffer, &desplazamiento, bytes);
    escribir_buffer(buffer, &desplazamiento, buffer_serializado, bytes);

    int bytes_enviados = send(socket, buffer, tamanio_buffer, 0);
    free(buffer);

    return bytes_enviados;
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
