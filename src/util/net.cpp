#include "net.hpp"

namespace net {
    void request(
	    const RequestBody& body,
	    void (*onsuccess)(emscripten_fetch_t*),
	    void (*onerror)(emscripten_fetch_t*),
	    void (*onprogress)(emscripten_fetch_t*),
	    void (*onreadystatechange)(emscripten_fetch_t*) )
    {
	const std::string_view &url = body.url;
	HTTPMethod method = body.method;

	auto cpy_headers = body.headers;
	cpy_headers.push_back(0);
	const char* const *headers = cpy_headers.data();
	const json &data = body.data;

	emscripten_fetch_attr_t attr;
	emscripten_fetch_attr_init(&attr);
	attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
	attr.userData = body.userData;

	// Set HTTP method
	const char* methodStr = nullptr;
	switch (method) {
	    case HTTPMethod::GET:
		methodStr = "GET";
		break;
	    case HTTPMethod::POST:
		methodStr = "POST";
		break;
	    default:
		throw std::invalid_argument("Unsupported HTTP method");
	}
	strncpy(attr.requestMethod, methodStr, sizeof(attr.requestMethod) - 1);

	// Set callbacks
	attr.onsuccess = onsuccess ? onsuccess : [](emscripten_fetch_t* fetch) {
	    printf("Request succeeded: %s\n", fetch->data);
	    emscripten_fetch_close(fetch);
	};
	attr.onerror = onerror ? onerror : [](emscripten_fetch_t* fetch) {
	    printf("Request failed: %s\n", fetch->url);
	    emscripten_fetch_close(fetch);
	};
	attr.onprogress = onprogress;
	attr.onreadystatechange = onreadystatechange;

	// Set headers
	attr.requestHeaders = headers;

	// Set request data
	std::string data_str = data.dump();
	attr.requestData = data_str.empty() ? nullptr : data_str.c_str();
	attr.requestDataSize = data_str.size();

	// Execute the request
	emscripten_fetch(&attr, url.data());
    }

}
