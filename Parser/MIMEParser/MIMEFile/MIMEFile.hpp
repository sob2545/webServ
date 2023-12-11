#pragma once

#include "../../../FileDescriptor/File/ReadFile.hpp"

namespace E_INDEX {
	enum E_INDEX {
		FILE = 0,
		LINE,
		COLUMN
	};
}

namespace MIME {
	class MIMEFile : public ReadFile {
	private:
		std::size_t				m_Pos[3];
		static MIMEFile*	instance;

		MIMEFile(const MIMEFile& other);
		MIMEFile& operator=(const MIMEFile& other);

	public:
		static void			InitInstance(const std::string& fileName);
		static MIMEFile*	getInstance();

		MIMEFile(const std::string& fileName);
		virtual ~MIMEFile();

		void	Destroy();
		std::size_t*	Pos();
	};
}