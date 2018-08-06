/*
$CODE_OWNER William_Clements
$SECONDARY_OWNERS Dmitry_Brumberg David_Becroft
$ACCESS_RESTRICTED No
*/

#include <stdafx.h>
// CppCallPlayer.cpp
#include <CppCallPlayer.h>

#include <ArgsReader.h>
#include <CppCallStream.h>
#include <CRSError.h>
#include <ctime>
#include <IHFDMmain.h>
#include <CppCallRecorder.h>
#include <time.h>

#undef max
#include <propertysets/hfdm.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
namespace CppCallPlayerFile
{
   static char THIS_FILE[] = __FILE__;
}
#define THIS_FILE CppCallPlayerFile::THIS_FILE
#endif
#define NEW_IS_REDEFINED 1

CppCallPlayer s_player;

// This is the complete calling sequence for playback
#if defined(IHFDM_HOME)
IHFDMAPI
#endif
void playbackCppCalls(std::string filename, IHFDMLoginCallback uiLoginFun)
{
   s_player.playbackCppCalls(filename, uiLoginFun);
}

// -------------------------------------------------------------------------------

void CppCallPlayer::initializeForPlayback(IHFDMLoginCallback uiLoginFun)
{
   m_callStream = std::make_unique<CppCallStream>();
   IHFDMtrackable::Recorder = CppCallRecordingManager();
   m_pIHFDMmain = std::make_shared<IHFDMmain>();
   m_pIHFDMmain->registerLoginCallback(uiLoginFun);
}

void CppCallPlayer::startPlayback()
{
   cppCallStream().onStartPlayback();
   TRACE1("adding to aliased \"%hs\"\n", cppCallStream().mainId().c_str());
   cppCallStream().aliased()[cppCallStream().mainId()] = m_pIHFDMmain;
}

void CppCallPlayer::finishPlayback()
{
   IHFDMtrackable::reportInfo(ERR_SUCCESS, "entering finishPlayback .. destroy m_PlaybackState\n");
   cppCallStream().finishPlayback();
   IHFDMtrackable::reportInfo(ERR_SUCCESS, "continuing finishPlayback .. destroy cppCallStream()\n");
   m_callStream.reset();
   IHFDMtrackable::reportInfo(ERR_SUCCESS, "exited finishPlayback\n");
   m_pIHFDMmain.reset();
}

void CppCallPlayer::playbackOne(int nFields)
{
   ArgsReader ar(cppCallStream());
   // scan
   ar.popHeader();
   // execute
   ar.callCpp(nFields);
}

void CppCallPlayer::playbackCppCalls(std::string filename, IHFDMLoginCallback uiLoginFun)
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
   finishPlayback();
}