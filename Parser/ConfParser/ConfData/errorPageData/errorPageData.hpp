#pragma once

#include <string>

namespace E_ERRORPAGE {
	enum E_ERRORPAGE {
		DEFAULT = 0,
		LOCATION = 1,
		REPLACE = 2,
	};
}

namespace CONF {
	struct errorPageData {
		unsigned char	m_Type;
		unsigned short	m_Replace;	
		std::string		m_Path;
	};
}