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

// CppRecorder.hpp - orchestrate recording

#pragma once

#include "ArgsReader.hpp"
#include "CallStream.hpp"
#include "IRecorder.hpp"
#include "NamespaceAliases.hpp"

class CppRecorder final : public IRecorder
{
   fs::path m_outputfilepath;

public:
   CppRecorder(CallMap& callMap)
      : IRecorder{ callMap }
      , m_outputfilepath()
   {}

   // Methods
public:
   void startRecording(fs::path outputfilepath)
   {
      m_outputfilepath = outputfilepath;
      callStream().startRecording();
   }
   void finishRecording(std::string mainId)
   {
      callStream().finishRecording(mainId, m_outputfilepath);
   }
};

// Inner function used in boilerplates
template<class ITarget>
using Targeted = std::function<void(ITarget& rThis, const ArgsReader&)>;

// This class stores information known about the call (in a map)
template<class ITarget>
class CppCall sealed
{
public:
   std::string m_api;

   CppCall(std::string api, Targeted<ITarget> fun)
      : m_api(api)
   {
      // In this constructor run during static initialization, we register one functor into the map
      TypeErased ucall = [fun](const ArgsReader& ar)
      {
         // Untargeted (type-erased) functor executed during playback
         // (first step: become typeful)
         auto pThis = std::dynamic_pointer_cast<ITarget>(ar.getThisTarget());
         Assert(!!pThis.get(), Assertions::NoSuchTarget);
         // Invoke targeted (typeful) functor
         fun(*pThis.get(), ar);
      };
      (void)ITarget::libraryCallMap().m_theMap.emplace(api, ucall);
   }
};

// This ties all the boilerplate together
#define AddToMap(CLASSNAME, APINAME)                              \
static inline CppCall<CLASSNAME> capture_ ## APINAME =            \
{ #CLASSNAME "::" #APINAME                                        \
  , [](CLASSNAME& pThis, const ArgsReader& ar) {                  \
    std::invoke(&CLASSNAME::playback_ ## APINAME , pThis , ar);}};
