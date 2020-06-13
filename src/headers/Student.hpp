#ifndef Student_hpp
#define Student_hpp

#include <string>
#include <cstdint>
#include <vector>

#include "MoBase.hpp"
#include "ISerialize.hpp"

class Student : public ISerialize
{
public:
    Student();

    std::string const& getName(void) const;
    std::string const& getForename(void) const;
    int32_t getYear(void) const;

    void setName(std::string const& name);
    void setForename(std::string const& forename);
    void setYear(const int32_t year);

    virtual void Deserialize(std::vector<uint8_t> vec);
	virtual std::vector<uint8_t> Serialize(void) const;
private:
    std::string name_;
    std::string forename_;
    int32_t year_;
};

typedef MoBaseT<Student> Mo_Student;

#endif