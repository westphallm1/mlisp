# slimp
SLIMP is a Lisp Interpreter, Minus Parentheses! A C implementation of a LISP-like language with some variations on its S-expression parsing. Currently in early Alpha
## Configuration
<code>config.h</code> contains several macros that define the memory region sizes of the interpreter.
These parameters can be set to an arbitrarily large or small value, but certain programs will require a build with
increased memory. The default settings are sufficient to run every example program.
## Installation
1. Build the executable. Build has only been tested on Ubuntu 16.04 LTS
<pre>$ make</pre>
Objects can be compiled with debug symbols using
<pre>$ make CFLAGS=-g</pre>
2. Test that the executable built correctly
<pre>$ python3 test.py
/* all tests should pass */</pre> 
## Usage
See <code>examples/</code> for program examples.
A program file can be run from the command line with:
<pre>$ /path/to/interpreter /path/to/source.sl &lt;command line arguments&gt;</pre>
