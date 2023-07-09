"""
.coec: Class Oriented Eanor's C
"""

INPUT_PATH = "main.coec"
from lark import Lark, Token, Transformer, Tree, v_args

with open(INPUT_PATH, "r") as f:
    text = f.read()

text = text.replace("#include", "$include")

# use lark to match c code and extract class definitions
grammar = r"""
    start: include* function*
    name: CNAME
    type: CNAME "*"* 
    function_argument: type name
    include: "$include" string
    function: type name "(" [function_argument ("," function_argument)*] ")" (("{" function_body "}") | ";")
    function_body: statement*
    define: "$define" CNAME statement
    statement: expression ";"
    expression: expression tuple
        | type expression
        | "*"* expression
        | expression "=" expression
        | expression "+" expression
        | expression "-" expression
        | expression "*" expression
        | expression "/" expression
        | expression "%" expression
        | expression "==" expression
        | expression "!=" expression
        | expression "<" expression
        | expression ">" expression
        | expression "<=" expression
        | expression ">=" expression
        | expression "&&" expression
        | expression "||" expression
        | "!" expression
        | expression "&" expression
        | expression "|" expression
        | expression "^" expression
        | expression "<<" expression
        | expression ">>" expression
        | expression "=" expression
        | expression "+=" expression
        | expression "-=" expression
        | expression "*=" expression
        | expression "/=" expression
        | expression "%=" expression
        | expression "&=" expression
        | expression "|=" expression
        | expression "^=" expression
        | expression "<<=" expression
        | expression ">>=" expression
        | expression "++"
        | expression "--"
        | expression "." CNAME
        | expression "->" CNAME
        | tuple
        | expression "[" expression "]"
        | constant
    tuple: "(" [expression ("," expression)*] ")"
    constant: NUMBER | string
    string: /\'[^\']*\'|\"[^\"]*\"/
    %import common.WS
    %import common.CPP_COMMENT
    %import common.C_COMMENT
    %import common.CNAME
    %import common.NUMBER
    %ignore WS
"""


@v_args(inline=True)    # Affects the signatures of the methods
class CalculateTree(Transformer):
    # for each class definition, replace it with:
    # CLASS(name);
    # #define SELF Person
    # PROPERTY(prop1);
    # ...
    # METHOD(method1);
    # ...
    # #undef SELF
    def __init__(self, visit_tokens: bool = True) -> None:
        super().__init__(visit_tokens)
    
    def classdef(self, name, body):
        return Tree("classdef", [name, body])

parser = Lark(grammar, parser="lalr")#, transformer=CalculateTree())
tree = parser.parse(text)
print(tree.pretty())


text = ''''''