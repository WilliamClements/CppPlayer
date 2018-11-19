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

// IIo.hpp -- pure interface that deals with call I/O

#pragma once

#include <chrono>
#include <functional>
#include "NamespaceAliases.hpp"
#include <variant>

class ITrackable;

using time_point = std::chrono::system_clock::time_point;
using PlayerOfOneCall = std::function<void(int nFields)>;
using OnStartPlaying = std::function<void()>;

// Function returns, live or from stream, are represented as variant
using ReturnVariant =
   std::variant<
      std::monostate
      , int64_t
      , double
      , std::string
      , std::vector<std::string>
      , std::shared_ptr<ITrackable>
   >;

struct CppFileHeader
{
   time_point                  m_startTime;
   time_point                  m_finishTime;
   uint64_t                    m_numCallsRecorded = 0;
   std::string                 m_mainId;
};

// Interface for CallStream to read and write to files; e.g. via JSON
class IIo
{
public:
	virtual ~IIo() {}

   virtual CppFileHeader       getFileHeader() = 0;
   virtual void                setFileHeader(CppFileHeader) = 0;

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
   virtual ReturnVariant       popVariant() const = 0;
};
