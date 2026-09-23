#include <stdio.h>
#include <stdlib.h>
#include "requisitos.h"

int requisito_cumplido(const char* codigo_requisito, Curso cursos[], int num_cursos) {
    Curso* curso = buscar_curso(cursos, num_cursos, codigo_requisito);
    if (curso == NULL) {
        return 0;
    }
    return curso->aprobado;
}

int correquisito_cumplido(const char* codigo_correquisito, Curso cursos[], int num_cursos) {
    Curso* curso = buscar_curso(cursos, num_cursos, codigo_correquisito);
    if (curso == NULL) {
        return 0;
    }
    if (curso->aprobado) {
        return 1;
    }
    else {
        for (int i = 0; i < curso->num_requisitos; i++) {
            if (!requisito_cumplido(curso->requisitos[i], cursos, num_cursos)) {
                return 0;
            }
        }
    }
    return 1;
}
void validar_requisitos(Curso cursos[], int num_cursos) {
    for (int i = 0; i < num_cursos; i++) {
        if (cursos[i].aprobado) {
            cursos[i].elegible = 0;
            continue;
        }

        int falla = 0;

        for (int j = 0; j < cursos[i].num_requisitos; j++) {
            if (!requisito_cumplido(cursos[i].requisitos[j], cursos, num_cursos)) {
                falla = 1;
                break;
            }
        }

        if (!falla) {
            for (int x = 0; x < cursos[i].num_correquisitos; x++) {
                if (!correquisito_cumplido(cursos[i].correquisitos[x], cursos, num_cursos)) {
                    falla = 1;
                    break;
                }
            }
        }

        if (falla) {
            cursos[i].elegible = 0;
        }
        else {
            cursos[i].elegible = 1;
        }
    }
}