/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CppCallPlayer.hpp

#include <CppCallPlayerInterface.hpp>
#include <IMain.hpp>
#include <ITrackable.hpp>
#include <ArgsReader.hpp>

class CppCallStream;
class GUIDvalue;
class IMain;
class ITrackable;

class CppCallPlayer final
{
   std::unique_ptr<CppCallStream> m_callStream;
   std::shared_ptr<IMain>     m_pIMain;

public:
   CppCallStream& cppCallStream() const
   {
      return *m_callStream.get();
   }
   void playbackCppCalls(std::string filename, ITrackableCallback uiLoginFun)
   {
      initializeForPlayback(uiLoginFun);
      cppCallStream().io().playbackAll(
         filename
         , [this]()
      {
         this->startPlayback();
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
   void initializeForPlayback(ITrackableCallback uiLoginFun)
   {
      m_callStream = std::make_unique<CppCallStream>();
      ITrackable::Recorder = CppCallRecordingManager();
      m_pIMain = std::make_shared<IMain>();
      // WOX m_pIMain->registerLoginCallback(uiLoginFun);
   }

   void startPlayback()
   {
#if WOX
      cppCallStream().onStartPlayback();
      cppCallStream().aliased()[cppCallStream().mainId()] = m_pIMain;
#endif
   }
   void finishPlayback()
   {
      ITrackable::reportInfo(ERR_SUCCESS, "entering finishPlayback .. destroy m_PlaybackState\n");
      cppCallStream().finishPlayback();
      ITrackable::reportInfo(ERR_SUCCESS, "continuing finishPlayback .. destroy cppCallStream()\n");
      m_callStream.reset();
      ITrackable::reportInfo(ERR_SUCCESS, "exited finishPlayback\n");
      m_pIMain.reset();
   }

   void execute(std::string);
};