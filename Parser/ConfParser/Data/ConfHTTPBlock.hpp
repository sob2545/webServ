#pragma once

#include "../Parser/AConfParser.hpp"
#include "../../../Trie/Trie.hpp"
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

// TODO: root, access_log, index, include 각각이 abs/rel 둘 중 어떤 것이 되는지 알아볼 것

namespace   CONF {
	class HTTPBlock : public AConfParser {
	private:
		typedef std::map<std::string, unsigned short>	statusMap;

		bool								m_Autoindex;
		unsigned short						m_Status;
		std::string							m_Default_type;
		std::string							m_Root;
		std::string							m_Access_log;
		std::string							m_Include;
		std::map<unsigned int, std::string>	m_Error_page;
		std::map<std::string, std::string>	m_Mime_types;
		std::map<std::string, ServerBlock>	m_Server_block;
		statusMap							m_HTTPStatusMap;
		Trie								m_Index;

	private:
		HTTPBlock(const HTTPBlock& other);
		HTTPBlock& operator=(const HTTPBlock& other);

		void					initHTTPStatusMap();

		const bool				context();
		const bool				blockContent();
		const unsigned short	directiveNameChecker(const std::string& name);

		const std::string		argument(const unsigned short& status);
		const bool				argumentChecker(const std::vector<std::string>& args, const unsigned short& status);
	
	public:
		HTTPBlock();
		virtual ~HTTPBlock();

		void					initialize();

	};
}
