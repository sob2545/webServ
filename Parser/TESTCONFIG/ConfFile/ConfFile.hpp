#pragma once

#include "../../../FileDescriptor/File/ReadFile.hpp"
#include <cstddef>

namespace E_INDEX {
	enum E_INDEX {
		FILE = 0,
		LINE,
		COLUMN
	};
}

namespace CONF {
	class ConfFile : public ReadFile {
	private:
		size_t				m_Pos[3];
		static ConfFile*	instance;

		ConfFile(const ConfFile& other);
		ConfFile& operator=(const ConfFile& other);

	public:
		static void			InitInstance(const std::string& fileName);
		static ConfFile*	getInstance();

		ConfFile(const std::string& fileName);
		~ConfFile();

		void	Destroy();
		size_t*	Pos();
	};
}