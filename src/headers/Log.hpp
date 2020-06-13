#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <string>
#include <cstdint>
#include <fstream>
#include <sstream>

class Log
{
public:
    Log(std::string const& fname);

    void setUp(void);
    void tearDown(void);

    Log& operator<<(std::string const& msg);
    Log& operator<<(const int32_t val);
    Log& operator<<(const char ch);
    Log& operator<<(decltype(std::endl<char, std::char_traits<char>>) ch);

private:
    std::stringstream sstr_;
    std::fstream ofile_;
    std::string logFName_;
};

#endif 