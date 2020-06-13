#include "../headers/Log.hpp"

#include <fstream>
#include <ctime>

Log::Log(std::string const& fname)
{
    logFName_ = fname;
    sstr_.str("");
}

void Log::setUp(void)
{
    ofile_.open(logFName_, std::fstream::out | std::fstream::trunc);
}

void Log::tearDown(void)
{
    ofile_.close();
}

Log& Log::operator<<(std::string const& msg)
{
    sstr_ << msg;
    return *this;
}

Log& Log::operator<<(const int32_t val)
{
    sstr_ << val;
    return *this;
}

Log& Log::operator<<(const char ch)
{
    sstr_ << ch;
    return *this;
}

Log& Log::operator<<(decltype(std::endl<char, std::char_traits<char>>) ch)
{
    std::time_t now = std::time(NULL);
    std::tm * tm = std::localtime(&now);

    ofile_ << "["<< (1900 + tm->tm_year) << "." << tm->tm_mon << "." << tm->tm_mday << "@";
    ofile_ << tm->tm_hour << ":" << tm->tm_min << ":" << tm->tm_sec << "]";

    ofile_ << sstr_.str() << std::endl;
    sstr_.str("");
    return *this;
}
