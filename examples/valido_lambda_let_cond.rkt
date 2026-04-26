(define (dobro_ou_zero x)
  (let ([y (* x 2)])
    (cond
      [(> y 0) y]
      [#t 0])))

(dobro_ou_zero 4)
