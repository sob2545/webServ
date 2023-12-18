#pragma once

#include "ConfEventBlock.hpp"
#include "ConfHTTPBlock.hpp"
#include "ConfLocationBlock.hpp"
#include <map>
#include <string>
#include <vector>


/**
 * @brief	Main Block Status
 * @details unsigned char : 1 byte
 *  
 *  0b					1 = env
 *  0b				   10 = worker_process
 *  0b				  100 = daemon
 *  0b				 1000 = timer_resolution
 *  0b			   1 0000 = error_log
 *  0b            10 0000 = http
 *  0b           100 0000 = events
 */

namespace   CONF {
	class MainBlock : public AConfParser {
	private:
		typedef std::map<std::string, std::string>		envMap;
		typedef std::map<std::string, unsigned char>	statusMap;
		typedef std::vector<std::string>				strVec;

		bool					m_BlockSwitch; // true Event, false HTTP
		bool					m_Daemon;
		unsigned char			m_Status;
		unsigned int			m_Worker_process;
		unsigned long			m_Timer_resolution;
		std::string				m_Error_log;
		envMap					m_Env;
		EventsBlock				m_Event_block;
		static HTTPBlock		m_HTTP_block;
		static statusMap		m_MainStatusMap;

	private:
		MainBlock(const MainBlock& other);
		MainBlock& operator=(const MainBlock& other);

		static void				initMainStatusMap();
		const std::string		envArgumentParser(std::string& argument);

		bool					context();
		bool					blockContent();
		unsigned short			directiveNameChecker(const std::string& name);

		const std::string		argument(const unsigned short& status);
		bool					argumentChecker(const std::vector<std::string>& args, const unsigned short& status);

	public:
		MainBlock();
		virtual ~MainBlock();

		void					initialize();

		const bool&				isDaemonOn() const;
		const unsigned int&		getWorkerProcess() const;
		const unsigned long& 	getTimeResolution() const;
		const std::string&		getErrorLog() const;
		const std::string&		getEnv(const std::string& key) const;
		const envMap&			getEnvMap() const;
		const unsigned int&		getWorkerConnections() const;
		const CONF::HTTPBlock&	getHTTPBlock() const;
		envMap&					setEnvMap();
	};
}
