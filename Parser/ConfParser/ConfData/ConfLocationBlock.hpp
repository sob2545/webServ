#pragma once

#include "../../../Trie/Trie.hpp"
#include "../AConfParser/AConfParser.hpp"
#include <string>

	/**
	 * @brief	Location Block Status
	 * @details unsigned char : 1 byte
	 *
	 *  0b					 1 = root
	 *  0b				    10 = index
	 *  0b				   100 = autoindex
	 *  0b				  1000 = error_page
	 *  0b			    1 0000 = access_log
	 *  0b             10 0000 = cgi
	 *	0b 1000 0000 0000 0000 = location
	*/

namespace   CONF {

	class LocationBlock : public AConfParser {
	private:
		typedef	std::vector<std::string>				StrVec_t;
		typedef std::map<std::string, LocationBlock>	LocationMap_t;
		typedef std::map<std::string, unsigned short> 	StatusMap_t;
		typedef std::map<unsigned short, errorPageData> ErrorPageMap_t;

		bool							m_Autoindex;
		unsigned short					m_Status;
		std::string						m_Root;
		ErrorPageMap_t					m_Error_page;
		std::string						m_Access_log;
		Trie							m_Index;
		std::string						m_LocationName;
		std::string						m_Cgi;
		LocationMap_t					m_LocationBlock;
		static StatusMap_t				m_LocationStatusMap;
	
	private:
		LocationBlock& operator=(const LocationBlock& other);

		static void			initLocationStatusMap();

		bool				context();
		bool				blockContent();
		unsigned short		directiveNameChecker(const std::string& name);

		const std::string	argument(const unsigned short& status);
		bool				argumentChecker(const std::vector<std::string>& args, const unsigned short& status);
	
	public:
		LocationBlock();
		LocationBlock(const LocationBlock& other);
		LocationBlock(const bool& autoIndex, const std::string& root, const std::string& accessLog, const ErrorPageMap_t& errorPage, const Trie& index);
		virtual ~LocationBlock();

		void	initialize();

		const LocationBlock&			operator[](const std::string& locationName) const;

		const std::string&				getRoot() const;
		const std::string&				getCgi() const;
		const std::string				getIndex(const std::string& uri) const;
		const bool&						getAutoindex() const;
		const ErrorPageMap_t&			getError_page() const;
		const std::string&				getAccess_log() const;
		const LocationMap_t&			getLocationBlock() const;

	};
}
