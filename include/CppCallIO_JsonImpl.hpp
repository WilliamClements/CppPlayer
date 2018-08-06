/*
$CODE_OWNER William_Clements
$SECONDARY_OWNERS David_Becroft, Dmitry_Brumberg
$ACCESS_RESTRICTED No
*/
#include <stdafx.h>

#include <CppCallError.h>
#include <CppCallIO_Json.h>
#include <CppCallStream.h>
#if !defined(IHFDM_HOME)
#include <StringUtil.h>
#endif

/* static */
std::unique_ptr<ICppCallIo> CppCallStream::makeIo()
{
   return std::make_unique<CppCallIO_Json>();
}

/* static */
JSON::Allocator& CppCallIO_Json::json_allocator()
{
   static JSON::Allocator the;
   return the;
}

CppCallIO_Json::CppCallIO_Json()
   : m_dom()
   , m_jsonHeader()
   , m_jsonBody()
   , m_vCalls()
   , m_currentCall()
   , m_callCounter()
{
   m_dom.SetObject();
   m_jsonHeader.SetObject();
   m_jsonBody.SetArray();
   m_vCalls.SetArray();
}
CppCallIO_Json::~CppCallIO_Json()
{}

CppCallFileHeader   CppCallIO_Json::getFileHeader()
{
   time_point dummyStart = std::chrono::system_clock::now();
   time_point dummyFinish = std::chrono::system_clock::now();
   JSON::Value header = std::move(m_dom["Header"]);
   JSON::Value body = std::move(m_dom["Body"]);

   uint64_t llCalls = m_jsonHeader["Record Count"].GetInt64();
   std::string mainId = m_jsonHeader["Main"].GetString();
   return CppCallFileHeader{ dummyStart, dummyFinish, llCalls, mainId };
}

void                CppCallIO_Json::setFileHeader(CppCallFileHeader header)
{
   JSON::Value startTime(formatTime(header.m_startTime), json_allocator());
   JSON::Value finishTime(formatTime(header.m_finishTime), json_allocator());
   JSON::Value numRecorded;
   numRecorded.SetInt64(header.m_numCppCallsRecorded);
   JSON::Value mainId(header.m_mainId, json_allocator());

   m_dom["Header"].AddMember(JSON::Value("Start Time"),   startTime,                  json_allocator());
   m_dom["Header"].AddMember(JSON::Value("Finish Time"),  finishTime,                 json_allocator());
   m_dom["Header"].AddMember(JSON::Value("Record Count"), numRecorded,                json_allocator());
   m_dom["Header"].AddMember(JSON::Value("Main"),         mainId,                     json_allocator());
}

void                CppCallIO_Json::startRecording()
{
   // BTW I tried Reserving a low number to make sure array relocation was not a problem
   // OTOH I always suppress RepoRequestMinder separate thread if recording
   m_vCalls.Reserve(8192, json_allocator());

   // Use temporaries. AddMember eats both of its arguments.
   {
      JSON::Value tempHeader;
      tempHeader.SetObject();
      m_dom.AddMember(JSON::Value("Header"), tempHeader, json_allocator());
   }
   {
      JSON::Value tempBody;
      tempBody.SetArray();
      m_dom.AddMember(JSON::Value("Body"), tempBody, json_allocator());
   }
}

void                CppCallIO_Json::finishRecording(std::string outputfilename)
{
   m_dom["Body"] = std::move(m_vCalls);

   JSON::stringify(m_dom, outputfilename);
}

void                CppCallIO_Json::playbackAll(std::string inputFileName, OnStartPlaying cb, PlayerOfOneCall playerOfOne)
{
   JSON::parse(m_dom, inputFileName);

   m_jsonHeader = std::move(m_dom["Header"]);
   m_jsonBody = std::move(m_dom["Body"]);

   // Notify caller we are about to get down to business
   // and that m_jsonHeader can now be perused.-
   cb();
   
   // This is the playback loop!
   for (JSON::Value& record : m_jsonBody.GetArray())
      playbackOne(std::move(record), playerOfOne);
}

void CppCallIO_Json::playbackOne(JSON::Value&& vArgs, PlayerOfOneCall playerOfOne)
{
   int nFields = vArgs.Size();
   m_oPlayData = std::make_unique<PlayData>(std::move(vArgs));
   playerOfOne(nFields);
}

void                 CppCallIO_Json::pushHeader()
{
   m_currentCall.SetArray();
}

void                CppCallIO_Json::pushInt(int64_t nn)
{
   JSON::Value item;
   item.SetInt64(nn);

   m_currentCall.PushBack(std::move(item), json_allocator());
}

void                CppCallIO_Json::pushDouble(double dd)
{
   JSON::Value item;
   item.SetDouble(dd);

   m_currentCall.PushBack(std::move(item), json_allocator());
}

void                 CppCallIO_Json::pushString(std::string ss)
{
   JSON::Value item(ss, json_allocator());
   m_currentCall.PushBack(item, json_allocator());
}

void                CppCallIO_Json::pushCurrentCall()
{
   ++m_callCounter;
   m_vCalls.PushBack(std::move(m_currentCall), json_allocator());
}

int64_t             CppCallIO_Json::popInt() const
{
   JSON::Value target;
   playData().popValue(&target);
   DBG_ASSERT(target.IsInt64());
   return target.GetInt();
}
std::string         CppCallIO_Json::popString() const
{
   JSON::Value target;
   playData().popValue(&target);
   DBG_ASSERT(target.IsString());
   std::string ret = target.GetString();
   // For voluminous but nearly complete diagnostics:
   // TRACE1("popString \"%hs\"\n", ret.c_str());
   return ret;
}
double              CppCallIO_Json::popDouble() const
{
   JSON::Value target;
   playData().popValue(&target);
   DBG_ASSERT(target.IsDouble());
   return target.GetDouble();
}

/* static */
std::string         CppCallIO_Json::formatTime(time_point tp)
{
   // Recast time_point as time_t
   time_t tt = std::chrono::system_clock::to_time_t(tp);

   // Create string with formatted time based on the time_t
   char buffer[512];
   (void)ctime_s(buffer, 512, &tt);
   std::string ss = buffer;

   // Trim string, return as JSON item 
   ss = ss.substr(0, ss.length() - 1);
   return ss;
}

namespace JSON
{
   void stringify(const Document& dom, std::string filename)
   {
      FILE* fp = nullptr;
      errno_t bad = fopen_s(&fp, filename.c_str(), "wb"); // non-Windows use "w"
      failUnlessPredicate(!bad, CppCallError_FileCannotBeCreated, filename);
      // good open, continue
      char writeBuffer[65536];
      FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
      Writer<FileWriteStream> writer(os);
      dom.Accept(writer);
      fclose(fp);
   }

   void parse(Document& dom, std::string filename)
   {
      FILE* fp = nullptr;
      errno_t bad = fopen_s(&fp, filename.c_str(), "rb"); // non-Windows use "r"
      failUnlessPredicate(!bad, CppCallError_FileDoesNotExist, filename);
      // good open, continue
      char readBuffer[65536];
      FileReadStream is(fp, readBuffer, sizeof(readBuffer));
      (void)dom.ParseStream<JSON::ParseFlag::kParseDefaultFlags, UTF8, FileReadStream>(is);
      fclose(fp);
   }
}