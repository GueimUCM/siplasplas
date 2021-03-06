#include "class.hpp"
#include <siplasplas/utility/exception.hpp>

using namespace cpp;
using namespace cpp::dynamic_reflection;

Class::Class(const SourceInfo& sourceInfo, const cpp::typeerasure::TypeInfo& typeInfo) :
    Entity{sourceInfo},
    _typeInfo{typeInfo}
{}

std::shared_ptr<Class> Class::create(const SourceInfo& sourceInfo, const cpp::typeerasure::TypeInfo& typeInfo)
{
    return std::shared_ptr<Class>{ new Class{sourceInfo, typeInfo} };
}

Class& Class::fromEntity(const std::shared_ptr<Entity>& entity)
{
    if(entity->sourceInfo().kind() == SourceInfo::Kind::CLASS)
    {
        return static_cast<Class&>(*entity);
    }
    else
    {
        throw cpp::exception<std::runtime_error>(
            "Entity '{}' is not a class",
            entity->fullName()
        );
    }
}

Enum& Class::enum_(const std::string& name)
{
    return Enum::fromEntity(getChildByName(name).pointer());
}

Class& Class::class_(const std::string& name)
{
    return Class::fromEntity(getChildByName(name).pointer());
}

Field& Class::field_(const std::string& name)
{
    return Field::fromEntity(getChildByName(name).pointer());
}

Function& Class::function_(const std::string& name)
{
    return Function::fromEntity(getChildByName(name).pointer());
}

const cpp::typeerasure::TypeInfo& Class::typeInfo() const
{
    return _typeInfo;
}

cpp::Any32 Class::create()
{
    cpp::Any32 any{typeInfo()};

    for(const auto& method : getChildrenNamesByKind(cpp::static_reflection::Kind::FUNCTION))
    {
        auto& entity = getChildByFullName(method);
        any(entity.name()) = Function::fromEntity(entity.pointer()).getFunction();
    }

    return any;
}
