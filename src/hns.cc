/*
 * @file hns.cc
 * @brief Hacker News Stream (HNS) obtains the latest stories from the Hacker News API.
 *
 * @author Simon Rowe <simon@wiremoons.com>
 * @license open-source released under "MIT License"
 * @source https://github.com/wiremoons/hns
 * @date originally created: 08 May 2022
 *
 * @details Program uses the Hacker News (HN) Application Programming Interface (API)
 * to monitor for newly posted Hacker News stories. The API can be found here:
 * https://github.com/HackerNews/API
 *
 */

#include <chrono>
#include <iostream>
#include <locale>
#include <string>
#include <thread>

#include <argparse/argparse.hpp>
#include <cpr/cpr.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

using json = nlohmann::json;

// TODO:
// - [ ] : detect and use system proxy settings
// - [ ] : use co-routines for infinite loop with 2 x workers
// - [ ] : add account info for poster
// - [ ] : track read articles

// APPLICATION EXAMPLE STORY OUTPUT FORMAT:
// Title:      'Safari Technology Preview Release 145'
// HN link:     https://news.ycombinator.com/item?id=31368509
// Story URL:   https://developer.apple.com/safari/technology-preview/release-notes/
// Posted by:  'clarity' account since 23-01-2014. [karma: 7815]
// Posted on:   Fri, 13 May 2022 14:57:42 GMT.
// Exec stats: '2' displayed. '0' omitted . '17' total scanned.

/// Download JSON (or web page body) for the URL provided
/// @brief Download the web site page content for the provided URL.
/// @return The obtained web site page content as a string
std::string getSiteJson(std::string const &full_url)
{
    if (full_url.empty()) {
        return "";
    }

    cpr::Response r = cpr::Get(cpr::Url{full_url});
    spdlog::debug("Requested URL is: '{}'", full_url);
    spdlog::debug("Site returned status code: {}\n", r.status_code);
    spdlog::debug("All headers are:");
    // if at debug level print out the headers
    if (spdlog::should_log(spdlog::level::debug)) {
        for (const std::pair<std::string, std::string> kv : r.header) {
            spdlog::debug("  {} : {}", kv.first, kv.second);
        }
    }
    return r.text;
}

/// Obtain the current 'maxitem' id on Hacker News
/// @brief Every new post to Hacker News is assigned an ID. The 'maxitem'
/// is therefore the last item posted and can be used to manage the apps
/// status for retrieved articles.
/// @return maxitem number from Hacker News or '-1' on error
/// @code https://hacker-news.firebaseio.com/v0/maxitem.json
int getMaxID(std::string const &base_url)
{
    auto const site_data = getSiteJson(base_url + "/maxitem.json");
    if (site_data.empty()) {
        return -1;
    }
    spdlog::debug("Raw JSON max ID is: '{}'", site_data);
    try {
        return std::stoi(site_data);
    } catch (std::exception &e) {
        fmt::print(stderr, "ERROR: failed to convert maxID '{}' due to exception: '{}'\n", site_data, e.what());
        return -1;
    }
}

/// Obtain the data for the provided item ID
/// @brief Obtain the data associated with the item ID
/// @return a JSON string containing the retrieved item data
/// @code https://hacker-news.firebaseio.com/v0/item/31371020.json
std::string getItemByID(std::string const &base_url, int const id)
{
    std::string site_data = getSiteJson(base_url + "/item/" + std::to_string(id) + ".json");
    spdlog::debug("Raw JSON for article '{}' : {}\n", id, site_data);
    if (spdlog::should_log(spdlog::level::debug)) {
        if (site_data == "null") {
            spdlog::debug("** EMPTY ARTICLE DETECTED ** - only contained text is word 'null'");
        }
    }
    // Hacker News API returns the word "null" if no article exists for the 'id' used.
    return (site_data == "null") ? "" : site_data;
}

/// Set global log level for spdlog
/// @brief If the application is compiled with debug enabled then also
/// enable spdlog 'debug level' output
/// @return no return
void setDebugLevel()
{
#ifdef DEBUG
    spdlog::set_level(spdlog::level::debug);
#endif
}

/// Get a string with the current local time
/// @brief Obtain the local time in 'HH:MM:SS'' format using '%T' format option.
/// @return the current time as a string
std::string getCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%T");
    spdlog::debug("current time: {}", ss.str());
    return ss.str();
}

/// Convert the provided Epoch time to a time and data string
/// @brief Convert a provided Epoch time into an equivalent string with
/// the format of: 'Sat 21 May 2022 @ 08:26'
// @return string containing the day, date and time
std::string convertEpochTime(long const epoch_time)
{
    if (epoch_time <= 0) {
        spdlog::debug("WARNING: Epoch <= 0. 'UNKNOWN' returned.");
        return "UNKNOWN";
    }
    std::stringstream ss;
    ss << std::put_time(std::localtime(&epoch_time), "%a %d %b %Y @ %R");
    spdlog::debug("Converted epoch: {}", ss.str());
    return ss.str();
}

/// Obtain the Curl version the application was compiled with
/// @brief Provide the version of the Curl library that the application
/// was compiled with, useful to check in case of security vulnerability checks
/// are required. Example returned string is: '1.2.3'
/// @return Applications Curl library version
std::string getCurlVersion()
{
    curl_version_info_data *cdinfo = curl_version_info(CURLVERSION_NOW);
    return cdinfo->version;
}

/// Obtain the nlohmann_json library version the application was compiled with
/// @brief Provide the version of the nlohmann_json library that the application
/// was compiled with, useful to check in case of security vulnerability checks
/// are required. Example returned string is: '1.2.3'
/// @return Applications nlohmann_json library version
std::string getNlohmannJsonVersion()
{
    std::string const njson_ver =
        fmt::format("{}.{}.{}", NLOHMANN_JSON_VERSION_MAJOR, NLOHMANN_JSON_VERSION_MINOR, NLOHMANN_JSON_VERSION_PATCH);
    spdlog::debug("nlohmann_json library version: {}", njson_ver);
    return njson_ver.empty() ? "UNKNOWN" : njson_ver;
}

/// Obtain the compiler name and version the application was compiled with
/// @brief Provide the version of the C++ compiler that the application
/// was compiled with, useful to check in case of security vulnerability checks
/// are required. Example returned string is: '13.1.6 (clang-1316.0.21.2.5)'
/// @return Applications C++ compiler version
std::string getCompilerVersion()
{
#ifdef __clang__
    return fmt::format("clang {}", __clang_version__);
#elif __GNUC__
    return fmt::format("GNU GCC {}", __VERSION__);
#elif _MSC_VER
    return = fmt::format("Microsoft {}", _MSC_FULL_VER);
#elif __MINGW64__
    return = fmt::format("mingw64 {}", __MINGW64_VERSION_MAJOR);
#else
    return = "UNKNOWN";
#endif
}

/// Obtain the applications build type
/// @brief Provide the type of build the C++ compiler performed for the application.
/// Example returned string is either : "Debug" or "Release".
/// @return Applications C++ build type as either: "Debug" or "Release"
std::string getBuildType()
{
#if DEBUG
    return "Debug";
#else
    return "Release";
#endif
}

/// Create a version string for output by 'argparse' options: '-v' or '--version'
/// @brief Provide the version of the program and any libraries that the application
/// was compiled with, useful to check in case of security vulnerability checks
/// are required.
/// @return Applications version and any library versions
std::string printVersion(std::string const &APP_NAME, std::string const &APP_VERSION)
{
    std::string version_output = fmt::format("\n'{}' version is: '{}'\n", APP_NAME, APP_VERSION);
    version_output.append(fmt::format("Compiled on: '{} @ {}'.\n", __DATE__, __TIME__));
    version_output.append(fmt::format("Copyright (c) 2022-2023 Simon Rowe.\n\n"));
    version_output.append(
        fmt::format("C++ source built as '{}' using compiler '{}'.\n\n", getBuildType(), getCompilerVersion()));
    version_output.append(fmt::format("Included library versions:\n"));
    version_output.append(fmt::format("- cpr version: '{}'\n", CPR_VERSION));
    version_output.append(fmt::format("- Curl library version: '{}'\n", getCurlVersion()));
    version_output.append(fmt::format("- fmt version: '{}'\n", FMT_VERSION));
    version_output.append(fmt::format("- nlohmann_json version: '{}'\n", getNlohmannJsonVersion()));
    version_output.append(fmt::format("- spdlog version: '{}'\n", SPDLOG_VERSION));
    version_output.append(fmt::format("\nFor licenses and further information visit:\n"
                                      "- Hacker News Stream (hns):   https://github.com/wiremoons/hns\n"
                                      "- HackerNews API:             https://github.com/HackerNews/API\n"
                                      "- argparse:                   https://github.com/p-ranav/argparse\n"
                                      "- curl:                       https://github.com/curl/curl\n"
                                      "- Curl for People (cpr):      https://github.com/libcpr/cpr\n"
                                      "- fmt:                        https://github.com/fmtlib/fmt\n"
                                      "- nlohmann_json:              https://github.com/nlohmann/json\n"
                                      "- spdlog:                     https://github.com/gabime/spdlog\n"));
    return version_output;
}

int main(int argc, char *argv[])
{
    // Fetch stories frequency. Every: '120' = 120 seconds (2 minutes)
    constexpr long long SLEEP_TIME{120};
    const std::string APP_VERSION{"0.5.9"};
    // TODO: trim `argv[0]` to be base file name only.
    const std::string APP_NAME = argv[0];

    std::string const base_url = "https://hacker-news.firebaseio.com/v0";

    // Set global spd::log level to 'debug' if appropriate
    setDebugLevel();
    spdlog::debug("This program was built in 'debug' mode.");

    argparse::ArgumentParser parser("hns", printVersion(APP_NAME, APP_VERSION));
    parser.add_description("Hacker News Stream (HNS) obtains the latest stories from the Hacker News API.");

    try {
        parser.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << parser;
        std::exit(1);
    }

    int const start_max_id = getMaxID(base_url);
    int max_id = start_max_id;
    int current_id = start_max_id;
    int stories_skipped{0};
    int stories_found{0};
    spdlog::debug("Hackernews story 'Max ID': '{}'\n", start_max_id);
    if (start_max_id == -1) {
        fmt::print(stderr, fg(fmt::terminal_color::red), "ERROR: failed to obtain HN Max ID. Exit.");
        return EXIT_FAILURE;
    }

    fmt::print(fg(fmt::terminal_color::green), "\nStarting Hacker News Stream (hns)...\n");
    fmt::print("Starting article ID: ");
    fmt::print(fg(fmt::terminal_color::blue), "{}\n", start_max_id);
    fmt::print("Waiting for new HN stories... checking every 2 minutes\n\n");

    bool get_next_article{true};

    while (get_next_article) {
        spdlog::debug("->> Starting infinite loop...\n");
        spdlog::debug("START STATUS:  current {} | max {} | skipped {}\n", current_id, max_id, stories_skipped);

        // ensure it is worth trying to get an article:
        if (current_id > max_id) {
            spdlog::debug("NO ARTICLE AVAILABLE:  current {} is greater than max {}\n", current_id, max_id);
            fmt::print("Last check: {}", getCurrentTime());
            std::cout << std::flush;
            std::this_thread::sleep_for(std::chrono::seconds(SLEEP_TIME));
            fmt::print("{}", "\r                    \r");
            std::cout << std::flush;
            max_id = getMaxID(base_url);
            continue;
        }

        std::string const article = getItemByID(base_url, current_id);
        spdlog::debug("Returned article '{}' is : {}", current_id, article);

        if (article.empty()) {
            // check if any more articles exist
            spdlog::debug("WARNING : No article returned for: {}", current_id);
            if (current_id < max_id) {
                current_id++;
                stories_skipped = ((current_id - start_max_id) - stories_found);
                spdlog::debug("MORE TO FETCH:  current {} | max {} | skipped {}", current_id, max_id, stories_skipped);
                std::cout << std::flush;
                continue;
            }

            fmt::print("Last check: {}", getCurrentTime());
            std::cout << std::flush;
            std::this_thread::sleep_for(std::chrono::seconds(SLEEP_TIME));
            fmt::print("{}", "\r                    \r");
            std::cout << std::flush;
            max_id = getMaxID(base_url);
            continue;
        }

        //  check valid JSON exists
        if (json::accept(article)) {
            spdlog::debug("Parsing JSON article...\n");
            json j = json::parse(article);

            // "type":story **but** deleted":true  or  "dead":true  --> do not include
            if ((j.value("type", "UNKNOWN") == "story") and
                not((j["dead"].is_boolean()) or (j["deleted"].is_boolean()))) {
                spdlog::debug("Parsed JSON complete: VALID ARTICLE\n");
                stories_found++;
                auto story_date = convertEpochTime(j.value("time", 0));

                // construct and display an output string 'story_output' with formatting
                std::string story_output = fmt::format("    Title:      '");
                story_output.append(fmt::format(fg(fmt::terminal_color::blue), "{}", j.value("title", "UNKNOWN")));
                story_output.append(fmt::format("'\n"));
                story_output.append(fmt::format("    NH Link:     "));
                story_output.append(fmt::format(fmt::emphasis::underline, "https://news.ycombinator.com/item?id={}\n",
                                                std::to_string(current_id)));
                story_output.append(fmt::format("    Story URL:   "));
                story_output.append(fmt::format(fmt::emphasis::underline, "{}\n", j.value("url", "UNKNOWN")));
                story_output.append(
                    fmt::format("    Posted by:  '{}' at '{}'\n", j.value("by", "UNKNOWN"), story_date));
                story_output.append(
                    fmt::format(std::locale("en_GB.UTF-8"),
                                "    Stats:      '{:L}' displayed. '{:L}' omitted. '{:L}' total scanned.\n",
                                stories_found, stories_skipped, (current_id - start_max_id)));
                std::cout << story_output << std::endl;
            }
            spdlog::debug("Parsed JSON complete: NO VALID ARTICLE\n");
        }
        stories_skipped = ((current_id - start_max_id) - stories_found);
        current_id++;
        spdlog::debug("Infinite loop restarting...\n");
    }

    return EXIT_SUCCESS;
}