/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CallMap.hpp

#include "Assertions.hpp"
#include <functional>
#include <memory>
#include <unordered_map>

class ArgsReader;
class ArgsWriter;

// Outer function which is accessible from the map
using UntargetedCall = std::function<void(const ArgsReader&)>;

using CallMapType = std::unordered_map<std::string, UntargetedCall, std::hash<std::string>>;

class CallMap final
{
   CallMapType              m_theMap;

public:
   CallMap()
      : m_theMap()
   {}
   ~CallMap()
   {}

   void emplaceMethod(std::string api, UntargetedCall ucall)
   {
      auto result = m_theMap.emplace(api, ucall);
      Assert(result.second, Assertions_DuplicateAPINames, api);
   }

   UntargetedCall& lookupMethod(std::string apiname)
   {
      auto result = m_theMap.find(apiname);
      Assert(m_theMap.end() != result, Assertions_NoSuchAPIName, apiname);
      return result->second;
   }
};