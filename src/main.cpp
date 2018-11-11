#include "http.hpp"
#include "show_wiki_page.hpp"

#include <toolbox/argv/parser.hpp>
#include <json/json.hpp>
#include <fmt/printf.h>
#include <sstream>
#include <algorithm>

using namespace std::string_literals;

void do_search(const std::string &query)
{
    const auto url = "https://en.wikipedia.org/w/api.php?action=query&list=search&utf8=&format=json&srsearch="s + query;

    std::string response = get_page(url);

    using json = nlohmann::json;

    auto j = json::parse(response);
    auto result = j["query"]["search"];
    for (const auto &e : result) {
        fmt::print("[{:^10}] {}\n", e["pageid"].get<unsigned int>(), e["title"].get<std::string>());
    }
}

void do_show_pageid(unsigned int id)
{
    const auto url = "https://en.wikipedia.org/w/api.php?action=parse&format=json&prop=wikitext&pageid="s;
    std::stringstream ss;
    ss << url << id;

    std::string response = get_page(ss.str());

    using json = nlohmann::json;
    auto j = json::parse(response);
    auto text = j["parse"]["wikitext"]["*"];

    show_wiki_page(text);
}

int main(int argc, char **argv)
{
    namespace o = toolbox::argv::options;

    auto search = o::option<std::string>(o::short_name('s'), o::long_name("search")).description("Search in wikipedia");
    auto pageid = o::option<unsigned int>(o::short_name('p'), o::long_name("pageid")).description("Show page by its id");

    auto parser = toolbox::argv::make_parser(search, pageid);
    parser.parse(argc, argv);

    if (search.found()) {
        do_search(search.value());
    } else if (pageid.found()) {
        do_show_pageid(pageid.value());
    }

    return 0;
}