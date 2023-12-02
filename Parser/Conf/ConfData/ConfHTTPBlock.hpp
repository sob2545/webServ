#pragma once

#include "ConfServerBlock.hpp"
#include <map>
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
		bool							autoindex;
		std::string						root;
		std::string						default_type;
		std::string						error_page;
		std::string						access_log;
		strVec							index;
		std::map<std::string, serverBlock>	server_block;
	};

}
