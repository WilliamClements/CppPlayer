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
using PlaybackCall
   = std::function<void(const ArgsReader&)>;

class  CallMapEntry
{
   std::string                         m_api;
   PlaybackCall                        m_call;
   bool                                m_bReturnsValue;
   int                                 m_numArgs;

public:
   explicit CallMapEntry(std::string api, PlaybackCall call, bool bReturnsValue, int numArgs)
      : m_api(api)
      , m_call(call)
      , m_bReturnsValue(bReturnsValue)
      , m_numArgs(numArgs)
   {}
   ~CallMapEntry()
   {}

   std::string api() const
   {
      return m_api;
   }
   PlaybackCall call() const
   {
      return m_call;
   }
   bool returnsValue() const
   {
      return m_bReturnsValue;
   }
   int numArgs() const
   {
      // This is the number of passed arguments
      // (not including other fields or return value)
      return m_numArgs;
   }
   int numExpectedFields() const
   {
      // The api string and the target trackable are always pushed
      return numArgs() + 2 + (returnsValue() ? 1 : 0);
   }
};

inline void failmap(Assertions err, const CallMapEntry* pEntry)
{
   fail(err, pEntry ? pEntry->api() : std::string{ "no entrypoint" });
}

inline void failUnlessPredicate(bool bPredicate, Assertions err, const CallMapEntry* pEntry = nullptr)
{
   if (!bPredicate)
      failmap(err, pEntry);
}

using CallMapType
   = std::unordered_map<std::string, std::unique_ptr<CallMapEntry>, std::hash<std::string>>;

class CallMap final
{
   CallMapType              m_theMap;

public:
   CallMap()
      : m_theMap()
   {}
   ~CallMap()
   {}

   CallMapEntry& emplaceMethod(std::unique_ptr<CallMapEntry> pEntry)
   {
      auto result = m_theMap.emplace(pEntry->api(), std::move(pEntry));
      auto* pRet = (result.first->second).get();
      failUnlessPredicate(result.second, Assertions_DuplicateAPINames, pRet);
      return *pRet;
   }

   CallMapEntry& lookupMethod(std::string apiname)
   {
      auto findResult = m_theMap.find(apiname);
      failUnlessPredicate(m_theMap.end() != findResult, Assertions_NoSuchAPIName, apiname);
      return *(findResult->second.get());
   }
};

inline CallMap& callMap()
{
   static CallMap m_callmap;
   return m_callmap;
}
