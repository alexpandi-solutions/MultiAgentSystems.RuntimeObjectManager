#include "../headers/Student.hpp"

#include <string>
#include <cstdint>
#include <vector>

template<> const ClassName Mo_Student::className_ = "STUDENT";

Student::Student()
{
    name_ = "";
    forename_ = "";
    year_ = 0;
}

std::string const& Student::getName(void) const
{
    return name_;
}

std::string const& Student::getForename(void) const
{
    return forename_;
}

int32_t Student::getYear(void) const
{
    return year_;
}

void Student::setName(std::string const& name)
{
    name_ = name;
}

void Student::setForename(std::string const& forename)
{
    forename_ = forename;
}

void Student::setYear(const int32_t year)
{
    year_ = year;
}

void Student::Deserialize(std::vector<uint8_t> vec)
{
    uint8_t len;
	uint8_t ofs1;

    len = vec[0];
    name_.reserve(len);
    name_ = std::string(vec.begin() + 1, vec.begin() + len + 1);

    ofs1 = len + 1;
    len = vec[ofs1];
    forename_.reserve(len);
    forename_ = std::string(vec.begin() + ofs1 + 1, vec.begin() + ofs1 + len + 1);

	ofs1 = ofs1 + len + 1 + 1;
	year_ = vec[ofs1];
}

std::vector<uint8_t> Student::Serialize(void) const
{
    std::vector<uint8_t> vec;

    vec.clear();
    vec.push_back(name_.length());
    for (auto& ch : name_)
    {
        vec.push_back(ch);
    }

    vec.push_back(forename_.length());
    for (auto& ch : forename_)
    {
        vec.push_back(ch);
    }

    vec.push_back(1);
    vec.push_back(year_);

    return vec;
}
