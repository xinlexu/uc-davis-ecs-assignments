; You may define helper functions here

(defun matrix-add (matrix1 matrix2)
  (mapcar #'(lambda (row1 row2) (mapcar #'+ row1 row2))
          matrix1
          matrix2))


(defun matrix-transpose (matrix)
  (apply #'mapcar #'list matrix))


(defun matrix-multiply (matrix1 matrix2)
  (let ((transposed-matrix2 (matrix-transpose matrix2)))
    (mapcar #'(lambda (row1)
                (mapcar #'(lambda (row2)
                            (reduce #'+ (mapcar #'* row1 row2)))
                        transposed-matrix2))
            matrix1)))
