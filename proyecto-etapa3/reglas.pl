
%% Lógica del módulo Prolog (etapa 3)
%% Auxiliares + regla 1, 2, 3 + razón de rechazo.

:- use_module(library(lists)).

%% combinacion/2 se carga dinámicamente desde main.pl (JSON).
:- dynamic combinacion/2.


creditos_totales(Combo, Total) :-
    findall(Cred, member(curso(_, _, Cred, _, _), Combo), Lista),
    sum_list(Lista, Total).

creditos_por_carrera(Combo, Carrera, Total) :-
    findall(Cred,
        member(curso(_, _, Cred, Carrera, _), Combo),
        Lista),
    sum_list(Lista, Total).

carreras_presentes(Combo, Carreras) :-
    findall(C, member(curso(_, _, _, C, _), Combo), Lista),
    sort(Lista, Carreras).

cursos_de_area(Combo, Area, Codigos) :-
    findall(Cod,
        member(curso(Cod, _, _, _, Area), Combo),
        Codigos).

areas_presentes(Combo, Areas) :-
    findall(A, member(curso(_, _, _, _, A), Combo), Lista),
    sort(Lista, Areas).

codigos_presentes(Combo, Codigos) :-
    findall(Cod, member(curso(Cod, _, _, _, _), Combo), Codigos).

%% Incompatibilidad simétrica
son_incompatibles(A, B) :- incompatible(A, B).
son_incompatibles(A, B) :- incompatible(B, A).

%% Regla 1: balance entre carreras

excede_porcentaje(Combo, Carrera) :-
    max_porcentaje_carrera(Carrera, Max),
    creditos_totales(Combo, Total),
    Total > 0,
    creditos_por_carrera(Combo, Carrera, Creditos),
    Creditos * 100 > Max * Total.

cumple_balance(Combo) :-
    carreras_presentes(Combo, Carreras),
    \+ (member(C, Carreras), excede_porcentaje(Combo, C)).

%% Regla 2: límite por área

excede_area(Combo, Area) :-
    max_cursos_por_area(Area, Max),
    cursos_de_area(Combo, Area, Codigos),
    length(Codigos, N),
    N > Max.

cumple_area(Combo) :-
    areas_presentes(Combo, Areas),
    \+ (member(A, Areas), excede_area(Combo, A)).

%% Regla 3: incompatibilidad

par_incompatible_en(Combo, A, B) :-
    codigos_presentes(Combo, Codigos),
    member(A, Codigos),
    member(B, Codigos),
    A @< B,
    son_incompatibles(A, B).

cumple_incompatibilidad(Combo) :-
    \+ par_incompatible_en(Combo, _, _).


%% Evaluación con razón de rechazo
%% Orden: 1 -> 2 -> 3 (de grueso a fino)

evaluar(Combo, cumple, ninguna) :-
    cumple_balance(Combo),
    cumple_area(Combo),
    cumple_incompatibilidad(Combo),
    !.

evaluar(Combo, incumple, balance_carrera(C)) :-
    excede_porcentaje(Combo, C),
    !.

evaluar(Combo, incumple, limite_area(A)) :-
    excede_area(Combo, A),
    !.

evaluar(Combo, incumple, incompatibilidad(A, B)) :-
    par_incompatible_en(Combo, A, B),
    !.