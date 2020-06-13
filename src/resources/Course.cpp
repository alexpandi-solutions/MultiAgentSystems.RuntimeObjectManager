#include "../headers/Course.hpp"

#include <string>
#include <cstdint>
#include <vector>

template<> const ClassName Mo_Course::className_ = "COURSE";

Course::Course()
{
    name_ = "";
    profName_ = "";
    teachingYear_ = 0;
}

std::string const& Course::getName(void) const
{
    return name_;
}

std::string const& Course::getProfName(void) const
{
    return profName_;
}

int32_t Course::getTeachingYear(void) const
{
    return teachingYear_;
}

void Course::setName(std::string const& name)
{
    name_ = name;
}

void Course::setProfName(std::string const& profName)
{
    profName_ = profName;
}

void Course::setTeachingYear(const int32_t teachingYear)
{
    teachingYear_ = teachingYear;
}

void Course::Deserialize(std::vector<uint8_t> vec)
{
    uint8_t len;
	uint8_t ofs1;

    len = vec[0];
    name_.reserve(len);
    name_ = std::string(
        vec.begin() + 1, 
        vec.begin() + len + 1);

    ofs1 = len + 1;
    len = vec[ofs1];
    profName_.reserve(len);
    profName_ = std::string(
        vec.begin() + ofs1 + 1, 
        vec.begin() + ofs1 + len + 1);

	ofs1 = ofs1 + len + 1 + 1;
	teachingYear_ = vec[ofs1];
}

std::vector<uint8_t> Course::Serialize(void) const
{
    std::vector<uint8_t> vec;

    vec.clear();
    vec.push_back(name_.length());
    for (auto& ch : name_)
    {
        vec.push_back(ch);
    }

    vec.push_back(profName_.length());
    for (auto& ch : profName_)
    {
        vec.push_back(ch);
    }

    vec.push_back(1);
    vec.push_back(teachingYear_);

    return vec;
}
