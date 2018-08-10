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
   CppCallRecorder         Recorder;
   CppCallPlayer           Player;
   fs::path                FilePath;
   unsigned int            StartFlags;

   // Construction
public:
   IMain()
   {}
   virtual ~IMain()
   {}

   void start(unsigned int nFlags, std::string filename)
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
   void finish()
   {
      Recorder.finishRecording();
      Player.finishPlayback();
   }
};

std::shared_ptr<IMain> getMain()
{
   static auto imain = std::make_shared<IMain>();
   return imain;
}

void cppCallFramework_start(unsigned int nFlags, std::string filename)
{
   getMain()->start(nFlags, filename);
}

void cppCallFramework_finish()
{
   getMain()->finish();
}