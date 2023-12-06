#pragma once

#include "ConfMainBlock.hpp"

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
	class ConfBlock {
	/**
	 * @brief	Singleton Instance
	 */
	public:
		static ConfBlock*		instance;
		static void				initInstance(const std::string& file, char** env);
		static ConfBlock*		getInstance();

	private:
		static CONF::MainBlock		m_MainBlock;
		static char**				m_ShellEnv;

		ConfBlock(const std::string& file, char** env);
		ConfBlock(const ConfBlock& other);
		ConfBlock&	operator=(const ConfBlock& other);
		virtual ~ConfBlock();

	/**
	 * @brief	public function
	 */
	public:
		// TODO: delete debug function
		void	print();

		const MainBlock&	getMainBlock() const;
		char**				getShellEnv() const;
	};
}
