#lang racket

(struct task (id title done? tags) #:transparent)
(struct project (name tasks) #:transparent)

(define sample-project
  (project
   "Curator"
   (list
    (task 1 "Criar parser" #t '(compiler racket))
    (task 2 "Validar AST" #f '(compiler tests))
    (task 3 "Gerar relatório" #f '(product metrics))
    (task 4 "Publicar versão alpha" #t '(startup release)))))

(define (task-open? t)
  (not (task-done? t)))

(define (count-open tasks)
  (cond
    [(empty? tasks) 0]
    [(task-open? (first tasks))
     (+ 1 (count-open (rest tasks)))]
    [else
     (count-open (rest tasks))]))

(define (find-task-by-id tasks wanted-id)
  (match tasks
    ['() #f]
    [(cons current remaining)
     (if (= (task-id current) wanted-id)
         current
         (find-task-by-id remaining wanted-id))]))

(define (tasks-with-tag tasks tag)
  (filter
   (lambda (t)
     (member tag (task-tags t)))
   tasks))

(define (task->string t)
  (let ([status (if (task-done? t) "done" "open")])
    (format "[~a] #~a ~a"
            status
            (task-id t)
            (task-title t))))

(define (project-summary p)
  (let* ([tasks (project-tasks p)]
         [total (length tasks)]
         [open (count-open tasks)]
         [done (- total open)])
    (hash
     'name (project-name p)
     'total total
     'open open
     'done done)))

(define (print-project p)
  (printf "Project: ~a\n" (project-name p))
  (printf "Tasks:\n")
  (for ([t (project-tasks p)]
        [index (in-naturals 1)])
    (printf "  ~a. ~a\n" index (task->string t))))

(define (complete-task p id)
  (define updated-tasks
    (for/list ([t (project-tasks p)])
      (if (= (task-id t) id)
          (task (task-id t)
                (task-title t)
                #t
                (task-tags t))
          t)))
  (project (project-name p) updated-tasks))

(define (main)
  (print-project sample-project)
  (newline)

  (define summary (project-summary sample-project))
  (printf "Summary: ~a\n" summary)

  (define compiler-tasks
    (tasks-with-tag (project-tasks sample-project) 'compiler))

  (printf "Compiler tasks:\n")
  (for ([t compiler-tasks])
    (printf "- ~a\n" (task-title t)))

  (define updated-project
    (complete-task sample-project 2))

  (newline)
  (printf "After completing task 2:\n")
  (print-project updated-project))

(module+ main
  (main))

(module+ test
  (require rackunit)

  (check-equal? (count-open (project-tasks sample-project)) 2)

  (check-equal?
   (task-title
    (find-task-by-id (project-tasks sample-project) 1))
   "Criar parser")

  (check-false
   (find-task-by-id (project-tasks sample-project) 99))

  (check-equal?
   (hash-ref (project-summary sample-project) 'done)
   2)

  (check-equal?
   (length (tasks-with-tag (project-tasks sample-project) 'compiler))
   2))