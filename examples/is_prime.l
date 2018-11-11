(= n (toint (argv 0)))
(= q 2)
(= prime 1)
(while (& (< q n) prime)
  (if (!(%n q)) (= prime 0))
  (+= q 1)
)

(if prime
  (swrite n "is prime!" endl)
else
  (swrite n "is not prime..." endl)
)
