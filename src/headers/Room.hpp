#ifndef Room_hpp
#define Room_hpp

#include "MoBase.hpp"
#include "ISerialize.hpp"

class Room : public ISerialize
{
public:
    Room();

    std::string const& getName(void) const;
    std::string const& getNumber(void) const;

    void setName(std::string const& name);
    void setNumber(std::string const& forename);

    virtual void Deserialize(std::vector<uint8_t> vec);
    virtual std::vector<uint8_t> Serialize(void) const;
private:
    std::string name_;
    std::string number_;
};

typedef MoBaseT<Room> Mo_Room;

#endif 
