#ifndef __ISERIALIZE_HPP__
#define __ISERIALIZE_HPP__

#include <cstdint>
#include <vector>

class ISerialize
{
public:
    virtual ~ISerialize() = default;

    virtual void Deserialize(std::vector<uint8_t> vec) {}
    virtual std::vector<uint8_t> Serialize(void) const { return {}; }
};

#endif
