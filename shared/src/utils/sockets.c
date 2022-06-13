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
    else
    {
        bool connect_exitoso = false;
        int maximo_intentos = 10;
        time_miliseg milisegundos_sleep = 1000;
        for (int intento_actual = 1; intento_actual <= maximo_intentos; intento_actual++)
        {
            log_trace_if_logger_not_null(logger, "Intento %d/%d de conexion con IP %s, PUERTO %s", intento_actual, maximo_intentos, ip, strPuerto);

            if (connect(socket_fd, servinfo->ai_addr, servinfo->ai_addrlen) != -1)
            {
                log_trace_if_logger_not_null(logger, "Intento %d/%d exitoso", intento_actual, maximo_intentos);
                connect_exitoso = true;
                break;
            }

            log_error_if_logger_not_null(logger, "Intento %d/%d fallido", intento_actual, maximo_intentos);
            usleep(milisegundos_a_microsegundos(milisegundos_sleep)); // Sleep para darle tiempo al servidor de levantar
        }

        if (connect_exitoso)
        {
            log_trace_if_logger_not_null(logger, "Conexion con IP %s, PUERTO %s establecida correctamente", ip, strPuerto);
        }
        else
        {
            log_trace_if_logger_not_null(logger, "No se pudo establecer la conexion con IP %s, PUERTO %s", ip, strPuerto);
            error = true;
        }
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
        int activado = 1;
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
    int socket_cliente = accept(socket_servidor, NULL, NULL);
    return socket_cliente;
}

int enviar_buffer_serializado_con_bytes_por_socket(int socket, void *buffer_serializado, int bytes)
{
    int tamanio_buffer = sizeof(uint32_t) // Tamanio total del buffer en bytes (para despues recibirlo del socket)
                         + bytes;
    void *buffer_con_tamanio = malloc(tamanio_buffer);

    int desplazamiento = 0;
    escribir_uint32_en_buffer(buffer_con_tamanio, &desplazamiento, bytes);
    escribir_en_buffer(buffer_con_tamanio, &desplazamiento, buffer_serializado, bytes);

    int bytes_enviados = enviar_buffer_por_socket(socket, buffer_con_tamanio, tamanio_buffer);
    free(buffer_con_tamanio);

    return bytes_enviados;
}

int enviar_buffer_por_socket(int socket, void *buffer_serializado, int bytes)
{
    int bytes_enviados = 0;
    while (bytes_enviados < bytes)
    {
        bytes_enviados += send(socket, buffer_serializado, bytes, 0);
    }
    return bytes_enviados;
}

int enviar_buffer_serializado_con_instruccion_y_bytes_por_socket(int socket, identificador_operacion operacion, void *buffer_serializado, int bytes)
{
    int tamanio_buffer = sizeof(identificador_operacion) // El identificador de la instruccion
                         + sizeof(uint32_t)              // Tamanio total del buffer en bytes (para despues recibirlo del socket)
                         + bytes;
    void *buffer_con_instruccion_y_tamanio = malloc(tamanio_buffer);

    int desplazamiento = 0;
    escribir_uint32_en_buffer(buffer_con_instruccion_y_tamanio, &desplazamiento, operacion);
    escribir_uint32_en_buffer(buffer_con_instruccion_y_tamanio, &desplazamiento, bytes);
    escribir_en_buffer(buffer_con_instruccion_y_tamanio, &desplazamiento, buffer_serializado, bytes);

    int bytes_enviados = enviar_buffer_por_socket(socket, buffer_con_instruccion_y_tamanio, tamanio_buffer);
    free(buffer_con_instruccion_y_tamanio);

    return bytes_enviados;
}

int enviar_string_por_socket(int socket, char *cadena)
{
    int bytes = strlen(cadena) + 1;
    char *alloc_cadena = malloc(bytes);
    memcpy(alloc_cadena, cadena, bytes);

    int result = enviar_buffer_por_socket(socket, cadena, bytes);

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

    int result = enviar_buffer_por_socket(socket, alloc_numero, sizeof(uint32_t));

    free(alloc_numero);
    return result;
}

int recibir_buffer_por_socket(int socket, void *buffer, int bytes)
{
    int bytes_recibidos = 0;
    while (bytes_recibidos < bytes)
    {
        bytes_recibidos += recv(socket, buffer, bytes, MSG_WAITALL);
    }
    return bytes_recibidos;
}

int recibir_buffer_con_bytes_por_socket(int socket, void **buffer)
{
    uint32_t bytes_buffer;
    recibir_uint32_por_socket(socket, &bytes_buffer);
    (*buffer) = malloc(bytes_buffer);
    return recibir_buffer_por_socket(socket, *buffer, bytes_buffer);
}

int recibir_string_con_longitud_por_socket(int socket, char **buffer)
{
    uint32_t tamanio;
    recibir_uint32_por_socket(socket, &tamanio);

    (*buffer) = malloc(tamanio);
    return recibir_buffer_por_socket(socket, (*buffer), tamanio);
}

int recibir_uint32_por_socket(int socket, uint32_t *numero)
{
    return recibir_buffer_por_socket(socket, numero, sizeof(uint32_t));
}

void liberar_conexion(int socket)
{
    close(socket);
}
