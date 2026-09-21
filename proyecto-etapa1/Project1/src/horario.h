#pragma once 
#ifndef HORARIO_H
#define HORARIO_H
#include "../include/constantes.h"

typedef struct {
	char codigo_curso[MAX_CODIGO];
	int grupo;
	char hora_inicio[MAX_HORA];
	char hora_fin[MAX_HORA];
	char dia[MAX_DIA];
} BloqueHorario;

typedef struct {
	int numero;
	BloqueHorario horarios[MAX_BLOQUES_GRUPO];
	int num_bloques;
	int tiene_choque;

} Grupo;

int horarios_chocan(const BloqueHorario* a, const BloqueHorario* b);
int parsear_horarios(const char* ruta, BloqueHorario bloques[], int max_bloques, int* num_bloques);

#endif
