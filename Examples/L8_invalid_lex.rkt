#lang racket

(struct task (id title done? tags) #:transparent)

(define sample
  (list (task 1 "Parser" #t '(compilers racket))
        (task 2 "AST" #f '(compilers tests))
        (task 3 "Report" #f '(docs writing))))

(define (open? t)
  (not (task-done? t)))

(define (count-open tasks)
  (cond
    [(empty? tasks) 0]
    [(open? (first tasks))
     (+ 1 (count-open (rest tasks)))]
    [else
     (count-open (rest tasks))]))

(define (summarize tasks)
  (let* ([total (length tasks)]
         [open (count-open tasks)]
         [done (- total open)])
    (hash 'total total 'open open 'done done)))

(module+ main
  (define s (summarize sample))
  (printf "Summary: ~a\n" s)
  (for ([t sample])
    (printf "- ~a\n" (task-title t))))

∞
