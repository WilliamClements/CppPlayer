/*
 * Copyright 2018 Autodesk
 */

#pragma once

#include <chrono>
#include <functional>

using time_point
   = std::chrono::system_clock::time_point;
using PlayerOfOneCall
   = std::function<void(int nFields)>;
using OnStartPlaying
   = std::function<void()>;

struct CppCallFileHeader
{
   time_point                  m_startTime;
   time_point                  m_finishTime;
   uint64_t                    m_numCppCallsRecorded;
   std::string                 m_mainId;
};

// Interface for the CppCallStreams to read and write to files; e.g. via JSON
class ICppCallIo
{
public:
   virtual ~ICppCallIo()       {}

   virtual CppCallFileHeader   getFileHeader() = 0;
   virtual void                setFileHeader(CppCallFileHeader) = 0;

   virtual void                startRecording() = 0;
   virtual void                finishRecording(fs::path outputfilepath) = 0;

   virtual void                playbackAll(fs::path inputFileName, OnStartPlaying cb, PlayerOfOneCall playerOfOne) = 0;

   virtual void                pushHeader() = 0;
   virtual void                pushInt(int64_t) = 0;
   virtual void                pushString(std::string) = 0;
   virtual void                pushDouble(double) = 0;

   virtual void                pushCurrentCall() = 0;

   virtual int64_t             popInt() const = 0;
   virtual std::string         popString() const = 0;
   virtual double              popDouble() const = 0;
};