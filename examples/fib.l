(if (argc)
  (= n (toint (argv 0)))
else
  (swrite "Usage: fib.l <max term>")
  (exit 1)
)

(= a 0)
(= b 1)
(while n
  (swrite b endl)
  (-= n 1)
  (+= b a)
  (= a (- b a))
)
(swrite b endl)
