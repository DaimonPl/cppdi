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

#ifndef CPPDI_INTERNAL_FUNCTION_PROVIDER_IMPL_H_
#define CPPDI_INTERNAL_FUNCTION_PROVIDER_IMPL_H_

#include "cppdi/internal/any.h"
#include "cppdi/internal/function_provider.h"

namespace cppdi {
namespace internal {

template<typename T, typename ...Args>
FunctionProvider<T, Args...>::FunctionProvider(
    const std::function<T(Args...)> &function)
    : function_(function) {
}

template<typename T, typename ...Args>
Any FunctionProvider<T, Args...>::Get() {
  return Any(function_(injector_->GetInstance<Args>()...));
}

template<typename T, typename ...Args>
void FunctionProvider<T, Args...>::Initialize(
    const std::shared_ptr<cppdi::Injector> &injector) {
  injector_ = injector;
}

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_FUNCTION_PROVIDER_IMPL_H_
