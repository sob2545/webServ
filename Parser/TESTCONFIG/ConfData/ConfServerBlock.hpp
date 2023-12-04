#pragma once

#include "../../../utils/utilFunctions.hpp"
#include "ConfLocationBlock.hpp"
#include <map>
#include <vector>

/**
 * @brief	Server Block Status
 * @details unsigned short : 2 byte
 *  
 *	0b			 		 1 = root
 *	0b  		 		10 = index
 *	0b         		   100 = autoindex
 *	0b         		  1000 = error_page
 *	0b       		1 0000 = access_log
 *	0b     		   10 0000 = keepalive_timeout
 *	0b     		  100 0000 = listen
 *	0b  	     1000 0000 = server_name
 *	0b 1000 0000 0000 0000 = location
 */

namespace   CONF {
	class serverBlock {
	private: 
		bool									m_Autoindex;
		unsigned short							m_Port;
		unsigned short							m_Status;
		std::string								m_Root;
		strVec									m_Server_name;
		std::string								m_Error_page;
		std::string								m_Access_log;
		strVec									m_Index;
		std::map<std::string, locationBlock>	m_Location_block;


	};
}