# MSDScript Documentation

##Description
MSDScript is a language interpreter based in C++ that is capable of performing:
* Addition
* Multiplication
* Simple comparisons
* If/then/else statements
* Variable creation and assignment
* Mathematical function calls

Input can be provided in the form of a string to be parsed into an MSDScript expression and then either printed in its expression form or interpreted down to a value form and then printed.

## Getting Started
### To use MSDScript:
1. Download the MSDScript directory
2. From the command line, navigate to within the MSDScript directory
3. Use the “make” command to compile and link the project files and create the msdscript executable. This may take some time due to the number of files
   * Note: Use “make libmsdscript.a” to create just the library alone
4. Run the executable from the command line with “./msdscript” and pass at least one argument in to tell MSDScript how you wish to use it
   * Possible arguments:
     - “./msdscript --help” will print all possible arguments
     - “./msdscript --test” will run the unit tests and print the results
     - “./msdscript --interp” will await user input to parse and then interpret it using an unlimited amount of nesting evaluations and print the final value
     - “./msdscript --step” will await user input to parse and then interpret it using its own Continuations classes to limit the amount of nested evaluations and print the final value
     - “./msdscript --print” will await user input to parse and then print with basic formatting
     - “./msdscript --pretty-print” will await user input to parse and then print with better formatting
     - Note: For more information about what --test, --interp, --step, --print, and --pretty-print do, see the User Manual: Execution Modes section below
5. If the program is awaiting user input then type in an expression, press enter, and then press control+d to signal the end of the input
   * Note: For more information about valid expressions, see the User Manual: Expressions section below
6. MSDScript will run using whatever arguments and input you provided, print the result, and then exit back to the command line
   * Note: For more information about possible errors that could occur at this stage, see the User Manual: Possible Errors section below

## User Guide
### Parsing
MSDScript’s primary use is to read in an input, in a human-readable string format, that the user provides and parse it into the equivalent MSDScript objects that all together form an MSDScript expression. This input can include any of the types of expressions listed in the following Expressions section as well as any parentheses surrounding any of these expressions, as long as the parentheses are all appropriately closed. In addition, any amount of whitespace can be included before, between, or following these expressions, including tabs or new lines. Once translated to the MSDScript language, more can be done with the expression, but the critical first step is parsing. This is done automatically when run on the command line with the --interp, --step, --print, or --pretty-print argument.

### Expressions
Parsing will initialize objects of the Expr subclasses that represent the pieces of the input expression. The total input will be an Expr, but typically that Expr is made up of many smaller Expr objects. The following are all of the valid expressions in MSDScript:

#### Numbers
  - Any whole number from -2147483648 through 2147483647
  - Represented as a continuous string of digits that may have a negative sign in front of it but may not have any internal punctuation or spacing
  - Examples:
    * 1
    * -99 
    * 2147483647
#### Variables
  - Any continuous string of alpha (a-z and A-Z) characters can be used to represent a variable name
  - Examples:
      * x
      * test
      * varName
      * ALLCAPSNAME 
#### Addition
  - Any two expressions with ‘+’ between them
  - Examples:
    * 1+1
    * 10000 + 2000 + 300 + 40 + 5
    * x + -99
#### Multiplication
  - Any two expressions with ‘*’ between them
  - Examples:
    * 2*3
    * 10 * varName * 10 
#### Equals Comparison
  - Any two expressions with “==” between them 
  - Examples:
    * _true == _false
    * 1 == 1
    * x == 0
#### Keywords
  - All keywords are denoted by an underscore immediately preceding the keyword
  - Valid keywords are:
    * Booleans
    * True is represented by “_true”
    * False is represented by “_false”
#### If/then/else expressions
  - Must be in the format of “_if” followed by an expression, then “_then” followed by an expression, and finally “_else” followed by an expression
  - If the expression following the “_if” can be interpreted to be true then the expression following the “_then” will be returned, but if it is false then the expression following the “_else” will be returned
  - Examples:
    * _if _true    _then 1    _else 0
    * _if x == 1  _then x    _else y
#### Let expressions
  - Must be in the format of “_let” followed by a variable, an equal sign, then an expression, and ending with “_in” and another expression
  - This will assign the first expression to the provided variable and then substitute every free instance of that variable within the second expression
  - Examples:
    * _let  x  =  5  _in  x  +  1
    * _let  y  =  5  _in  x  +  1
    * _let x = 5
    _in    (_let y = 3
    _in y + 2)   +   x
#### Functions
  - Must be in the format of “_fun” followed by a variable surrounded by parentheses and then an expression
  - This represents a mathematical function with no argument yet (that is where the function call will come in)
  - Examples:
    * _fun (x) x+1
    * _fun (a) a*a
    * _fun (x) (_fun (y) ((x\*x) + (y\*y))
#### Function calls
  - Must be in the format of an expression followed by a second expression, with the second expression surrounded by parentheses
  - This will pass the second expression as the argument to the first expression (and so this only works properly when the first expression is a function expression)
  - Examples:
    * a(6)
    * (_fun (x) x)(2)
    * (_fun (x) (_fun (y) ((x\*x)+(y\*y)))(3))(2)

### Precedence and Parentheses
MSDScript uses right associativity and uses the following order of operator precedence (in order of priority):

1. Any parenthesized expression
2. All keyword operations (booleans, if/then/else expressions, let expressions, functions, function calls)
3. Multiplication
4. Addition
5. Equals Comparison

Thus, parentheses should be used in the input to control how the expression is parsed. For instance, “1+2+3” will parse into the equivalent of “1+(2+3)” so in order add the 1 and 2 first you must parenthesize the left addition expression like so, “(1+2)+3”. MSDScript is capable of parsing even with unnecessary parentheses, so for clarity it may be best to parenthesize early and often.

#### Execution Modes
Once parsed into MSDScript objects, the entire expression can be used in multiple ways. This will depend on the argument that is passed in using the command line.
* --interp
  - Interprets the parsed expression, trying to simplify the expression piece by piece until a final value representing the interpretation of the entire expression is reached. This frequently includes interpreting each of the smaller individual expressions that make up the overall expression and so many calls to the interp() method are placed on the stack and can result in a seg fault.
  - In the end, this will print the value resulting from the interpretation.
  - Interp mode may be faster than step mode sometimes, but it can fail more easily. If a segmentation fault occurs when trying to use interp mode then try the same expression but in step mode instead.
* --step
  - This is very similar to --interp, but it uses MSDScript’s Continuations class to manage the workflow of interpreting the many smaller expressions to form the overall expression, which limits the nesting of evaluations and thus results in fewer seg faults.
  - In the end, this will print the value resulting from the interpretation.
* --print
  - This will translate the MSDScript objects back into a human-readable string representation and print it.
  - The printed format will be very verbose with many parentheses and compact spacing
* --pretty-print
  - This will similarly translate the MSDScript objects back into a human-readable string representation and print it, but with much smarter and more readable formatting.
  - The printed format will use parentheses only when necessary and will include much more spacing both between expressions and within certain expressions, including new lines and vertical alignment for easier reading.

#### Possible Errors
An error may be thrown at many points while running MSDScript and in each case this will cause the execution to exit and print the error message to the screen. Here are all of the errors that could be encountered and their common causes:
* Parse Errors:
  - “unexpected input after expression”
    * This can occur when an expression is parsed, but there is still more input after the end of that expression and no way to connect (no +, *, etc.) the expressions
    * Examples:
      - 1 + 1   5
      - x   _fun (x) x+1
  - “missing close parenthesis”
    * This can occur at multiple points while parsing but indicates that parentheses were not closed properly
    * Examples:
      - (1
      - (  _fun (x) x  ) (0
  - “invalid input”
    * This can occur as a result of an unrecognized character or lack of any character where one was expected
    * Examples:
      - An empty input will cause and invalid input error
      - ()
      - 0 +
      - _thisIsNotAValidKeyword
  - “parse_num caused int overflow”
    * This can occur when a number is recognized but cannot be stored as an integer type (less than -2147483648 or greater than 2147483647)
    * Examples:
      - -2147483649
      - 1000000000000000000
  - "missing or invalid location of equals sign in _let expression"
    * This can occur when an equals sign is not found where expected in a let expression
    * Examples:
      - _let x 5 _in 1
  - "missing or invalid location of '_in' in _let expression"
    * This can occur when the _in keyword is not found where expected in a let expression
    * Examples:
      - _let x = 5 _on 1
  - “missing or invalid location of '_then' in _if expression”
    * This can occur when the _then keyword is not found where expected in an if expression
    * Examples:
      - _if _true _else _true
  - "missing or invalid location of '_else' in _if expression"
    * This can occur when the _else keyword is not found where expected in an if expression
    * Examples:
      - _if x == 2 _then 2 _eeeeeeeelse 0
  - “missing or invalid location of '(' in _fun expression”
    * This can occur when the opening parenthesis before the variable of the fun expression is not found where expected
    * Examples:
      - _fun y 1
  - "missing or invalid location of ')' in _fun expression"
    * This can occur when the closing parenthesis following the variable of the fun expression is not found where expected
    * Examples:
      - _fun (y 1
  - “consume mismatch” and “consume string mismatch”
    * Both of these are internal checks that should never occur to a standard user but indicate an issue in the code where the character or string found in a call to consume() is different than what is expected
* Value Errors:
  - "Cannot add_to() with a BoolVal"
    * This can occur when interpreting an expression that tries to add any expression to a boolean expression
  - “Cannot multiply_by() with a BoolVal"
    * This can occur when interpreting an expression that tries to multiply any expression with a boolean expression
  - "Cannot call() a BoolVal" and “Cannot call_step() a BoolVal”
    * These can both occur when interpreting a function call expression with a boolean as the body rather than a function expression
  - "Cannot add_to() with a FunVal"
    * This can occur when interpreting an expression that tries to add any expression to a function expression
  - “Cannot multiply_by() with a FunVal"
    * This can occur when interpreting an expression that tries to multiply any expression with a function expression
  - "Cannot use is_true() on a FunVal"
    * This can occur when interpreting an if expression with a function as the condition rather than an equals comparison expression
  - "Cannot use add_to() with a non-number"
    * This can occur when interpreting an expression that tries to add anything that does not simplify down to a number value
  - "Cannot use multiply_by() with a non-number"
    * This can occur when interpreting an expression that tries to multiply with anything that does not simplify down to a number value
  - "Cannot use is_true() on a NumVal"
    * This can occur when interpreting an if expression with a number as the condition rather than an equals comparison expression
  - "Cannot call() a NumVal" and “Cannot call_step() a NumVal”
    * These can both occur when interpreting a function call expression with a number as the body rather than a function expression
* Environment Error:
  - "free variable: "
    * This can occur when interpreting an expression that contains a free variable (a variable that has no value bound to it ever and thus can not be interpreted)
* Continuation Error:
  - "Cannot step_continue() with a DoneCont"
    * This can occur when interpreting by steps if the interpreter tries to do more steps but the continuation is a DoneCont, indicating it has no more steps to do

## API Documentation
In this implementation of MSDScript, each of the expressions, values, environments, and continuations have their own file. There are many subclasses of the base classes Expr, Val, Env, and Cont for each specific use and case, such as AddExpr, NumVal, or RightThenMultCont. If necessary, you could use these classes directly by constructing and initializing them with their constructors and then manipulate them however you would like using their methods. For instance, all Expr subclasses have interp(), step_interp(), print(), and pretty_print_at() methods. However, these do the same things, respectively, as using the --interp, --steps, --print, or --pretty-print arguments discussed previously in the User Guide: Execution Modes section.

The most likely use for MSDScript is to perform calculations. For example, if multiple calculations were to be done using the same formula but varying inputs, a program could be written that took the varying input as a parameter and put it in the appropriate location within the formula and then used MSDScript to parse and interpret the resulting expression. This would require the MSDScript library named libmsdscript.a and use of the parse, interpret, and to_string capabilities.

To parse you can use one of two options depending on the type of your input:
* PTR(Expr) parse(std::istream &in)
  - Parses and returns an MSDScript Expr from the provided input stream in
* PTR(Expr) parse_string(std::string s)
  - Parses and returns an MSDScript Expr from the provided string s

If there were no errors in the input and parsing process, then these will both return an Expr that represents the entire input. This Expr can then be interpreted using either:
* PTR(Val) interp(PTR(Env) env)
  - Interprets the Expr that it was called from using the provided environment (should always begin with an empty environment though) and returns the value representation of the interpretation
* PTR(Val) interp_by_steps(PTR(Expr) e)
  - Interprets the Expr that is passed in as a parameter and returns the value representation similarly to before, but using Step and Continuation classes to store information about the interpretation process rather than using the C++ stack
  - This will be much less likely to result in a seg fault if the calculation being done is very large and recursive-heavy

Finally, the Val object that was returned cannot be printed directly, but it does have a to_string() method to get the human readable representation of the value:
* std::string to_string()
  - Converts the Val to a human-readable string, which is much better for printing a result
