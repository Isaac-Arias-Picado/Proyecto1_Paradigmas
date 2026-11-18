:- encoding(utf8).

%% Regla 1: balance entre carreras
%% Porcentaje máximo de créditos que una carrera puede aportar al total.
%% Los nombres deben ser idénticos al campo "carrera" que exporta C.
max_porcentaje_carrera('Ingenieria en Computadores', 70).
max_porcentaje_carrera('Ingenieria en Materiales', 70).

%% Regla 2: límite por área
%% Máximo N de cursos de una misma área en una combinación (el mismo N para todas).
max_cursos_area(2).

%% Regla 3: incompatibilidades
%% Pares de cursos que no pueden estar simultáneamente en una combinación.
%% La simetría se resuelve en reglas.pl con son_incompatibles/2.
%% Computadores
incompatible('CE3101', 'CE3201').
incompatible('MA2105', 'CE1107').
%% Materiales
incompatible('CM3501', 'CM4202').
incompatible('MI3115', 'MI4510').