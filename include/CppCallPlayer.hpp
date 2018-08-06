/*
 * Copyright 2018 Autodesk
 */

#pragma once

// CppCallPlayer.hpp

#include <CppCallPlayerInterface.h>
#include <IHFDMtrackable.h>

class ArgsReader;
class CppCallStream;
class GUIDvalue;
class IHFDMmain;
class IHFDMtrackable;

class CppCallPlayer final
{
   std::unique_ptr<CppCallStream> m_callStream;
   std::shared_ptr<IHFDMmain>     m_pIHFDMmain;

public:
   CppCallStream& cppCallStream() const
   {
      return *m_callStream.get();
   }
   void playbackCppCalls(std::string filename, IHFDMLoginCallback uiLoginFun);
   void playbackOne(int nFields);
   void initializeForPlayback(IHFDMLoginCallback uiLoginFun);

   void startPlayback();
   void finishPlayback();

   void execute(std::string);
};