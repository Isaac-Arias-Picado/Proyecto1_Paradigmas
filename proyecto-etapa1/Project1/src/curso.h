#pragma once
#ifndef CURSO_H
#define CURSO_H

#include "../include/constantes.h"

typedef struct {
    char codigo[MAX_CODIGO];
    char nombre[MAX_NOMBRE];
    int creditos;
    int horas;
    char requisitos[MAX_REQUISITOS][MAX_NOMBRE];
    int num_requisitos;
    char correquisitos[MAX_CORREQUISITOS][MAX_NOMBRE];
    int num_correquisitos;
    int semestre;
    int tiene_choque;
    int elegible;
} Curso;

Curso* crear_curso();
void liberar_curso(Curso* c);
void imprimir_curso(Curso* c);

#endif