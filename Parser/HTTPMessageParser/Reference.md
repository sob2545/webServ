HTTP-message   = start-line CRLF * (field-line CRLF) CRLF [ message-body ]
HTTP-name      = %x48.54.54.50  ; HTTP
HTTP-version   = HTTP-name "/" DIGIT "." DIGIT


start-line     = request-line / status-line

; reqeuest
request-line   = method SP request-target SP HTTP-version
request-target = origin-form / absolute-form / authority-form / asterisk-form

origin-form    = absolute-path [ "?" query ]
absolute-form  = absolute-URI
authority-form = uri-host ":" port
asterisk-form  = "*"

absolute-URI   = scheme ":" hier-part [ "?" query ]

method         = token


; field (Header)
field-line     = field-name ":" OWS field-value OWS
field-name     = token
field-value    = *field-content
field-content  = field-vchar [ 1*( SP / HTAB / field-vchar ) field-vchar ]
field-vchar    = VCHAR / obs-text


; message body
message-body   = *OCTET


; chunk part
chunk          = chunk-size [ chunk-ext ] CRLF chunk-data CRLF
chunk-data     = 1*OCTET
chunk-ext      = *( BWS ";" BWS chunk-ext-name
									[ BWS "=" BWS chunk-ext-val ] )
chunk-ext-name = token
chunk-ext-val  = token / quoted-string
chunk-size     = 1*HEXDIG
chunked-body   = 1*chunk last-chunk trailer-section CRLF

last-chunk     = 1*"0" [ chunk-ext ] CRLF



; text
token          = 1*tchar

tchar          = "!" / "#" / "$" / "%" / "&" / "'" / "*"
               / "+" / "-" / "." / "^" / "_" / "`" / "|"
               / "~" / DIGIT / ALPHA 
							 ; any VCHAR, except delimeters

obs-text       = %x80-FF  ; unsigned char 128 - 254 (No more use in HTTP/1.1

OWS            = *( SP / HTAB )
               ; optional whitespace

RWS            = 1*( SP / HTAB )
               ; required whitespace

BWS            = OWS
               ; "bad" whitespace