/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CppCallRecorderImpl.hpp

#include <CppCallRecorder.h>

#include <CppCallError.h>
#include <CppCallStream.h>
#include <ctime>
#include <IHFDMmain.h>
#include <sstream>
#include <time.h>

static std::shared_ptr<CppCallRecorder> s_CppCallRecorder;
std::shared_ptr<IHFDMmain> CppCallRecorder::s_IHFDMmain;

/* static */
std::shared_ptr<IHFDMmain> CppCallRecorder::start_IHFDM(unsigned int flags, std::string jsonfilename)
{
   // These flags are for the whole session
   s_IHFDMmain = std::make_shared<IHFDMmain>();
   ITrackable::StartFlags = flags;
   // instantiate our singleton
   (void)CppCallRecordingManager();
   CppCallRecordingManager()->start_IHFDM_private(jsonfilename);
   return s_IHFDMmain;
}

void CppCallRecorder::start_IHFDM_private(std::string jsonfilename)
{
   if ((Do_Recording | Do_Playback) & ITrackable::StartFlags)
   {
      failUnlessPredicate(!jsonfilename.empty() && fileExists(jsonfilename), CppCallError_FileDoesNotExist);
      this->m_outputfilename = jsonfilename;
   }

   if (Do_Recording & ITrackable::StartFlags)
   {
      ITrackable::Recorder = shared_from_this();
      this->startRecording();
   }
}

/* static */
void CppCallRecorder::finish_IHFDM()
{
   if (Do_Recording & ITrackable::StartFlags)
   {
      CppCallRecordingManager()->finishRecording();
   }
   ITrackable::Recorder.reset();
   s_CppCallRecorder.reset();
}

void CppCallRecorder::startRecording()
{
   m_callStream = std::make_unique<CppCallStream>();
   cppCallStream().startRecording();
}

void CppCallRecorder::finishRecording()
{
   cppCallStream().finishRecording(s_IHFDMmain->m_objectKey, m_outputfilename);
   m_callStream.reset();
}

CppCallRecorder::CppCallRecorder()
   : m_outputfilename()
{}
CppCallRecorder::~CppCallRecorder()
{}

/* static */
bool CppCallRecorder::fileExists(std::string ss)
{
   CFileStatus localStatus;
   std::wstring ws(ss.length(), L' ');
   std::copy(ss.begin(), ss.end(), ws.begin());
   bool bStatusValid = !!CFile::GetStatus(ws.c_str(), localStatus);
   return (bStatusValid && !(localStatus.m_attribute & CFile::directory));
}

std::shared_ptr<CppCallRecorder> CppCallRecordingManager()
{
   if (!s_CppCallRecorder)
      s_CppCallRecorder = std::make_shared<CppCallRecorder>();
   return s_CppCallRecorder;
}