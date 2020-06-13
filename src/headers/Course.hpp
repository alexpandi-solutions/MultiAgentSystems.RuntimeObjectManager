#ifndef Course_hpp
#define Course_hpp

#include <string>
#include <cstdint>
#include <vector>

#include "MoBase.hpp"
#include "ISerialize.hpp"

class Course : public ISerialize
{
public:
    Course();

    std::string const& getName(void) const;
    std::string const& getProfName(void) const;
    int32_t getTeachingYear(void) const;

    void setName(std::string const& name);
    void setProfName(std::string const& profName);
    void setTeachingYear(const int32_t teachingYear);

    virtual void Deserialize(std::vector<uint8_t> vec);
	virtual std::vector<uint8_t> Serialize(void) const;

private:
    std::string name_;
    std::string profName_;
    int32_t teachingYear_;
};

typedef MoBaseT<Course> Mo_Course;

#endif 
