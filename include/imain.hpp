/*
 * Copyright 2018 Autodesk
 */

#pragma once

#include <Aliases.hpp>
#include <CppCallRecorder.hpp>
#include <CppCallPlayer.hpp>

enum IMain_Start_Flags
{
   Do_Execution = 0x1
   , Do_Recording = 0x2
   , Do_Playback = 0x4
};

class IMain : public ITrackable
{
   CppCallPlayer           Player;
   CppCallRecorder         Recorder;
   fs::path                FilePath;
   unsigned int            StartFlags;

   // Construction
public:
   IMain()
   {
      pplayer() = &Player;
      precorder() = &Recorder;
   }
   virtual ~IMain() = 0
   {
      pplayer() = nullptr;
      precorder() = nullptr;
   }

   void startCpp(unsigned int nFlags, std::string filename)
   {
      FilePath = filename;
      bool bExists = fs::exists(FilePath);
      if (Do_Recording & nFlags)
      {
         failUnlessPredicate(!bExists, CppCallError_FileCannotBeCreated);
         Recorder.startRecording(FilePath);
      }
      else if (Do_Playback & nFlags)
      {
         failUnlessPredicate(bExists, CppCallError_FileDoesNotExist);
         Player.playbackCppCalls(FilePath, shared_from_this());
      }
   }
   void finishCpp()
   {
      Recorder.finishRecording();
      Player.finishPlayback();
   }
};