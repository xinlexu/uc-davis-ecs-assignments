(defun min-mean-max (numbers)
  (let* ((min (reduce #'min numbers))
         (max (reduce #'max numbers))
         (sum (reduce #'+ numbers))
         (count (length numbers))
         (gcd (reduce #'gcd (list sum count)))
         (simplified-numerator (/ sum gcd))
         (simplified-denominator (/ count gcd)))
    (list min
          (if (= simplified-denominator 1)
              simplified-numerator
              (/ simplified-numerator simplified-denominator))
          max)))
