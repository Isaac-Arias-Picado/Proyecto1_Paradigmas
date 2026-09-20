#include <stdio.h>
#include "choques.h"

int grupos_chocan(const Grupo* a, const Grupo* b) {
    for (int i = 0; i < a->num_bloques; i++) {
        for (int j = 0; j < b->num_bloques; j++) {
            if (horarios_chocan(&a->horarios[i], &b->horarios[j])) {
                return 1;
            }
        }
    }
    return 0;
}

void detectar_choques(Curso cursos[], int num_cursos) {
    for (int i = 0; i < num_cursos; i++) {
        for (int j = i + 1; j < num_cursos; j++) {
            for (int x = 0; x < cursos[i].num_grupos; x++) {
                for (int y = 0; y < cursos[j].num_grupos; y++) {
                    if (grupos_chocan(&cursos[i].grupos[x], &cursos[j].grupos[y])) {
                        cursos[i].grupos[x].tiene_choque = 1;
                        cursos[i].tiene_choque = 1;
                        cursos[j].grupos[y].tiene_choque = 1;
                        cursos[j].tiene_choque = 1;
                    }
                }
            }
        }
    }
}