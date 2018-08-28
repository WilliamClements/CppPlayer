/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CallMap.hpp

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
   Calls              m_theMap;
   unsigned int       StartFlags;
   CppPlayer*         m_pPlayer;
   CppRecorder*       m_pRecorder;

   CallMap()
      : m_theMap()
      , StartFlags()
      , m_pPlayer()
      , m_pRecorder()
   {}

   void onStart(CppPlayer& player, CppRecorder& recorder)
   {
      m_pPlayer = &player;
      m_pRecorder = &recorder;
   }

   CppPlayer& player()
   {
      return *m_pPlayer;
   }

   CppRecorder& recorder()
   {
      return *m_pRecorder;
   }

   TypeErased& lookupMethod(std::string apiname)
   {
      auto result = m_theMap.find(apiname);
      Assert(m_theMap.end() != result, Assertions_NoSuchAPIName, apiname);
      return result->second;
   }
};