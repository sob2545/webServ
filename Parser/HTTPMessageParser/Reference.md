HTTP-message   = start-line CRLF * (field-line CRLF) CRLF [ message-body ]
HTTP-name      = %x48.54.54.50  ; HTTP
HTTP-version   = HTTP-name "/" DIGIT "." DIGIT


start-line     = request-line

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
field-line     = field-name ":" OWS field-value
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


# HTTP HEADERS
## 일반 헤더 필드
<!-- Cache-Control:		캐싱 메커니즘을 제어합니다. -->
Connection:			현재의 연결 옵션을 지정합니다.
Date:				메시지가 생성된 날짜와 시간을 표시합니다.
<!-- Pragma:				구현 특정 지시어를 포함합니다. -->
<!-- Trailer:            메시지의 트레일러 섹션에 포함될 헤더 필드를 나타냅니다. -->
Transfer-Encoding:  메시지 본문에 대한 인코딩 형식을 지정합니다.
<!-- Upgrade:			사용할 다른 프로토콜을 나타냅니다. -->
<!-- Via:				메시지가 전달된 중간자를 표시합니다. -->
<!-- Warning:			메시지와 관련된 경고를 포함합니다. -->

## 요청 헤더 필드
<!-- Accept:					사용자 에이전트가 처리할 수 있는 미디어 타입을 지정합니다. -->
<!-- Accept-Charset:			사용자 에이전트가 선호하는 문자 인코딩을 지정합니다. -->
<!-- Accept-Encoding:		사용자 에이전트가 지원하는 콘텐츠 인코딩을 지정합니다. -->
<!-- Accept-Language:		사용자 에이전트가 선호하는 자연 언어를 지정합니다. -->
<!-- Authorization:			사용자 인증 정보를 포함합니다. // 실질적인 동작은 알아볼 것 : cookie에서 사용하는 등 -->
Expect:					서버에 특정 동작을 기대한다는 것을 나타냅니다.
<!-- From:					요청을 생성한 사용자의 이메일 주소를 나타냅니다. -->
Host:					요청된 리소스의 호스트와 포트를 지정합니다.
<!-- If-Match:				문서의 특정 버전에 대한 조건부 요청을 나타냅니다. -->
<!-- If-Modified-Since:		지정된 시간 이후에 변경된 경우에만 문서를 요청합니다. -->
<!-- If-None-Match:			문서의 특정 버전이 아닌 경우에만 요청합니다. -->
<!-- If-Range:				부분적인 GET 요청의 조건을 지정합니다. -->
<!-- If-Unmodified-Since:	지정된 시간 이후에 변경되지 않은 경우에만 요청합니다. -->
<!-- Max-Forwards:			메시지가 전달될 수 있는 최대 횟수를 지정합니다. -->
<!-- Proxy-Authorization:	프록시 서버에 대한 인증 정보를 포함합니다. -->
<!-- Range:					리소스의 특정 부분을 요청합니다. -->
<!-- Referer:				현재 요청된 페이지의 링크 이전의 웹 페이지 주소를 포함합니다. -->
<!-- TE:						전송 인코딩의 확장을 지정합니다. -->
<!-- User-Agent:				사용자 에이전트의 정보를 포함합니다. -->

## 응답 헤더 필드
<!-- Accept-Ranges:			서버가 바이트 범위 요청을 수용하는지 여부를 나타냅니다. -->
<!-- Age:					객체가 프록시 캐시에 저장된 후 경과된 시간을 나타냅니다. -->
<!-- ETag:					콘텐츠의 특정 버전을 식별하는 태그를 제공합니다. -->
Location:				리디렉션된 페이지의 주소를 제공합니다.
<!-- Proxy-Authenticate:		프록시 서버에서 클라이언트 인증을 요구합니다. -->
<!-- Retry-After:			클라이언트가 다음 요청을 시도해야 하는 시간을 나타냅니다. -->
Server:					서버의 소프트웨어 정보를 제공합니다.
<!-- Vary:					캐시된 응답을 선택하는 데 사용되는 헤더 필드를 지정합니다. -->
<!-- WWW-Authenticate:		HTTP 접근 인증을 위한 데이터를 포함합니다. -->
Allow:					리소스에 대해 허용되는 HTTP 메소드를 나타냅니다.
Last-Modified:			본문 데이터가 마지막으로 수정된 날짜와 시간을 나타냅니다.

## 엔티티 헤더 필드
<!-- Content-Encoding:		본문 데이터에 적용된 인코딩을 나타냅니다. -->
<!-- Content-Language:		본문 데이터의 언어를 나타냅니다. -->
Content-Length:			본문 데이터의 길이를 바이트 단위로 나타냅니다.
Content-Location:		본문 데이터에 대한 대체 URI를 제공합니다.
<!-- Content-MD5:			본문 데이터의 MD5 체크섬을 제공합니다. -->
<!-- Content-Range:			본문 데이터의 일부를 나타내는 경우 그 범위를 지정합니다. -->
Content-Type:			본문 데이터의 MIME 타입을 지정합니다.
<!-- Expires:				본문 데이터가 만료되는 날짜와 시간을 나타냅니다. -->

## 쿠키 헤더 필드
<!-- Request -->
https://developer.mozilla.org/ko/docs/Web/HTTP/Headers/Cookie
Cookie:			name=value; name2=value2; name3=value3
Cookie:			sessionId=abc123; username=JohnDoe

<!-- Response -->
https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Set-Cookie
Set-Cookie:		<cookie-name>=<cookie-value>
Set-Cookie:		<cookie-name>=<cookie-value>; Domain=<domain-value>
Set-Cookie:		<cookie-name>=<cookie-value>; Expires=<date>
Set-Cookie:		<cookie-name>=<cookie-value>; HttpOnly
Set-Cookie:		<cookie-name>=<cookie-value>; Max-Age=<number>
Set-Cookie:		<cookie-name>=<cookie-value>; Partitioned
Set-Cookie:		<cookie-name>=<cookie-value>; Path=<path-value>
Set-Cookie:		<cookie-name>=<cookie-value>; Secure

Set-Cookie:		<cookie-name>=<cookie-value>; SameSite=Strict
Set-Cookie:		<cookie-name>=<cookie-value>; SameSite=Lax
Set-Cookie:		<cookie-name>=<cookie-value>; SameSite=None; Secure

<!-- Multiple attributes are also possible, for example: -->
Set-Cookie:		<cookie-name>=<cookie-value>; Domain=<domain-value>; Secure; HttpOnly


Set-Cookie:		sessionId=abc123; Expires=Wed, 09 Jun 2021 10:18:14 GMT