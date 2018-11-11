
(if (!= (argc) 3)
  (swrite "Usage: is_right s1 s2 s3"endl)
  (exit 1)
)
//read side lengths from command line
(= a (toint(argv 0)))
(= b (toint(argv 1)))
(= c (toint(argv 2)))
//square each side
(*= a a)
(*= b b)
(*= c c)

//check each combination of side 
(= isright 0)
(+= isright (== (+ a b)c))
(+= isright (== (+ b c)a))
(+= isright (== (+ c a)b))
(if isright (swrite "Triangluar."endl))
