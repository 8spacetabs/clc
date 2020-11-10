# CLC

## Installation
Install GNU readline and run `make && make install` in the root directory of the project

## Usage
Running `clc` with no arguments will enter a REPL in which arithmetic expressions can be entered,
in this mode clc will exit on EOF. Line editing is supported in this mode as well as session history via GNU readline.

Running `clc "<expression>"` will evaluate the expression and exit after printing the result.

### Supported operations
```
+       addition
-       subtraction & negation
*       multiplication
/       division
^       exponentiation
\       square root
()      forcing precedence
```
### Valid operands
```
[0-9]*\.*[0-9]*        integer/real numbers
a                      previous answer
```
All operations are done on `long double`s. Binary operators are left-associative. Precedence is BIDMAS.
