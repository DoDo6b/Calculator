# Calculator
The grammar for input is:

    calculate:
        print
        help
        quit
        enterpoint

    enterpoint:
        declaration
        expression

    print:
        ';'

    quit:
        'q'

    declaration:
        "let" name '=' expression

    expression:
        term
        expression '+' term
        expression '-' term

    term:
        secondary
        term '*' secondary
        term '/' secondary
        term '%' secondary

    secondary:
        primary '!' 
        "Sqrt(" expression ')'
        "Pow(" expression ',' expression ')'

    primary:
        number
        '(' expression ')'
        '{' expression '}'
        '-' primary
        '+' primary
        variable
        "sqrt(" expression ')'
        "pow(" expression ',' expression ')'
        "sin(" expression ')'
        "cos(" expression ')'

    number:
        float(floating-point-literal)

    Input from cin through the Token_stream called tstream.