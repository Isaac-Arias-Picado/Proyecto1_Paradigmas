#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curso.h"
#include "parser.h"
#include "exportador.h"
#include "../include/constantes.h"
#include <windows.h>

#define MAX_HISTORIAL 100

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

    exportar_a_json(cursos, num_cursos1, "data/salida/catalogo_materiales.json");

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

    exportar_a_json(cursos, num_cursos2, "data/salida/catalogo_computadores.json");

    // TODO: Validar requisitos
    // TODO: Detectar choques (necesita horarios)


    return 0;
}