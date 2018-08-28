/*
 * Copyright 2018 Autodesk
 */

#pragma once

// IMain.hpp

#include "CppPlayer.hpp"
#include "CppRecorder.hpp"
#include "IOperational.hpp"
#include "NamespaceAliases.hpp"

enum IMain_Start_Flags
{
   Do_Execution = 0x1
   , Do_Recording = 0x2
   , Do_Playback = 0x4
};

class IMain : public IOperational
{
protected:
   CallMap&            m_callMap;
   CppPlayer           Player;
   CppRecorder         Recorder;
   fs::path            FilePath;

   // Construction
public:
   IMain(CallMap& callMap)
      : m_callMap(callMap)
      , Player { callMap }
      , Recorder{ callMap }
   {}
   virtual ~IMain() = 0
   {}

   unsigned int& StartFlags()
   {
      return m_callMap.StartFlags;
   }
   void startCpp(unsigned int nFlags)
   {
      StartFlags() = nFlags;
   }
   void recordCpp(std::string filename)
   {
      Assert(
         !!(Do_Recording & StartFlags())
         , Assertions_StartFlagsProblem);

      FilePath = filename;
      bool bExists = fs::exists(FilePath);
      Assert(!bExists, Assertions_FileCannotBeCreated);

      Recorder.startRecording(FilePath);
   }
   void playbackCpp(std::string filename)
   {
      Assert(
         !!(Do_Playback & StartFlags())
         , Assertions_StartFlagsProblem);

      FilePath = filename;
      bool bExists = fs::exists(FilePath);
      Assert(bExists, Assertions_FileDoesNotExist);

      Player.playbackCppCalls(FilePath, shared_from_this());
   }
   void finishCpp()
   {
      Recorder.finishRecording(objectKey);
      Player.finishPlayback();
   }
};
