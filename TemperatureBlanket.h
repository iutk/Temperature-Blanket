#ifndef TEMPERATUREBLANKET_H
#define TEMPERATUREBLANKET_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <curl/curl.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

struct TempRange {
    int minTemp;
    int maxTemp;
    std::string color;
};

class TemperatureBlanket {
public:
    TemperatureBlanket();

    void showMenu();

private:
    std::vector<TempRange> tempRanges;
    std::string location;
    bool isTempRangeFilled;
    bool isLocationFilled;

    void fillTempRanges();
    void fillLocation();
    void getWeatherBlanket();
    std::string getColorForTemperature(double temperature) const;
    static std::string addDays(const std::string &date, int days);
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s);
};

#endif // TEMPERATUREBLANKET_H
