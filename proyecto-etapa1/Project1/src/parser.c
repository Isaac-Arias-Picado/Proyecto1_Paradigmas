#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "curso.h"

int parsear_plan_estudios(const char* ruta, Curso cursos[], int* num_cursos) {
    FILE* archivo = fopen(ruta, "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir %s\n", ruta);
        return 0;
    }

    char linea[MAX_LINEA];
    *num_cursos = 0;
    int leyendo_curso = 0;
    int semestre_actual = -1;
    int campo_actual = 0; // 0=nada, 1=nombre, 2=creditos, 3=codigo, 4=requisitos, 5=correquisitos

    Curso* c = NULL;

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = 0;

        // Detectar semestre
        if (strstr(linea, "Semestre") != NULL) {
            char* p = strstr(linea, "Semestre");
            sscanf(p + 8, "%d", &semestre_actual);
            continue;
        }

        if (strlen(linea) == 0) {
            leyendo_curso = 0;
            continue;
        }

        if (linea[0] != ' ' && linea[0] != '\t' &&
            strstr(linea, "Créditos") == NULL &&
            strstr(linea, "Código") == NULL &&
            strstr(linea, "Corequisitos") == NULL &&
            strstr(linea, "Requisitos") == NULL &&
            strstr(linea, "Nombres Equivalentes") == NULL &&
            strstr(linea, "Nota") == NULL &&
            strstr(linea, "Periodo") == NULL &&
            strstr(linea, "Semestre") == NULL) {

            if (c != NULL && c->codigo[0] != '\0') {
                cursos[(*num_cursos)++] = *c;
                free(c);
            }

            c = crear_curso();
            if (c == NULL) {
                printf("Error: No se pudo crear curso\n");
                fclose(archivo);
                return 0;
            }
            strcpy(c->nombre, linea);
            c->semestre = semestre_actual;
            leyendo_curso = 1;
            continue;
        }
        if (strstr(linea, "Créditos") != NULL) {
            sscanf(linea, "%d Créditos / %d Horas", &c->creditos, &c->horas);
            continue;
        }

        // Parsear código
        if (strstr(linea, "Código de curso") != NULL) {
            char* p = strstr(linea, ": ");
            if (p != NULL) {
                strcpy(c->codigo, p + 2);
            }
            continue;
        }

        // Parsear requisitos
        if (strstr(linea, "Requisitos:") != NULL) {
            char* p = strstr(linea, ": ");
            if (p != NULL) {
                char* reqs = p + 2;
                if (strstr(reqs, "No posee requisitos") == NULL) {
                    char* token = strtok(reqs, ",");
                    while (token != NULL && c->num_requisitos < MAX_REQUISITOS) {
                        while (*token == ' ') token++;
                        strcpy(c->requisitos[c->num_requisitos++], token);
                        token = strtok(NULL, ",");
                    }
                }
            }
            continue;
        }

        // Parsear correquisitos
        if (strstr(linea, "Corequisitos:") != NULL) {
            char* p = strstr(linea, ": ");
            if (p != NULL) {
                char* reqs = p + 2;
                if (strstr(reqs, "No posee corequisitos") == NULL) {
                    char* token = strtok(reqs, ",");
                    while (token != NULL && c->num_correquisitos < MAX_CORREQUISITOS) {
                        while (*token == ' ') token++;
                        strcpy(c->correquisitos[c->num_correquisitos++], token);
                        token = strtok(NULL, ",");
                    }
                }
            }
            continue;
        }
    }

    if (c != NULL && c->codigo[0] != '\0') {
        cursos[(*num_cursos)++] = *c;
        free(c);
    }

    fclose(archivo);
    return 1;
}

int cargar_historial(const char* ruta, char historial[][MAX_CODIGO], int* num_historial) {
    FILE* archivo = fopen(ruta, "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir %s\n", ruta);
        return 0;
    }

    char linea[MAX_CODIGO];
    *num_historial = 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = 0;
        if (strlen(linea) > 0) {
            strcpy(historial[*num_historial], linea);
            (*num_historial)++;
        }
    }

    fclose(archivo);
    return 1;
}