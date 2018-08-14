/*
 * Copyright 2018 Autodesk
 */

#pragma once
#pragma message("WOX -> IMain")

// IMain.hpp

#include <Aliases.hpp>
#include <CppCallRecorder.hpp>
#include <CppCallPlayer.hpp>
#include <ICallable.hpp>

enum IMain_Start_Flags
{
   Do_Execution = 0x1
   , Do_Recording = 0x2
   , Do_Playback = 0x4
};

class IMain : public ICallable
{
   CppCallPlayer           Player;
   CppCallRecorder         Recorder;
   unsigned int            StartFlags;
   fs::path                FilePath;

   // Construction
public:
   IMain()
   {
      p_player() = &Player;
      p_recorder() = &Recorder;
   }
   virtual ~IMain() = 0
   {
      p_player() = nullptr;
      p_recorder() = nullptr;
   }

   void startCpp(unsigned int nFlags)
   {
      StartFlags = nFlags;
   }
   void recordCpp(std::string filename)
   {
      failUnlessPredicate(
         !!(Do_Recording & StartFlags)
         , CppCallError_StartFlagsProblem);

      FilePath = filename;
      bool bExists = fs::exists(FilePath);
      failUnlessPredicate(!bExists, CppCallError_FileCannotBeCreated);

      Recorder.startRecording(FilePath);
   }
   void playbackCpp(std::string filename)
   {
      failUnlessPredicate(
         !!(Do_Playback & StartFlags)
         , CppCallError_StartFlagsProblem);

      FilePath = filename;
      bool bExists = fs::exists(FilePath);
      failUnlessPredicate(bExists, CppCallError_FileDoesNotExist);

      Player.playbackCppCalls(FilePath, shared_from_this());
   }
   void finishCpp()
   {
      Recorder.finishRecording(objectKey);
      Player.finishPlayback();
   }
};

#pragma message("WOX <- IMain")