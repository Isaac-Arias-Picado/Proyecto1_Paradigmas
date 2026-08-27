#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curso.h"

Curso* crear_curso() {
    Curso* c = (Curso*)malloc(sizeof(Curso));
    if (c == NULL) return NULL;
    memset(c, 0, sizeof(Curso));
    return c;
}

void liberar_curso(Curso* c) {
    if (c != NULL) free(c);
}

void imprimir_curso(Curso* c) {
    if (c == NULL) return;
    printf("Codigo: %s\n", c->codigo);
    printf("Nombre: %s\n", c->nombre);
    printf("Creditos: %d\n", c->creditos);
    printf("Semestre: %d\n", c->semestre);
    printf("Requisitos: ");
    for (int i = 0; i < c->num_requisitos; i++) {
        printf("%s ", c->requisitos[i]);
    }
    printf("\n");
}