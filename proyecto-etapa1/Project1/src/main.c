#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curso.h"
#include "parser.h"
#include "horario.h"
#include "catalogo.h"
#include "choques.h"
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

    return 1;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    Curso cursos[MAX_CURSOS];
    int num_cursos1 = 0;
    int num_cursos2 = 0;
    char historial[MAX_HISTORIAL][MAX_CODIGO];
    int num_historial1 = 0;
    int num_historial2 = 0;

    // 1. Cargar plan de estudios
    if (!parsear_plan_estudios(RUTA_PLANM, cursos, &num_cursos1)) {
        printf("Error al cargar el plan de estudios\n");
        return 1;
    }

    // 2. Cargar historial
    if (!cargar_historial(RUTA_HISTORIALM, historial, &num_historial1)) {
        printf("Error al cargar el historial\n");
        return 1;
    }

    // 3. Mostrar primeros cursos para verificar
    printf("\nPrimeros 20 cursos\n");
    for (int i = 0; i < num_cursos1 && i < 20; i++) {
        imprimir_curso(&cursos[i]);
        printf("\n");
    }

    // 1. Cargar plan de estudios
    if (!parsear_plan_estudios(RUTA_PLANC, cursos, &num_cursos2)) {
        printf("Error al cargar el plan de estudios\n");
        return 1;
    }

    // 2. Cargar historial
    if (!cargar_historial(RUTA_HISTORIALC, historial, &num_historial2)) {
        printf("Error al cargar el historial\n");
        return 1;
    }

    // 3. Mostrar primeros cursos para verificar
    printf("\nPrimeros 20 cursos\n");
    for (int i = 0; i < num_cursos2 && i < 20; i++) {
        imprimir_curso(&cursos[i]);
        printf("\n");
    }


    // TODO: Validar requisitos
    // TODO: Exportar JSON

    return 0;
}