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
	struct httpBlock{
		bool								m_Autoindex;
		unsigned short						m_Status;
		strVec								m_Root;
		std::string							m_Default_type;
		std::string							m_Error_page;
		std::string							m_Access_log;
		strVec								m_Index;
		std::map<std::string, std::string>	m_Mime_types;
		std::map<std::string, serverBlock>	m_Server_block;
	};

}
