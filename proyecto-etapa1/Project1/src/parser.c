#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
    int semestre_actual = -1;
    Curso* c = NULL;

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = 0;
        linea[strcspn(linea, "\r")] = 0;

        if (strlen(linea) == 0) {
            continue;
        }

        if (strncmp(linea, "Semestre", 8) == 0) {
            sscanf(linea + 8, "%d", &semestre_actual);
            continue;
        }

        if (linea[0] >= '0' && linea[0] <= '9' && strstr(linea, "Créditos") != NULL) {
            if (c != NULL) {
                sscanf(linea, "%d Créditos / %d Horas", &c->creditos, &c->horas);
            }
            continue;
        }

        if (strncmp(linea, "Código de curso", 15) == 0) {
            if (c != NULL) {
                char* p = strstr(linea, ": ");
                if (p != NULL) {
                    strcpy(c->codigo, p + 2);
                }
            }
            continue;
        }

        if (strncmp(linea, "Corequisitos:", 13) == 0) {
            if (c != NULL) {
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
            }
            continue;
        }

        if (strncmp(linea, "Requisitos:", 11) == 0) {
            if (c != NULL) {
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
            }
            continue;
        }

        if (strncmp(linea, "Nombres Equivalentes", 20) == 0 ||
            strncmp(linea, "Sin nombres equivalentes", 24) == 0 ||
            strncmp(linea, "Nota", 4) == 0 ||
            strncmp(linea, "Periodo", 7) == 0 ||
            strncmp(linea, "No posee", 8) == 0) {
            continue;
        }

        if (isdigit((unsigned char)linea[0]) && linea[1] == '.') {
            continue;
        }

        if (c != NULL && c->codigo[0] != '\0') {
            cursos[(*num_cursos)++] = *c;
            free(c);
            c = NULL;
        }

        c = crear_curso();
        if (c == NULL) {
            printf("Error: No se pudo crear curso\n");
            fclose(archivo);
            return 0;
        }
        strcpy(c->nombre, linea);
        c->semestre = semestre_actual;
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
        linea[strcspn(linea, "\r")] = 0;
        if (strlen(linea) > 0) {
            strcpy(historial[*num_historial], linea);
            (*num_historial)++;
        }
    }

    fclose(archivo);
    return 1;
}