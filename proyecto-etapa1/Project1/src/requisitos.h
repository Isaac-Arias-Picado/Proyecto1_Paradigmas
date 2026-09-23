#pragma once
#pragma once
#ifndef REQUISITOS_H
#define REQUISITOS_H

#include "curso.h"
#include "catalogo.h"

int requisito_cumplido(const char* codigo_requisito, Curso cursos[], int num_cursos);
int correquisito_cumplido(const char* codigo_correquisito, Curso* curso_actual, Curso cursos[], int num_cursos);
void validar_requisitos(Curso cursos[], int num_cursos);
#endif