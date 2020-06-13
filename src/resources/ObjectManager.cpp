#include <map>
#include <algorithm>

#include "../headers/ObjectManager.hpp"

int32_t ObjectManager::uniqueSubscriptionId = 0;

ObjectManager::ObjectManager(Log& log) : log_(log)
{
    log_ << "[ObjectManager::ObjectManager]" << " " << "ObjectManager created." << std::endl;
}

void ObjectManager::setUp(void)
{
    moStorage_.clear();
    viewClasses_.clear();
    subscriptions_.clear();
    log_ << "[ObjectManager::setUp]" << " " << "ObjectManager is active" << std::endl;
}

void ObjectManager::tearDown(void)
{
    log_ << "[ObjectManager::tearDown]" << " " << "ObjectManager stopped" << std::endl;
}

void ObjectManager::viewExpand(std::vector<ClassName> view)
{
    for (auto const& it : view)
    {
        viewClasses_.push_back(it);
        log_ << "[ObjectManager::viewExpand]" << " " << "Expand w/ClassName " << it << std::endl;
    }
}

SubscriptionId ObjectManager::subscribe(const SubscriptionType type, ClassName const& className,
                              std::function<void (DistName const& distName)> subscriber)
{
    log_ << "[ObjectManager::subscribe]" << " " << "Subscribe to " << getNameOfSubscriptionType(type) << " for class " << className << std::endl;
    auto pos = std::find(viewClasses_.begin(), viewClasses_.end(), className);

    if (pos == viewClasses_.end())
    {
        log_ << "[ObjectManager::subscribe]" << " " << "Unknwon class " << className << ". No subscription registered" << std::endl;
        return -1;
    }

    auto tuple = std::make_tuple<const SubscriptionType&, ClassName const&,
                                 std::function<void (DistName const& distName)> const&>(type, className, subscriber);

    ObjectManager::uniqueSubscriptionId += 1;
    subscriptions_[ObjectManager::uniqueSubscriptionId] = tuple;

    log_ << "[ObjectManager::subscribe]" << " " << "Subscription ID is " << ObjectManager::uniqueSubscriptionId << std::endl;

    return ObjectManager::uniqueSubscriptionId;
}

bool ObjectManager::unsubscribe(const SubscriptionId id)
{
    log_ << "[ObjectManager::unsubscribe]" << " " << "Remove subscription with ID " << id << std::endl;

    auto found = subscriptions_.find(id);
    if (found != subscriptions_.end())
    {
        subscriptions_.erase(found);
        return true;
    }
    log_ << "[ObjectManager::unsubscribe]" << " " << "Error, subscription with ID " << id << " not found" << std::endl;
    return false;
}

void ObjectManager::logObjects(void) const
{
	for (const auto& p : moStorage_)
	{
	    log_ << "[ObjectManager::logObjects]" << " " << "DistName=" << p.first << " ClassName=" << p.second->getClassName() << " UniqueId=" << p.second->getUniqueId() << std::endl;
	}
}

void ObjectManager::callSubscriptions(const SubscriptionType type, std::shared_ptr<MoBase> mo) const
{
    log_ << "[ObjectManager::callSubscriptions]" << " " << "type=" << getNameOfSubscriptionType(type) << " className=" << mo->getClassName() << std::endl;

    bool called = false;

    for(const auto& it : subscriptions_)
    {
        auto tuple = it.second;
        auto subscribedType = std::get<0>(tuple);
        auto className = std::get<1>(tuple);
        auto callback = std::get<2>(tuple);

        if (type == subscribedType && mo->getClassName() == className)
        {
            log_ << "[ObjectManager::callSubscriptions]" << " " << "Call subscription with ID " << it.first << std::endl;
            callback(mo->getDistName());
            called = true;
        }
    }

    if (called == false)
    {
        log_ << "[ObjectManager::callSubscriptions]" << " " << "Nothing to call" << std::endl;
    }
}

const std::string ObjectManager::getNameOfSubscriptionType(const SubscriptionType type) const
{
    switch (type)
    {
        case SubscriptionType::Create:
            return "Create";
            break;
        case SubscriptionType::Remove:
            return "Remove";
            break;
        case SubscriptionType::Update:
            return "Update";
            break;
        default:
            return std::string();
    }
}
