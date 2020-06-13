#include <iostream>
#include <vector>

#include "../headers/Student.hpp"
#include "../headers/Course.hpp"
#include "../headers/Room.hpp"
#include "../headers/ObjectManager.hpp"
#include "../headers/Log.hpp"

namespace utils
{
    template <typename T>
    std::shared_ptr<T> cloneAs(Log& log, std::shared_ptr<const MoBase> mo)
    {
        log << "[utils::cloneAs]" << " " << "Cloning object with Name = " << mo->getDistName() << std::endl;

        auto nonConstMo = std::const_pointer_cast<MoBase>(mo);
        auto newMo = nonConstMo->clone(log);
        auto moT = std::static_pointer_cast<T>(newMo);

        return moT;
    }
}

std::vector<ClassName> expandView(void)
{
    static std::vector<ClassName> classNameList =
    {
        Mo_Student::className_,
        Mo_Course::className_
    };
    return classNameList;
}

void OnCreate(Log * log, DistName const& distName)
{
    *log << "[main::OnCreate]" << " " << distName << std::endl;
}

void OnRemove(Log * log, DistName const& distName)
{
    *log << "[main::OnRemove]" << " " << distName << std::endl;
}

// Subscription callback
void OnUpdate(Log * log, DistName const& distName)
{
    *log << "[main::OnUpdate]" << " " << distName << std::endl;
}

int main(int argc, const char * argv[])
{
    std::cout << "Hello, World!\n";
    std::cout << "See <results/syslog.log>\n";

    Log log("results/syslog.log");
    log.setUp();

	ObjectManager manager(log);

    manager.setUp();

	manager.viewExpand(expandView());

    auto onCreateId = manager.subscribe(
        SubscriptionType::Create, Mo_Course::className_, std::bind(
            OnCreate, &log, std::placeholders::_1));
    auto onUpdateId = manager.subscribe(
        SubscriptionType::Update, Mo_Student::className_, std::bind(
            OnUpdate, &log, std::placeholders::_1));
    auto onRemoveId = manager.subscribe(
        SubscriptionType::Remove, Mo_Room::className_, std::bind(
            OnRemove, &log, std::placeholders::_1));

    auto room1 = Mo_Room::newUniqueObject(log, "ROOM-1");

    (*room1)->Deserialize({11, 65, 108, 97, 110, 32, 84, 117, 114, 105, 110, 103,
                           10, 69, 77, 84, 45, 48, 48, 46, 49, 52, 57});

    manager.commit<Mo_Room>(room1);

    manager.viewExpand({Mo_Room::className_});

    onRemoveId = manager.subscribe(
        SubscriptionType::Remove, Mo_Room::className_, std::bind(
            OnRemove, &log, std::placeholders::_1));

    manager.commit<Mo_Room>(room1);

    auto stud1 = Mo_Student::newUniqueObject(log, "STUDENT-1");
    (*stud1)->setName("TEST-NAME-1");
    (*stud1)->setForename("TEST-FORENAME-1");
    (*stud1)->setYear(1);

    auto curs1 = Mo_Course::newUniqueObject(log, "COURSE-1");
    (*curs1)->setName("TEST-COURSE");
    (*curs1)->setProfName("TEST-NAME");
    (*curs1)->setTeachingYear(1);

    auto stud2 = Mo_Student::newUniqueObject(log, "STUDENT-2");
    (*stud2)->setName("TEST-NAME-2");
    (*stud2)->setForename("TEST-FORENAME-2");
    (*stud2)->setYear(1);

	manager.commit<Mo_Student>(stud1);
	manager.commit<Mo_Course>(curs1);

    manager.unsubscribe(onCreateId);

	manager.commit<Mo_Student>(stud2);

	manager.logObjects();

    auto mo = manager.fetch("STUDENT-2");
    if (mo)
    {
        auto mo_stud2 = utils::cloneAs<Mo_Student>(log, mo);
        log << "[main]" << " " << "Name = " << (*mo_stud2)->getName() << std::endl;

        (*mo_stud2)->setName("TEST-NAME-2");
        (*mo_stud2)->setYear(2);

        manager.commit<Mo_Student>(mo_stud2);
    }

    auto mo_stud2 = utils::cloneAs<Mo_Student>(log, manager.fetch("STUDENT-2"));
    if (mo_stud2)
    {
        log << "[main]" << " " << "Name = " << (*mo_stud2)->getName() << std::endl;
        log << "[main]" << " " << "Year = " << (*mo_stud2)->getYear() << std::endl;
    }

    auto mo_room1 = utils::cloneAs<Mo_Room>(log, manager.fetch("ROOM-1"));
    if (mo_room1)
    {
        log << "[main]" << " " << "Name = " << (*mo_room1)->getName() << std::endl;
        log << "[main]" << " " << "Number = " << (*mo_room1)->getNumber() << std::endl;
    }

    manager.remove("ROOM-1");

    auto all = manager.fetchAll();
    log << "[main] List objects stored in ObjectManager" << std::endl;

    for (auto const& it : all)
    {
        log << "[main]"  << " " << "ClassName " << it->getClassName() << " uniqueId " << it->getUniqueId() << std::endl;
        auto ser = utils::cloneAs<MoBaseT<ISerialize>>(log, it);
        auto vec = (*ser)->Serialize();
        log << "[main]Bytes" << " ";
        for (auto& ch : vec)
        {
            log << '[' << static_cast<int32_t>(ch) << ']' << ' ';
        }
        log << std::endl;
    }

    manager.unsubscribe(onUpdateId);
    manager.unsubscribe(onRemoveId);

    manager.tearDown();
    log.tearDown();

    return 0;
}
