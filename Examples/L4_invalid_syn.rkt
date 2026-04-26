(define (classifica n)
  (cond
    ((> n 10) "alto")
    [(> n 5) "medio"]
    [else "baixo"])

(classifica 8)
