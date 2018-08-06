/*
$CODE_OWNER William_Clements
$SECONDARY_OWNERS David_Becroft, Dmitry_Brumberg
$ACCESS_RESTRICTED No
*/
#pragma once

#include <ICppCallIo.h>

#include <include_purerapidjson.h>

class PlayData;

// implements the CppCallIo interface with JSON I/O
class CppCallIO_Json final : public ICppCallIo
{
   JSON::Document              m_dom;
   JSON::Value                 m_jsonHeader;
   JSON::Value                 m_jsonBody;
   JSON::Value                 m_vCalls;
   JSON::Value                 m_currentCall;
   int64_t                     m_callCounter;
   std::unique_ptr<PlayData>   m_oPlayData;

public:
   CppCallIO_Json();
   ~CppCallIO_Json();

   CppCallFileHeader           getFileHeader() override;
   void                        setFileHeader(CppCallFileHeader) override;

   void                        startRecording() override;
   void                        finishRecording(std::string outputfilename) override;

   void                        playbackAll(std::string inputFileName, OnStartPlaying cb, PlayerOfOneCall playerOfOne) override;
   void                        playbackOne(JSON::Value&& vArgs, PlayerOfOneCall playerOfOne);

   void                        pushHeader() override;
   void                        pushInt(int64_t) override;
   void                        pushString(std::string) override;
   void                        pushDouble(double) override;
   void                        pushCurrentCall() override;

   int64_t                     popInt() const override;
   std::string                 popString() const override;
   double                      popDouble() const override;

private:
   PlayData&                   playData() const
   {
      return *m_oPlayData.get();
   }

private:
   static JSON::Allocator&     json_allocator();
   static std::string          formatTime(time_point);
};

class PlayData
{
public:
   JSON::Value k_Args;
   int k_counter{ 0 };
   PlayData(JSON::Value&& inArgs)
      : k_Args(std::move(inArgs))
   {}

   void popValue(JSON::Value* pValue)
   {
      *pValue = k_Args.GetArray()[k_counter++];
   }
};