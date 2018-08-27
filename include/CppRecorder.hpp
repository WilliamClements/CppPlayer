/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CppRecorder.hpp

#include "ArgsWriter.hpp"
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
   ~CppRecorder()
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
using TargetedCall = std::function<void(ITarget& rThis, const ArgsReader&)>;

// This class stores information known about the call (in a map)
template<class ITarget>
class CppCall sealed
{
public:
   std::string m_api;

   CppCall(std::string api, TargetedCall<ITarget> fun)
      : m_api(api)
   {
      // In this constructor run during static initialization, we register one functor into the map
      UntargetedCall executeFun =
         [fun](const ArgsReader& ar)
      {
         // Body of untargeted functor executed during playback
         auto pThis = std::dynamic_pointer_cast<ITarget>(ar.getThisTarget());
         Assert(!!pThis.get(), Assertions_NoSuchTarget);
         // Invoke targeted functor
         fun(*pThis.get(), ar);
      };
      (void)callMap().emplaceMethod(api, executeFun);
   }
};

