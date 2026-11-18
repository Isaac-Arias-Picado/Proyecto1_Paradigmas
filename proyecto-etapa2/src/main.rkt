#lang typed/racket

(require "constantes.rkt")
(require "modelo.rkt")
(require "lectura.rkt")
(require "generador.rkt")
(require "exportador.rkt")

(: procesar-carrera :  String String String String -> Void)
(define (procesar-carrera nombre ruta-catalogo ruta-prefs ruta-salida)
  (define catalogo (leer-catalogo ruta-catalogo))
  (define preferencias (leer-preferencias ruta-prefs))
  (define combinaciones (generar-desde-datos catalogo preferencias))
  (define combinaciones-filtradas (filtrar-por-creditos combinaciones preferencias))
  (exportar-combinaciones combinaciones-filtradas preferencias ruta-salida)
  (displayln (string-append nombre ": exportado en " ruta-salida)))

;; Combinaciones mixtas: el catálogo de Computadores (carrera principal) se une
;; con el de Materiales. La lógica de generación y filtrado es la misma.
(: procesar-mixto : String String String String -> Void)
(define (procesar-mixto ruta-principal ruta-secundario ruta-prefs ruta-salida)
  (define catalogo (unir-catalogos (leer-catalogo ruta-principal)
                                   (leer-catalogo ruta-secundario)))
  (define preferencias (leer-preferencias ruta-prefs))
  (define combinaciones (generar-desde-datos catalogo preferencias))
  (define combinaciones-filtradas (filtrar-por-creditos combinaciones preferencias))
  (exportar-combinaciones combinaciones-filtradas preferencias ruta-salida)
  (displayln (string-append "Mixto: exportado en " ruta-salida)))

(procesar-carrera "Computadores" RUTA-CATALOGO-COMPUTADORES RUTA-PREFS-COMPUTADORES RUTA-SALIDA-COMPUTADORES)
(procesar-carrera "Materiales" RUTA-CATALOGO-MATERIALES RUTA-PREFS-MATERIALES RUTA-SALIDA-MATERIALES)
(procesar-mixto RUTA-CATALOGO-COMPUTADORES RUTA-CATALOGO-MATERIALES RUTA-PREFS-MIXTAS RUTA-SALIDA-MIXTAS)
