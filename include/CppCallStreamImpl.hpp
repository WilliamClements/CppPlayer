/*
$CODE_OWNER William_Clements
$SECONDARY_OWNERS Dmitry_Brumberg David_Becroft
$ACCESS_RESTRICTED No
*/

#include <stdafx.h>
// CppCallStream.cpp
#include <CppCallStream.h>

#include <CppCallError.h>
#include <CppCallMap.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
namespace CppCallStreamFile
{
   static char THIS_FILE[] = __FILE__;
}
#define THIS_FILE CppCallStreamFile::THIS_FILE
#endif
#define NEW_IS_REDEFINED 1

CppCallStream::CppCallStream()
   : m_u64CallsCounter()
   , m_u64CppCallsPreRecorded()
   , m_mainId()
   , m_io(makeIo())
{}
CppCallStream::~CppCallStream()
{}

void CppCallStream::startRecording()
{
   m_fileheader.m_startTime = std::chrono::system_clock::now();
   m_io->startRecording();
}

void CppCallStream::finishRecording(std::string mainId, std::string outputfilename)
{
   m_fileheader.m_finishTime = std::chrono::system_clock::now();
   m_fileheader.m_numCppCallsRecorded = callsCounter();
   m_fileheader.m_mainId = mainId;
   m_io->setFileHeader(m_fileheader);
   m_io->finishRecording(outputfilename);
}

void CppCallStream::onStartPlayback()
{
   m_fileheader = m_io->getFileHeader();
   m_u64CppCallsPreRecorded = m_fileheader.m_numCppCallsRecorded;
   m_mainId = m_fileheader.m_mainId;
}

void CppCallStream::finishPlayback()
{}

std::string CppCallStream::breakStringVector(const std::vector<std::string>& vs)
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

std::vector<std::string> CppCallStream::recomposeStringVector(std::string ss)
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
         ret.push_back(ss.substr(currentPos, nextPos-currentPos));
         currentPos = nextPos + 1;
      }
   }
   return ret;
}

void fail(CppCallError err, const CppCallMapEntry* pEntry)
{
   fail(err, pEntry->api());
}

void fail(CppCallError err, std::string ssId)
{
   std::string base = ssId + "/";
   switch (err)
   {
   case CppCallError_UnequalReturnResult:
      throw std::logic_error(base + "result of operation different from recording to playback");
   case CppCallError_DuplicateAPINames:
      throw std::logic_error(base + "only one entry should be registered per apiname");
   case CppCallError_NoSuchAPIName:
      throw std::logic_error(base + "api name lookup failed, did you forget to register a entry for it?");
   case CppCallError_FileCannotBeCreated:
      throw std::logic_error(base + "recording file could not be created");
   case CppCallError_FileDoesNotExist:
      throw std::logic_error(base + "playback file does not exist");
   case CppCallError_NoSuchTarget:
      throw std::logic_error(base + "lookup target failed; i.e. target referenced before it was created");
   case CppCallError_NoSuchTrackable:
      throw std::logic_error(base + "lookup trackable failed; i.e. trackable referenced before it was created");
   case CppCallError_WrongNumberOfFields:
      throw std::logic_error(base + "wrong number of fields pushed or popped");
   case CppCallError_InvalidPath:
      throw std::logic_error(base + "path cannot be correct");
   case CppCallError_NoSuchURN:
      throw std::logic_error(base + "lookup URN failed; i.e. URN referenced before it was created");
   case CppCallError_UnmatchedFulfillVersusReserve:
      throw std::logic_error(base + "tracking mismatch");
   case CppCallError_TrackableMustBeNull:
      throw std::logic_error(base + "non-NULL trackable unexpected");
   // catch all...keep it last
   case CppCallError_OutOfSequence:
      throw std::logic_error(base + "playback sequence did not match recording");
   default:
      throw std::logic_error(base + "please add a description for this error");
   }
}