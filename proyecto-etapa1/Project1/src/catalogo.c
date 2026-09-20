#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "catalogo.h"

Curso* buscar_curso(Curso cursos[], int num_cursos, const char* codigo) {
    for (int i = 0; i < num_cursos; i++) {
        if (strcmp(cursos[i].codigo, codigo) == 0) {
            return &cursos[i];
        }
    }
    
    return NULL;
}

Grupo* buscar_o_crear_grupo(Curso* curso, int numero) {
    for (int i = 0; i < curso->num_grupos; i++) {
        if (curso->grupos[i].numero == numero) {
            return &curso->grupos[i];
        }
    }
    if (curso->num_grupos >= MAX_GRUPOS) {
        printf("No es posible crear mas grupos para el curso: %s\n", curso->nombre);
        return NULL;
    }
        Grupo* nuevo = &curso->grupos[curso->num_grupos];
        nuevo->numero = numero;
        nuevo->num_bloques = 0;
        nuevo->tiene_choque = 0;
        curso->num_grupos++;
        return nuevo;
}

int asociar_horarios(Curso cursos[], int num_cursos, BloqueHorario bloques[], int num_bloques) {
    int asociados = 0;
    for (int i = 0; i < num_bloques; i++) {
        int curso_encontrado = 0;

        for (int j = 0; j < num_cursos; j++) {
            if (strcmp(cursos[j].codigo, bloques[i].codigo_curso) == 0) {
                curso_encontrado = 1;
                Grupo* g = buscar_o_crear_grupo(&cursos[j], bloques[i].grupo);
                if (g == NULL) {
                    break;
                }
                else if (g->num_bloques >= MAX_BLOQUES_GRUPO) {
                    printf("Aviso: el grupo %d de %s ya tiene el maximo de bloques\n", g->numero, cursos[j].codigo);
                }
                else {
                    g->horarios[g->num_bloques] = bloques[i];
                    g->num_bloques++;
                    asociados++;
                }
                break;
            }
        }

        if (!curso_encontrado) {
            printf("Aviso: el curso %s del horario no esta en el plan\n", bloques[i].codigo_curso);
        }
    }
    return asociados;
}

void reportar_cursos_sin_grupos(Curso cursos[], int num_cursos) {
    printf("Cursos sin horarios:\n");
    int cursos_sin_horario = 0;
    for (int i = 0; i < num_cursos; i++) {
        if (cursos[i].num_grupos == 0) {
            printf("Codigo: %s, Nombre: %s, Semestre: %d\n",cursos[i].codigo,cursos[i].nombre,cursos[i].semestre);
            cursos_sin_horario++;
        }
    }
    printf("Hay %d cursos sin horario\n", cursos_sin_horario);
}