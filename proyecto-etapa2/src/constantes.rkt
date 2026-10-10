#lang typed/racket

(provide RUTA-CATALOGO-COMPUTADORES
         RUTA-CATALOGO-MATERIALES
         RUTA-PREFS-COMPUTADORES
         RUTA-PREFS-MATERIALES
         RUTA-SALIDA-COMPUTADORES
         RUTA-SALIDA-MATERIALES)

;; --- Salidas de la etapa 1 ---
(define RUTA-CATALOGO-COMPUTADORES
  "../../proyecto-etapa1/Project1/data/salida/catalogo_computadores.json")

(define RUTA-CATALOGO-MATERIALES
  "../../proyecto-etapa1/Project1/data/salida/catalogo_materiales.json")

;; --- Entradas propias de la etapa 2 ---
(define RUTA-PREFS-COMPUTADORES
  "../data/entrada/preferencias_computadores.json")

(define RUTA-PREFS-MATERIALES
  "../data/entrada/preferencias_materiales.json")

;; --- Salidas de la etapa 2 ---
(define RUTA-SALIDA-COMPUTADORES
  "../data/salida/combinaciones_computadores.json")

(define RUTA-SALIDA-MATERIALES
  "../data/salida/combinaciones_materiales.json")