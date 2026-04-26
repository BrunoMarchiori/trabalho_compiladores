#lang racket

(struct item (id title) #:transparent)

(define items
  (list (item 1 "a")
        (item 2 "b")
        (item 3 "c")))

(define ids
  (for/list ([it items])
    (item-id it)))

(define picked
  (match ids
    ['() #f]
    [(cons first rest) first]))

(module+ main
  (for ([x ids])
    (displayln x))
  picked)
