#pragma once

#include "../../../utils/utilFunctions.hpp"
#include "../../../utils/utilABNFFunctions.hpp"
#include "../../URIParser/URIParser.hpp"
#include "../IConfParser/IConfParser.hpp"
#include "ConfEventBlock.hpp"
#include "ConfHTTPBlock.hpp"
#include <cstddef>
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
			WORKER_CONNECTIONS	= 0b00100000,
            HTTP_BLOCK			= 0b01000000,
			EVENT_BLOCK			= 0b10000000
		};
	}

	class mainBlock : public IConfParser {
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
		const std::string		m_FileName;
		errorLogLocation		m_Error_log;
		envMap					m_Env;
		eventsBlock				m_Event_block;
		// static httpBlock		m_Http_block;
		statusMap				m_StatusMap;
		
		mainBlock(const mainBlock& other);
		mainBlock&	operator=(const mainBlock& other);
		mainBlock(const std::string& file);
		virtual ~mainBlock();

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
		void				initStatusMap();
		void				argumentChecker(const std::vector<std::string>& args, const unsigned char& status, const size_t* index);
		void				directives(const std::string& file, size_t* index);
		const bool			contextLines(const std::string& file, size_t* index);
		const bool			blockContent(const std::string& file, size_t* index);
		const bool			context(const std::string& file, size_t* index);
		const std::string	argument(const std::string& file, size_t* index, const unsigned char& status);
		const unsigned char	directiveNameChecker(const std::string& name, size_t* index);
		const unsigned char	directiveName(const std::string& file, size_t* index);


	/**
	 * @brief	Debug Function
	 * 
	*/
	public:
		void			print();
	};
}
