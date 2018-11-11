# bandged together test script
import subprocess

CMDS=dict(
COLLATZ="./interpreter examples/collatz.l 6",
FIB="./interpreter examples/fib.l 20",
ATOI="./interpreter examples/atoi.l 30",
STRCMP1="./interpreter examples/strcmp.l foo",
STRCMP2="./interpreter examples/strcmp.l bar",
SREAD="echo mlisp | ./interpreter examples/sread.l",
RETYPE="./interpreter examples/retype.l",
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
SREAD="Hello, mlisp!\n",
RETYPE="Hello, World!\n25 \n",
)

def test():
    for key in OUTS:
        out = subprocess.check_output(CMDS[key],shell=True).decode('ascii')
        if out.strip() != OUTS[key].strip():
            print("Failed test {}. Expected:\n{}Got:\n{}".format(key,OUTS[key],out))
        else:
            print("Passed test {}.".format(key))

test()
