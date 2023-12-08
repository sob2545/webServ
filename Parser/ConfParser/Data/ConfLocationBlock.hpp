#pragma once

#include "../../../utils/utilFunctions.hpp"
#include "../Parser/AConfParser.hpp"
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

	class LocationBlock : public AConfParser {
	private:
		typedef	std::vector<std::string>	strVec;

		bool							m_Autoindex;
		bool							m_Internal;
		unsigned short					m_LocationStatus;
		std::string						m_Root;
		std::string						m_Error_page;
		std::string						m_Access_log;
		strVec							m_Index;
	
	private:
		LocationBlock(const LocationBlock& other);
		LocationBlock& operator=(const LocationBlock& other);

		void					initHTTPStatusMap();

		const bool				context();
		const bool				blockContent();
		const unsigned short	directiveNameChecker(const std::string& name);

		const std::string		argument(const unsigned short& status);
		const bool				argumentChecker(const std::vector<std::string>& args, const unsigned short& status);
	
	public:
		LocationBlock();
		virtual ~LocationBlock();

		void	initialize();

	};
}
