#include "node.h"

nlohmann::json denarius::node::JSONEncode()
{
	nlohmann::json nodeJSON;
	nodeJSON["netloc"] = this->netloc_;
	nodeJSON["port_"] = this->port_;
	return nlohmann::json();
}

std::string denarius::node::operator[](std::string key)
{
	if (key == "netloc") {
		return this->netloc_;
	}
	else if (key == "port") {
		return this->port_;
	}
	else {
		throw std::invalid_argument("Invalid key. Denarius Node only accepts \"netloc\" or \"port\" as keys.");
	}
}

bool denarius::operator==(const node& lhs, const node& rhs) noexcept
{
	if ((lhs.port_ == rhs.port_) && (lhs.netloc_ == rhs.netloc_)) {
		return true;
	}
	else {
		return false;
	}
}
