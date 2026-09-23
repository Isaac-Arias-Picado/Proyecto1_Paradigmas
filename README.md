# Proyecto Etapa 1: Analizador y Exportador de Planes de Estudio

**Institución:** Instituto Tecnológico de Costa Rica (ITCR)  
**Curso:** CE1106 - Paradigmas de Programación
**Grupo:** 1
**Autores:**  Isaac Arias Picado, Jose Chavarria Gamboa, Alexander Angulo Serrano 

## 1. Arquitectura del Proyecto
El sistema fue desarrollado utilizando el paradigma imperativo en el lenguaje C. La arquitectura es modular, separando la lógica de negocio en múltiples archivos fuente y cabeceras para aislar responsabilidades. Se implementó un archivo específico `constantes.h` para centralizar todos los valores fijos del sistema, cumpliendo con las normativas operativas del proyecto[cite: 5].

Los módulos principales son:
*   **`main.c`**: Archivo orquestador. Ejecuta el flujo principal mediante la función `procesar_carrera`, la cual carga los datos, asocia los horarios, marca aprobados, detecta choques, valida requisitos y finalmente llama al exportador JSON.
*   **`parser.c / parser.h`**: Módulo encargado de la ingesta de datos. Lee archivos de texto plano para extraer la información estructurada de los planes de estudio y el historial académico.
*   **`horario.c / horario.h`**: Procesa las cadenas de texto de los horarios, convirtiendo el formato de horas ("HH:MM") a un valor entero en minutos absolutos para facilitar el cálculo matemático de traslapes.
*   **`requisitos.c / requisitos.h`**: Motor de reglas académicas. Evalúa el historial del estudiante contra las dependencias del curso utilizando funciones recursivas indirectas (`correquisito_cumplido` llama a `requisito_cumplido`).
*   **`choques.c / choques.h`**: Implementa los algoritmos de comparación temporal. Itera sobre matrices de grupos y bloques de horarios para detectar colisiones (traslapes) entre cualquier par de cursos.
*   **`exportador.c / exportador.h`**: Módulo de salida que serializa el catálogo de cursos desde las estructuras en memoria de C hacia archivos `.json`.

## 2. Decisiones de Diseño

### 2.1 Justificación de decisiones propias y del dataset
Se decidió trabajar con los planes de estudio de la carrera obligatoria de **Ingeniería en Computadores** y la carrera de **Ingeniería de Materiales**[cite: 5]. Para mantener la pureza en el manejo de memoria en C y evitar fugas (`memory leaks`), se optó por definir las estructuras principales mediante arreglos estáticos dimensionados por constantes máximas (ej. `MAX_CURSOS`, `MAX_GRUPOS`).
En cuanto al dataset de horarios, se decidió aislar la extracción de los componentes temporales utilizando delimitadores y lectura formateada (`sscanf` y `strtok`), lo que permite tolerar ligeras variaciones en los espacios en blanco del archivo original copiado de la Guía de Horarios institucional.

### 2.2 Caso límite real encontrado y resolución
**Caso:** Validación circular o encadenada de correquisitos. 
**Problema:** Un curso "A" puede tener como correquisito un curso "B". Según la regla académica, el estudiante puede matricular "A" si "B" ya está aprobado, o si "B" se puede matricular simultáneamente. Sin embargo, ¿qué sucede si "B" tiene sus propios requisitos que el estudiante no cumple?
**Resolución:** En `requisitos.c`, la función `correquisito_cumplido` se diseñó para no solo verificar si el correquisito está en el historial, sino que, si no lo está, delega la verificación iterando sobre el arreglo de requisitos del propio correquisito. Si alguno de los requisitos del curso "B" falla, la función retorna `0`, impidiendo lógicamente la matrícula del curso "A".

### 2.3 Justificación del formato de salida
Se eligió **JSON** como formato de serialización por tratarse de un estándar jerárquico. Dado que este archivo servirá como insumo para las siguientes etapas en Racket, Prolog y Java[cite: 5], JSON permite anidar de forma natural la relación de uno a muchos (Un Curso -> Múltiples Grupos -> Múltiples Bloques Horarios). Utilizar un formato plano como CSV hubiera requerido duplicar excesivamente los datos del curso por cada bloque horario, complicando innecesariamente el parseo en los lenguajes funcionales y lógicos posteriores.

## 3. Estructuras de Datos Desarrolladas
Se hizo un uso estricto de `structs` para modelar la información académica[cite: 5]:

1.  **`BloqueHorario`**: Estructura base que almacena los metadatos de un bloque de clases (día, hora_inicio, hora_fin) y las llaves foráneas lógicas (`codigo_curso`, `grupo`).
2.  **`Grupo`**: Estructura intermedia que agrupa un arreglo de `BloqueHorario`s asociados a un mismo número de grupo, y mantiene un indicador booleano (`tiene_choque`) específico para esa combinación.
3.  **`Curso`**: Estructura principal tipo "Grafo/Árbol". Almacena metadatos escalares (código, nombre, créditos, semestre), indicadores de estado (aprobado, elegible, tiene_choque), arreglos de cadenas para requisitos y correquisitos, y un arreglo anidado de tipo `Grupo` con toda la disponibilidad temporal de la asignatura.
