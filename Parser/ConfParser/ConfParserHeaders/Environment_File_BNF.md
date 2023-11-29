```
Environments = Key "=" Value LF

```

[a-zA-Z_]+[a-zA-Z0-9_]*



<EnvVar> ::= <VariableName> "=" <Value>
<VariableName> ::= <Letter> { <Letter> | <Digit> }
<Value> ::= <Char> { <Char> }
<Letter> ::= ALPHA | "_"
<Char> ::= <Letter> | <Digit> | <SpecialChar>

<SpecialChar> ::= "!" | "@" | "#" | "$" | "%" | "^" | "&" | "*" | "(" | ")" | ... 