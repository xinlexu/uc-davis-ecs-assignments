; You may define helper functions here


(defun reachable (trans-func start-state final-state input-seq)
  (cond
    ((null input-seq) (eql start-state final-state))
    (t (let ((next-states (funcall trans-func start-state (first input-seq))))
         (some #'(lambda (state) (reachable trans-func state final-state (rest input-seq)))
               next-states)))))
