#ifndef MoBase_h
#define MoBase_h

#include "Log.hpp"

#include <string>
#include <cstdint>
#include <memory>

typedef std::string DistName;

typedef std::string ClassName;

class MoBase
{
public:
    MoBase() : distName_(), uniqueId_(MoBase::uniqueIdCounter)
    {
        MoBase::uniqueIdCounter += 1;
    };

    explicit MoBase(DistName const& distName) : distName_(distName), uniqueId_(MoBase::uniqueIdCounter)
    {
        MoBase::uniqueIdCounter += 1;
    };

    virtual ~MoBase() {};

    virtual std::shared_ptr<MoBase> clone(Log& log) const = 0;

    virtual DistName const& getClassName() const = 0;

    virtual DistName const& getDistName() const
    {
        return distName_;
    }

    virtual int32_t getUniqueId(void) const
    {
        return uniqueId_;
    }
private:
    DistName distName_;
    int32_t uniqueId_;

    static int32_t uniqueIdCounter;
};

template <typename T>
class MoBaseT : public MoBase
{
public:
    MoBaseT() = delete;

    static const ClassName className_;

    explicit MoBaseT(std::string const& distName) : MoBase(distName), data_() {};

    static std::shared_ptr<MoBaseT<T>> newUniqueObject(Log& log, DistName const& distName)
    {
        log << "[MoBaseT<T>::newUniqueObject]" << " " << "Create new unique object with Name = " << distName << std::endl;
        return std::make_shared<MoBaseT<T>>(distName);
    }

    T* operator->() { return &data_; }

	std::shared_ptr<MoBase> clone(Log& log) const
	{
        log << "[MoBaseT<T>::clone]" << " " << "Cloning object with Name = " << getDistName() << " and uniqueId " << getUniqueId() << std::endl;

	    std::shared_ptr<MoBaseT<T>> shptr = std::make_shared<MoBaseT<T>>(getDistName());

        (*shptr).data_ = data_;

        log << "[MoBaseT<T>::clone]" << " " << "Cloned object has uniqueId " << (*shptr).getUniqueId() << std::endl;
        return shptr;
	}

    const ClassName& getClassName() const { return className_; }
private:
    T data_;
};

#endif 
