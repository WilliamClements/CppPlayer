/*
 * Copyright 2018 Autodesk
 */

#pragma once

#include <GUIDvalue.h>
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
   CppCallStream();
   ~CppCallStream();

   ICppCallIo& io()                         { return *m_io.get(); }
   Aliased& aliased()                       { return m_aliased; }
   AliasedURNs& aliasedURNs()               { return m_aliasedURNs; }
   uint64_t& callsCounter()                 { return m_u64CallsCounter; }
   std::string& mainId()                    { return m_mainId; }

public:
   void startRecording();
   void finishRecording(std::string mainId, std::string outputfilename);

   void onStartPlayback();
   void finishPlayback();

   static std::unique_ptr<ICppCallIo> makeIo();
   std::string breakStringVector(const std::vector<std::string>& vs);
   std::vector<std::string> recomposeStringVector(std::string ss);
};