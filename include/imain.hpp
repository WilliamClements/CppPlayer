/*
 * Copyright 2018 Autodesk
 */

#pragma once

#include <ITrackable.hpp>
#include <CppCallRecorder.hpp>
#include <CppCallPlayer.hpp>

// Root class making it possible to address the automation wrappers
class IMain : public ITrackable
{
   // Construction
public:
   IMain()
   {}
   virtual ~IMain()
   {}

   void executive()
   {
      CppCallRecorder().startRecording();
      CppCallPlayer().playbackCppCalls("D:\\SampleRecording.json", [](std::shared_ptr<std::exception>) {});
   }
};

std::shared_ptr<IMain> getMain()
{
   static auto imain = std::make_shared<IMain>();
   return imain;
}