#include <../include/planificador.h>

void planificar_nuevo_proceso(proceso_t* proceso, t_log* logger)
{
    list_add(pcbs_new, proceso);
    log_info(logger, "Se crea el proceso <%d> en NEW", proceso->pid);
    if(procesos_activos < grado_multiprogramacion)
    {
        log_info(logger, "PID: <%d> - Estado Anterior: <NEW> - Estado Actual: <READY>", proceso->pid);
        procesos_activos++;
        list_add(pcbs_ready,proceso);
        list_remove_element(pcbs_new, proceso);
        if(list_is_empty(pcbs_exec))
        {
            log_info(logger, "PID: <%d> - Estado Anterior: <READY> - Estado Actual: <EXEC>", proceso->pid);
            list_add(pcbs_exec, proceso);
            list_remove_element(pcbs_ready,proceso);
            ejecutar_proceso(proceso, logger);
        }
    }
}

void ejecutar_proceso(proceso_t* proceso, t_log* logger) {
    log_info(logger, "Algoritmo: %s", algoritmo_planificacion);
    if(!strcmp(algoritmo_planificacion, "FIFO"))
    {
        log_info(logger, "Se envia el proceso <%d> a CPU", proceso->pid);
        enviar_proceso_a_cpu(proceso);
    }
    else if(algoritmo_planificacion == "RR" || algoritmo_planificacion == "VRR")
    {
        enviar_proceso_a_cpu_con_timer(proceso);
    }
    esperar_contexto_de_ejecucion(proceso);
}

void enviar_proceso_a_cpu(proceso_t* proceso)
{
    void* stream = malloc(sizeof(op_code) + 9 * sizeof(uint32_t) + 4 * sizeof(uint8_t));
    int offset = 0;
    agregar_opcode(stream, &offset, ENVIAR_PCB);
    agregar_uint32_t(stream, &offset, proceso->pid);
    agregar_uint32_t(stream, &offset, proceso->quantum);
    agregar_uint32_t(stream, &offset, proceso->registros->PC);
    agregar_uint8_t(stream, &offset, proceso->registros->AX);
    agregar_uint8_t(stream, &offset, proceso->registros->BX);
    agregar_uint8_t(stream, &offset, proceso->registros->CX);
    agregar_uint8_t(stream, &offset, proceso->registros->DX);
    agregar_uint32_t(stream, &offset, proceso->registros->EAX);
    agregar_uint32_t(stream, &offset, proceso->registros->EBX);
    agregar_uint32_t(stream, &offset, proceso->registros->ECX);
    agregar_uint32_t(stream, &offset, proceso->registros->EDX);
    agregar_uint32_t(stream, &offset, proceso->registros->SI);
    agregar_uint32_t(stream, &offset, proceso->registros->DI);
    send(cpu_dispatch_fd, stream, offset, 0);
}

void agregar_pcb(void* stream, int* offset, proceso_t* proceso)
{
    agregar_opcode(stream, &offset, ENVIAR_PCB);
    agregar_uint32_t(stream, &offset, proceso->pid);
    agregar_uint32_t(stream, &offset, proceso->quantum);
    agregar_uint32_t(stream, &offset, proceso->registros->PC);
    agregar_uint8_t(stream, &offset, proceso->registros->AX);
    agregar_uint8_t(stream, &offset, proceso->registros->BX);
    agregar_uint8_t(stream, &offset, proceso->registros->CX);
    agregar_uint8_t(stream, &offset, proceso->registros->DX);
    agregar_uint32_t(stream, &offset, proceso->registros->EAX);
    agregar_uint32_t(stream, &offset, proceso->registros->EBX);
    agregar_uint32_t(stream, &offset, proceso->registros->ECX);
    agregar_uint32_t(stream, &offset, proceso->registros->EDX);
    agregar_uint32_t(stream, &offset, proceso->registros->SI);
    agregar_uint32_t(stream, &offset, proceso->registros->DI);
}

void enviar_proceso_a_cpu_con_timer(proceso_t* proceso) {

}

void esperar_contexto_de_ejecucion(proceso_t* proceso)
{
    uint32_t pid;
    uint32_t quantum;
    uint32_t PC;
    uint8_t AX;
    uint8_t BX;
    uint8_t CX;
    uint8_t DX;
    uint32_t EAX;
    uint32_t EBX;
    uint32_t ECX;
    uint32_t EDX;
    uint32_t SI;
    uint32_t DI;
    recv(cpu_dispatch_fd, &pid, sizeof(uint32_t), 0);
    recv(cpu_dispatch_fd, &quantum, sizeof(uint32_t), 0);
    recv(cpu_dispatch_fd, &PC, sizeof(uint32_t), 0);
    recv(cpu_dispatch_fd, &AX, sizeof(uint8_t), 0);
    recv(cpu_dispatch_fd, &BX, sizeof(uint8_t), 0);
    recv(cpu_dispatch_fd, &CX, sizeof(uint8_t), 0);
    recv(cpu_dispatch_fd, &DX, sizeof(uint8_t), 0);
    recv(cpu_dispatch_fd, &EAX, sizeof(uint32_t), 0);
    recv(cpu_dispatch_fd, &EBX, sizeof(uint32_t), 0);
    recv(cpu_dispatch_fd, &ECX, sizeof(uint32_t), 0);
    recv(cpu_dispatch_fd, &EDX, sizeof(uint32_t), 0);
    recv(cpu_dispatch_fd, &SI, sizeof(uint32_t), 0);
    recv(cpu_dispatch_fd, &DI, sizeof(uint32_t), 0);  
}