#pragma once

#include "../../../utils/utilFunctions.hpp"

namespace   CONF {
	struct eventsBlock {
		unsigned int	worker_connections;

		eventsBlock(const std::string& file, size_t* index);
	};
}
