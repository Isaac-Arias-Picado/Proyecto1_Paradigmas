#pragma once
#ifndef CATALOGO_H
#define CATALOGO_H

#include "curso.h"
#include "horario.h"

Curso* buscar_curso(Curso cursos[], int num_cursos, const char* codigo);
Grupo* buscar_o_crear_grupo(Curso* curso, int numero);
int asociar_horarios(Curso cursos[], int num_cursos, BloqueHorario bloques[], int num_bloques);

#endif