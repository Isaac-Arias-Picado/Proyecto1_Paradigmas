#lang typed/racket

(require "modelo.rkt")
(require "choques.rkt")

(provide Streamof
         stream-vacio
         stream-vacio?
         stream-primero
         stream-resto
         stream-longitud
         stream->lista
         filtrar-catalogo-por-preferencias
         generar-combinaciones
         generar-combinaciones-estrictas
         generar-desde-datos)


(struct (A) Stream-Cons ([primero : A] [resto : (Promise (Streamof A))]))
(define-type (Streamof A) (U Null (Stream-Cons A)))
(define-syntax-rule (crear-stream a b)(Stream-Cons a (delay b)))
(define stream-vacio '())

(: stream-vacio? : (All (A) (-> (Streamof A) Boolean)))
(define (stream-vacio? s)
  (null? s))

(: stream-primero : (All (A) (-> (Streamof A) A)))
(define (stream-primero s)
  (if (null? s)
      (error 'stream-primero "stream vacío")
      (Stream-Cons-primero s)))

(: stream-resto : (All (A) (-> (Streamof A) (Streamof A))))
(define (stream-resto s)
  (if (null? s)
      (error 'stream-resto "stream vacío")
      (force (Stream-Cons-resto s))))

(: stream-longitud : (All (A) (-> (Streamof A) Integer)))
(define (stream-longitud s)
  (: aux : (Streamof A) Integer -> Integer)
  (define (aux actual acc)
    (if (stream-vacio? actual)
        acc
        (aux (stream-resto actual) (+ acc 1))))
  (aux s 0))

(: stream->lista : (All (A) (-> (Streamof A) (Listof A))))
(define (stream->lista s)
  (if (stream-vacio? s)
      '()
      (cons (stream-primero s)
            (stream->lista (stream-resto s)))))

;;Filtrar por preferencias
(: miembro-entero? : Integer (Listof Integer) -> Boolean)
(define (miembro-entero? n lista)
  (cond
    [(null? lista) #f]
    [(= n (first lista)) #t]
    [else (miembro-entero? n (rest lista))]))

(: filtrar-grupos-por-numeros : (Listof Grupo) (Listof Integer) -> (Listof Grupo))
(define (filtrar-grupos-por-numeros grupos numeros-interes)
  (cond
    [(null? numeros-interes) grupos]
    [(null? grupos) '()]
    [(miembro-entero? (Grupo-numero (first grupos)) numeros-interes)
     (cons (first grupos)
           (filtrar-grupos-por-numeros (rest grupos) numeros-interes))]
    [else
     (filtrar-grupos-por-numeros (rest grupos) numeros-interes)]))
(: buscar-curso : String (Listof Curso) -> (Option Curso))
(define (buscar-curso codigo catalogo)
  (cond
    [(null? catalogo) #f]
    [(string=? codigo (Curso-codigo (first catalogo))) (first catalogo)]
    [else (buscar-curso codigo (rest catalogo))]))
(: filtrar-catalogo-aux : (Listof (List String (Listof Integer))) (Listof Curso) -> (Listof Curso))
(define (filtrar-catalogo-aux interes catalogo)
  (cond
    [(null? interes) '()]
    [else
     (define par (first interes))
     (define codigo (first par))
     (define grupos-pedidos (second par))
     (define curso-encontrado (buscar-curso codigo catalogo))
     (cond
       [(not curso-encontrado)
        (filtrar-catalogo-aux (rest interes) catalogo)]
       [else
        (define grupos-validos
          (filtrar-grupos-por-numeros (Curso-grupos curso-encontrado) grupos-pedidos))
        (if (null? grupos-validos)
            (filtrar-catalogo-aux (rest interes) catalogo)
            (cons (Curso (Curso-codigo curso-encontrado)
                         (Curso-creditos curso-encontrado)
                         (Curso-carrera curso-encontrado)
                         grupos-validos)
                  (filtrar-catalogo-aux (rest interes) catalogo)))])]))

(: filtrar-catalogo-por-preferencias : (Listof Curso) Pref -> (Listof Curso))
(define (filtrar-catalogo-por-preferencias catalogo pref)
  (filtrar-catalogo-aux (Pref-interes pref) catalogo))

;;Generador de combinaciones
(: unir-streams-lazy : (All (A) (-> (Streamof A) (-> (Streamof A)) (Streamof A))))
(define (unir-streams-lazy s1 producir-s2)
  (if (stream-vacio? s1)
      (producir-s2)
      (crear-stream (stream-primero s1)
                    (unir-streams-lazy (stream-resto s1) producir-s2))))

(: explorar-grupos-de-curso
   : Curso
     (Listof Grupo)
     (Listof Curso)
     (Listof Grupo)
     (Listof (List String Integer String))
     Integer
     Boolean
     -> (Streamof Combo))
(define (explorar-grupos-de-curso curso
                                  grupos-disponibles
                                  otros-cursos
                                  grupos-elegidos
                                  seleccion-actual
                                  creditos-actuales
                                  permitir-subconjuntos?)
  (cond
    [(null? grupos-disponibles) stream-vacio]
    [else
     (define grupo-actual (first grupos-disponibles))
     (define resto-grupos (rest grupos-disponibles))
    
     (if (grupo-compatible-con-lista? grupo-actual grupos-elegidos)
         (unir-streams-lazy
          (explorar-cursos
           otros-cursos
           (cons grupo-actual grupos-elegidos)
           (cons (list (Curso-codigo curso)
                       (Grupo-numero grupo-actual)
                       (Curso-carrera curso))
                 seleccion-actual)
           (+ creditos-actuales (Curso-creditos curso))
           permitir-subconjuntos?)
          (λ ()
            (explorar-grupos-de-curso curso
                                      resto-grupos
                                      otros-cursos
                                      grupos-elegidos
                                      seleccion-actual
                                      creditos-actuales
                                      permitir-subconjuntos?)))
         (explorar-grupos-de-curso curso
                                   resto-grupos
                                   otros-cursos
                                   grupos-elegidos
                                   seleccion-actual
                                   creditos-actuales
                                   permitir-subconjuntos?))]))
(: explorar-cursos
   : (Listof Curso)
     (Listof Grupo)
     (Listof (List String Integer String))
     Integer
     Boolean
     -> (Streamof Combo))
(define (explorar-cursos cursos-restantes
                         grupos-elegidos
                         seleccion-actual
                         creditos-actuales
                         permitir-subconjuntos?)
  (cond
    [(null? cursos-restantes)
     (if (null? seleccion-actual)
         stream-vacio
         (crear-stream (Combo (reverse seleccion-actual) creditos-actuales)
                       stream-vacio))]
    [else
     (define curso-actual (first cursos-restantes))
     (define otros-cursos (rest cursos-restantes))
     (define stream-con-curso
       (explorar-grupos-de-curso curso-actual
                                 (Curso-grupos curso-actual)
                                 otros-cursos
                                 grupos-elegidos
                                 seleccion-actual
                                 creditos-actuales
                                 permitir-subconjuntos?))
     (if permitir-subconjuntos?
         (unir-streams-lazy
          stream-con-curso
          (λ ()
            (explorar-cursos otros-cursos
                             grupos-elegidos
                             seleccion-actual
                             creditos-actuales
                             permitir-subconjuntos?)))
         stream-con-curso)]))

(: generar-combinaciones : (Listof Curso) -> (Streamof Combo))
(define (generar-combinaciones cursos-habilitados)
  (explorar-cursos cursos-habilitados '() '() 0 #t))

(: generar-combinaciones-estrictas : (Listof Curso) -> (Streamof Combo))
(define (generar-combinaciones-estrictas cursos-habilitados)
  (explorar-cursos cursos-habilitados '() '() 0 #f))

(: generar-desde-datos : (Listof Curso) Pref -> (Streamof Combo))
(define (generar-desde-datos catalogo pref)
  (define cursos-interes (filtrar-catalogo-por-preferencias catalogo pref))
  (generar-combinaciones cursos-interes))