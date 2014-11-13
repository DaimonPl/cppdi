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

#ifndef CPPDI_CYCLE_VERIFIER_H_
#define CPPDI_CYCLE_VERIFIER_H_

#include<cassert>
#include<deque>
#include<set>

#include "cppdi/errors.h"
#include "cppdi/internal/key.h"

namespace cppdi {
namespace internal {

#ifdef _CPPDI_DEBUG_MODE_

class CycleCheckGuard;

class CycleVerifier {
  void verify(const Key &key) {
    if (key_set_.find(key) != key_set_.end()) {
      std::string msg("Cycle detected: ");

      for (Key &k : keys_) {
        msg += k.GetFullName() + " -> ";
      }
      msg += key.GetFullName();

      throw InjectionCycleError(msg);
    }

    key_set_.emplace(key);
    keys_.push_back(key);
  }

  void pop() {
    assert(!key_set_.empty());

    key_set_.erase(keys_.back());
    keys_.pop_back();
  }

  std::deque<Key> keys_;
  std::set<Key> key_set_;

  friend CycleCheckGuard;
};

class CycleCheckGuard {
 public:
  explicit CycleCheckGuard(CycleVerifier * verifier, const Key &key)
      : verifier_(verifier) {
    verifier_->verify(key);
  }

  ~CycleCheckGuard() {
    verifier_->pop();
  }

  // make DisposeGuard an non-copyable, non-assignable, stack-only type
  CycleCheckGuard(CycleCheckGuard && guard) = delete;
  CycleCheckGuard(const CycleCheckGuard &) = delete;
  void operator=(const CycleCheckGuard &) = delete;
  void operator=(CycleCheckGuard &&) = delete;
  void *operator new(size_t) = delete;
  void operator delete(void *) = delete;
  void *operator new[](size_t) = delete;
  void operator delete[](void *) = delete;
 private:
  CycleVerifier *verifier_;
};

#endif

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_CYCLE_VERIFIER_H_
