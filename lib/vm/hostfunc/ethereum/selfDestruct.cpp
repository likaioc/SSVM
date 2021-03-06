// SPDX-License-Identifier: Apache-2.0
#include "vm/hostfunc/ethereum/selfDestruct.h"

namespace SSVM {
namespace Executor {

ErrCode EEISelfDestruct::body(VM::EnvironmentManager &EnvMgr,
                              Instance::MemoryInstance &MemInst,
                              uint32_t AddressOffset) {
  evmc_context *Cxt = Env.getEVMCContext();

  /// Get address data.
  evmc_address Addr;
  if (ErrCode Status = loadAddress(MemInst, Addr, AddressOffset);
      Status != ErrCode::Success) {
    return Status;
  }
  evmc_address Self = Env.getAddressEVMC();

  /// Take additional gas if call new account.
  if (!Cxt->host->account_exists(Cxt, &Addr)) {
    if (!EnvMgr.addCost(25000ULL)) {
      return ErrCode::CostLimitExceeded;
    }
  }

  /// Call selfdestruct.
  Cxt->host->selfdestruct(Cxt, &Self, &Addr);
  return ErrCode::Terminated;
}

} // namespace Executor
} // namespace SSVM
