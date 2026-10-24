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

---

# Proyecto Etapa 2: Generador de Combinaciones de Horario

## 1. Arquitectura del Proyecto
Esta etapa se desarrolló con el paradigma funcional en Typed Racket. El programa toma el catálogo que genera el módulo de C, ya con los choques y la elegibilidad de cada curso calculados, junto con un archivo de preferencias del estudiante para cada carrera. Con eso genera las combinaciones de horario que no tienen choques, las filtra según el rango de créditos que pidió el estudiante y las exporta a un archivo JSON, que es el que va a leer la etapa de Prolog.

El flujo es el mismo para las dos carreras. Primero se leen el catálogo y las preferencias, después se dejan solo los cursos y grupos que le interesan al estudiante, se generan las combinaciones como un stream, se filtran por créditos y al final se exportan. El programa se ejecuta corriendo `main.rkt` desde la carpeta `proyecto-etapa2/src`, ya que las rutas de los archivos son relativas a esa carpeta.

Los módulos principales son:
*   **`main.rkt`**: Archivo orquestador. La función `procesar-carrera` ejecuta el flujo completo para una carrera, y se llama una vez para Computadores y otra para Materiales.
*   **`lectura.rkt`**: Lee el catálogo de C y las preferencias, y los convierte a las estructuras del programa. Solo conserva los cursos que el estudiante puede matricular y que tienen al menos un grupo, porque los demás no pueden formar parte de ninguna combinación.
*   **`choques.rkt`**: Determina si dos grupos se traslapan en el tiempo, comparando sus bloques de horario convertidos a minutos.
*   **`generador.rkt`**: Contiene la implementación de los streams, el filtro de cursos según la lista de interés, el generador de combinaciones y el filtro polimórfico `stream-filtrar`, que usa `filtrar-por-creditos`.
*   **`exportador.rkt`**: Recorre el stream de combinaciones ya filtradas, le asigna un identificador a cada una y escribe el archivo de salida.
*   **`modelo.rkt`**: Define los tipos de datos del programa.
*   **`constantes.rkt`**: Agrupa las rutas de entrada y salida, igual que `constantes.h` en la etapa 1.
*   **`test_lectura.rkt`**: Pruebas por consola de la lectura, los choques, el generador y el filtro por créditos.

## 2. Decisiones de Diseño

### 2.1 Justificación de decisiones propias
Se implementaron streams propios, donde cada elemento guarda el primer valor ya calculado y el resto de la secuencia como una promesa (`delay`). Gracias a esto, ni el generador ni el filtro por créditos calculan combinaciones por adelantado. El stream solo se recorre al final, cuando el exportador lo necesita para escribir el archivo.

El generador funciona como un árbol de decisiones. Para cada curso prueba sus grupos uno por uno y, antes de seguir con el siguiente curso, revisa que el grupo no choque con los que ya se eligieron. Si choca, esa rama se descarta en ese momento y la combinación nunca se termina de armar. Esta optimización solo se pide a los grupos de cuatro integrantes, pero se mantuvo porque evita trabajo innecesario. Los choques se vuelven a calcular en Racket porque el campo `tiene_choque` de C indica si un grupo choca con algún otro del catálogo, pero no dice con cuál, y para armar combinaciones hace falta saber si choca con los grupos elegidos en esa combinación en particular.

La función `stream-filtrar` es polimórfica. No sabe qué tipo de datos hay en el stream ni qué criterio se aplica, solo recibe una función que decide si cada elemento se queda o no. `filtrar-por-creditos` la reutiliza pasándole como criterio una función que revisa si los créditos de una combinación están dentro del rango, y la misma función serviría para cualquier otro criterio. En todo el programa se evitó la mutación. Por ejemplo, el identificador de cada combinación no sale de un contador que se va modificando, sino que se pasa como parámetro y aumenta en uno en cada llamada recursiva.

Para extender los planes de estudio a seis semestres se agregaron los semestres 5 y 6 a los archivos de entrada de la etapa 1, sin tocar su lógica, y se volvió a correr el módulo de C para regenerar los catálogos. Se mantuvo el mismo criterio de datos que antes, el período 2 de 2026 en la sede de Cartago. Se dejaron fuera los grupos de Tutoría y RN, ya que están pensados para estudiantes que repiten el curso y el historial no modela esa situación. Algunos grupos aparecen dos veces en la Guía de Horarios porque los imparten dos profesores, y se dejaron en una sola fila para no duplicar sus bloques de horario. Los historiales se actualizaron para simular un estudiante que entra al semestre 5 con un curso pendiente del semestre 4, así los cursos nuevos se pueden matricular y al mismo tiempo se ve a la validación de requisitos bloqueando otros.

Las preferencias de prueba incluyen más de un grupo por curso, y algunos de esos grupos chocan entre sí a propósito para comprobar que el generador los descarta. En las preferencias de Materiales se incluyó también un curso que el estudiante no puede matricular por requisitos, CM3207, y el programa simplemente lo ignora.

### 2.2 Caso límite real encontrado y resolución
**Caso:** Un grupo por cada curso habilitado deja sin sentido el filtro por créditos.
**Problema:** El enunciado pide combinaciones con un grupo por cada curso habilitado. Si eso se aplica de forma literal, todas las combinaciones tienen los mismos cursos y suman los mismos créditos, así que el filtro por rango deja pasar todas o ninguna. Esto se vio en las primeras pruebas con Computadores, donde los cursos de interés sumaban 11 créditos y el rango pedido era de 12 a 18, por lo que el resultado siempre era cero combinaciones sin importar qué grupos se eligieran.
**Resolución:** Se decidió que el generador permita subconjuntos de cursos. En cada curso se abren dos caminos, incluirlo con alguno de sus grupos compatibles o saltarlo, y esto se controla con el parámetro `permitir-subconjuntos?`. De esta forma salen combinaciones con distintos totales de créditos y el filtro por rango cumple su función. Con los datos actuales, Computadores produce 65 combinaciones entre 12 y 18 créditos. Se conservó también `generar-combinaciones-estrictas`, que solo genera combinaciones con todos los cursos, por si se necesita la interpretación literal.

### 2.3 Justificación del formato de salida
Se eligió **JSON**, el mismo formato de la etapa 1. Cada combinación tiene una cantidad variable de cursos, y JSON lo representa de forma natural con una lista dentro de cada combinación. En un formato plano como CSV habría que repetir los datos de la combinación por cada curso que contiene. Cada curso se exporta como un objeto con `codigo`, `grupo` y `carrera`, en lugar de un arreglo, para que Prolog pueda acceder a cada dato por su nombre sin depender del orden.

Además de los campos obligatorios, se agregó un `id` a cada combinación, porque Prolog va a descartar algunas al aplicar sus reglas y Java necesita una forma de referirse a las que quedaron. También se agregaron `creditos_min`, `creditos_max` y `total_combinaciones` al inicio del archivo, a raíz de un caso que apareció en las pruebas. Cuando ninguna combinación cumple el rango de créditos, el exportador igualmente escribe un JSON válido con la lista vacía, en lugar de dejar un archivo vacío o fallar, y con esos campos queda claro con qué rango se generó y por qué no hubo resultados. Los horarios de cada grupo no se repiten en la salida, ya que están en el catálogo de C y se pueden obtener con el código del curso y el número de grupo.

### 2.4 Validaciones que quedan para Prolog
Las combinaciones exportadas no tienen choques y cumplen el rango de créditos, pero pueden incluir un curso sin su correquisito, por ejemplo CE3201 sin CE1107. El módulo de C ya validó que cada curso por separado se puede matricular, incluyendo sus correquisitos. Revisar que un curso y su correquisito vengan juntos en la misma combinación es una regla de compatibilidad entre cursos, y el enunciado indica que este módulo no debe aplicar reglas de restricción más allá de los créditos, por lo que esa validación le corresponde a la etapa de Prolog.

## 3. Estructuras de Datos Desarrolladas
Todas las estructuras se definen en `modelo.rkt` como `struct` tipados e inmutables.

1.  **`Bloque`**: Un bloque de horario de un grupo. Guarda el día y las horas de inicio y fin convertidas a minutos desde la medianoche, para poder compararlas directamente.
2.  **`Grupo`**: El número de grupo y su lista de bloques de horario.
3.  **`Curso`**: Código, créditos, carrera de origen y la lista de grupos disponibles. Solo se crean para los cursos que el estudiante puede matricular.
4.  **`Pref`**: Las preferencias del estudiante, con los créditos mínimos y máximos y la lista de cursos de interés, cada uno con los números de grupo que desea.
5.  **`Combo`**: Una combinación generada, con los cursos elegidos (código, grupo y carrera) y la suma de sus créditos.
6.  **`Streamof A`**: El tipo de stream polimórfico. Puede estar vacío o tener un primer elemento junto con la promesa del resto.
