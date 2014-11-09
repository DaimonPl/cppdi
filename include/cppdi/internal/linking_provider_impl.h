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

#ifndef CPPDI_INTERNAL_LINKING_PROVIDER_IMPL_H_
#define CPPDI_INTERNAL_LINKING_PROVIDER_IMPL_H_

#include <memory>

#include "cppdi/internal/linking_provider.h"

namespace cppdi {
namespace internal {

template<typename F, typename T>
LinkingProvider<F, T>::LinkingProvider(const std::string &t_name) {
  t_name_ = t_name;
}

template<typename F, typename T>
Any LinkingProvider<F, T>::Get() {
  std::shared_ptr<T> instance = injector_->GetInstance<std::shared_ptr<T>>(
      t_name_);

  return Any(std::static_pointer_cast<F>(instance));
}

template<typename F, typename T>
void LinkingProvider<F, T>::Initialize(
    const std::shared_ptr<cppdi::Injector> &injector) {
  injector_ = injector;
}

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_LINKING_PROVIDER_IMPL_H_
