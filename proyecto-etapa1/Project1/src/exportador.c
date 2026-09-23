#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "exportador.h"

void exportar_a_json(Curso cursos[], int num_cursos, const char* ruta_salida) {
    FILE* archivo = fopen(ruta_salida, "w");
    if (archivo == NULL) {
        return;
    }

    fprintf(archivo, "[\n");

    for (int i = 0; i < num_cursos; i++) {
        fprintf(archivo, "  {\n");
        fprintf(archivo, "    \"codigo\": \"%s\",\n", cursos[i].codigo);
        fprintf(archivo, "    \"nombre\": \"%s\",\n", cursos[i].nombre);
        fprintf(archivo, "    \"creditos\": %d,\n", cursos[i].creditos);

        fprintf(archivo, "    \"requisitos\": [");
        for (int j = 0; j < cursos[i].num_requisitos; j++) {
            fprintf(archivo, "\"%s\"", cursos[i].requisitos[j]);
            if (j < cursos[i].num_requisitos - 1) {
                fprintf(archivo, ", ");
            }
        }
        fprintf(archivo, "],\n");

        fprintf(archivo, "    \"correquisitos\": [");
        for (int j = 0; j < cursos[i].num_correquisitos; j++) {
            fprintf(archivo, "\"%s\"", cursos[i].correquisitos[j]);
            if (j < cursos[i].num_correquisitos - 1) {
                fprintf(archivo, ", ");
            }
        }
        fprintf(archivo, "],\n");

        fprintf(archivo, "    \"tiene_choque\": %s,\n", cursos[i].tiene_choque ? "true" : "false");

        fprintf(archivo, "    \"puede_matricular\": false,\n");

        fprintf(archivo, "    \"grupos\": [\n");
        for (int j = 0; j < cursos[i].num_grupos; j++) {
            fprintf(archivo, "      {\n");
            fprintf(archivo, "        \"numero\": %d,\n", cursos[i].grupos[j].numero);
            fprintf(archivo, "        \"tiene_choque\": %s,\n", cursos[i].grupos[j].tiene_choque ? "true" : "false");
            fprintf(archivo, "        \"horarios\": [\n");
            for (int k = 0; k < cursos[i].grupos[j].num_bloques; k++) {
                fprintf(archivo, "          {\n");
                fprintf(archivo, "            \"dia\": \"%s\",\n", cursos[i].grupos[j].horarios[k].dia);
                fprintf(archivo, "            \"hora_inicio\": \"%s\",\n", cursos[i].grupos[j].horarios[k].hora_inicio);
                fprintf(archivo, "            \"hora_fin\": \"%s\"\n", cursos[i].grupos[j].horarios[k].hora_fin);
                fprintf(archivo, "          }");
                if (k < cursos[i].grupos[j].num_bloques - 1) {
                    fprintf(archivo, ",");
                }
                fprintf(archivo, "\n");
            }
            fprintf(archivo, "        ]\n");
            fprintf(archivo, "      }");
            if (j < cursos[i].num_grupos - 1) {
                fprintf(archivo, ",");
            }
            fprintf(archivo, "\n");
        }
        fprintf(archivo, "    ]\n");

        fprintf(archivo, "  }");
        if (i < num_cursos - 1) {
            fprintf(archivo, ",");
        }
        fprintf(archivo, "\n");
    }

    fprintf(archivo, "]\n");
    fclose(archivo);
}