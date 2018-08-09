/*
 * Copyright 2018 Autodesk
 */

#pragma once

#include <CppCallMap.hpp>
#include <CppCallRecorder.hpp>
#include <functional>

class CppCallRecorder;
enum Err;

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
   virtual ~ITrackable()
   {}

   // Methods
public:
   // The derived I-classes use "recordCppCall" to trigger recording for each call.
   // This is coded so that all template arguments are deduced.
   template<class ITarget, bool returnsValue, int numArg, typename... Args>
   void recordCppCall(const CppCall<ITarget, returnsValue, numArg>& cc, Args... args) const
   {
      if (recording())
         cppCallRecorder()->recordOne<ITarget>(cc.api(), std::dynamic_pointer_cast<const ITarget>(shared_from_this()), args...);
   }

   // Utilities
protected:
   static std::shared_ptr<CppCallRecorder> cppCallRecorder()
   {
      return Recorder.lock();
   }
   static bool recording()
   {
      return (!inPlayer());
   }
   static bool inPlayer()
   {
      // TODO: differentiate some way other than changing following to true
      return false;
   }
   CppCallStream& cppCallStream();

public:
   static void reportError(const char* operation, const std::shared_ptr<std::exception>& sException);
   static void reportError(const char* operation, const std::exception& e);
   static void reportInfo(Err, std::string ss);

   // Attributes
public:
   std::string m_objectKey;

   // Globals
public:
   static unsigned int                   StartFlags;
   static std::weak_ptr<CppCallRecorder> Recorder;
   static ITrackableCallback             Callback;
};

// Inner function used in boilerplates
template<class ITarget>
using TargetPlaybackCall
   = std::function<void(ITarget& rThis, const ArgsReader&)>;

// This class stores information known about the call (in a map)
template<class ITarget, bool returnsValue, int numArgs>
class CppCall sealed
{
   CppCallMapEntry* m_pEntry;
public:
   CppCall(std::string api, TargetPlaybackCall<ITarget> fun)
   {
      // In this constructor run during static initialization, we create and registry one entry into the map
      PlaybackCall executeFun = 
         [fun] (const ArgsReader& ar)
         {
            auto pThis = std::dynamic_pointer_cast<ITarget>(ar.getThisTarget());
            failUnlessPredicate(!!pThis.get(), CppCallError_NoSuchTarget);
            fun(*pThis.get(), ar);
         };
      auto pEntry = std::make_unique<CppCallMapEntry>(api, executeFun, returnsValue, numArgs);
      m_pEntry = &cppCallMap().emplaceMethod(std::move(pEntry));
   }
   std::string api() const
   {
      return m_pEntry->api();
   }
};