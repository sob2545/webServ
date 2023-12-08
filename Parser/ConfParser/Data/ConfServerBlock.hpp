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
	class ServerBlock : public AConfParser {
	private: 
		bool									m_Autoindex;
		unsigned short							m_Port;
		unsigned short							m_Status;
		std::string								m_Root;
		strVec									m_Server_name;
		std::string								m_Error_page;
		std::string								m_Access_log;
		strVec									m_Index;
		std::map<std::string, LocationBlock>	m_Location_block;

	private:
		ServerBlock(const ServerBlock& other);
		ServerBlock& operator=(const ServerBlock& other);

		void					initHTTPStatusMap();

		const bool				context();
		const bool				blockContent();
		const unsigned short	directiveNameChecker(const std::string& name);

		const std::string		argument(const unsigned short& status);
		const bool				argumentChecker(const std::vector<std::string>& args, const unsigned short& status);
	
	public:
		ServerBlock();
		virtual ~ServerBlock();

		void	initialize();

		const std::string&	getServerName() const;

	};
}