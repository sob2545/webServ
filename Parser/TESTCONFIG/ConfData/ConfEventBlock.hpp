#pragma once

#include "../../../utils/utilFunctions.hpp"
#include "../ConfParser/AConfParser.hpp"

namespace   CONF {
	class EventsBlock : public AConfParser {
	private:
		EventsBlock(const EventsBlock& other);
		EventsBlock& operator=(const EventsBlock& other);

		const bool				context();
		const unsigned short	directiveNameChecker(const std::string& name);

		const std::string		argument(const unsigned short& status);
		const bool				argumentChecker(const std::vector<std::string>& args, const unsigned short& status);

	public:
		bool			m_Status;
		unsigned int	m_Worker_connections;

		EventsBlock();
		virtual ~EventsBlock();

		void			initialize();
	};
}
