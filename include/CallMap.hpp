/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CallMap.hpp - provide map of instrumented api calls

#include "Assertions.hpp"
#include <functional>
#include <unordered_map>

class ArgsReader;
class CppPlayer;
class CppRecorder;

// Lookup TypeErased functor in Calls map
using TypeErased = std::function<void(const ArgsReader&)>;
using Calls = std::unordered_map<std::string, TypeErased, std::hash<std::string>>;

class CallMap final
{
public:
   Calls                            m_theMap;
   unsigned int                     StartFlags;
   std::weak_ptr<CppPlayer>         m_pPlayer;
   std::weak_ptr<CppRecorder>       m_pRecorder;

   CallMap()
      : m_theMap()
      , StartFlags()
      , m_pPlayer()
      , m_pRecorder()
   {}

   void clear()
   {
      m_theMap.clear();
   }

   void onStart(std::shared_ptr<CppPlayer> player, std::shared_ptr<CppRecorder> recorder)
   {
      m_pPlayer = player;
      m_pRecorder = recorder;
   }

   CppPlayer& player()
   {
      return *m_pPlayer.lock();
   }

   CppRecorder& recorder()
   {
      return *m_pRecorder.lock();
   }

   TypeErased& lookupMethod(std::string apiname)
   {
      auto result = m_theMap.find(apiname);
      Assert(m_theMap.end() != result, Assertions_NoSuchAPIName, apiname);
      return result->second;
   }
};