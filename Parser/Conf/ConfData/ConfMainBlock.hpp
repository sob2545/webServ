#pragma once

#include "ConfEventBlock.hpp"
#include "ConfHTTPBlock.hpp"
#include <map>
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
	struct MainBlock {
	/**
	 * @brief	Singleton Instance
	 * 
	 */
	public:
		typedef std::map<std::string, std::string>		envMap;
		typedef std::map<std::string, unsigned char>	statusMap;
		typedef std::vector<std::string>				errorLogLocation;

		bool					m_Daemon;
		unsigned char			m_Status;
		unsigned int			m_Worker_process;
		unsigned long			m_Timer_resolution;
		const std::string		m_FileName;
		errorLogLocation		m_Error_log;
		envMap					m_Env;
		eventsBlock				m_Event_block;
		static httpBlock		m_HTTP_block;
		statusMap				m_StatusMap;
		
	};
}
