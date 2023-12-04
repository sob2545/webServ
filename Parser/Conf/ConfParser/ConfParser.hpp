#pragma once

#include "../../../utils/utilABNFFunctions.hpp"
#include "../../../File/File.hpp"
#include "../../URIParser/URIParser.hpp"
#include "../ConfData/ConfMainBlock.hpp"

#include "Exception/ConfParserException.hpp"
#include "ConfParserUtils.hpp"
#include <cstddef>
#include <string>
#include <stack>

namespace	E_CONF {
	enum E_CONF {
		LBRACE = '{',
		RBRACE = '}',
	};
}

namespace   CONF {
	class ConfParser {
	public:
		typedef std::map<std::string, bool>				statusBoolMap;
		typedef std::map<std::string, unsigned short>	statusShortMap;

	private:
		const std::string&	m_FileName;

		std::stack<unsigned char>	m_BlockStack;
		CONF::MainBlock*	m_MainBlock;
		std::string			m_FileContent;
		size_t				m_Pos[3];
		size_t				m_FileSize;
		statusShortMap		m_MainStatusMap;
		statusBoolMap		m_EventStatusMap;
		statusShortMap		m_HTTPstatusMap;
		statusShortMap		m_ServerStatusMap;
		statusShortMap		m_LocationStatusMap;

		ConfParser(const ConfParser& other);
		ConfParser&	operator=(const ConfParser& other);
	
	private:
		void	initStatusMap();
		void	initMainStatusMap();
		void	initEventStatusMap();
		void	initHTTPStatusMap();
		void	initServerStatusMap();
		void	initLocationStatusMap();

	/**
	 * @brief	Config Parsing Functions
	 */
	public:
		ConfParser(const std::string& file);
		~ConfParser();
		void	Parsing(CONF::MainBlock& mainBlock);

	private:
		const bool				mainBlockArgumentChecker(const std::vector<std::string>&args, const unsigned char& status);
		const bool				evnetsBlockArgumentChecker(const std::vector<std::string>&args, const unsigned char& status);
		const bool				httpBlockArgumentChecker(const std::vector<std::string>&args, const unsigned char& status);
		const bool				serverBlockArgumentChecker(const std::vector<std::string>&args, const unsigned char& status);
		const bool				locationBlockArgumentChecker(const std::vector<std::string>&args, const unsigned char& status);

		const bool				argumentChecker(const std::vector<std::string>& args, const unsigned char& status);
		const bool				contextLines();
		const bool				blockContent();
		const bool				context();

		const bool				isMultipleDirective(const unsigned char& block_status, const unsigned short& directive_status);
		const unsigned short	directiveNameChecker(const std::string& name);
		const unsigned short	directiveName();
		const bool				directives();



		// TODO: implement directiveNameChecker functions
		const unsigned short	mainDirectiveNameChecker(const std::string& name);
		const unsigned short	eventDirectiveNameChecker(const std::string& name);
		const unsigned short	httpDirectiveNameChecker(const std::string& name);
		// const unsigned short	serverDirectiveNameChecker(const std::string& name);
		// const unsigned short	locationDirectiveNameChecker(const std::string& name);

		const std::string		mainArgument(const unsigned short& status);
		const std::string		eventArgument(const unsigned short& status);
		const std::string		httpArgument(const unsigned short& status);
		// const std::string		serverArgument(const unsigned short& status);
		// const std::string		locationArgument(const unsigned short& status);

		const std::string		argument(const unsigned char& block_status, const unsigned short& directive_status);

		// Utils
		const bool			fileName(std::string& argument);
		void				absPathArgumentParser(strVec& argument);
		void				digitArgumentParser(std::string& argument);

		void				errorPageArgumentParser(std::string& argument);
		void				indexArgumentParser(std::string& argument);


	/**
	 * @brief	Debug Function
	*/
	public:
		void			print();
	};
}
