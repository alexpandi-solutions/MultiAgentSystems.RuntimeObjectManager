#ifndef _ObjectManager_HPP__
#define _ObjectManager_HPP__

#include <cstdint>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <utility>
#include <tuple>

#include "MoBase.hpp"
#include "Log.hpp"

typedef int32_t SubscriptionId;

enum class SubscriptionType : int32_t
{
    Create = 0,
    Remove = 1,
    Update = 2
};

class ObjectManager
{
public:
    ObjectManager() = delete; 

    explicit ObjectManager(Log& log);

    void setUp(void);

    void tearDown(void);

	void viewExpand(std::vector<ClassName> view);

    template <class T>
    void commit(std::shared_ptr<T> mo)
    {
        SubscriptionType stype = SubscriptionType::Create;

        ClassName className = mo->getClassName();
        DistName distName = mo->getDistName();

        log_ << "[ObjectManager::commit]" << " " << "Object w/ClassName: " << className << " and DistName: " << distName << std::endl;

        auto pos = std::find(viewClasses_.begin(), viewClasses_.end(), className);

        if (pos == viewClasses_.end())
        {
            log_ << "[ObjectManager::commit]" << " " << "Commit error; class " << className << " not known!" << std::endl;
            return;
        }

        auto isStored = moStorage_.find(mo->getDistName());

        if (isStored != moStorage_.end())
        {
            log_ << "[ObjectManager::commit]" << " " << "MO update requested" << std::endl;
            stype = SubscriptionType::Update;
        }

        std::shared_ptr<MoBase> newMo = mo->clone(log_);
        moStorage_[newMo->getDistName()] = newMo;

        callSubscriptions(stype, newMo);
    }

    std::shared_ptr<const MoBase> fetch(DistName const& distName)
    {
        auto ret = moStorage_.find(distName);
        if (ret != moStorage_.end())
        {
            log_ << "[ObjectManager::fetch]" << " " << "Fetch object with distName " << distName << std::endl;
            return ret->second;
        }
        else
        {
            log_ << "[ObjectManager::fetch]" << " " << "Object " << distName << " not found!" << std::endl;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<const MoBase>> fetchAll(void) const
    {
        std::vector<std::shared_ptr<const MoBase>> vec;

        log_ << "[ObjectManager::fetchAll]" << std::endl;
        for (auto const& it : moStorage_)
        {
            vec.push_back(it.second);
        }
        return vec;
    }

    void remove(DistName const& distName)
    {
        log_ << "[ObjectManager::remove]" << " " << "Remove object with distName " << distName << std::endl;

        auto ret = moStorage_.find(distName);

        if (ret != moStorage_.end())
        {
            log_ << "[ObjectManager::remove]" << " " << "Object with distName " << distName << ", unique ID " << (*ret->second).getUniqueId() << " removed" << std::endl;
            callSubscriptions(SubscriptionType::Remove, moStorage_[distName]);
            moStorage_.erase(ret);
        }
        else
        {
            log_ << "[ObjectManager::remove]" << " " << "Object with distName " << distName << " not found!" << std::endl;
        }
    }

    SubscriptionId subscribe(const SubscriptionType type, ClassName const& className,
                             std::function<void (DistName const& distName)> subscriber);
    bool unsubscribe(const SubscriptionId id);
	void logObjects(void) const;

private:
    void callSubscriptions(const SubscriptionType type, std::shared_ptr<MoBase> mo) const;
    const std::string getNameOfSubscriptionType(const SubscriptionType type) const;

    Log& log_;
    std::map<DistName, std::shared_ptr<MoBase> > moStorage_;
    std::vector<ClassName> viewClasses_;
    std::map<SubscriptionId, std::tuple<SubscriptionType, ClassName, std::function<void (DistName const& distName)> > > subscriptions_;

    static int32_t uniqueSubscriptionId;
};

#endif 
