/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CppCallMap.hpp

#include <functional>
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
   explicit CppCallMapEntry(std::string api, PlaybackCall call, bool bReturnsValue, int numArgs);
   virtual ~CppCallMapEntry();

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
   CppCallMap();
   ~CppCallMap();

   CppCallMapEntry& emplaceMethod(std::unique_ptr<CppCallMapEntry> pEntry);
   CppCallMapEntry& lookupMethod(std::string apiname);
};

CppCallMap& cppCallMap();
