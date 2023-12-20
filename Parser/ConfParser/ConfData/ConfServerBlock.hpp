#pragma once

#include "ConfLocationBlock.hpp"
#include <set>
#include <string>

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
 *	0b  100 0000 0000 0000 = listen checker bit
 *	0b 1000 0000 0000 0000 = location
 */

namespace   CONF {
	class ServerBlock : public AConfParser {
	private: 
		typedef std::map<unsigned short, errorPageData> errorPageMap;
		typedef std::map<std::string, unsigned short> 	statusMap;
		typedef std::map<std::string, LocationBlock> 	locationBlockMap;

		bool						m_Autoindex;
		unsigned short				m_Port;
		unsigned short				m_Status;
		unsigned int				m_KeepAliveTime;
		std::string					m_Root;
		errorPageMap				m_Error_page;
		std::string					m_Access_log;
		std::string					m_IP;
		Trie						m_Index;
		std::string					m_LocationName;
		std::set<std::string>		m_Server_name;
		locationBlockMap			m_LocationBlock;
		static statusMap			m_ServerStatusMap;

	private:
		ServerBlock& operator=(const ServerBlock& other);

		static void			initServerStatusMap();

		bool				context();
		bool				blockContent();
		unsigned short		directiveNameChecker(const std::string& name);

		const std::string	argument(const unsigned short& status);
		bool				argumentChecker(const std::vector<std::string>& args, const unsigned short& status);
	
	public:
		ServerBlock();
		ServerBlock(const ServerBlock& other);
		ServerBlock(const bool& autoIndex, const unsigned int& keepAliveTime, const std::string& root, const std::string& accessLog, const errorPageMap& errorPage, const Trie& index);
		virtual ~ServerBlock();

		void	initialize();

		const bool&						getAutoindex() const;
		const unsigned int&				getKeepAliveTime() const;
		const unsigned short&			getPort() const;
		const std::string&				getDefault_type() const;
		const std::string&				getRoot() const;
		const std::string&				getIP() const;
		const std::string&				getAccess_log() const;
		const std::string&				getInclude() const;
		const std::string				getIndex(const std::string& uri) const;
		const errorPageMap&				getError_page() const;
		const std::set<std::string>&	getServerNames() const;
		const locationBlockMap&			getLocationMap() const;
	};
}