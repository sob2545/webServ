#pragma once

#include "../../utils/utilFunctions.hpp"
#include "../../utils/utilABNFFunctions.hpp"
#include "ConfEventBlock.hpp"
#include "ConfHTTPBlock.hpp"
#include <map>
#include <vector>

namespace   CONF {
	class mainBlock {
	public:
		typedef std::map<std::string, std::string>	envMap;

		static mainBlock*		instance;
		static mainBlock*		initInstance(const std::string& file);
		static mainBlock*		getInstance();

	private:
		bool					m_Daemon;
		unsigned int			m_Worker_process;
		unsigned long			m_Time_resolution;
		std::string				m_Error_log;
		envMap					m_Env;
		eventsBlock				m_Event_block;
		static httpBlock		m_Http_block;
		
		mainBlock(const mainBlock& other);
		mainBlock&	operator=(const mainBlock& other);
		mainBlock(const std::string& file);
		~mainBlock();

	public:
		static const httpBlock&	getHttpBlock();
		const bool				isDaemonOn();
		const unsigned int&		getWorkerProcess();
		const unsigned long& 	getTimeResolution();
		const std::string&		getErrorLog();
		const envMap&			getEnvMap();
		const std::string&		getEnv(const std::string& key);
		const unsigned int&		getWorkerConnections();

	private:
		bool	blockContent(const std::string& file, size_t& pos);
		bool	simpleContent(const std::string& file, size_t& pos);
		bool	context(const std::string& file, size_t& pos);
		void	contextLines(const std::string& file, size_t& pos);
	};
}
