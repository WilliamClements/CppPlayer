/*
 * Copyright 2018 Autodesk
 */

#pragma once

// IOperational.hpp

#include <functional>
#include "IPlayer.hpp"
#include "IRecorder.hpp"
#include "ITrackable.hpp"

// Supports automation wrappers in derived classes
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
