#pragma once

#include "../AConfParser/AConfParser.hpp"
#include "../../../Trie/Trie.hpp"
#include "../../MIMEParser/Exception/MIMEParserException.hpp"
#include "ConfServerBlock.hpp"
#include "../../../Utils/SmartPointer.hpp"

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
	public:
		typedef std::pair<std::string, unsigned short>					serverKey;
		typedef std::map<serverKey, ft::shared_ptr<CONF::ServerBlock> >	serverMap;

	private:
		typedef std::map<std::string, unsigned short>				statusMap;
		typedef std::map<std::string, std::vector<std::string> >	TypeMap ;
		typedef std::map<unsigned short, errorPageData>				errorPageMap;

		bool									m_Autoindex;
		unsigned short							m_Status;
		unsigned int							m_KeepAliveTime;
		std::string								m_Default_type;
		std::string								m_Root;
		std::string								m_Access_log;
		std::string								m_Include;
		Trie									m_Index;
		errorPageMap							m_Error_page;
		TypeMap									m_Mime_types;
		serverMap								m_Server_block;
		static statusMap						m_HTTPStatusMap;

	private:
		HTTPBlock(const HTTPBlock& other);
		HTTPBlock& operator=(const HTTPBlock& other);

		static void			initHTTPStatusMap();

		bool				context();
		bool				blockContent();
		unsigned short		directiveNameChecker(const std::string& name);

		const std::string	argument(const unsigned short& status);
		bool				argumentChecker(const std::vector<std::string>& args, const unsigned short& status);
	
	public:
		HTTPBlock();
		const ft::shared_ptr<CONF::ServerBlock>&	operator[](const serverKey& key) const;
		virtual ~HTTPBlock();

		void					initialize();

		const bool&				getAutoindex() const;
		const unsigned int&		getKeepAliveTime() const;
		const std::string&		getDefault_type() const;
		const std::string&		getRoot() const;
		const std::string&		getAccess_log() const;
		const std::string&		getInclude() const;
		const std::string		getIndex(const std::string& uri) const;
		const errorPageMap&		getError_page() const;
		const TypeMap&			getMime_types() const;
		const std::map<std::pair<std::string, unsigned short>, ft::shared_ptr<CONF::ServerBlock> >	getServerMap() const;
	};
}
