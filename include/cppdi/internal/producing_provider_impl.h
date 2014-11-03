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

#ifndef CPPDI_INTERNAL_PRODUCING_PROVIDER_IMPL_H_
#define CPPDI_INTERNAL_PRODUCING_PROVIDER_IMPL_H_

#include <memory>

#include "cppdi/provider.h"
#include "cppdi/internal/producing_provider.h"

namespace cppdi {
namespace internal {

template<typename T, typename... Args>
Any ProducingProvider<T, Args...>::Get() {
  if (instance_.is_null()) {
    T *t = new T(injector_->GetInstance<Args>()...);

    instance_ = Any(std::shared_ptr<T>(t));

    // instance was created, pointer to injector can be removed
    injector_.reset();
  }

  return instance_;
}

template<typename T, typename... Args>
void ProducingProvider<T, Args...>::Initialize(const std::shared_ptr<cppdi::Injector> &injector) {
  injector_ = injector;
}

}
  // namespace internal
} // namespace cppdi

#endif  // CPPDI_INTERNAL_PRODUCING_PROVIDER_IMPL_H_
