#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <curl/curl.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <ctime>

struct TempRange {
    int minTemp;
    int maxTemp;
    std::string color;
};

std::vector<TempRange> tempRanges;
std::string location;
bool isTempRangeFilled = false;
bool isLocationFilled = false;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    } catch(std::bad_alloc &e) {
        return 0;
    }
    return newLength;
}

// Function to add days to a date
std::string addDays(const std::string &date, int days) {
    std::tm t = {};
    std::stringstream ss(date);
    int year, month, day;
    char dash1, dash2;

    if (!(ss >> year >> dash1 >> month >> dash2 >> day) || dash1 != '-' || dash2 != '-') {
        throw std::runtime_error("Failed to parse date: " + date);
    }

    // Setting up the tm structure
    t.tm_year = year - 1900; // tm_year is years since 1900
    t.tm_mon = month - 1;    // tm_mon is 0-based
    t.tm_mday = day;
    t.tm_isdst = -1;         // Not setting Daylight Saving Time

    // Normalize the tm structure
    time_t tt = std::mktime(&t);
    tt += days * 24 * 3600; // Add days in seconds

    // Convert back to tm structure
    std::tm *timePtr = std::localtime(&tt);

    // Create a string from tm structure
    std::ostringstream oss;
    oss << (timePtr->tm_year + 1900) << '-'
        << std::setfill('0') << std::setw(2) << (timePtr->tm_mon + 1) << '-'
        << std::setfill('0') << std::setw(2) << timePtr->tm_mday;

    return oss.str();
}

std::string getColorForTemperature(double temperature) {
    for (const auto& range : tempRanges) {
        if (temperature >= range.minTemp && temperature <= range.maxTemp) {
            return range.color;
        }
    }
    return "Unknown"; // Default color if no range matches
}

void fillTempRanges() {
    std::cout << "Enter color codes for the following temperature ranges:" << std::endl;
    std::vector<std::pair<int, int>> tempPairs = {{-10, -6}, {-5, -1}, {0, 4}, {5, 9}, {10, 14}, {15, 19}, {20, 24}, {25, 29}, {30, 34}, {35, 40}};

    for (auto& pair : tempPairs) {
        TempRange range;
        range.minTemp = pair.first;
        range.maxTemp = pair.second;
        std::cout << pair.first << "/" << pair.second << ": ";
        std::cin >> range.color;
        tempRanges.push_back(range);
    }
    isTempRangeFilled = true;
}

void fillLocation() {
    std::cout << "Enter location: ";
    std::cin >> location;
    isLocationFilled = true;
}

void getWeatherBlanket() {
    if (!isTempRangeFilled || !isLocationFilled) {
        std::cout << "Please complete the necessary steps first." << std::endl;
        return;
    }

    std::string startDate, endDate;
    std::cout << "Enter start date (YYYY-MM-DD): ";
    std::cin >> startDate;
    std::cout << "Enter end date (YYYY-MM-DD): ";
    std::cin >> endDate;

    std::ofstream outFile("weather_blanket.txt");

    for (std::string date = startDate; date <= endDate; date = addDays(date, 1)) {
        std::string readBuffer;
        CURL *curl = curl_easy_init();
        if (curl) {
            std::string url = "https://api.worldweatheronline.com/premium/v1/past-weather.ashx?key=037094f33a004d78b51200720232512&q=" + location + "&date=" + date;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if (res == CURLE_OK) {
                boost::property_tree::ptree pt;
                std::istringstream iss(readBuffer);
                boost::property_tree::read_xml(iss, pt);
                double maxTempC = std::stod(pt.get<std::string>("data.weather.maxtempC", "0"));
                double minTempC = std::stod(pt.get<std::string>("data.weather.mintempC", "0"));

                double avgTempC = (maxTempC + minTempC) / 2.0;
                std::string color = getColorForTemperature(avgTempC);

                if (outFile.is_open()) {
                    outFile << date << ": Color - " << color << std::endl;
                }
            } else {
                std::cerr << "CURL Error for date " << date << ": " << curl_easy_strerror(res) << std::endl;
            }
        }
    }

    if (outFile.is_open()) {
        outFile.close();
        std::cout << "Weather blanket data written to weather_blanket.txt" << std::endl;
    }
}

void showMenu() {
    while (true) {
        std::cout << "Welcome to Temperature Blanket App." << std::endl;
        std::cout << "Please choose an option:" << std::endl;
        std::cout << "1) Fill your color-date range map." << std::endl;
        std::cout << "2) Fill your location." << std::endl;
        std::cout << "3) Get your daily weather blanket." << std::endl;
        std::cout << "4) Exit." << std::endl;

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                fillTempRanges();
                break;
            case 2:
                fillLocation();
                break;
            case 3:
                getWeatherBlanket();
                break;
            case 4:
                std::cout << "Exiting the program." << std::endl;
                return;
            default:
                std::cout << "Invalid choice, please try again." << std::endl;
                break;
        }
    }
}

int main() {
    showMenu();
    return 0;
}
