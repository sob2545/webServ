

/*
	rulelist      = 1title *WSP 1typelist

	title         = 5ALPHA

	typelist      = "{" *[LF | WSP] *type 1 *[LF | WSP] "}"

	type          = typename *WSP 1*extension ";"

	typename      = group "/" elements

	extension     = 1*[ALPHA | DIGIT]

	group         = 1*ALPHA

	elements      = 1*(ALPHA | DIGIT] * PCHAR

	PCHAR         = ALPHA | DIGIT | "+" | "-" | "."
*/