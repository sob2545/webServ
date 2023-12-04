#pragma once

#include "../../../utils/utilFunctions.hpp"
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
 *	0b     		   10 0000 = internal
 *	0b 1000 0000 0000 0000 = location
 */

namespace   CONF {
	typedef	std::vector<std::string>	strVec;

	struct locationBlock {
		bool							m_Autoindex;
		bool							m_Internal;
		unsigned short					m_LocationStatus;
		std::string						m_Root;
		std::string						m_Error_page;
		std::string						m_Access_log;
		strVec							m_Index;
	};
}
