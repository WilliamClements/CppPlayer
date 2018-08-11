/*
 * Copyright 2018 Autodesk
 */

#pragma once

#include <CppCallPlayerInterface.hpp>
#include <CppCallRecorderInterface.hpp>
#include <functional>

enum
{
   ReturnsAValue = true
   , ReturnsVoid = false
};

template<class ITarget, bool returnsValue, int numArgs>
class CppCall;

using ITrackableCallback
   = std::function<void(std::shared_ptr<std::exception>)>;

// Root class making it possible to address the automation wrappers
class ITrackable : public std::enable_shared_from_this<ITrackable>
{
   // Construction
public:
   ITrackable()
   {}
   virtual ~ITrackable() = 0
   {}

   // Accessors
public:
   std::string key() const
   {
      return m_objectKey;
   }
   // Methods
public:
   // The derived I-classes use "recordCppCall" to trigger recording for each call.
   // This is coded so that all template arguments are deduced.
   template<class ITarget, bool returnsValue, int numArg, typename... Args>
   void recordCppCall(const CppCall<ITarget, returnsValue, numArg>& cc, Args... args) const
   {
      if (recording())
         recorder().recordOne<ITarget>(cc.api(), std::dynamic_pointer_cast<const ITarget>(shared_from_this()), args...);
   }

   // Utilities
protected:
   static CppCallPlayerInterface*& pplayer()
   {
      static CppCallPlayerInterface* pPlayer = nullptr;
      return pPlayer;
   }
   static CppCallRecorderInterface*& precorder()
   {
      static CppCallRecorderInterface* pRecorder = nullptr;
      return pRecorder;
   }
   static CppCallPlayerInterface& player()
   {
      return *pplayer();
   }
   static CppCallRecorderInterface& recorder()
   {
      return *precorder();
   }
   static bool playing()
   {
      return player().streaming();
   }
   static bool recording()
   {
      return recorder().streaming();
   }

   // Attributes
public:
   std::string m_objectKey;

   // Globals
public:
};