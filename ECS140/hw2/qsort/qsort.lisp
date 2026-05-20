(defun pivot (n xs)
  (loop for x in xs
        with less = nil
        with greater-or-equal = nil
        do (if (< x n)
               (push x less)
               (push x greater-or-equal))
        finally (return (list (nreverse less) (nreverse greater-or-equal)))))

(defun quicksort (xs)
  (if (or (null xs) (null (rest xs)))
      xs
      (let* ((pivot (first xs))
             (rest (rest xs)))
        (multiple-value-bind (less greater-or-equal) (pivot pivot rest)
          (append (quicksort less) (list pivot) (quicksort greater-or-equal))))))
