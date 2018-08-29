#include <stdafx.h>
// WorkspaceWrapper.cpp
#include <WorkspaceWrapper.h>

#include <CppPlayer.hpp>
#include <CRSError.h>
#include <future>
#include <GUIDvalue.h>
#include <PropertySetWrapper.h>
#include <RootPropertyWrapper.h>

#undef max
#undef SYNCHRONIZE
#include <lynx/core/urn_utils.h>
#include "propertysets/property_factory.h"
#include <propertysets/repository/graph/nodes/branch_node.h>
#include <propertysets/repository/graph/nodes/commit_node.h>
#include <propertysets/commit_options.h>
#include <propertysets/initialize_options.h>
#include <propertysets/workspace.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
namespace WorkspaceWrapperFile
{
   static char THIS_FILE[] = __FILE__;
}
#define THIS_FILE WorkspaceWrapperFile::THIS_FILE
#endif
#define NEW_IS_REDEFINED 1

namespace LPS = Lynx::PropertySets;

WorkspaceWrapper::WorkspaceWrapper(std::shared_ptr<Lynx::PropertySets::Workspace> pNativeWorkspace)
   : m_pNativeWorkspace(pNativeWorkspace)
{}

WorkspaceWrapper::~WorkspaceWrapper()
{}


void invoke_WorkspaceWrapper_function(WorkspaceWrapper& pThis, MemFn pMember, const ArgsReader& ar)
{
   std::invoke(pMember, pThis, ar);
}

/////////////////////////////////////////////////////////////////////////////
// IWorkspace::initialize + record/playback

bool WorkspaceWrapper::initialize(std::string strURN, std::string* newURN)
{
   // execute
   bool bResult = true;
   const bool bLocal = false;

   std::promise<void> checkoutPromise;
   Lynx::PropertySets::InitializeOptions ioptions;
   if (!strURN.empty())
      ioptions.urn = Lynx::Core::UrnUtils::branchUrn(strURN);
   bool bNew = ioptions.urn.empty();
   ioptions.local = bLocal;
   m_pNativeWorkspace->initialize(
      ioptions
      , [this, &newURN, &checkoutPromise, &bResult, bNew](std::shared_ptr<std::exception> error, std::shared_ptr<Lynx::PropertySets::BranchNode>)
   {
      if (error)
      {
         CppCallInfo::reportError("RevitLynx_initialize", error);
         bResult = false;
      }
      else
      {
         std::string activeUrn = m_pNativeWorkspace->getActiveBranch()->getGuid();
         if (bNew)
            *newURN = activeUrn;
         char buffer[1000];
         sprintf_s(buffer, sizeof(buffer), "RevitLynx_initialize successful .... branch=%hs\n", activeUrn.c_str());
         CppCallInfo::reportInfo(ERR_SUCCESS, buffer);
      }
      checkoutPromise.set_value();
   });
   checkoutPromise.get_future().wait();

   {
      // This diagnostic is useful
      std::string ss = "WorkspaceWrapper::initialize Repo ";
      ss += strURN;
      ss += " , ";
      ss += *newURN;
      CppCallInfo::reportInfo(ERR_SUCCESS, ss);
   }

   // log
   recordFunction(capture_initialize, bResult, strURN, *newURN);
   return bResult;
}
void WorkspaceWrapper::playback_initialize(const ArgsReader& ar)
{
   // scan
   bool bMemorex = !!ar.popInt();
   std::string strURN = ar.unaliasURN(ar.popString());
   std::string newURN;
   // execute
   bool bLive = initialize(strURN, &newURN);
   ar.aliasURN(newURN, ar.popString());
   // post-assert
#if !WOX
   bMemorex;
   bLive;
#else
   ar.compareBeforeAndAfter(bMemorex, bLive);
#endif
}

void WorkspaceWrapper::onBranchHeadMoved_connect(
   std::function<void(
      std::shared_ptr<Lynx::PropertySets::BranchNode>
      , std::shared_ptr<Lynx::PropertySets::CommitNode>)> callback)
{
   // disable this path for now (REVIT-120373)
   if (false)
   {
      m_pNativeWorkspace->onBranchHeadMoved.connect(
         [this, &callback](Lynx::PropertySets::Repository&
            , std::shared_ptr<Lynx::PropertySets::BranchNode> branchNode
            , std::shared_ptr<Lynx::PropertySets::CommitNode> commitNode)
      {
         callback(branchNode, commitNode);
      });
   }
}

/////////////////////////////////////////////////////////////////////////////
// IWorkspace::processEventQueue + record/playback

void WorkspaceWrapper::processEventQueue()
{
   // execute
   this->m_pNativeWorkspace->processEventQueue();
   // log
   recordMethod(capture_processEventQueue);
}
void WorkspaceWrapper::playback_processEventQueue(const ArgsReader&)
{
   // execute
   processEventQueue();
};

/////////////////////////////////////////////////////////////////////////////
// IWorkspace::getRootProperty + record/playback

std::shared_ptr<IRootProperty> WorkspaceWrapper::getRootProperty() const
{
   auto root = std::make_shared<RootPropertyWrapper>(m_pNativeWorkspace->getRoot());
   recordFunction(capture_getRootProperty, root);
   return root;
}
void WorkspaceWrapper::playback_getRootProperty(const ArgsReader& ar)
{
   ar.functionReturn(getRootProperty());
}

/////////////////////////////////////////////////////////////////////////////
// IWorkspace::createChild + record/playback

bool WorkspaceWrapper::createChild(const PropertyId& id, const std::string& typeId)
{
   bool bRet = m_pNativeWorkspace->createChild(id.getId(), typeId);
   recordFunction(capture_createChild, bRet, id.getAbsolutePath(), typeId);
   return bRet;
}
void WorkspaceWrapper::playback_createChild(const ArgsReader& ar)
{
   ar.functionReturn(
      createChild(
         PropertyId{ ar.popStringVector() }
         , ar.popString()));
}

std::shared_ptr<IPropertySet> WorkspaceWrapper::factory(const std::string& typeId)
{
   return std::make_shared<PropertySetWrapper>(LPS::GetPropertyFactory().createProperty(typeId));
}

std::vector<std::string> WorkspaceWrapper::getIds() const
{
   std::vector<std::string> vRet = m_pNativeWorkspace->getIds();
   recordFunction(capture_getIds, vRet);
   return vRet;
}

void WorkspaceWrapper::playback_getIds(const ArgsReader& ar)
{
   // scan
   (void)ar.popString();
   // execute (throw away result)
   auto ids = getIds();
}

void WorkspaceWrapper::pushNotificationDelayScope()
{
   m_pNativeWorkspace->pushModifiedEventScope();
   recordMethod(capture_pushNotificationDelayScope);
}
void WorkspaceWrapper::popNotificationDelayScope()
{
   m_pNativeWorkspace->popModifiedEventScope();
   recordMethod(capture_popNotificationDelayScope);
}
void WorkspaceWrapper::playback_pushNotificationDelayScope(const ArgsReader&)
{
   pushNotificationDelayScope();
};
void WorkspaceWrapper::playback_popNotificationDelayScope(const ArgsReader&)
{
   popNotificationDelayScope();
};

/////////////////////////////////////////////////////////////////////////////
// IWorkspace::commit + record/playback

void WorkspaceWrapper::commit(
   const std::string& msg
   , std::function<void(std::shared_ptr<std::exception>, std::shared_ptr<Lynx::PropertySets::CommitNode>)> callback)
{
   // The msg argument disappeared from the interface?
   Lynx::PropertySets::CommitOptions coptions;
   // execute
   m_pNativeWorkspace->commit(coptions, callback);
   // log
   recordMethod(capture_commit, msg);
}
void WorkspaceWrapper::robotic_commit(const std::string& msg)
{
   commit(
      msg
      , [](std::shared_ptr<std::exception>, std::shared_ptr<Lynx::PropertySets::CommitNode>)
   {
   });
}
void WorkspaceWrapper::playback_commit(const ArgsReader& ar)
{
   robotic_commit(ar.popString());
}

/////////////////////////////////////////////////////////////////////////////
// IWorkspace::rebase + record/playback

Err WorkspaceWrapper::rebase(std::string)
{
   // execute
   bool bSuccess = m_pNativeWorkspace->rebase();
   Err sStatus =
      bSuccess
      ? ERR_SUCCESS
      : ERR_FAILURE;
   // log
   recordFunction(capture_rebase, sStatus);
   return sStatus;
}
void WorkspaceWrapper::playback_rebase(const ArgsReader& ar)
{
   ar.functionReturn(rebase(ar.unaliasURN(ar.popString())));
};

/////////////////////////////////////////////////////////////////////////////
// IWorkspace accessors

GUIDvalue WorkspaceWrapper::getCurrentBranch() const
{
   GUIDvalue ret =  GUIDvalue(int(), m_pNativeWorkspace->getActiveBranch()->getGuid());
   return ret;
}

GUIDvalue WorkspaceWrapper::getCurrentCommit() const
{
   GUIDvalue ret = GUIDvalue(int(), m_pNativeWorkspace->getActiveCommit()->getGuid());
   return ret;
}