IDENT = 1     # identifiers
TYPE = 2      # string, integer, and foreign types
EOF = 3       # end of file

def next_token(typenames, word):
    word = "".join(word)
    if word in typenames:
        return (TYPE, word)
    else:
        return (IDENT, word)

def chars(filename):
    with open(filename, "rt") as f:
        while True:
            c = f.read(1)
            if not c:
                break
            yield c
        
def lexer(filename, typenames):
    word = []
    newtype = None
    for c in chars(filename):
        if not c.isalnum() and c != '_':
            if word:
                newtype = (yield next_token(typenames, word))
                word = []
            if not c.isspace():
                newtype = yield (c, c)
        else:
            word.append(c)
        if newtype is not None:
            typenames.append(newtype)
            newtype = None
    if word:
        yield next_token(typenames, word)

def expect(lex, *pargs, **kwargs):
    try:
        if "send" in kwargs:
            t, w = lex.send(kwargs["send"])
        else:
            t, w = next(lex)
    except StopIteration:
        if EOF not in pargs:
            raise ValueError("Unexpected end of file")
        return None
    if t not in pargs:
        raise TypeError("Expect {}, got {}".format(str(pargs), t))
    return w

def column_parser(lex):
    while True:
        name = expect(lex, IDENT, "}")
        if name == "}":
            break
        expect(lex, ":")
        type = expect(lex, TYPE)    # we expect type here (including table names)
        expect(lex, ";")
        yield (name, type)

def table_parser(lex):
    name = None
    while True:
        name = expect(lex, IDENT, EOF, send=name)
        if name is None:
            break
        expect(lex, "{")
        columns = [c for c in column_parser(lex)]
        yield (name, columns)
                
def parser(filename):
    typenames = ["integer", "string", "float"]
    lex = lexer(filename, typenames)
    return [t for t in table_parser(lex)]

if __name__ == "__main__":
    print(parser("default.txt"))


    
                