// SPDX-License-Identifier: Apache-2.0
#pragma once

#include "wasi.h"

namespace SSVM {
namespace Executor {

class WasiFdFdstatSetFlags : public Wasi<WasiFdFdstatSetFlags> {
public:
  WasiFdFdstatSetFlags(VM::WasiEnvironment &HostEnv) : Wasi(HostEnv, "fd_fdstat_set_flags") {}

  ErrCode body(VM::EnvironmentManager &EnvMgr,
               Instance::MemoryInstance &MemInst, uint32_t &ErrNo, int32_t Fd,
               uint32_t FsFlags);
};

} // namespace Executor
} // namespace SSVM
