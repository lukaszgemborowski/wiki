#ifndef _HTTP_GET_HPP_
#define _HTTP_GET_HPP_

#include <string>
#include <algorithm>

#include <toolbox/curl/easy.hpp>
#include <toolbox/curl/global.hpp>

namespace
{
inline size_t get_page_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    std::string *out = reinterpret_cast<std::string *>(userdata);

    std::copy(ptr, ptr + nmemb, std::back_inserter(*out));
    return nmemb;
}
}

inline std::string get_page(const std::string &url)
{
    auto curl = toolbox::curl::global(toolbox::curl::flags::Default);
    std::string response;

    namespace opt = toolbox::curl::options;
	auto easy = toolbox::curl::make_easy(
		opt::verbose(false),
		opt::url(url),
		opt::writedata(&response),
        opt::writefunction(&get_page_write_callback)
	);

	easy.perform();
    return response;
}

#endif // _HTTP_GET_HPP_