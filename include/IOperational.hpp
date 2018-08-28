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
   // The derived I-classes use "recordCall" to trigger recording for each call.
   template<class ITarget, typename... Args>
   void recordFunction(const CppCall<ITarget>& cc, Args... args) const
   {
      recordCall(ITarget::libraryCallMap(), cc, true, args...);
   }

   template<class ITarget, typename... Args>
   void recordMethod(const CppCall<ITarget>& cc, Args... args) const
   {
      recordCall(ITarget::libraryCallMap(), cc, false, args...);
   }

private:
   template<class ITarget, typename... Args>
   void recordCall(CallMap& callMap, const CppCall<ITarget>& cc, bool bReturnsValue, Args... args) const
   {
      if (callMap.recorder().streaming())
         callMap.recorder().recordCall<ITarget>(
            cc.m_api
            , bReturnsValue
            , std::dynamic_pointer_cast<const ITarget>(shared_from_this())
            , args...);
   }
};
