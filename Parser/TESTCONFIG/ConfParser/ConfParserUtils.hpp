#pragma once

namespace   CONF {
	/**
	 * @brief	Config Block Status
	 * @details unsigned char : 1 byte
	 *
	 *  0b					1 = main
	 *  0b				   10 = event
	 *  0b				  100 = http
	 *  0b				 1000 = server
	 *  0b			   1 0000 = location
	*/
	namespace   E_BLOCK_STATUS {
		enum E_BLOCK_STATUS {
			MAIN			= 0b00000001,
			EVENT			= 0b00000010,
			HTTP			= 0b00000100,
			SERVER			= 0b00001000,
			LOCATION		= 0b00010000,
		};
	}


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

	namespace	E_EVENT_BLOCK_STATUS {
		enum E_EVENT_BLOCK_STATUS {
			WORKER_CONNECTIONS	= 0b01
		};
	}

	/**
	* @brief	HTTP Block Status
	* @details unsigned short : 2 byte
	*  
	*	0b		  	 		 1 = root
	*	0b		     		10 = index
	*	0b		 		   100 = autoindex
	*	0b		 		  1000 = error_page
	*	0b	    		1 0000 = access_log 
	*	0b			   10 0000 = keepalive_timeout
	*	0b	  		  100 0000 = include
	*	0b	 		 1000 0000 = default_type
	* 	0b 1000 0000 0000 0000 = server
	*/
	namespace   E_HTTP_BLOCK_STATUS {
		enum E_HTTP_BLOCK_STATUS {
            ROOT					= 0b00000001,
			INDEX					= 0b00000010,
            AUTOINDEX				= 0b00000100,
            ERROR_PAGE				= 0b00001000,
            ACCESS_LOG				= 0b00010000,
            KEEPALIVE_TIMEOUT		= 0b00100000,
			INCLUDE					= 0b01000000,
			DEFAULT_TYPE			= 0b10000000,
			SERVER					= 0b1000000000000000
		};
	}

	/**
	* @brief	Server Block Status
	* @details unsigned short : 2 byte
	*  
	*	0b			 		 1 = root
	*	0b  		 		10 = index
	*	0b         		   100 = autoindex
	*	0b         		  1000 = error_page
	*	0b       		1 0000 = access_log
	*	0b     		   10 0000 = keepalive_timeout
	*	0b     		  100 0000 = listen
	*	0b  	     1000 0000 = server_name
	*	0b 1000 0000 0000 0000 = location
	*/

	namespace   E_SERVER_BLOCK_STATUS {
		enum E_SERVER_BLOCK_STATUS {
            ROOT					= 0b00000001,
			INDEX					= 0b00000010,
            AUTOINDEX				= 0b00000100,
            ERROR_PAGE				= 0b00001000,
            ACCESS_LOG				= 0b00010000,
            KEEPALIVE_TIMEOUT		= 0b00100000,
			LISTEN					= 0b01000000,
			SERVER_NAME				= 0b10000000,
			LOCATION				= 0b1000000000000000
		};
	}

	/**
	 * @brief	Location Block Status
	 * @details unsigned char : 1 byte
	 *
	 *  0b					 1 = root
	 *  0b				    10 = index
	 *  0b				   100 = autoindex
	 *  0b				  1000 = error_page
	 *  0b			    1 0000 = access_log
	 *  0b             10 0000 = internal
	 *  0b            100 0000 = cgi
	 *	0b 1000 0000 0000 0000 = location

	*/
	namespace	E_LOCATION_BLOCK_STATUS {
		enum E_LOCATION_BLOCK_STATUS {
			ROOT					= 0b00000001,
			INDEX					= 0b00000010,
			AUTOINDEX				= 0b00000100,
			ERROR_PAGE				= 0b00001000,
			ACCESS_PAGE				= 0b00010000,
			INTERNAL				= 0b00100000,
			CGI						= 0b01000000,
			LOCATION				= 0b1000000000000000
		};
	
	}
}