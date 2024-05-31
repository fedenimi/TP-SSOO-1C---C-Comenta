#ifndef MAIN_H_
#define MAIN_H_

#include <stdlib.h>
#include <stdio.h>
#include <../include/init.h>
#include <../include/conexion.h>
#include <../include/consola.h>
#include <pthread.h>
#include <commons/collections/list.h>
#include <commons/collections/dictionary.h>

char* puerto_memoria;
char* ip_memoria;
char* puerto_cpu_dispatch;
char* ip_cpu;
char* puerto_cpu_interrupt;
char* puerto_escucha;
pthread_t hilo_consola;
int quantum;
char** recursos;
int* instancias_recursos;
int cantidad_recursos;
char* algoritmo_planificacion;
int grado_multiprogramacion;
t_list* pcbs_new;
t_list* pcbs_ready;
t_list* pcbs_ready_prioritarios;
t_list* pcbs_exec;
t_queue* pcbs_exit;
t_list* pcbs_generica;
t_list* pcbs_stdin;
t_list* pcbs_stdout;
t_list* pcbs_dialfs;
t_list** pcbs_recursos;
int procesos_activos;
uint32_t pid_siguiente;
int cpu_dispatch_fd;
int cpu_interrupt_fd;
int memoria_interrupt_fd;
t_dictionary* diccionario_interfaces;
t_log* logger_kernel;
pthread_mutex_t mutex_new_list;
pthread_mutex_t mutex_ready_list;
pthread_mutex_t mutex_ready_prioritario_list;
pthread_mutex_t mutex_exec_list;
pthread_mutex_t mutex_exit_queue;
pthread_mutex_t mutex_generica_list;
pthread_mutex_t mutex_generica_exec;
pthread_mutex_t mutex_stdin_list;
pthread_mutex_t mutex_stdin_exec;
pthread_mutex_t mutex_stdout_list;
pthread_mutex_t mutex_stdout_exec;
pthread_mutex_t mutex_dialfs_list;
pthread_mutex_t* mutex_recursos_list;
pthread_mutex_t* mutex_recursos_instancias;
sem_t multiprogramacion;
sem_t pcb_esperando_ready;
sem_t pcb_esperando_exec;
sem_t pcb_esperando_exit;
sem_t pcb_esperando_generica;
sem_t pcb_esperando_stdin;
sem_t pcb_esperando_stdout;
sem_t pcb_esperando_dialfs;
sem_t vuelta_io_gen_sleep;
sem_t vuelta_io_stdin_read;
sem_t vuelta_io_stdout_write;
sem_t* pcb_esperando_recurso;
uint32_t pid_a_finalizar;
int fin_a_proceso_sleep;
int disminuciones_multiprogramacion;
#endif