/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CppPlayer.hpp

#include <ArgsReader.hpp>
#include <CppCallError.hpp>
#include <IPlayer.hpp>
#include <CppCallStream.hpp>
#include <ITrackable.hpp>

class IMain;

class CppPlayer final : public IPlayer
{
   mutable CppCallStream m_callStream;

public:
   CppPlayer()
   {}
     
   CppCallStream& cppCallStream() const
   {
      return m_callStream;
   }
   void playbackCppCalls(fs::path filepath, std::shared_ptr<ITrackable> pIMain)
   {
      cppCallStream().startStreaming();
      cppCallStream().io().playbackAll(
         filepath
         , [this, pIMain]()
      {
         this->startPlayback(pIMain);
      }
         , [this](int nFields)
      {
         this->playbackOne(nFields);
      });
   }
   void playbackOne(int nFields)
   {
      ArgsReader ar(cppCallStream());
      // scan
      ar.popHeader();
      // execute
      ar.callCpp(nFields);
   }

   void startPlayback(std::shared_ptr<ITrackable> pIMain)
   {
      cppCallStream().onStartPlayback();
      cppCallStream().aliased()[cppCallStream().mainId()] = pIMain;
   }
   void finishPlayback()
   {
      CppCallInfo::reportInfo(ERR_SUCCESS, "entering finishPlayback .. destroy m_PlaybackState\n");
      cppCallStream().finishPlayback();
      CppCallInfo::reportInfo(ERR_SUCCESS, "continuing finishPlayback .. destroy cppCallStream()\n");
      CppCallInfo::reportInfo(ERR_SUCCESS, "exited finishPlayback\n");
   }

   void execute(std::string);
};
