#lang typed/racket

(require "constantes.rkt")
(require "lectura.rkt")
(require "choques.rkt")
(require "modelo.rkt")  
;; 1. Probar lectura de catálogo
(displayln "=== Catálogo ===")
(define cursos (leer-catalogo RUTA-CATALOGO-COMPUTADORES))
(displayln (string-append "Cursos leídos: " (number->string (length cursos))))
(define primer-curso (first cursos))
(displayln (string-append "Primer curso: " (Curso-codigo primer-curso)))
(displayln (string-append "  Créditos:   " (number->string (Curso-creditos primer-curso))))
(displayln (string-append "  Grupos:     " (number->string (length (Curso-grupos primer-curso)))))

;; 2. Probar lectura de preferencias
(displayln "")
(displayln "=== Preferencias ===")
(define p (leer-preferencias RUTA-PREFS-COMPUTADORES))
(displayln (string-append "Créditos min: " (number->string (Pref-creditos-min p))))
(displayln (string-append "Créditos max: " (number->string (Pref-creditos-max p))))
(displayln (string-append "Cursos de interés: " (number->string (length (Pref-interes p)))))

;; 3. Probar detección de choques
(displayln "")
(displayln "=== Choques ===")

(define b1 (Bloque "MAR" 450 560))   ; 07:30 - 09:20
(define b2 (Bloque "MAR" 510 620))   ; 08:30 - 10:20
(define b3 (Bloque "MAR" 570 680))   ; 09:30 - 11:20
(define b4 (Bloque "JUE" 450 560))   ; otro día

(displayln (string-append "b1 vs b2 (esperado #t): " (if (bloques-chocan? b1 b2) "true" "false")))
(displayln (string-append "b1 vs b3 (esperado #f): " (if (bloques-chocan? b1 b3) "true" "false")))
(displayln (string-append "b2 vs b3 (esperado #t): " (if (bloques-chocan? b2 b3) "true" "false")))
(displayln (string-append "b1 vs b4 (esperado #f): " (if (bloques-chocan? b1 b4) "true" "false")))