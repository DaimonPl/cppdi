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

#ifndef CPPDI_INTERNAL_CONCRETE_PROVIDER_WRAPPER_H_
#define CPPDI_INTERNAL_CONCRETE_PROVIDER_WRAPPER_H_

#include <memory>

#include "cppdi/internal/any.h"

namespace cppdi {
namespace internal {

template<typename T>
class ConcreteProviderWrapper : public Provider<Any> {
 public:
  explicit ConcreteProviderWrapper(
      const std::shared_ptr<Provider<T>> &concrete_provider)
      : concrete_provider_(concrete_provider) {
  }

  virtual ~ConcreteProviderWrapper() = default;

  Any Get() override {
    return Any(concrete_provider_->Get());
  }
 private:
  std::shared_ptr<Provider<T>> concrete_provider_;
};

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_CONCRETE_PROVIDER_WRAPPER_H_
