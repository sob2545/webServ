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
	namespace   E_SERVER_BLOCK_STATUS {
		enum E_SERVER_BLOCK_STATUS {
            ROOT					= 0b00000001,
			INDEX					= 0b00000010,
            AUTOINDEX				= 0b00000100,
            ERROR_PAGE				= 0b00001000,
            ACCESS_LOG				= 0b00010000,
            KEEPALIVE_TIMEOUT		= 0b00100000,
			LISTEN					= 0b01000000,
			SERVER_NAME				= 0b10000000,
			LOCATION				= 0b1000000000000000
		};
	}
	
	class serverBlock {
	private: 
		bool									m_Autoindex;
		unsigned short							m_Port;
		unsigned short							m_ServerStatus;
		std::string								m_Root;
		strVec									m_Server_name;
		std::string								m_Error_page;
		std::string								m_Access_log;
		strVec									m_Index;
		std::map<std::string, locationBlock>	m_Location_block;

		serverBlock(const serverBlock& other);

	public:
		serverBlock(const std::string& file, size_t pos);
		~serverBlock();

		serverBlock& 			operator=(const serverBlock& other);
		const locationBlock&	operator[](const std::string& key);
		const bool				isAutoIndex() const;
		const unsigned short&	getPort() const;
		const strVec&			getServerNames() const;
		const strVec&			getIndexes() const;
		const bool				isValidServerName(const std::string& key) const;
		const bool				isValidIndex(const std::string& key) const;
		const std::string&		getErrorPage() const;
		const std::string&		getAccessLog() const;
	};
}
