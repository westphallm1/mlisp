## Grammar 
<pre>
exprs -> expr exprs
      |   
      ;   
expr -> ( args )
      ;   
args -> arg args
      |   
      ;   
arg  -> ATOM
      | expr
      ;   
</pre>
## Data Types
slimp supports integers (32 bits, signed) and character strings. Booleans are handled C-style 
(0 = false, nonzero = true). There are no lists, structures, functions, or floating points 
supported in this version.

## Variables
Any lower case string that is not a keyword can be used as a variable. Variables 
are instantiated dynamically, and are dynamically typed.

## Functions
Functions are invoked using the syntax <code>(fn arg1 arg2 ... argn)</code>, where
each arg may also be a function invocation. Functions return a single value, either
an integer or a character pointer. 
### Reserved IDs
Several variable names have pre-set values, and cannot be modified.

| id             | Value
|--------------- | -----
|      endl      | Newline string
|      strlit    | Emtpy string
|      int       | 0
|      else      | Negates the condition of parent if statement


### Zero argument functions
Syntax: <code>(fn)</code>  

|fn              | Description
|--------------- | -----------
|      argc      | Number of command line arguments as an integer
|      sread     | Read any number of characters from stdin
|      exit      | Exit with status 0
          
### One argument functions
Syntax: <code>(fn arg)</code>

| fn             |  arg             | Description
|--------------- | ---------------- | -----------
|       !        |    int           | Logical not 
|       ~        |    int           | Bitwise not 
|     sread      |    int           | Read `arg` characters from stdin
|     argv       |    int           | Get the `arg`th command line argument
|     toint      |    string        | Converts string to integer
|     exit       |    int           | exit with status `arg`

### Two argument functions
Syntax: <code>(fn arg1 arg2)</code>

| fn             |  args             | Description
|----------------|-------------------|-------------
|%, &lt;, &gt;, &lt;=, &gt;=| int, int| arg1 `fn` arg2
| =              | id, any           | copy the value and type of arg2 into arg1
| +=, -=, \*=, /=| id, int           | set arg1's value to its own value `fn` arg2
| strcmp         | string, string    | return glibc's strcmp of the two strings

### N-argument functions

| fn             |  args             | Description
|----------------|-------------------|-------------
|+, -, * , /     | int ... int       | arithmetic functions: arg1 `fn` arg2 ... argn
| |, &, ^        | int ... int       | bitwise functions: arg1 `fn` arg2 ... argn
| swrite         | any ... any       | print all arguments to stdout

