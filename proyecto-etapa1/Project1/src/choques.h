#ifndef CHOQUES_H
#define CHOQUES_H

#include "curso.h"

int grupos_chocan(const Grupo* a, const Grupo* b);
void detectar_choques(Curso cursos[], int num_cursos);
int cursos_incompatibles(const Curso* a, const Curso* b);
void reportar_choques_totales(Curso cursos[], int num_cursos);

#endif