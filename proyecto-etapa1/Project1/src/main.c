#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curso.h"
#include "parser.h"
#include "exportador.h"
#include "horario.h"
#include "catalogo.h"
#include "choques.h"
#include "requisitos.h"
#include "../include/constantes.h"
#include <windows.h>

#define MAX_HISTORIAL 100

static int procesar_carrera(const char* ruta_plan, const char* ruta_historial, const char* ruta_horarios,
    Curso cursos[], int* num_cursos) {
    memset(cursos, 0, sizeof(Curso) * MAX_CURSOS);

    if (!parsear_plan_estudios(ruta_plan, cursos, num_cursos)) {
        printf("Error al cargar el plan de estudios\n");
        return 0;
    }

    char historial[MAX_HISTORIAL][MAX_CODIGO];
    int num_historial = 0;
    if (!cargar_historial(ruta_historial, historial, &num_historial)) {
        printf("Error al cargar el historial\n");
        return 0;
    }

    BloqueHorario bloques[MAX_BLOQUES_HORARIO];
    int num_bloques = 0;
    if (!parsear_horarios(ruta_horarios, bloques, MAX_BLOQUES_HORARIO, &num_bloques)) {
        printf("Error al cargar los horarios\n");
        return 0;
    }

    asociar_horarios(cursos, *num_cursos, bloques, num_bloques);
    marcar_aprobados(cursos, *num_cursos, historial, num_historial);
    detectar_choques(cursos, *num_cursos);
    validar_requisitos(cursos, *num_cursos);

    return 1;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // 1. Procesamiento de Ingeniería en Materiales
    Curso cursos_materiales[MAX_CURSOS];
    int num_materiales = 0;
    procesar_carrera(RUTA_PLANM, RUTA_HISTORIALM, RUTA_HORARIOSM, cursos_materiales, &num_materiales);

    // Exportar catálogo de Materiales a JSON
    exportar_a_json(cursos_materiales, num_materiales, "data/salida/catalogo_materiales.json");

    // 2. Procesamiento de Ingeniería en Computadores
    Curso cursos_computadores[MAX_CURSOS];
    int num_computadores = 0;
    procesar_carrera(RUTA_PLANC, RUTA_HISTORIALC, RUTA_HORARIOSC, cursos_computadores, &num_computadores);

    // Exportar catálogo de Computadores a JSON
    exportar_a_json(cursos_computadores, num_computadores, "data/salida/catalogo_computadores.json");

    return 0;
}