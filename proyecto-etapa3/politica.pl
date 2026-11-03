
%% Regla 1: balance entre carreras
%% Porcentaje máximo de créditos que una carrera puede aportar al total. (hay que ajustarlo)
max_porcentaje_carrera(ingenieria_computadores, 100).
max_porcentaje_carrera(ingenieria_materiales, 100).

%% Regla 2: límite por área (Hay que ajustarlo)
%% Máximo de cursos permitidos por área en una combinación.
max_cursos_por_area(programacion, 2).
max_cursos_por_area(matematicas, 2).
max_cursos_por_area(hardware, 2).
max_cursos_por_area(quimica, 2).
max_cursos_por_area(fisica, 2).
max_cursos_por_area(redes, 2).
max_cursos_por_area(humanidades, 1).
max_cursos_por_area(ciencias_sociales, 1).

%% Regla 3: incompatibilidades
%% Pares de cursos que no pueden estar simultáneamente en una combinación.
%% La simetría se resuelve en reglas.pl con son_incompatibles/2.
%% Computadores
incompatible('CE3101', 'CE3201').
incompatible('MA2105', 'CE1107').
%% Materiales
incompatible('CM3501', 'CM4202').
incompatible('MI3115', 'MI4510').