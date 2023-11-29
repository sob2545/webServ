#pragma once

#include "../../../utils/utilFunctions.hpp"
#include "../../../utils/utilABNFFunctions.hpp"
#include "../../URIParser/URIParser.hpp"
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
	namespace   E_MAIN_BLOCK_STATUS {
		enum E_MAIN_BLOCK_STATUS {
            ENV					= 0b00000001,
			WORKER_PROCESS		= 0b00000010,
            DAEMON				= 0b00000100,
            TIMER_RESOLUTION	= 0b00001000,
            ERROR_LOG			= 0b00010000,
            HTTP_BLOCK			= 0b00100000,
			EVENT_BLOCK			= 0b01000000
		};
	}

	class mainBlock {
	/**
	 * @brief	Singleton Instance
	 * 
	 */
	public:
		typedef std::map<std::string, std::string>		envMap;
		typedef std::map<std::string, unsigned char>	statusMap;
		typedef std::vector<std::string>				errorLogLocation;

		static mainBlock*		instance;
		static mainBlock*		initInstance(const std::string& file);
		static mainBlock*		getInstance();

	private:
		bool					m_Daemon;
		unsigned char			m_Status;
		unsigned int			m_Worker_process;
		unsigned long			m_Time_resolution;
		errorLogLocation		m_Error_log;
		envMap					m_Env;
		eventsBlock				m_Event_block;
		// static httpBlock		m_Http_block;
		statusMap				m_StatusMap;
		
		mainBlock(const mainBlock& other);
		mainBlock&	operator=(const mainBlock& other);
		mainBlock(const std::string& file);
		~mainBlock();

	/**
	 * @brief	public function
	 * 
	 */
	public:
		// static const httpBlock&	getHttpBlock();
		const bool				isDaemonOn();
		const unsigned int&		getWorkerProcess();
		const unsigned long& 	getTimeResolution();
		const errorLogLocation&	getErrorLog();
		const envMap&			getEnvMap();
		const std::string&		getEnv(const std::string& key);
		const unsigned int&		getWorkerConnections();

	/**
	 * @brief	Config Parsing Functions
	 * 
	 */
	private:
		void			initStatusMap();
		void			contextLines(const std::string& file, size_t& pos);
		void			argumentChecker(const std::vector<std::string>& args, const unsigned char& status);
		void			directives(const std::string& file, size_t& pos);
		bool			blockContent(const std::string& file, size_t& pos);
		bool			simpleContent(const std::string& file, size_t& pos);
		bool			context(const std::string& file, size_t& pos);
		std::string		argument(const std::string& file, size_t& pos, const unsigned char& status);
		unsigned char	directiveNameChecker(const std::string& name);
		unsigned char	directiveName(const std::string& file, size_t& pos);

	public:
		void			print();
	};
}
