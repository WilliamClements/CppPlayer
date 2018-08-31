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
   fs::path                               m_outputfilepath;

public:
   CppRecorder()
      : m_outputfilepath()
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
using TargetPlaybackCall
= std::function<void(ITarget& rThis, const ArgsReader&)>;

// This class stores information known about the call (in a map)
template<class ITarget, bool returnsValue, int numArgs>
class CppCall sealed
{
   CallMapEntry* m_pEntry;
public:
   CppCall(std::string api, TargetPlaybackCall<ITarget> fun)
   {
      // In this constructor run during static initialization, we create and registry one entry into the map
      PlaybackCall executeFun =
         [fun](const ArgsReader& ar)
      {
         auto pThis = std::dynamic_pointer_cast<ITarget>(ar.getThisTarget());
         failUnlessPredicate(!!pThis.get(), Assertions_NoSuchTarget);
         fun(*pThis.get(), ar);
      };
      auto pEntry = std::make_unique<CallMapEntry>(api, executeFun, returnsValue, numArgs);
      m_pEntry = &callMap().emplaceMethod(std::move(pEntry));
   }
   std::string api() const
   {
      return m_pEntry->api();
   }
};

#define AddToMap(CLASSNAME, APINAME)          \
static const int capture_ ## APINAME = 0;
