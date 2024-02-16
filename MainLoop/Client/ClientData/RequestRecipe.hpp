#pragma once

#include "../../../Utils/SmartPointer.hpp"
#include <string>
#include <vector>
#include <map>

namespace E_HTTP {
	enum	E_HEADER {
		CONNECTION			= 0b1,
		DATE				= 0b10,
		TRANSFER_ENCODING	= 0b100,
		HOST				= 0b1000,
		CONTENT_LENGTHS		= 0b10000,
		CONTENT_TYPE		= 0b100000,
		COOKIE				= 0b1000000,
	};

	enum	E_METHOD {
		GET					= 0b1,
		POST				= 0b10,
		DELETE				= 0b100,
		PUT					= 0b1000,
		PATCH				= 0b10000,
		OPTIONS				= 0b100000,
	};

	enum	E_DELIMETER {
		COMMA				= ',',
		SEMICOLON			= ';',
		DEFAULT				= ' ',
	};

	enum	E_STATUS {
		BLANK				= 0,
		READ_HEADER			= 0b1,
		READ_BODY			= 0b10,
		WRITE_SUCCESS 		= 0b100,
		WRITE_FAIL	 		= 0b1000,
	};
}

namespace HTTP {
/**
 *		Method ENUM
 *	00000001		GET
 *	00000010		POST
 *	00000100		DELETE
 *	00001000		PUT
 *	00010000		PATCH
 *	00100000		OPTIONS
 *	01000000
*/

/**
 *		HTTP VERSION ENUM
 *	 00000001		0.9
 *	 00000010		1.0
 *	 00000100		1.1
*/

/**
 *				Header ENUM
 *	00000000 00000001		Connection
 *	00000000 00000010		Date
 *	00000000 00000100		Transfer-Encoding // chunked만 처리
 *	00000000 00001000		Host
 *	00000000 00010000		Content-Lengths
 *	00000000 00100000		Content-Type
 *	00000000 01000000		Cookie
 *	00000000 10000000		
 
 *	00000001 00000000		
 *	00000010 00000000		
 *	00000100 00000000		
 *	00001000 00000000		
 *	00010000 00000000		
 *	00100000 00000000		
 *	01000000 00000000		
 *	10000000 00000000	
*/

	struct	RequestData {
		typedef std::map<std::string, std::vector<std::string> > QueryMap_t;

		unsigned short	m_Port;

		std::string		m_URI;
		std::string		m_Path;
		QueryMap_t		m_Query;
	};

	struct	RequestRecipe {
		typedef	std::map<std::string, std::string> CookieMap_t;

		unsigned char					m_Method;
		unsigned char					m_RecipeStatus;
		unsigned char					m_Version;
		unsigned short					m_HTTPStatus;

		// bool			m_Form; // is it URI or Path : origin-form vs absolute-form / authoirty-form

		std::string						m_RequestTarget;

		std::vector<unsigned char>		m_BodyMessage;
		std::map<unsigned short, void*>	m_HeaderMap;
		CookieMap_t						m_Cookie;
		RequestData						m_RequestData;

	};
}
