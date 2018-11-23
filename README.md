# slimp
slimp (Matt's Lisp Compiler) is a bare-bones partial implementation of Lisp designed with an emphasis on
efficient memory utilization.
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
See <code>examples/</code> for program examples and <code>LANGUAGE.md</code> 
for a more comprehensive list of language features. <code>examples/demo.l</code> contains an example of every language feature.
A program file can be run from the command line with:
<pre>$ /path/to/interpreter /path/to/source.l &lt;command line arguments&gt;</pre>
A REPL shell can be invoked as:
<pre>$ /path/to/interpreter -i
&gt; (= a 5)
5
&gt; (+= a 2)
7
&gt; (swrite "this is the slimp shell" endl)
this is the slimp shell
0
&gt; (exit)
$</pre>
