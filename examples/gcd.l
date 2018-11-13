Use the Euclidean Algorithm
(if (< (argc) 2)
 (swrite "Usage: gcd.l a b" endl)
 (exit 1)
)
(= a (toint(argv 0)))
(= b (toint(argv 1)))
(while b
  (= tmp b)
  (= b (% a b))
  (= a tmp)
)
(swrite a endl)
