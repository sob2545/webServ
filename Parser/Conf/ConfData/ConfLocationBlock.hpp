#pragma once

#include "../../../utils/utilFunctions.hpp"
#include <map>
#include <vector>

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
