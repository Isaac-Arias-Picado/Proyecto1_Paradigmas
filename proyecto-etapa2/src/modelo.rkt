#lang typed/racket

(provide (struct-out Bloque)
         (struct-out Grupo)
         (struct-out Curso)
         (struct-out Pref)
         (struct-out Combo))

(struct Bloque ([dia : String]
                [inicio : Integer]   ; minutos desde 00:00
                [fin : Integer]))

(struct Grupo ([numero : Integer]
               [bloques : (Listof Bloque)]))

(struct Curso ([codigo : String]
               [creditos : Integer]
               [carrera : String]
               [grupos : (Listof Grupo)]))

(struct Pref ([creditos-min : Integer]
              [creditos-max : Integer]
              [interes : (Listof (List String (Listof Integer)))]))

(struct Combo ([seleccion : (Listof (List String Integer String))]
               [creditos-totales : Integer]))