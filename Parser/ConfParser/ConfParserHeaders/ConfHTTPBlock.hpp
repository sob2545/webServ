#pragma once

#include "../../../utils/utilFunctions.hpp"
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
		namespace   E_HTTP_BLOCK_STATUS {
		enum E_HTTP_BLOCK_STATUS {
            ROOT					= 0b00000001,
			INDEX					= 0b00000010,
            AUTOINDEX				= 0b00000100,
            ERROR_PAGE				= 0b00001000,
            ACCESS_LOG				= 0b00010000,
            KEEPALIVE_TIMEOUT		= 0b00100000,
			INCLUDE					= 0b01000000,
			DEFAULT_TYPE			= 0b10000000,
			SERVER					= 0b1000000000000000
		};
	}

	class httpBlock {
	private:
		bool							autoindex;
		std::string						root;
		std::string						default_type;
		std::string						error_page;
		std::string						access_log;
		strVec							index;
		std::map<std::string, serverBlock>	server_block;

		httpBlock(const httpBlock& other);
		httpBlock&	operator=(const httpBlock& other);

	public:
		httpBlock();
		~httpBlock();

		const serverBlock&	operator[](const std::string& key);
		const bool			isAutoindex() const;
		const std::string&	getRoot() const;
		const std::string&	getDefaultType() const;
		const std::string&	getErrorPage() const;
		const std::string&	getAccessLog() const;
		const strVec&		getIndexes() const;
		const bool			isValidIndex(const std::string& index_page) const;
	};
}
