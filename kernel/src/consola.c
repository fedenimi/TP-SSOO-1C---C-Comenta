#include <../include/consola.h>

void empezar_hilo_consola(pthread_t* hilo_consola, t_log* logger, int socket) {
    leer_consola_t* args = malloc(sizeof(leer_consola_t));;
    args->logger = logger;
    args->socket = socket;
    pthread_create(hilo_consola, NULL, (void*) leer_consola, (void*) args);
}

void leer_consola(void* args_void)
{
    leer_consola_t* args = (leer_consola_t*) args_void;
    t_log* logger = args->logger;
    int socket = args->socket;
    char *leido;

    while (1) {
       leido = readline("> ");
       if (!strcmp(leido, "")) {
           break;
       }
       procesar_instruccion(leido, logger, socket);
       free(leido);
   }
   free(leido);
}

void procesar_instruccion(char* instruccion, t_log* logger, int socket) {  
    log_info(logger, "INSTRUCCION: %s", instruccion);
    char** substrings = string_split(instruccion, " ");
    char* comando = substrings[0];
    log_info(logger, "Comando: %s", comando);
    if(!strcmp(comando, "EJECUTAR_SCRIPT"))
    {
        char* path_script = substrings[1];
            if(existe_archivo(path_script)) {
                ejecutar_script(path_script, logger, socket);
            }
    }
    else if(!strcmp(comando, "INICIAR_PROCESO")) {
        
        char* path_proceso = substrings[1];
        enviar_inicio_proceso(socket, path_proceso, logger);
        uint32_t pid;
        recv(socket, &pid, sizeof(uint32_t), 0);
        log_info(logger, "PID: %d", pid);
        if(pid != 0) {
            proceso_t* proceso = crear_pcb(pid);
            pthread_t hilo_proceso;
            nuevo_proceso_t* new_proceso = malloc(sizeof(nuevo_proceso_t));
            new_proceso->proceso = proceso;
            new_proceso->logger = logger;
            pthread_create(&hilo_proceso, NULL, (void*) planificar_nuevo_proceso, (void*) new_proceso);
            pthread_detach(hilo_proceso);
        }
        }
    else if(!strcmp(comando, "FINALIZAR_PROCESO"))
    {

    }
    else if(!strcmp(comando, "DETENER_PLANIFICACION"))
    {

    }
    else if(!strcmp(comando, "INICIAR_PLANIFICACION"))
    {

    }
    else if(!strcmp(comando, "MULTIPROGRAMACION"))
    {

    }
    else if(!strcmp(comando, "PROCESO_ESTADO"))
    {

    }

    string_array_destroy(substrings);
    //le mandamos a la memoria --> send(socket, estructura con opcode y path, tamanio de struct, )
}

void enviar_inicio_proceso(int socket, char* path, t_log* logger) {
    log_info(logger, "PATH: %s", path);
    void* stream = malloc(sizeof(op_code) + 2* sizeof(uint32_t) + string_length(path) + 1);
    int offset = 0;
    agregar_opcode(stream, &offset, INICIAR_PROCESO);
    agregar_string(stream, &offset, path);
    agregar_uint32_t(stream, &offset, pid_siguiente);
    send(socket, stream, offset, 0);
    pid_siguiente++;
    free(stream);
}

void ejecutar_script(char* path, t_log* logger, int socket) {
    FILE *f = fopen(path, "r");
    char* linea;
    size_t longitud = 0;
    ssize_t leidos;
    fseek(f, 0, SEEK_SET);
    while((leidos = getline(&linea, &longitud, f)) != -1){

        if (linea[leidos - 1] == '\n') {
            linea[leidos - 1] = '\0';
        }
        log_info(logger, "LINEA: %s", linea);
        procesar_instruccion(linea, logger, socket);
    }
    fclose(f);
}

bool existe_archivo(char* path) {
    return fopen(path, "r") != NULL;
}