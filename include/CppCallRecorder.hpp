/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CppCallRecorder.hpp

#include <CppCallMap.hpp>
#include <CppCallStream.hpp>
#include <GUIDvalue.hpp>
#include <unordered_map>

class CppCallRecorder;
class CppCallStream;
class IMain;

enum IHFDM_Start_Flags
{
   Do_Execution = 0x1
   , Do_Recording = 0x2
   , Do_Playback = 0x4
};

std::shared_ptr<CppCallRecorder> CppCallRecordingManager()
{
   static auto rr = std::make_shared<CppCallRecorder>();
   return rr;
}

class CppCallRecorder : public std::enable_shared_from_this<CppCallRecorder>
{
   std::unique_ptr<CppCallStream>         m_callStream;
   std::string                           m_outputfilename;
   static std::shared_ptr<IMain>      s_IMain;

public:
   static std::shared_ptr<IMain> start_IHFDM(unsigned int, std::string jsonfilename)
   {
      // These flags are for the whole session
      s_IMain = std::make_shared<IMain>();
      // WOX ITrackable::StartFlags = flags;
      CppCallRecordingManager()->start_IHFDM_private(jsonfilename);
      return s_IMain;
   }

   static void finish_IHFDM()
   {
      if (true) // WOX Do_Recording & ITrackable::StartFlags)
      {
         CppCallRecordingManager()->finishRecording();
      }
      // WOX ITrackable::Recorder.reset();
      CppCallRecordingManager().reset();
   }

   CppCallRecorder()
      : m_outputfilename()
   {}

   ~CppCallRecorder()
   {}

   CppCallStream& cppCallStream() const
   {
      return *m_callStream.get();
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
private:
   void start_IHFDM_private(std::string jsonfilename);
public:
   void startRecording()
   {
      m_callStream = std::make_unique<CppCallStream>();
      cppCallStream().startRecording();
   }
   void finishRecording()
   {
      cppCallStream().finishRecording("" /* WOX s_IMain->m_objectKey */, m_outputfilename);
      m_callStream.reset();
   }

   // Utilities
private:
   static bool fileExists(std::string ss)
   {
#if !WOX
      ss;
      return true;
#else
      CFileStatus localStatus;
      std::wstring ws(ss.length(), L' ');
      std::copy(ss.begin(), ss.end(), ws.begin());
      bool bStatusValid = !!CFile::GetStatus(ws.c_str(), localStatus);
      return (bStatusValid && !(localStatus.m_attribute & CFile::directory));
#endif
   }
};