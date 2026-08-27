#pragma once
#ifndef PARSER_H
#define PARSER_H

#include "curso.h"

int parsear_plan_estudios(const char* ruta, Curso cursos[], int* num_cursos);
int cargar_historial(const char* ruta, char historial[][MAX_CODIGO], int* num_historial);

#endif