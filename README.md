# Proyecto Etapa 1: Analizador y Exportador de Planes de Estudio

**Institución:** Instituto Tecnológico de Costa Rica (ITCR)  
**Curso:** CE1106 - Paradigmas de Programación
**Grupo:** 1
**Autores:**  Isaac Arias Picado, Jose Chavarria Gamboa, Alexander Angulo Serrano 

## 1. Arquitectura del Proyecto
El sistema fue desarrollado utilizando el paradigma imperativo en el lenguaje C. Se organizó el código por módulos, separando la lógica del programa en múltiples archivos para mejorar el orden de trabajo y división de las tareas. Se usó un archivo `constantes.h` para agrupar todos los valores constantes, lo cual hace más sencillo el poder modificar dichos valores en todo el código a la vez y facilita poder identificar dichos valores dentro del código. 

Los módulos principales son:
*   **`main.c`**: Archivo orquestador. Se encarga de manejar el flujo principal del programa usando `procesar_carrera`, esta carga los datos de los archivos de entrada, asocia usando horarios, deetcta choques y valida los requisitos para generar el archivo final en JSON.
*   **`parser.c / parser.h`**: Módulo encargado de la ingesta de datos. Lee archivos de texto plano para extraer la información estructurada de los planes de estudio y el historial académico.
*   **`horario.c / horario.h`**: Procesa las cadenas de texto de los horarios, conviritendo el formato de horas a un valor en minutos y analisando los choques entre grupos y cursos según hora de inicio y final de las clases.
*   **`requisitos.c / requisitos.h`**: Analiza cursos elegibles según los requisitos e historial académico. Evalúa el historial del estudiante contra las dependencias del curso utilizando funciones como `correquisito_cumplido` que llama a `requisito_cumplido` para identificar casos de falta de requisitos o correquisitos para poder llevar un curso.
*   **`choques.c / choques.h`**: Implementa los algoritmos de comparación temporal. Itera sobre matrices de grupos y bloques de horarios para detectar colisiones (traslapes) entre cualquier par de cursos.
*   **`exportador.c / exportador.h`**: Módulo de salida que serializa el catálogo de cursos desde las estructuras en memoria de C hacia archivos `.json`.

## 2. Decisiones de Diseño

### 2.1 Justificación de decisiones propias y del dataset
Para mejorar el manejo de memoria en C y evitar errores como fugas de memoria, se agrupó la información en arreglos estáticos usando constantes de valor máximo posible, todo eso definido en `constantes.h`.
En cuanto al dataset de horarios, se decidió aislar la extracción de los componentes temporales utilizando delimitadores y lectura formateada (`sscanf` y `strtok`), lo que permite tolerar ligeras variaciones en los espacios en blanco del archivo original copiado de la Guía de Horarios institucional. Se utilizaron los horarios correspondientes al período 2 de 2026, sede Cartago, modalidad semestre, ya que los datos recolectados inicialmente correspondían al período 2025, pero se prefirió usar información más vigente, en este caso el periodo actual. Como la Guía de Horarios no separa el número de grupo en una columna propia, este se tuvo que extraer de la guía en una pestaña marcada con "+" en cada fila, se agregó como una columna extra al formato de entrada del parser.

### 2.2 Caso límite real encontrado y resolución
**Caso:** Validación circular o encadenada de correquisitos. 
**Problema:** Un curso "A" puede tener como correquisito un curso "B". Al igual como se maneja en el sistema de matricula del TEC, el estudiante puede matricular "A" si "B" ya está aprobado, o si "B" se puede matricular simultáneamente. Si es que "B" no se ah aprobado, se necesita hacer un análisis de sus requisitos para comprobar que efectivamente tanto "A" como "B" son posibles de llevar a la vez, de lo contrario, el curso con correquisito "A" no puede ser matriculado.
**Resolución:** En `requisitos.c` la función `correquisito_cumplido` se diseñó para no solo verificar si el correquisito está en el historial, sino también para iterar sobre los requisitos del curso correquisito. Si alguno de los requisitos del curso "B" falla, la función retorna `0`, lo cual impide la matrícula del curso "A".

### 2.3 Justificación del formato de salida
Se eligió **JSON** como formato de serialización por tratarse de un estándar jerárquico. Dado que este archivo servirá como insumo para las siguientes etapas en Racket, Prolog y Java, JSON permite anidar de forma natural la relación de uno a muchos (Un Curso -> Múltiples Grupos -> Múltiples Bloques Horarios). Utilizar un formato plano como CSV hubiera requerido duplicar excesivamente los datos del curso por cada bloque horario, complicando innecesariamente el parseo en los lenguajes funcionales y lógicos posteriores.

## 3. Estructuras de Datos Desarrolladas
Se hizo un uso estricto de `structs` para modelar la información académica:

1.  **`BloqueHorario`**: Almacena los datos que caracterizan a cada curso (día, hora_inicio, hora_fin) y también información útil para identificarlos (`codigo_curso`, `grupo`).
2.  **`Grupo`**: Estructura intermedia que agrupa un arreglo de `BloqueHorario`s asociados a un mismo número de grupo, y mantiene un indicador booleano representado con 0 o 1 (`tiene_choque`) específico para esa combinación.
3.  **`Curso`**: Es la estructura principal del programa. Almacena información importante (código, nombre, créditos, semestre), indicadores de estado (aprobado, elegible, tiene_choque), arreglos para almacenar los requisitos y correquisitos y un arreglo anidado de tipo `Grupo` con toda la disponibilidad temporal de la asignatura.
