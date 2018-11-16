/*

Copyright 2018 William Clements, Autodesk

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

// IRecorder.hpp - methods, if any, for a recording type of IAgent

#pragma once

#include "ArgsWriter.hpp"
#include "IAgent.hpp"
#include "NamespaceAliases.hpp"

class IRecorder : public IAgent
{
public:
   IRecorder(CallMap& callMap)
      : IAgent{ callMap }
   {}
   virtual ~IRecorder() = 0;

   // Used by recordFunction and returnMethod, this adds a
   // call to the stream.
   template<class ITarget, typename ReturnType, typename... Args>
   void recordCall(
      std::string api
      , bool returnsValue
      , ReturnType retValue
      , std::shared_ptr<const ITarget> pTrackable
      , Args... args)
   {
      ArgsWriter aw{ callStream() };
      // Push the api name, whether function or method, then the target object
      aw.pushHeader(api, returnsValue, retValue, pTrackable);
      // Push the arguments, in order
      aw.pushArgs(args...);
      callStream().pushCall();
   }
};

