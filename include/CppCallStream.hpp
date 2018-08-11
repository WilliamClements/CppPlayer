/*
 * Copyright 2018 Autodesk
 */

#pragma once

#include <CppCallMap.hpp>
#include <CppCallIO_Json.hpp>
#include <GUIDvalue.hpp>
#include <ICppCallIo.hpp>
#include <unordered_map>

class ITrackable;

using Aliased
   = std::unordered_map<std::string, std::shared_ptr<ITrackable>, std::hash<std::string>>;
using AliasedURNs
   = std::unordered_map<std::string, std::string, std::hash<std::string>>;

class CppCallStream final
{
   std::unique_ptr<ICppCallIo>       m_io;
   CppCallFileHeader                 m_fileheader;
   Aliased                           m_aliased;
   AliasedURNs                       m_aliasedURNs;
   uint64_t                          m_u64CallsCounter;
   uint64_t                          m_u64CppCallsPreRecorded;
   std::string                       m_mainId;

public:
   CppCallStream()
      : m_u64CallsCounter()
      , m_u64CppCallsPreRecorded()
      , m_mainId()
      , m_io()
   {}

   ~CppCallStream()
   {}

   ICppCallIo& io()                  { return *m_io.get(); }
   Aliased& aliased()                { return m_aliased; }
   AliasedURNs& aliasedURNs()        { return m_aliasedURNs; }
   uint64_t& callsCounter()          { return m_u64CallsCounter; }
   std::string& mainId()             { return m_mainId; }

public:
   bool streaming() const
   {
      return !!m_io;
   }
   void startStreaming()
   {
      m_io = makeIo();
   }
   void startRecording()
   {
      startStreaming();
      m_fileheader.m_startTime = std::chrono::system_clock::now();
      m_io->startRecording();
   }
   void finishRecording(std::string mainId, fs::path outputfilepath)
   {
      if (m_io)
      {
         m_fileheader.m_finishTime = std::chrono::system_clock::now();
         m_fileheader.m_numCppCallsRecorded = callsCounter();
         m_fileheader.m_mainId = mainId;
         m_io->setFileHeader(m_fileheader);
         m_io->finishRecording(outputfilepath);
      }
   }
   void onStartPlayback()
   {
      m_fileheader = m_io->getFileHeader();
      m_u64CppCallsPreRecorded = m_fileheader.m_numCppCallsRecorded;
      m_mainId = m_fileheader.m_mainId;
   }
   void finishPlayback()
   {}
   static std::unique_ptr<ICppCallIo> makeIo()
   {
      return std::make_unique<CppCallIO_Json>();
   }
   std::string breakStringVector(const std::vector<std::string>& vs)
   {
      std::string ret = "{";
      bool bFirst = true;
      for (const auto& id : vs)
      {
         if (!bFirst)
            ret += ",";
         ret += id;
         bFirst = false;
      }
      ret += "}";
      return ret;
   }
   std::vector<std::string> recomposeStringVector(std::string ss)
   {
      std::vector<std::string> ret;
      size_t totalLength = ss.size();
      size_t currentPos = 0;
      while ((currentPos < totalLength) && ('}' != ss[currentPos]))
      {
         if (0 == currentPos)
         {
            if ('{' != ss[currentPos])
               break;
            ++currentPos;
         }
         else
         {
            size_t nextPos = ss.find_first_of(",}", currentPos);
            if (std::string::npos == nextPos)
               break;
            ret.push_back(ss.substr(currentPos, nextPos - currentPos));
            currentPos = nextPos + 1;
         }
      }
      return ret;
   }
};