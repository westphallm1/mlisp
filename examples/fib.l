(if (argc)
  (= n (argv 0))
else
  (swrite "Usage: fib.l <max term>")
  (exit 1)
)

(= a 1)
(= b 1)
(while n
  (swrite b)
  (-= n 1)
  (+= b a)
  (= a (- b a))
)
(swrite b)
