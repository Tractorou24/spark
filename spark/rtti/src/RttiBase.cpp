#include "spark/rtti/RttiBase.h"

#include <string>
#include <utility>

namespace spark::rtti
{
    RttiBase::RttiBase(std::string class_name)
        : m_className(std::move(class_name)) {}

    std::string RttiBase::getClassName() const
    {
        return m_className;
    }
}
