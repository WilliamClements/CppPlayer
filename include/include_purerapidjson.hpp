/*
 * Copyright 2018 Autodesk
 */

#pragma once

#include <Aliases.hpp>

#undef min
#undef max

#define RAPIDJSON_HAS_CXX11_NOEXCEPT 1
#define RAPIDJSON_HAS_CXX11_RANGE_FOR 1
#define RAPIDJSON_HAS_CXX11_RVALUE_REFS 1
#define RAPIDJSON_HAS_CXX11_TYPETRAITS 1
#define RAPIDJSON_HAS_STDSTRING 1

#pragma warning(push)
#pragma warning(disable: 4995)
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#pragma warning(pop)

//--------------------------------------------------------------------
namespace JSON
{
   // simple "aliasing" of namespace items rapidjson -> JSON
   using CrtAllocator
      = rapidjson::CrtAllocator;
   using Document
      = rapidjson::Document;
   using Value
      = rapidjson::Value;
   using SizeType
      = rapidjson::SizeType;
   using StringBuffer
      = rapidjson::StringBuffer;
   using FileReadStream
      = rapidjson::FileReadStream;
   using FileWriteStream
      = rapidjson::FileWriteStream;
   template<class _StreamType>
   using Writer
      = rapidjson::Writer<_StreamType>;
   using ParseFlag
      = rapidjson::ParseFlag;

   // These are slightly more opinionated
   using MemoryPoolAllocator
      = rapidjson::MemoryPoolAllocator<CrtAllocator>;
   using Allocator
      = rapidjson::MemoryPoolAllocator<CrtAllocator>;
   using UTF8
      = rapidjson::UTF8<char>;
   using BasicPrettyWriter
      = rapidjson::PrettyWriter<StringBuffer, UTF8, UTF8, CrtAllocator>;
   using ConstArrayOfValue
      = rapidjson::GenericArray<true, Value>;
   using ArrayOfValue
      = rapidjson::GenericArray<false, Value>;

   // These are homegrown utilities

   // Writes string representation of the JSON document to a file.
   void stringify(const Document& dom, fs::path filepath)
   {
      FILE* fp = nullptr;
      std::string filename = filepath.parent_path().generic_string();
      filename += filepath.filename().generic_string();
      errno_t bad = fopen_s(&fp, filename.c_str(), "wb"); // non-Windows use "w"
      failUnlessPredicate(!bad, CppCallError_FileCannotBeCreated, filename);
      // good open, continue
      char writeBuffer[65536];
      FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
      Writer<FileWriteStream> writer(os);
      dom.Accept(writer);
      fclose(fp);
   }
   // Reads string representation from file and populates JSON document. 
   void parse(Document& dom, fs::path filepath)
   {
      FILE* fp = nullptr;
      std::string filename = filepath.parent_path().generic_string();
      filename += filepath.filename().generic_string();
      errno_t bad = fopen_s(&fp, filename.c_str(), "rb"); // non-Windows use "r"
      failUnlessPredicate(!bad, CppCallError_FileDoesNotExist, filename);
      // good open, continue
      char readBuffer[65536];
      FileReadStream is(fp, readBuffer, sizeof(readBuffer));
      (void)dom.ParseStream<JSON::ParseFlag::kParseDefaultFlags, UTF8, FileReadStream>(is);
      fclose(fp);
   }
}