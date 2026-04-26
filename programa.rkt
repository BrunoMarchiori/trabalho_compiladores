#lang racket

(struct task (id title done? tags estimate) #:transparent)
(struct project (name owner tasks metadata) #:transparent)

(define sample-project
  (project
   "Compiler Delivery"
   "team-alpha"
   (list
    (task 1 "Design scanner" #t  '(compiler scanner) 3)
    (task 2 "Implement parser" #t '(compiler parser) 8)
    (task 3 "Write tests" #f '(tests quality) 5)
    (task 4 "Document architecture" #f '(docs architecture) 4)
    (task 5 "Optimize automata" #t '(compiler performance) 6)
    (task 6 "Prepare release" #f '(release ops) 2))
   (hash
    'status 'active
    'priority 9
    'sprint "S12"
    'domain '(compilers education))))

(define (task-open? t)
  (not (task-done? t)))

(define (task-score t)
  (let* ([base (if (task-done? t) 50 20)]
         [complexity (task-estimate t)]
         [tag-bonus (if (member 'compiler (task-tags t)) 15 5)])
    (+ base complexity tag-bonus)))

(define (count-open tasks)
  (letrec ([loop (lambda (xs acc)
                   (if (empty? xs)
                       acc
                       (if (task-open? (first xs))
                           (loop (rest xs) (+ acc 1))
                           (loop (rest xs) acc))))])
    (loop tasks 0)))

(define (sum-estimates tasks)
  (let ([vals (for/list ([t tasks])
                (task-estimate t))])
    (foldl + 0 vals)))

(define (find-task-by-id tasks target-id)
  (match tasks
    ['() #f]
    [(cons current remaining)
     (if (= (task-id current) target-id)
         current
         (find-task-by-id remaining target-id))]))

(define (tasks-with-tag tasks tag)
  (filter
   (lambda (t)
     (member tag (task-tags t)))
   tasks))

(define (group-by-state tasks)
  (let ([open-tasks (filter task-open? tasks)]
        [done-tasks (filter (lambda (t) (task-done? t)) tasks)])
    (hash
     'open open-tasks
     'done done-tasks
     'open-count (length open-tasks)
     'done-count (length done-tasks))))

(define (task->string t)
  (let ([flag (if (task-done? t) "done" "open")])
    (format "[~a] #~a ~a (~a h)"
            flag
            (task-id t)
            (task-title t)
            (task-estimate t))))

(define (project-summary p)
  (let* ([tasks (project-tasks p)]
         [total (length tasks)]
         [open (count-open tasks)]
         [done (- total open)]
         [effort (sum-estimates tasks)]
         [score (foldl + 0 (for/list ([t tasks]) (task-score t)))])
    (hash
     'name (project-name p)
     'owner (project-owner p)
     'total total
     'open open
     'done done
     'effort effort
     'score score
     'status (hash-ref (project-metadata p) 'status)
     'domain (hash-ref (project-metadata p) 'domain))))

(define (print-project p)
  (printf "Project: ~a (~a)\n" (project-name p) (project-owner p))
  (printf "Tasks:\n")
  (for ([t (project-tasks p)]
        [idx (in-naturals 1)])
    (printf "  ~a. ~a\n" idx (task->string t)))
  (newline)
  (printf "Open by tag 'compiler': ~a\n"
          (length (tasks-with-tag (project-tasks p) 'compiler))))

(define (complete-task p id)
  (define updated
    (for/list ([t (project-tasks p)])
      (if (= (task-id t) id)
          (task
           (task-id t)
           (task-title t)
           #t
           (task-tags t)
           (task-estimate t))
          t)))
  (project
   (project-name p)
   (project-owner p)
   updated
   (project-metadata p)))

(define (complex-transform p)
  (let* ([tasks (project-tasks p)]
         [state-map (group-by-state tasks)]
         [ids (for/list ([t tasks]) (task-id t))]
         [titles (for/list ([t tasks]) (task-title t))]
         [pairs (for/list ([i ids] [title titles])
                  (cons i title))]
         [selected (match pairs
                     ['() 'none]
                     [(cons first rest) first])]
         [q `(project ,(project-name p)
                      owner ,(project-owner p)
                      selected ,selected
                      totals ,(hash-ref state-map 'open-count)
                      ,(hash-ref state-map 'done-count))])
    (hash
     'state-map state-map
     'pairs pairs
     'selected selected
     'quoted q)))

(define (calc-risk p)
  (let* ([summary (project-summary p)]
         [open (hash-ref summary 'open)]
         [effort (hash-ref summary 'effort)])
    (cond
      [(> open 4) "high"]
      [(> effort 20) "medium"]
      [else "low"])))

(define (emit-report p)
  (define summary (project-summary p))
  (define transformed (complex-transform p))

  (printf "Summary: ~a\n" summary)
  (printf "Risk: ~a\n" (calc-risk p))
  (printf "Selected pair: ~a\n" (hash-ref transformed 'selected))

  (for ([entry (hash-ref transformed 'pairs)])
    (printf "Pair ~a => ~a\n" (car entry) (cdr entry)))

  (let ([domain (hash-ref (project-metadata p) 'domain)])
    (printf "Domain: ~a\n" domain))

  transformed)

(define (main)
  (print-project sample-project)
  (newline)

  (define transformed-before (emit-report sample-project))
  (newline)

  (define after-two (complete-task sample-project 2))
  (define after-four (complete-task after-two 4))

  (printf "After updates:\n")
  (print-project after-four)

  (define transformed-after (emit-report after-four))
  (newline)

  (printf "Quoted snapshot before: ~a\n" (hash-ref transformed-before 'quoted))
  (printf "Quoted snapshot after: ~a\n" (hash-ref transformed-after 'quoted)))

(module+ main
  (main))

(module+ test
  (require rackunit)

  (check-equal? (count-open (project-tasks sample-project)) 3)
  (check-true (task-open? (find-task-by-id (project-tasks sample-project) 6)))
  (check-equal? (length (tasks-with-tag (project-tasks sample-project) 'compiler)) 3)
  (check-equal? (hash-ref (project-summary sample-project) 'total) 6)
  (check-false (find-task-by-id (project-tasks sample-project) 999))
  (check-equal? (calc-risk sample-project) "medium"))
