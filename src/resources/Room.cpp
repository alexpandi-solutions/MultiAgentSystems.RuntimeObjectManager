#include "../headers/Room.hpp"

template<> const ClassName Mo_Room::className_ = "ROOM";

Room::Room()
{
    name_ = "";
    number_ = "";
}

std::string const& Room::getName(void) const
{
    return name_;
}

std::string const& Room::getNumber(void) const
{
    return number_;
}

void Room::setName(std::string const& name)
{
    name_ = name;
}

void Room::setNumber(std::string const& number)
{
    number_ = number;
}

void Room::Deserialize(std::vector<uint8_t> vec)
{
    uint8_t len;
    uint8_t ofs1;
    
    len = vec[0];
    name_.reserve(len);
    name_ = std::string(vec.begin() + 1, vec.begin() + len + 1);

    ofs1 = len + 1;
    len = vec[ofs1];
    number_.reserve(len);
    number_ = std::string(vec.begin() + ofs1 + 1, vec.begin() + ofs1 + len + 1);
}

std::vector<uint8_t> Room::Serialize(void) const
{
    std::vector<uint8_t> vec;
    
    vec.clear();
    vec.push_back(name_.length());
    for (auto& ch : name_)
    {
        vec.push_back(ch);
    }

    vec.push_back(number_.length());
    for (auto& ch : number_)
    {
        vec.push_back(ch);
    }
    
    return vec;
}
