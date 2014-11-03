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

namespace cppdi {
namespace internal {

LinkingProvider::LinkingProvider(const Key &target)
    : target_(target) {
}

std::shared_ptr<void> LinkingProvider::Get() {
  return injector_->GetInstance(target_);
}

void LinkingProvider::Initialize(
    const std::shared_ptr<cppdi::Injector> &injector) {
  injector_ = injector;
}

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_LINKING_PROVIDER_IMPL_H_
