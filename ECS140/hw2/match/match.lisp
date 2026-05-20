; You may define helper functions here

(defun match (pattern assertion)
  ;; TODO: incomplete function.


  (cond
    ((equal pattern assertion) T)
    ((or (null pattern) (null assertion)) NIL)
    ((eq (first pattern) '?)
     (and assertion (match (rest pattern) (rest assertion))))
    ((eq (first pattern) '!)
     (if (null (rest pattern))
         T
         (or (match (rest pattern) assertion)
             (and assertion (match pattern (rest assertion))))))
    ((equal (first pattern) (first assertion))
     (match (rest pattern) (rest assertion)))
    (T NIL)))
