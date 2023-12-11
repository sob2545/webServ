#pragma once

#include "../../../FileDescriptor/File/ReadFile.hpp"

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
		std::size_t				m_Pos[3];
		static ConfFile*	instance;

		ConfFile(const ConfFile& other);
		ConfFile& operator=(const ConfFile& other);

	public:
		static void			InitInstance(const std::string& fileName);
		static ConfFile*	getInstance();

		ConfFile(const std::string& fileName);
		virtual ~ConfFile();

		void	Destroy();
		std::size_t*	Pos();
	};
}