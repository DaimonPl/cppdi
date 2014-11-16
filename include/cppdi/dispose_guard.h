/*
 * Copyright (C) 2014 Damian Momot <momotdamian@gmail.com>
 *
 * This file is part of cppdi - C++ Dependency Injection library
 * (http://code.google.com/p/cppdi/)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CPPDI_DISPOSE_GUARD_H_
#define CPPDI_DISPOSE_GUARD_H_

#include "cppdi/injector.h"

namespace cppdi {

/**
 * Guard which ensures that Dispose() is called on Injector
 */
class DisposeGuard {
 public:
  explicit DisposeGuard(const std::shared_ptr<Injector> &injector)
      : injector_(injector) {}

  ~DisposeGuard() {
    injector_->Dispose();
  }

 private:
  std::shared_ptr<Injector> injector_;

  // make DisposeGuard an non-copyable, non-assignable, stack-only type
  DisposeGuard(DisposeGuard && guard) = delete;
  DisposeGuard(const DisposeGuard &) = delete;
  void operator=(const DisposeGuard &) = delete;
  void operator=(DisposeGuard &&) = delete;
  void *operator new(size_t) = delete;
  void operator delete(void *) = delete;
  void *operator new[](size_t) = delete;
  void operator delete[](void *) = delete;
};

}  // namespace cppdi

#endif  // CPPDI_DISPOSE_GUARD_H_
