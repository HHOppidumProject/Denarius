#ifndef DENARIUS_PARSE_H
#define DENARIUS_PARSE_H

#include <string>
#include <map>
#include <algorithm>
#include <vector>

namespace denarius {
	namespace network {
		namespace parse {
			std::map<std::string, std::string> urlparse(std::string url);
			std::vector<std::string> splitParams(std::string url);
			std::vector<std::string> spitnetloc(std::string url, int start);
			std::map<std::string, std::string> urlsplit(std::string url, std::string scheme = "", bool allow_fragments = true);

		};
	}
}

#endif // !DENARIUS_PARSE_H