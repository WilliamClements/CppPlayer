/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CppCallRecorder.hpp

#include <Aliases.hpp>
#include <CppCallMap.hpp>
#include <CppCallStream.hpp>
#include <GUIDvalue.hpp>
#include <unordered_map>

class CppCallRecorder final
{
   mutable CppCallStream                  m_callStream;
   fs::path                               m_outputfilepath;

public:
   CppCallRecorder()
      : m_outputfilepath()
   {}
   ~CppCallRecorder()
   {}

   CppCallStream& cppCallStream() const
   {
      return m_callStream;
   }

   template<class ITarget, typename... Args>
   void recordOne(std::string methodname, std::shared_ptr<const ITarget> pTrackable, Args... args)
   {
      ArgsWriter aw(cppCallStream());
      // Push the api name, then the object
      aw.pushHeader(methodname, pTrackable);
      // Push the arguments, in order
      aw.pushArgs(args...);
      aw.pushCall();
   }

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