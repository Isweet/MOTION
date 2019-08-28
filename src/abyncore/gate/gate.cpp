// MIT License
//
// Copyright (c) 2019 Oleksandr Tkachenko
// Cryptography and Privacy Engineering Group (ENCRYPTO)
// TU Darmstadt, Germany
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "gate.h"

#include "wire/wire.h"

#include "base/backend.h"
#include "base/register.h"

namespace ABYN::Gates::Interfaces {

void Gate::RegisterWaitingFor(std::size_t wire_id) {
  std::scoped_lock lock(mutex_);
  wire_dependencies_.insert(wire_id);
}

void Gate::SignalDependencyIsReady() {
  num_ready_dependencies_++;
  IfReadyAddToProcessingQueue();
}

void Gate::SetOnlineIsReady() {
  online_is_ready_ = true;
  for (auto &wire : output_wires_) {
    assert(wire);
    wire->SetOnlineFinished();
  }
}

void Gate::IfReadyAddToProcessingQueue() {
  if (AreDependenciesReady() && !added_to_active_queue) {
    auto ptr_backend = backend_.lock();
    assert(ptr_backend);
    ptr_backend->GetRegister()->AddToActiveQueue(gate_id_);
    added_to_active_queue = true;
  }
}

void Gate::Clear() {
  setup_is_ready_ = false;
  online_is_ready_ = false;
  added_to_active_queue = false;
  num_ready_dependencies_ = 0;

  for (auto &wire : output_wires_) {
    wire->Clear();
  }
}

std::shared_ptr<Register> Gate::GetRegister() {
  auto ptr_backend = backend_.lock();
  assert(ptr_backend);
  return ptr_backend->GetRegister();
}

std::shared_ptr<Configuration> Gate::GetConfig() {
  auto ptr_backend = backend_.lock();
  assert(ptr_backend);
  return ptr_backend->GetConfig();
}

std::shared_ptr<Logger> Gate::GetLogger() {
  auto ptr_backend = backend_.lock();
  assert(ptr_backend);
  return ptr_backend->GetLogger();
}

}