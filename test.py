# bandged together test script
import subprocess

CMDS=dict(
COLLATZ="./interpreter examples/collatz.l 6",
FIB="./interpreter examples/fib.l 20",
ATOI="./interpreter examples/atoi.l 30",
STRCMP1="./interpreter examples/strcmp.l foo",
STRCMP2="./interpreter examples/strcmp.l bar",
SREAD="echo slimp | ./interpreter examples/sread.l",
RETYPE="./interpreter examples/retype.l",
FIZZBUZZ="./interpreter examples/fizzbuzz.l",
)
OUTS=dict(
COLLATZ="""\
3 
10 
5 
16 
8 
4 
2 
1 

""",
FIB="""\
1 
1 
2 
3 
5 
8 
13 
21 
34 
55 
89 
144 
233 
377 
610 
987 
1597 
2584 
4181 
6765 
10946

""",
ATOI="60 \n",
STRCMP1="hello world!\n",
STRCMP2="goodbye world!\n",
SREAD="Hello, slimp!\n",
RETYPE="Hello, World!\n25 \n",
FIZZBUZZ="""\
1 
2 
Fizz
4 
Buzz
Fizz
7 
8 
Fizz
Buzz
11 
Fizz
13 
14 
FizzBuzz
16 
17 
Fizz
19 
Buzz
Fizz
22 
23 
Fizz
Buzz
26 
Fizz
28 
29 
FizzBuzz
31 
32 
Fizz
34 
Buzz
Fizz
37 
38 
Fizz
Buzz
41 
Fizz
43 
44 
FizzBuzz
46 
47 
Fizz
49 
Buzz
Fizz
52 
53 
Fizz
Buzz
56 
Fizz
58 
59 
FizzBuzz
61 
62 
Fizz
64 
Buzz
Fizz
67 
68 
Fizz
Buzz
71 
Fizz
73 
74 
FizzBuzz
76 
77 
Fizz
79 
Buzz
Fizz
82 
83 
Fizz
Buzz
86 
Fizz
88 
89 
FizzBuzz
91 
92 
Fizz
94 
Buzz
Fizz
97 
98 
Fizz
Buzz

"""
)

def test():
    for key in OUTS:
        out = subprocess.check_output(CMDS[key],shell=True).decode('ascii')
        if out.strip() != OUTS[key].strip():
            print("Failed test {}. Expected:\n{}Got:\n{}".format(key,OUTS[key],out))
        else:
            print("Passed test {}.".format(key))

test()
