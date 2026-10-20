#lang typed/racket

(require "modelo.rkt")

(require/typed json
  [string->jsexpr (-> String Any)])

(provide leer-catalogo
         leer-preferencias)

(: js->string : Any -> String)
(define (js->string v)
  (cond [(string? v) v]
        [else (error 'js->string "se esperaba string, se obtuvo: ~a" v)]))

(: js->integer : Any -> Integer)
(define (js->integer v)
  (cond [(integer? v) (cast v Integer)]
        [(real? v) (exact-round (cast v Real))]
        [else (error 'js->integer "se esperaba número, se obtuvo: ~a" v)]))

(: js->list : Any -> (Listof Any))
(define (js->list v)
  (cond [(list? v) v]
        [else (error 'js->list "se esperaba lista, se obtuvo: ~a" v)]))

(: js->hash : Any -> (HashTable Any Any))
(define (js->hash v)
  (cond [(hash? v) (cast v (HashTable Any Any))]
        [else (error 'js->hash "se esperaba hash, se obtuvo: ~a" v)]))

(: hora->minutos : String -> Integer)
(define (hora->minutos s)
  (define partes (string-split s ":"))
  (define h (string->number (first partes)))
  (define m (string->number (second partes)))
  (cond [(and h m (real? h) (real? m))
         (+ (* 60 (exact-round h)) (exact-round m))]
        [else (error 'hora->minutos "hora inválida: ~a" s)]))

(: json->bloque : Any -> Bloque)
(define (json->bloque j)
  (define h (js->hash j))
  (Bloque (js->string (hash-ref h 'dia))
          (hora->minutos (js->string (hash-ref h 'hora_inicio)))
          (hora->minutos (js->string (hash-ref h 'hora_fin)))))

(: json->grupo : Any -> Grupo)
(define (json->grupo j)
  (define h (js->hash j))
  (Grupo (js->integer (hash-ref h 'numero))
         (map json->bloque (js->list (hash-ref h 'horarios)))))

(: json->curso : Any -> Curso)
(define (json->curso j)
  (define h (js->hash j))
  (Curso (js->string (hash-ref h 'codigo))
         (js->integer (hash-ref h 'creditos))
         (js->string (hash-ref h 'carrera))
         (map json->grupo (js->list (hash-ref h 'grupos)))))

(: leer-catalogo : String -> (Listof Curso))
(define (leer-catalogo ruta)
  (define contenido (file->string ruta))
  (define datos (js->list (string->jsexpr contenido)))
  (define datos-elegibles
    (filter (λ ([d : Any])
              (equal? (hash-ref (js->hash d) 'puede_matricular #f) #t))
            datos))
  (define cursos (map json->curso datos-elegibles))
  (filter (λ ([c : Curso]) (positive? (length (Curso-grupos c))))
          cursos))
(: json->par-interes : Any -> (List String (Listof Integer)))
(define (json->par-interes j)
  (define h (js->hash j))
  (list (js->string (hash-ref h 'codigo))
        (map js->integer (js->list (hash-ref h 'grupos)))))

(: leer-preferencias : String -> Pref)
(define (leer-preferencias ruta)
  (define contenido (file->string ruta))
  (define h (js->hash (string->jsexpr contenido)))
  (Pref (js->integer (hash-ref h 'creditos_min))
        (js->integer (hash-ref h 'creditos_max))
        (map json->par-interes (js->list (hash-ref h 'interes)))))