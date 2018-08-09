/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CppCallMap.hpp

#include <CppCallError.hpp>
#include <functional>
#include <memory>
#include <unordered_map>

class ArgsReader;
class ArgsWriter;

// Outer function which is accessible from the map
using PlaybackCall
   = std::function<void(const ArgsReader&)>;

class  CppCallMapEntry
{
   std::string                         m_api;
   PlaybackCall                        m_call;
   bool                                m_bReturnsValue;
   int                                 m_numArgs;

public:
   explicit CppCallMapEntry(std::string api, PlaybackCall call, bool bReturnsValue, int numArgs)
      : m_api(api)
      , m_call(call)
      , m_bReturnsValue(bReturnsValue)
      , m_numArgs(numArgs)
   {}
   ~CppCallMapEntry()
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

using CppCallMapType
   = std::unordered_map<std::string, std::unique_ptr<CppCallMapEntry>, std::hash<std::string>>;

class CppCallMap final
{
   CppCallMapType              m_theMap;

public:
   CppCallMap()
      : m_theMap()
   {}
   ~CppCallMap()
   {}

   CppCallMapEntry& emplaceMethod(std::unique_ptr<CppCallMapEntry> pEntry)
   {
      auto result = m_theMap.emplace(pEntry->api(), std::move(pEntry));
      auto* pRet = (result.first->second).get();
      failUnlessPredicate(result.second, CppCallError_DuplicateAPINames, pRet);
      return *pRet;
   }

   CppCallMapEntry& lookupMethod(std::string apiname)
   {
      auto findResult = m_theMap.find(apiname);
      failUnlessPredicate(m_theMap.end() != findResult, CppCallError_NoSuchAPIName, apiname);
      return *(findResult->second.get());
   }
};

CppCallMap& cppCallMap()
{
   static CppCallMap s_CppCallMap;
   return s_CppCallMap;
}

