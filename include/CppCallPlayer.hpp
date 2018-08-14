/*
 * Copyright 2018 Autodesk
 */

#pragma once
#pragma message("WOX -> CppCallPlayer")

// CppCallPlayer.hpp

#include <ArgsReader.hpp>
#include <CppCallError.hpp>
#include <CppCallPlayerInterface.hpp>
#include <CppCallStream.hpp>
#include <ITrackable.hpp>

class IMain;

class CppCallPlayer final : public CppCallPlayerInterface
{
   mutable CppCallStream m_callStream;

public:
   CppCallPlayer()
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

#pragma message("WOX <- CppCallPlayer")