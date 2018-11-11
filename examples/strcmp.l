(= a (argv 0))
(if (!(strcmp a "foo"))
  (swrite "hello world!")
else
  (swrite "goodbye world!")
)
