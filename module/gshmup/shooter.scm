(define-module (gshmup shooter)
  #:use-module (gshmup core)
  #:use-module (gshmup coroutine)
  #:use-module (gshmup helpers)
  #:export (init-shooter))

(add-hook! player-shoot-hook (lambda () (test-shot)))

(define-coroutine (test-shot)
  (when (player-shooting?)
    (let ((speed 15)
          (p (player-position)))
      (emit-bullet p speed 268)
      (emit-bullet p speed 270)
      (emit-bullet p speed 272)
      (emit-bullet p speed 180)
      (emit-bullet p speed 225)
      (emit-bullet p speed 315)
      (emit-bullet p speed 360))
    (wait 5)
    (test-shot)))

(add-hook! shooter-init-hook (lambda () (spawn-test-enemy)))

(define (spawn-test-enemy)
  (spawn-enemy (make-vector2 240 240) (lambda () (test-pattern))))

(define-coroutine (test-pattern)
  (let fire ((a 0)
             (n 16))
    (repeat n (lambda (i)
                (emit-bullet (make-vector2 240 240) 5 (+ a (* 360 (/ i n))))))
    (wait 6)
    (fire (+ a 5) n)))
