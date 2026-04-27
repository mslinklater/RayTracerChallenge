#pragma once

#include <json/json.h>

class ISerializable
{
    virtual void Serialize(Json::Value& json) const = 0;
    virtual void Deserialize(const Json::Value& json) = 0;
};
