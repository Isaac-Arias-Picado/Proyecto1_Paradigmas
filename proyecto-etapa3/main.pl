:- encoding(utf8).
%% Punto de entrada del módulo Prolog (etapa 3)
%% Carga JSON, evalúa cada combinación, exporta evaluaciones JSON.

:- use_module(library(http/json)).
:- use_module(library(lists)).

:- consult('reglas.pl').
:- consult('politica.pl').

%% Carga de combinaciones desde JSON

cargar_combinaciones(Archivo) :-
    setup_call_cleanup(
        open(Archivo, read, Stream),
        json_read_dict(Stream, Dict, [value_string_as(atom)]),
        close(Stream)
    ),
    get_dict(combinaciones, Dict, ListaCombos),
    forall(member(ComboDict, ListaCombos), cargar_combo(ComboDict)).

cargar_combo(ComboDict) :-
    Id = ComboDict.id,
    CursosDict = ComboDict.cursos,
    maplist(curso_dict_a_termino, CursosDict, Cursos),
    assertz(combinacion(Id, Cursos)).

curso_dict_a_termino(Dict, curso(Codigo, Grupo, Creditos, Carrera, Area)) :-
    Codigo   = Dict.codigo,
    Grupo    = Dict.grupo,
    Creditos = Dict.creditos,
    Carrera  = Dict.carrera,
    Area     = Dict.area.

%% Pares curso-grupo (para la salida)

pares_curso_grupo(Combo, Pares) :-
    findall([Cod, G], member(curso(Cod, G, _, _, _), Combo), Pares).

%% Recorrido por backtracking y evaluación

evaluar_todas(Resultados) :-
    findall(Dict,
        ( combinacion(Id, Combo),
          pares_curso_grupo(Combo, Pares),
          evaluar(Combo, Veredicto, Razon),
          dict_resultado(Id, Pares, Veredicto, Razon, Dict) ),
        Resultados).

dict_resultado(Id, Pares, Veredicto, Razon, ResultDict) :-
    ( Veredicto == cumple -> Cumple = true ; Cumple = false ),
    razon_a_json(Razon, RazonJson),
    ResultDict = _{
        id: Id,
        pares_curso_grupo: Pares,
        cumple: Cumple,
        razon: RazonJson
    }.

%% La razón se exporta como objeto {regla, detalle} para que Java no tenga
%% que parsear texto. =.. separa el functor (la regla) de sus argumentos.
razon_a_json(ninguna, null) :- !.
razon_a_json(Razon, _{regla: Regla, detalle: Detalle}) :-
    Razon =.. [Regla | Detalle].

%% Exportación

exportar(ArchivoSalida) :-
    evaluar_todas(Resultados),
    setup_call_cleanup(
        open(ArchivoSalida, write, Stream),
        json_write_dict(Stream, _{resultados: Resultados}, [width(0)]),
        close(Stream)
    ).

%% Procesamiento por carrera

procesar_carrera(Nombre, RutaEntrada, RutaSalida) :-
    retractall(combinacion(_, _)),
    cargar_combinaciones(RutaEntrada),
    exportar(RutaSalida),
    format('~w: exportado en ~w~n', [Nombre, RutaSalida]).

%% Entry point

main :-
    procesar_carrera('Computadores',
                     'datos/entradas/combinaciones_computadores.json',
                     'datos/salidas/evaluaciones_computadores.json'),
    procesar_carrera('Materiales',
                     'datos/entradas/combinaciones_materiales.json',
                     'datos/salidas/evaluaciones_materiales.json'),
    procesar_carrera('Mixto',
                     'datos/entradas/combinaciones_mixtas.json',
                     'datos/salidas/evaluaciones_mixtas.json').

%% Uso (desde la carpeta proyecto-etapa3):
%%   swipl -g main -t halt main.pl