#ifndef DENARIUS_NODE
#define DENARIUS_NODE

#include <string>

#include <nlohmann/json.hpp>

namespace denarius {
	class node {
	public:
		node(std::string netloc, std::string port) : netloc_(netloc), port_(port) {}
		std::string getNetloc() { return this->netloc_; };
		std::string getPort() { return this->port_; };
		nlohmann::json JSONEncode();

		std::string operator[](std::string key);

		friend bool operator==(const node& lhs, const node& rhs) noexcept;

	private:
		std::string netloc_;
		std::string port_;
	};
}

#endif //!DENARIUS_NODE