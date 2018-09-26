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

// IOperational.hpp - supports automation wrappers in derived classes

#pragma once

#include <functional>
#include "IPlayer.hpp"
#include "IRecorder.hpp"
#include "ITrackable.hpp"

class IOperational : public ITrackable, public std::enable_shared_from_this<IOperational>
{
   // Construction
public:
   IOperational()
   {}
   virtual ~IOperational() = 0
   {}

   // Methods
public:
   // Derived wrapper classes use "recordFunction" or "recordMethod" to
   // trigger recording for each call.
   template<class ITarget, typename ReturnType, typename... Args>
   ReturnType recordFunction(const CppCall<ITarget>& cc, ReturnType retValue, Args... args) const
   {
      auto& callMap = ITarget::libraryCallMap();
      if (callMap.recorder().streaming())
         recordCall(callMap, cc, true, retValue, args...);
      return retValue;
   }

   template<class ITarget, typename... Args>
   void recordMethod(const CppCall<ITarget>& cc, Args... args) const
   {
      auto& callMap = ITarget::libraryCallMap();
      if (callMap.recorder().streaming())
         recordCall(callMap, cc, false, 0, args...);
   }

private:
   // This rationalizes Function and Method 
   template<class ITarget, typename ReturnType, typename... Args>
   void recordCall(CallMap& callMap, const CppCall<ITarget>& cc, bool bReturnsValue, ReturnType retValue, Args... args) const
   {
      callMap.recorder().recordCall<ITarget>(
         cc.m_api
         , bReturnsValue
         , retValue
         , std::dynamic_pointer_cast<const ITarget>(shared_from_this())
         , args...);
   }
};
