#include <stdlib.h>
#include <stdio.h>
#include <../include/init.h>
#include <../include/main.h>

void get_config(t_config* config) {
    puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");
    ip_kernel = config_get_string_value(config, "IP_KERNEL");
}

int main(int argc, char* argv[]) {
    t_log* logger_io = iniciar_logger("io.log", "I/O: ");
  int init;
  scanf("%d", &init);
  log_info(logger_io, "%d", init);
   op_code interfaz;
    switch(init)
    {
        case 1:
            interfaz = GENERICA;
            break;

        case 2:
            interfaz = STDIN;
            break;

        case 3:
            interfaz = STDOUT;
            break;

        case 4:
            interfaz = DIALFS;
            break;

        default:
        log_error(logger_io, "%s", "Interfaz inválida");
        return;

    }
    log_info(logger_io, "INTERFAZ: %d", interfaz);
    t_config* config_io = iniciar_config("io.config");
    get_config(config_io);

// Se conecta al kernel
    int kernel_fd = generar_conexion(logger_io, "kernel", ip_kernel, puerto_kernel, config_io);

    //Se conecta a la memoria
    int memoria_fd = generar_conexion(logger_io, "memoria", ip_memoria, puerto_memoria, config_io);

    

    terminar_programa(logger_io, config_io);
    liberar_conexion(memoria_fd);
    liberar_conexion(kernel_fd);
    return 0;
}
