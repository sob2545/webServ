#pragma once

#include "ConfLocationBlock.hpp"
#include "ConfServerBlock.hpp"
#include <map>
#include <string>
#include <vector>

/**
 * @brief	HTTP Block Status
 * @details unsigned short : 2 byte
 *  
 *	0b		  	 		 1 = root
 *	0b		     		10 = index
 *	0b		 		   100 = autoindex
 *	0b		 		  1000 = error_page
 *	0b	    		1 0000 = access_log 
 *	0b			   10 0000 = keepalive_timeout
 *	0b	  		  100 0000 = include
 *	0b	 		 1000 0000 = default_type
 * 	0b 1000 0000 0000 0000 = server
 */


namespace   CONF {
	class httpBlock{
		
		bool								m_Autoindex;
		unsigned short						m_Status;
		std::string							m_Default_type;
		strVec								m_Root;
		strVec								m_Access_log;
		// DISCUSSION HTTP message에서 uri 뒤에 path로 서버를 찾아야 되는데,
		// 이미 path를 분리하여 string vector로 저장해 놨음.
		// 아마 map으로 전체적인 path URI를 저장하고, value 값으로 string vector를 저장하는게 좋을듯.
		strVec								m_Index;
		strVec								m_Include;
		std::map<unsigned int, strVec>		m_Error_page;
		std::map<std::string, std::string>	m_Mime_types;
		std::map<std::string, serverBlock>	m_Server_block;
	};

}
