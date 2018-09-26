/*

Copyright 2018 William Clements, Autodesk

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

// JsonIo.hpp - concrete JSON implementation of the IIo interface

#pragma once

#include "Assertions.hpp"
#include "IIo.hpp"
#include "NamespaceAliases.hpp"

#undef min
#undef max

#define RAPIDJSON_HAS_CXX11_NOEXCEPT 1
#define RAPIDJSON_HAS_CXX11_RANGE_FOR 1
#define RAPIDJSON_HAS_CXX11_RVALUE_REFS 1
#define RAPIDJSON_HAS_CXX11_TYPETRAITS 1
#define RAPIDJSON_HAS_STDSTRING 1
#define RAPIDJSON_NOMEMBERITERATORCLASS 1

#pragma warning(push)
#pragma warning(disable: 4995)
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#pragma warning(pop)

using Allocator = rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>;
using UTF8 = rapidjson::UTF8<char>;

// data extracted from JSON for one call
class PlayData
{
public:
   rapidjson::Value k_Args;
   int k_counter{ 0 };
   PlayData(rapidjson::Value&& inArgs)
      : k_Args(std::move(inArgs))
   {}

   void popValue(rapidjson::Value* pValue)
   {
      *pValue = k_Args.GetArray()[k_counter++];
   }
};

// implements the IIo interface with JSON I/O
class JsonIo final : public IIo
{
   rapidjson::Document              m_dom;
   rapidjson::Value                 m_jsonHeader;
   rapidjson::Value                 m_jsonBody;
   rapidjson::Value                 m_vCalls;
   rapidjson::Value                 m_currentCall;
   int64_t                          m_callCounter = 0;
   std::unique_ptr<PlayData>        m_oPlayData;

private:
   auto& playData() const
   {
      return *m_oPlayData.get();
   }
   static auto& json_allocator()
   {
      static Allocator allor;
      return allor;
   }

public:
   JsonIo()
   {
      m_dom.SetObject();
      m_jsonHeader.SetObject();
      m_jsonBody.SetArray();
      m_vCalls.SetArray();
   }
   ~JsonIo()
   {}

   CppFileHeader getFileHeader() override
   {
      time_point dummyStart = std::chrono::system_clock::now();
      time_point dummyFinish = std::chrono::system_clock::now();
      rapidjson::Value header = std::move(m_dom["Header"]);
      rapidjson::Value body = std::move(m_dom["Body"]);

      uint64_t llCalls = m_jsonHeader["Record Count"].GetInt64();
      std::string mainId = m_jsonHeader["Main"].GetString();
      return CppFileHeader{ dummyStart, dummyFinish, llCalls, mainId };
   }
   void setFileHeader(CppFileHeader header) override
   {
      auto [tStart, tFinish, nRecords, mainId] = header;
      
      m_dom["Header"].AddMember(rapidjson::Value("Start Time")       , rapidjson::Value(formatTime(tStart), json_allocator()), json_allocator());
      m_dom["Header"].AddMember(rapidjson::Value("Finish Time")      , rapidjson::Value(formatTime(tFinish), json_allocator()), json_allocator());
      m_dom["Header"].AddMember(rapidjson::Value("Record Count")     , rapidjson::Value().SetInt64(nRecords), json_allocator());
      m_dom["Header"].AddMember(rapidjson::Value("Main")             , rapidjson::Value(mainId, json_allocator()), json_allocator());
   }
   void startRecording() override
   {
      // BTW I tried Reserving a low number to make sure array relocation was not a problem
      // Be aware.
      m_vCalls.Reserve(8192, json_allocator());

      // Use temporaries. AddMember eats both of its arguments.
      {
         rapidjson::Value tempHeader;
         tempHeader.SetObject();
         m_dom.AddMember(rapidjson::Value("Header"), tempHeader, json_allocator());
      }
      {
         rapidjson::Value tempBody;
         tempBody.SetArray();
         m_dom.AddMember(rapidjson::Value("Body"), tempBody, json_allocator());
      }
   }
   void finishRecording(fs::path outputfilepath) override
   {
      m_dom["Body"] = std::move(m_vCalls);

      stringifyCpp(m_dom, outputfilepath);
   }
   void playbackAll(fs::path inputFileName, OnStartPlaying cb, PlayerOfOneCall playerOfOne) override
   {
      parseCpp(m_dom, inputFileName);

      m_jsonHeader = std::move(m_dom["Header"]);
      m_jsonBody = std::move(m_dom["Body"]);

      // Notify caller we are about to get down to business
      // and that m_jsonHeader can now be perused.-
      cb();

      // This is the playback loop!
      for (rapidjson::Value& record : m_jsonBody.GetArray())
         playbackOne(std::move(record), playerOfOne);
   }
   void playbackOne(rapidjson::Value&& vArgs, PlayerOfOneCall playerOfOne)
   {
      int nFields = vArgs.Size();
      m_oPlayData = std::make_unique<PlayData>(std::move(vArgs));
      playerOfOne(nFields);
   }
   void pushHeader() override
   {
      m_currentCall.SetArray();
   }
   void pushInt(int64_t nn) override
   {
      rapidjson::Value item;
      item.SetInt64(nn);
      m_currentCall.PushBack(std::move(item), json_allocator());
   }
   void pushString(std::string ss) override
   {
      rapidjson::Value item(ss, json_allocator());
      m_currentCall.PushBack(std::move(item), json_allocator());
   }
   void pushDouble(double dd) override
   {
      rapidjson::Value item;
      item.SetDouble(dd);
      m_currentCall.PushBack(std::move(item), json_allocator());
   }
   void pushCurrentCall() override
   {
      ++m_callCounter;
      m_vCalls.PushBack(std::move(m_currentCall), json_allocator());
   }
   int64_t popInt() const override
   {
      rapidjson::Value target;
      playData().popValue(&target);
      assert(target.IsInt64());
      return target.GetInt();
   }
   std::string popString() const override
   {
      rapidjson::Value target;
      playData().popValue(&target);
      assert(target.IsString());
      std::string ret = target.GetString();
      // For voluminous but nearly complete diagnostics:
      // TRACE1("popString \"%hs\"\n", ret.c_str());
      return ret;
   }
   double popDouble() const override
   {
      rapidjson::Value target;
      playData().popValue(&target);
      assert(target.IsDouble());
      return target.GetDouble();
   }
   ReturnVariant popVariant() const override
   {
      ReturnVariant ret;
      rapidjson::Value target;
      playData().popValue(&target);
      if (target.IsArray())
      {
         std::vector<std::string> unpack;
         for (auto& member : target.GetArray())
         {
            Assert(member.IsString(), Assertions::UnsupportedArgumentType);
            unpack.push_back(member.GetString());
         }
         ret = unpack;
      }
      else if (target.IsString())
      {
         ret = target.GetString();
      }
      else if (target.IsDouble())
      {
         ret = target.GetDouble();
      }
      else if (target.IsInt64())
      {
         ret = target.GetInt64();
      }
      Assert(!std::holds_alternative<std::monostate>(ret), Assertions::UnsupportedArgumentType);
      return ret;
   }

   static std::string formatTime(time_point tp)
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

   // Writes string representation of the JSON document to a file.
   static void stringifyCpp(const rapidjson::Document& dom, fs::path filepath)
   {
      FILE* fp = nullptr;
      std::string filename = filepath.parent_path().generic_string();
      filename += "\\";
      filename += filepath.filename().generic_string();
      errno_t bad = fopen_s(&fp, filename.c_str(), "wb"); // non-Windows use "w"
      Assert(!bad, Assertions::FileCannotBeCreated, filename);
      // good open, continue
      char writeBuffer[65536];
      rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
      rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
      dom.Accept(writer);
      fclose(fp);
   }
   // Reads string representation from file and populates JSON document. 
   static void parseCpp(rapidjson::Document& dom, fs::path filepath)
   {
      FILE* fp = nullptr;
      std::string filename = filepath.parent_path().generic_string();
      filename += "\\";
      filename += filepath.filename().generic_string();
      errno_t bad = fopen_s(&fp, filename.c_str(), "rb"); // non-Windows use "r"
      Assert(!bad, Assertions::FileDoesNotExist, filename);
      // good open, continue
      char readBuffer[65536];
      rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
      (void)dom.ParseStream<rapidjson::ParseFlag::kParseDefaultFlags, UTF8, rapidjson::FileReadStream>(is);
      fclose(fp);
   }
};

std::unique_ptr<IIo> makeIo()
{
   return std::make_unique<JsonIo>();
}
