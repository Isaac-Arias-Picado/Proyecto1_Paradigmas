%% Punto de entrada del módulo Prolog (etapa 3)
%% Carga JSON, evalúa cada combinación, exporta salida JSON.

:- use_module(library(http/json)).
:- use_module(library(lists)).

:- consult('reglas.pl').
:- consult('politica.pl').

%% combinacion/2 ya declarada dinámica en reglas.pl

%% Carga de combinaciones desde JSON

cargar_combinaciones(Archivo) :-
    setup_call_cleanup(
        open(Archivo, read, Stream),
        json_read_dict(Stream, Dict),
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
    ( Razon == ninguna -> RazonStr = null
    ; term_to_atom(Razon, RazonStr)
    ),
    ResultDict = _{
        id: Id,
        pares_curso_grupo: Pares,
        cumple: Cumple,
        razon: RazonStr
    }.

%% Exportación

exportar(ArchivoSalida) :-
    evaluar_todas(Resultados),
    setup_call_cleanup(
        open(ArchivoSalida, write, Stream),
        json_write_dict(Stream, _{resultados: Resultados}, [width(0)]),
        close(Stream)
    ).

%% Entry point

main :-
    retractall(combinacion(_, _)),
    cargar_combinaciones('datos/combinaciones_computadores.json'),
    cargar_combinaciones('datos/combinaciones_materiales.json'),
    exportar('salida.json'),
    writeln('Salida escrita en salida.json').

%% Uso:
%%   cd prolog
%%   swipl -g main -t halt main.pl