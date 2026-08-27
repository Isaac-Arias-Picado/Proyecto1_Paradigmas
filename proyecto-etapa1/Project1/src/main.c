#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curso.h"
#include "parser.h"
#include "../include/constantes.h"

#define MAX_HISTORIAL 100

int main() {
    Curso cursos[MAX_CURSOS];
    int num_cursos = 0;
    char historial[MAX_HISTORIAL][MAX_CODIGO];
    int num_historial = 0;

    // 1. Cargar plan de estudios
    if (!parsear_plan_estudios(RUTA_PLAN, cursos, &num_cursos)) {
        printf("Error al cargar el plan de estudios\n");
        return 1;
    }

    // 2. Cargar historial
    if (!cargar_historial(RUTA_HISTORIAL, historial, &num_historial)) {
        printf("Error al cargar el historial\n");
        return 1;
    }

    // 3. Mostrar primeros cursos para verificar
    printf("\nPrimeros 20 cursos\n");
    for (int i = 0; i < num_cursos && i < 20; i++) {
        imprimir_curso(&cursos[i]);
        printf("\n");
    }

    // TODO: Validar requisitos
    // TODO: Detectar choques (necesita horarios)
    // TODO: Exportar JSON

    return 0;
}