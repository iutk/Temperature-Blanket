#include <curl/curl.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>
#include <string>

// Callback function for writing data received by libcurl
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    } catch(std::bad_alloc &e) {
        // Handle memory problem
        return 0;
    }
    return newLength;
}

int main() {
    CURL *curl;
    CURLcode res;
    std::string readBuffer; // Declare the string to hold the response

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.worldweatheronline.com/premium/v1/past-weather.ashx?key=037094f33a004d78b51200720232512&q=Ankara&date=2022-12-20"); // Replace with your API URL
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        // Check for successful response
        if (res == CURLE_OK) {
            boost::property_tree::ptree pt;
            std::istringstream iss(readBuffer);

            try {
                boost::property_tree::read_xml(iss, pt);

                // Debug: Print the received XML data
                std::cout << "Received XML Data:\n" << readBuffer << "\n\n";

                // XML parsing and processing goes here
                // Adjust the path according to your XML structure
                for (const auto& tool : pt.get_child("Profile.Tools")) {
                    std::cout << "Tool:";
                    for (const auto& attr : tool.second) {
                        std::cout << " " << attr.first << "=" << attr.second.data();
                    }
                    std::cout << std::endl;
                }
            } catch (const boost::property_tree::xml_parser_error& e) {
                std::cerr << "XML Parsing Error: " << e.what() << std::endl;
            } catch (const boost::property_tree::ptree_bad_path& e) {
                std::cerr << "Path Error: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "CURL Error: " << curl_easy_strerror(res) << std::endl;
        }
    }

    return 0;
}
