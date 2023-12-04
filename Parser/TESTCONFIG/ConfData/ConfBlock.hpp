#pragma once

#include "../ConfParser/Exception/ConfParserException.hpp"
#include "ConfMainBlock.hpp"
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
	class ConfBlock {
	/**
	 * @brief	Singleton Instance
	 */
	public:
		static ConfBlock*		instance;
		static void				initInstance(const std::string& file);
		static ConfBlock*		getInstance();

	private:
		static CONF::MainBlock		m_MainBlock;

		ConfBlock(const std::string& file);
		ConfBlock(const ConfBlock& other);
		ConfBlock&	operator=(const ConfBlock& other);
		virtual ~ConfBlock();

	/**
	 * @brief	public function
	 */
	public:
		// TODO: delete debug function
		void	print();

		const MainBlock&	getHttpBlock() const;
	};
}
