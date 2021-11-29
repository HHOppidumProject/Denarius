#include "network"
#include "parse.h"

std::string scheme_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-.";
const char unsafe_bytes_to_remove[3] = { '\t', '\r', '\n' };
std::vector<std::string> uses_params = { "", "ftp", "hdl", "prospero", "http", "imap", "https", "shttp", "rtsp", "rtspu", "sip", "sips", "mms", "sftp", "tel" };

std::map<std::string, std::string> denarius::network::parse::urlparse(std::string url) {
	std::map<std::string, std::string> split_result = urlsplit(url);
	std::string params;
	url = split_result["path"];

	if ((std::find(uses_params.begin(), uses_params.end(), split_result.at("scheme")) != uses_params.end()) && (split_result.at("path").find(';') != std::string::npos)) {
		std::vector<std::string> res = splitParams(url);
		res[0] = url;
		res[1] = params;
	}
	else {
		params = "";
	}

	split_result["params"] = params;
	split_result["path"] = url;

	return split_result;
}

std::vector<std::string> denarius::network::parse::splitParams(std::string url) {
	size_t i;
	if (url.find('/') != std::string::npos) {
		i = url.find(';', url.rfind('/'));
		if (i < 0) {
			return { url, "" };
		}
	}
	else {
		i = url.find(';');
	}
	return { url.substr(0, i), url.substr(i + 1, url.length()) };
}

std::vector<std::string> denarius::network::parse::spitnetloc(std::string url, int start) {
	size_t delim = url.length();
	for (char c : "/?#") {
		size_t wdelim = url.find_first_of(c, start) - 1;
		if (wdelim >= 0) {
			delim = std::min(delim, wdelim);
		}
	}
	std::vector<std::string> _return = { url.substr(start, delim-1), url.substr(delim, url.length() - 1) };
	return _return;
}

std::map<std::string, std::string> denarius::network::parse::urlsplit(std::string url, std::string scheme, bool allow_fragments) {
	std::string key[3] = { url, scheme, std::to_string(allow_fragments) };
	std::string netloc = "";
	std::string port = "";
	std::string query = "";
	std::string fragment = "";
	bool didBreak = false;

	if (url.at(url.length() - 1) == '\0') {
		url = std::string(url.begin(), url.end() - 1);
	}
	
	size_t i = url.find(":");
	if (i > 0) {
		for (char c : url.substr(0, i - 1)) {
			if (scheme_chars.find(c) == std::string::npos) {
				didBreak = true;
				break;
			}
		}
		if (!didBreak) {
			scheme = url.substr(0, i);
			url = url.substr(i+1, url.length() - 1);
		}
	}
	didBreak = false;

	for (char b : unsafe_bytes_to_remove) {
		url.erase(std::remove(url.begin(), url.end(), b), url.end());
	}

	if (url.substr(0, 2) == "//") {
		std::vector<std::string> res = spitnetloc(url, 2);
		netloc = res[0];
		url = res[1];
		if (((netloc.find('[') != std::string::npos) && (netloc.find(']') == std::string::npos))
			|| ((netloc.find('[') == std::string::npos) && (netloc.find(']') != std::string::npos))) {
			// Need to throw invalid IPv6 error.
		}
		if (netloc.find(":") != std::string::npos) {
			port = netloc.substr(netloc.find(":") + 1, netloc.length());
			netloc = netloc.substr(0, netloc.find(":"));
		}

	}

	if (allow_fragments && (url.find('#') != std::string::npos)) {
		fragment = url.substr(url.find('#') + 1, url.length() - 1);
		url = url.substr(0, url.find('#'));
	}

	if (url.find('?') != std::string::npos) {
		query = url.substr(url.find('?') + 1, url.length() - 1);
		url = url.substr(0, url.find('?'));
	}

	std::map<std::string, std::string> return_;
	return_["scheme"] = scheme;
	return_["netloc"] = netloc;
	return_["port"] = port;
	return_["path"] = url;
	return_["query"] = query;
	return_["fragment"] = fragment;
	return return_;
}