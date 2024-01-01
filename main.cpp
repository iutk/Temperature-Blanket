#include <curl/curl.h>
#include <pugixml.hpp>
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
            pugi::xml_document doc;
            pugi::xml_parse_result result = doc.load_string(readBuffer.c_str());

            if (result) {
                // XML parsing and processing goes here
                // For example, navigating to a specific node and printing its content
                for (pugi::xml_node tool = doc.child("Profile").child("Tools").child("Tool"); tool; tool = tool.next_sibling("Tool")) {
                    std::cout << "Tool:";
                    for (pugi::xml_attribute attr = tool.first_attribute(); attr; attr = attr.next_attribute()) {
                        std::cout << " " << attr.name() << "=" << attr.value();
                    }
                    std::cout << std::endl;
                }
            } else {
                std::cerr << "XML Parsing Error: " << result.description() << std::endl;
            }
        } else {
            std::cerr << "CURL Error: " << curl_easy_strerror(res) << std::endl;
        }
    }

    return 0;
}
