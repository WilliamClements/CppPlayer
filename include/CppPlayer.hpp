/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CppPlayer.hpp

#include "ArgsReader.hpp"
#include "Assertions.hpp"
#include "IPlayer.hpp"
#include "ITrackable.hpp"

class CppPlayer final : public IPlayer
{
public:
   CppPlayer(CallMap& callMap)
      : IPlayer(callMap)
   {}
     
   void playbackCppCalls(fs::path filepath, std::shared_ptr<ITrackable> pRoot)
   {
      callStream().startStreaming();
      callStream().io().playbackAll(
         filepath
         , [this, pRoot]()
      {
         this->startPlayback(pRoot);
      }
         , [this](int nFields)
      {
         nFields;
         this->playbackOne();
      });
   }
   void playbackOne()
   {
      ArgsReader ar(callStream());
      // scan
      ar.popHeader();
      // execute
      ar.invoke();
   }

   void startPlayback(std::shared_ptr<ITrackable> pIMain)
   {
      callStream().onStartPlayback();
      callStream().swizzled()[callStream().mainId()] = pIMain;
   }
   void finishPlayback()
   {
      CppCallInfo::reportInfo(Err(0), "entering finishPlayback .. destroy m_PlaybackState\n");
      callStream().finishPlayback();
      CppCallInfo::reportInfo(Err(0), "continuing finishPlayback .. destroy callStream()\n");
      CppCallInfo::reportInfo(Err(0), "exited finishPlayback\n");
   }
};
