#pragma once

#include "../../utils/utilFunctions.hpp"
#include <map>
#include <vector>

namespace   CONF {
	typedef	std::vector<std::string>	strVec;

	struct locationBlock {
		bool							autoindex;
		bool							internal;
		std::string						root;
		std::string						error_page;
		std::string						access_log;
		strVec							index;
	};
}
