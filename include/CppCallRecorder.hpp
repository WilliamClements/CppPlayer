PERSISTENCEDB_HEADER
FILE_DB_ONLY
/*
$CODE_OWNER William_Clements
$SECONDARY_OWNERS Dmitry_Brumberg David_Becroft
$ACCESS_RESTRICTED No
*/

#pragma once

#include <CppCallMap.h>
#include <GUIDvalue.h>
#include <unordered_map>

class CppCallStream;
class IHFDMmain;

enum IHFDM_Start_Flags
{
   IHFDM_Do_Execution = 0x1
   , IHFDM_Do_Recording = 0x2
   , IHFDM_Do_Playback = 0x4
};

class CppCallRecorder : public std::enable_shared_from_this<CppCallRecorder>
{
   std::unique_ptr<CppCallStream>         m_callStream;
   std::string                           m_outputfilename;
   static std::shared_ptr<IHFDMmain>      s_IHFDMmain;

public:
   static std::shared_ptr<IHFDMmain> start_IHFDM(unsigned int, std::string jsonfilename);
   static void finish_IHFDM();

   CppCallRecorder();
   ~CppCallRecorder();

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
   void startRecording();
   void finishRecording();

   // Utilities
private:
   static bool fileExists(std::string ss);
};

std::shared_ptr<CppCallRecorder> CppCallRecordingManager();
