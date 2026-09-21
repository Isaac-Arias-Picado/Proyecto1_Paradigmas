#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "horario.h"

static int hora_a_minutos(const char* hhmm) {
    int h = 0, m = 0;
    sscanf(hhmm, "%d:%d", &h, &m);
    return h * 60 + m;
}

int horarios_chocan(const BloqueHorario* a, const BloqueHorario* b) {
    if (strcmp(a->dia, b->dia) != 0) return 0;
    int inicioA = hora_a_minutos(a->hora_inicio);
    int finA = hora_a_minutos(a->hora_fin);
    int inicioB = hora_a_minutos(b->hora_inicio);
    int finB = hora_a_minutos(b->hora_fin);
    return (inicioA < finB) && (inicioB < finA);
}

static int es_codigo_curso(const char* texto) {
    int len = (int)strlen(texto);
    if (len != LONG_CODIGO) return 0;
    if (!isupper((unsigned char)texto[0]) || !isupper((unsigned char)texto[1])) return 0;
    for (int i = 2; i < len; i++) {
        if (!isdigit((unsigned char)texto[i])) return 0;
    }
    return 1;
}

static void agregar_bloques_de_horario(const char* codigo, int grupo, char* horario_texto, BloqueHorario bloques[], int max_bloques, int* num_bloques) {
    char* token = strtok(horario_texto, " ");
    char formato[MAX_FORMATO];
    snprintf(formato, sizeof(formato), "%%%d[^[][%%%d[^-]-%%%d[^]]]", LONG_DIA, LONG_HORA, LONG_HORA);
    while (token != NULL) {
        char dia[MAX_DIA] = { 0 };
        char hora_inicio[MAX_HORA] = { 0 };
        char hora_fin[MAX_HORA] = { 0 };
        if (sscanf(token, formato, dia, hora_inicio, hora_fin) == CAMPOS_POR_BLOQUE) {
            if (*num_bloques < max_bloques) {
                strcpy(bloques[*num_bloques].codigo_curso, codigo);
                bloques[*num_bloques].grupo = grupo;
                strcpy(bloques[*num_bloques].dia, dia);
                strcpy(bloques[*num_bloques].hora_inicio, hora_inicio);
                strcpy(bloques[*num_bloques].hora_fin, hora_fin);
                (*num_bloques)++;
            }
        }
        token = strtok(NULL, " ");
    }
}

int parsear_horarios(const char* ruta, BloqueHorario bloques[], int max_bloques, int* num_bloques) {
    FILE* archivo = fopen(ruta, "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir %s\n", ruta);
        return 0;
    }

    char linea[MAX_LINEA];
    *num_bloques = 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\r\n")] = 0;
        if (strlen(linea) == 0) continue;

        char* campos[MAX_CAMPOS];
        int num_campos = 0;
        char* token = strtok(linea, "\t");
        while (token != NULL && num_campos < MAX_CAMPOS) {
            campos[num_campos++] = token;
            token = strtok(NULL, "\t");
        }
        if (num_campos < 2) continue;

        int pos_codigo = -1;
        for (int i = 0; i < num_campos; i++) {
            if (es_codigo_curso(campos[i])) {
                pos_codigo = i;
                break;
            }
        }
        if (pos_codigo == -1) continue;

        char* codigo = campos[pos_codigo];
        int grupo = 0;
        sscanf(campos[pos_codigo + OFFSET_GRUPO], "%d", &grupo);
        char* horario = campos[num_campos - 1];
        agregar_bloques_de_horario(codigo, grupo, horario, bloques, max_bloques, num_bloques);
    }
    fclose(archivo);
    return 1;
}