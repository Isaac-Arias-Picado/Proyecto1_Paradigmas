#lang typed/racket

(require "modelo.rkt")

(provide bloques-chocan?
         grupos-chocan?
         grupo-compatible-con-lista?)

(: bloques-chocan? : Bloque Bloque -> Boolean)
(define (bloques-chocan? a b)
  (and (string=? (Bloque-dia a) (Bloque-dia b))
       (< (Bloque-inicio a) (Bloque-fin b))
       (< (Bloque-inicio b) (Bloque-fin a))))

(: bloque-choca-con-lista? : Bloque (Listof Bloque) -> Boolean)
(define (bloque-choca-con-lista? ba bbs)
  (ormap (λ ([bb : Bloque]) (bloques-chocan? ba bb)) bbs))

(: grupos-chocan? : Grupo Grupo -> Boolean)
(define (grupos-chocan? a b)
  (ormap (λ ([ba : Bloque]) (bloque-choca-con-lista? ba (Grupo-bloques b)))
         (Grupo-bloques a)))

(: grupo-compatible-con-lista? : Grupo (Listof Grupo) -> Boolean)
(define (grupo-compatible-con-lista? g gs)
  (for/and ([otro (in-list gs)])
    (not (grupos-chocan? g otro))))