(define (calc a b)
  (let ((x (+ a b))
        [y (* a b)])
    (let* ([z (+ x 1)]
           (w (- y 1)))
      (letrec ([pick (lambda (n)
                       (if (> n 0) n w))])
        (pick z)))))

(calc 3 ¬2)
