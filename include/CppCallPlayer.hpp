/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CppCallPlayer.hpp

#include <ArgsReader.hpp>
#include <CppCallPlayerInterface.hpp>
#include <CppCallStream.hpp>
#include <ITrackable.hpp>

class IMain;

class CppCallPlayer final
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
      ITrackable::reportInfo(ERR_SUCCESS, "entering finishPlayback .. destroy m_PlaybackState\n");
      cppCallStream().finishPlayback();
      ITrackable::reportInfo(ERR_SUCCESS, "continuing finishPlayback .. destroy cppCallStream()\n");
      ITrackable::reportInfo(ERR_SUCCESS, "exited finishPlayback\n");
   }

   void execute(std::string);
};