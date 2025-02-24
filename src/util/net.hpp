#pragma once

#include <pch.h>

using json = nlohmann::json;

namespace net
{

	enum class HTTPMethod
	{
		GET,
		POST,
	};

	struct FetchedData
	{
		std::string data;
		std::string url;
		std::string status;

		float progress; // Value between 0-1
	};

	struct RequestBody
	{
		std::string url;
		HTTPMethod method;
		json data;
		std::vector<const char *> headers;
		void *userData;
	};

	void request(const RequestBody &body,
				 void (*onsuccess)(emscripten_fetch_t *) = nullptr,
				 void (*onerror)(emscripten_fetch_t *) = nullptr,
				 void (*onprogress)(emscripten_fetch_t *) = nullptr,
				 void (*onreadystatechange)(emscripten_fetch_t *) = nullptr);

}
