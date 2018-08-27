/*
 * Copyright 2018 Autodesk
 */

#pragma once

// ICallable.hpp

#include <functional>
#include "IPlayer.hpp"
#include "IRecorder.hpp"
#include "ITrackable.hpp"

// Root class enabling automation wrappers to be written
class ICallable : public ITrackable, public std::enable_shared_from_this<ICallable>
{
   // Construction
public:
   ICallable()
   {}
   virtual ~ICallable() = 0
   {}

   // Methods
public:
   // The derived I-classes use "recordCall" to trigger recording for each call.
   template<class ITarget, typename... Args>
   void recordFunction(const CppCall<ITarget>& cc, Args... args) const
   {
      if (recording())
         recorder().recordCall<ITarget>(
            cc.m_api
            , std::dynamic_pointer_cast<const ITarget>(shared_from_this())
            , args...);
   }

   template<class ITarget, typename... Args>
   void recordMethod(const CppCall<ITarget>& cc, Args... args) const
   {
      if (recording())
         recorder().recordCall<ITarget>(
            cc.m_api
            , std::dynamic_pointer_cast<const ITarget>(shared_from_this())
            , args...);
   }

   // Utilities
protected:
   static IPlayer*& p_player()
   {
      static IPlayer* pPlayer = nullptr;
      return pPlayer;
   }
   static IRecorder*& p_recorder()
   {
      static IRecorder* pRecorder = nullptr;
      return pRecorder;
   }
   static IPlayer& player()
   {
      return *p_player();
   }
   static IRecorder& recorder()
   {
      return *p_recorder();
   }
   static bool playing()
   {
      return player().streaming();
   }
   static bool recording()
   {
      return recorder().streaming();
   }
};
