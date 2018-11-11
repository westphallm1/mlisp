"((((((((((((((((((((((Mlisp Language Tour))))))))))))))))))))))"
Statements outside parentheses are comments
Statements are formatted:
"(" function arg0 ... argN ")", where each arg might also be 
a function enclosed in parentheses

Functions can take an arbitrary number of arguments
(= c (+ 1 2 3 4 5 6 7 8 9 10))

Basic math is supported
(= a 5)
(= b 10)

Integers are auto-converted to ascii for printing
(swrite "Ops on " a "and " b": " (* a b) (+ a b) (- a b) (/ b a) (% a b) 
 (| a b) (& a b) (^ a b) (~a) (!b) endl)

ASCII must be converted using "toint"
(= b (toint "10"))

Compound assignment is supported
(+= a 0)
(-= a 0)
(*= a 1)
(/= a 1)

Functions are interpretted recursively, eg. "(/ b a)" is evaluated first
(swrite (+ (* a (/ b a)) a) endl)

Variables are dynamically typed
(= b "World")
(swrite "Hello, " b "!" endl)

Whitespace is usually optional
(swrite"Goodbye, "b"!"endl)

Control flow is also implemented as parenthesized lists
(if (< a 10) (swrite a "is less than 10!" endl))
(while (< a 10) 
  (swrite a)
  (+= a 1) 
)
(swrite endl)

String comparison is supported
(= mlisp "awesome")
(if (!(strcmp mlisp "awesome"))
  (swrite "mlisp is " mlisp "!" endl)
)


Command line arguments can be accessed with argc and argv
try running as interpreter apples pears
(= i 0)
(while (< i (argc))
  (swrite "argv["i"]: " (argv i) endl)
  (+= i 1)
)

Reading from stdin is supported
(swrite "Enter your favorite fruit: ")
(= answer (sread))
(swrite "My favorite fruit is also " answer "!" endl)

Things that are not currently supported:
* Many common string operations
* Floating points
* Lists
