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

#ifndef CPPDI_INTERNAL_FUNCTION_PROVIDER_H_
#define CPPDI_INTERNAL_FUNCTION_PROVIDER_H_

#include "cppdi/internal/any.h"

namespace cppdi {
namespace internal {

template<typename T, typename ...Args>
class FunctionProvider : public Provider<Any> {
 public:
  explicit FunctionProvider(const std::function<T(Args...)> &function);

  Any Get() override;
 private:
  std::shared_ptr<cppdi::Injector> injector_;
  std::function<T(Args...)> function_;

  void Initialize(const std::shared_ptr<cppdi::Injector> &injector) override;
};

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_FUNCTION_PROVIDER_H_
