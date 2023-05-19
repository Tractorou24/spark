#include "spark/rtti/RttiDatabase.h"

namespace spark::rtti
{
    RttiBase* RttiDatabase::Get(const std::string& class_name)
    {
        const auto& storage = dataStorage();
        if (const auto it = storage.find(class_name); it != storage.end())
            return it->second.get();

        return nullptr;
    }

    std::unordered_map<std::string, std::unique_ptr<RttiBase>>& RttiDatabase::dataStorage()
    {
        static std::unordered_map<std::string, std::unique_ptr<RttiBase>> data_storage;
        return data_storage;
    }
}
