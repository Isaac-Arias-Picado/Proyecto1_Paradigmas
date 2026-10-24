#lang typed/racket

(require typed/json)
(require "modelo.rkt")
(require "generador.rkt")

(provide exportar-combinaciones)

(: curso->json : (List String Integer String) -> JSExpr)
(define (curso->json curso)
  (hash 'codigo (first curso) 'grupo (second curso) 'carrera (third curso)))
(: combo->json : Combo Integer -> JSExpr)
(define (combo->json combo id)
  (hash 'id id 'creditos_totales (Combo-creditos-totales combo) 'cursos (map curso->json (Combo-seleccion combo))))
(: combos->json : (Streamof Combo) Integer -> (Listof JSExpr))
(define (combos->json s id)
  (if (stream-vacio? s)
     '()
    (cons (combo->json (stream-primero s) id) (combos->json (stream-resto s) (+ id 1)))))

(: exportar-combinaciones : (Streamof Combo) Pref String -> Void)
(define (exportar-combinaciones combos pref ruta)
  (define lista (combos->json combos 1))
  (define datos : JSExpr
    (hash 'creditos_min (Pref-creditos-min pref)
          'creditos_max (Pref-creditos-max pref)
          'total_combinaciones (length lista)
          'combinaciones lista))
  (display-to-file (jsexpr->string datos) ruta #:exists 'replace))