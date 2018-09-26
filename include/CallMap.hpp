/*

Copyright 2018 Autodesk

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

// CallMap.hpp - provide map of instrumented api calls

#pragma once

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
   unsigned int                     StartFlags = 0;
   std::weak_ptr<CppPlayer>         m_pPlayer;
   std::weak_ptr<CppRecorder>       m_pRecorder;

   CallMap()
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
      Assert(m_theMap.end() != result, Assertions::NoSuchAPIName, apiname);
      return result->second;
   }
};