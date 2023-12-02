```
Context-Lines			= 1*(*WSP (Context | c-nl) )

Context					= Directives (Block-Content | c-nl)

Directives				= Directive-Name 1*WSP *[ARG WSP]

Block-Content			= [c-nl | *WSP] '{' [c-nl] 1*Context-Lines [c-nl] '}' [c-nl]
// Block-Context-Directives
	- events
	- http
	- server
	- location
	- limit_except + [GET POST ...]

<!-- Simple-Content		= c-nl -->
// Simple-Content-Directives
	- env
	- worker_proccess
	- worker_connection
	- daemon
	- timer_resolution
	- error_log
	- server_name
	- root
	- listen
	- proxy_pass
	- error_page
	- autoindex
	- index
	- access_log
	- cgi
	- internal
	- keepalive_timeout
	- include
	- default_type

// Enable duplicate directives
	- server
	- location
	- env
	- include


Directive-Name			= ALPHA *(ALPHA | '_')

ARG						= *(ALPHA | DIGIT | '_' | '/' | '.' | '=') // 대충 정의
// abs_path		=>	error_log, access_log, root, location
// host			=>	server_name, cgi
// URIParser	=>	proxy_pass

c-wsp					= WSP | (c-nl WSP)

c-nl					= comment / LF

comment					= ";" *(WSP | VCHAR) LF

VCHAR					= %x21-7E
// Visible Character(33 ~ 126)


```
