/*
  Copyright (c) 2011 NVIDIA Corporation
  Copyright (c) 2011-2012 Cass Everitt
  Copyright (c) 2012 Scott Nations
  Copyright (c) 2012 Mathias Schott
  Copyright (c) 2012 Nigel Stewart
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
  OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "pch.h" /* For MS precompiled header support */

#include "RegalUtil.h"

REGAL_GLOBAL_BEGIN

#include "RegalConfig.h"
#include "RegalDispatcher.h"

REGAL_GLOBAL_END

REGAL_NAMESPACE_BEGIN

void InitDispatchTableDebug    (DispatchTable &tbl);
void InitDispatchTableError    (DispatchTable &tbl);
void InitDispatchTableEmu      (DispatchTable &tbl);
void InitDispatchTableLog      (DispatchTable &tbl);
void InitDispatchTableLoader   (DispatchTable &tbl);
void InitDispatchTableNacl     (DispatchTable &tbl);
void InitDispatchTableStaticES2(DispatchTable &tbl);
void InitDispatchTableMissing  (DispatchTable &tbl);

Dispatcher::Dispatcher()
: current(0)
{
  #if REGAL_DEBUG
  InitDispatchTableDebug(debug);
  push_back(debug,Config::enableDebug);
  #endif

  #if REGAL_ERROR
  InitDispatchTableError(error);
  push_back(error,Config::enableError);
  #endif

  #if REGAL_EMULATION
  ::memset(&emulation,0,sizeof(DispatchTable));
  InitDispatchTableEmu(emulation);               // emulated functions only
  push_back(emulation,Config::enableEmulation || Config::forceEmulation);
  #endif

  #if REGAL_LOG
  InitDispatchTableLog(logging);
  push_back(logging,Config::enableLog);
  #endif

  #if REGAL_DRIVER
  #if REGAL_STATIC_ES2
  ::memset(&driver,0,sizeof(DispatchTable));
  InitDispatchTableStaticES2(driver);           // ES 2.0 functions only
  #elif defined(__native_client__)
  ::memset(&driver,0,sizeof(DispatchTable));
  InitDispatchTableNacl(driver);                // ES 2.0 functions only
  #else
  InitDispatchTableLoader(driver);              // Desktop/ES2.0 lazy loader
  #endif
  push_back(driver,Config::enableDriver);
  #endif

  InitDispatchTableMissing(missing);
  push_back(missing,true);
}

Dispatcher::~Dispatcher()
{
}

void
Dispatcher::push_back(DispatchTable &table, bool enable)
{
  // Disabling the missing table would be bad!
  RegalAssert(&table!=&missing || enable==true);

  RegalAssert(enabled.size()==disabled.size());
  if (enable)
  {
    enabled.push_back(&table);
    disabled.push_back(NULL);
  }
  else
  {
    disabled.push_back(&table);
    enabled.push_back(NULL);
  }
}

void
Dispatcher::enable(DispatchTable &table)
{
  RegalAssert(enabled.size()==disabled.size());
  for (std::size_t i=0; i<disabled.size(); ++i)
    if (disabled[i]==&table)
      std::swap(enabled[i],disabled[i]);
}

void
Dispatcher::disable(DispatchTable &table)
{
  // Disabling the missing table would be bad!
  RegalAssert(&table!=&missing);

  RegalAssert(enabled.size()==disabled.size());
  for (std::size_t i=0; i<enabled.size(); ++i)
    if (enabled[i]==&table)
      std::swap(enabled[i],disabled[i]);
}

bool
Dispatcher::isEnabled(DispatchTable &table) const
{
  RegalAssert(enabled.size()==disabled.size());
  for (std::size_t i=0; i<enabled.size(); ++i)
  {
    if (enabled[i]==&table)  return true;
    if (disabled[i]==&table) return false;
  }

  return false;
}

REGAL_NAMESPACE_END
