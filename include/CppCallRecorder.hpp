/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CppCallRecorder.hpp

#include <Aliases.hpp>
#include <ArgsWriter.hpp>
#include <CppCallRecorderInterface.hpp>
#include <CppCallStream.hpp>
#include <GUIDvalue.hpp>

class CppCallRecorder final : public CppCallRecorderInterface
{
   fs::path                               m_outputfilepath;

public:
   CppCallRecorder()
      : m_outputfilepath()
   {}
   ~CppCallRecorder()
   {}

   // Methods
public:
   void startRecording(fs::path outputfilepath)
   {
      m_outputfilepath = outputfilepath;
      cppCallStream().startRecording();
   }
   void finishRecording()
   {
      cppCallStream().finishRecording(cppCallStream().mainId(), m_outputfilepath);
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
   CppCallMapEntry* m_pEntry;
public:
   CppCall(std::string api, TargetPlaybackCall<ITarget> fun)
   {
      // In this constructor run during static initialization, we create and registry one entry into the map
      PlaybackCall executeFun =
         [fun](const ArgsReader& ar)
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