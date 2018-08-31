/*
 * Copyright 2018 Autodesk
 */

#pragma once

// IOperational.hpp

#include <functional>
#include "IPlayer.hpp"
#include "IRecorder.hpp"
#include "ITrackable.hpp"

enum
{
   ReturnsAValue = true
   , ReturnsVoid = false
};

template<class ITarget, bool returnsValue, int numArgs>
class CppCall;

using ICallableCallback
   = std::function<void(std::shared_ptr<std::exception>)>;

// Root class enabling automation wrappers to be written
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
   // The derived I-classes use "recordCppCall" to trigger recording for each call.
   template<typename... Args>
   void recordFunction(Args...) const
   {}
   template<typename... Args>
   void recordMethod(Args...) const
   {}

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
