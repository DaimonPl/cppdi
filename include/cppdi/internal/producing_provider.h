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

#ifndef CPPDI_INTERNAL_PRODUCING_PROVIDER_H_
#define CPPDI_INTERNAL_PRODUCING_PROVIDER_H_

#include <memory>

#include "cppdi/provider.h"

namespace cppdi {

class Injector;

namespace internal {

template<typename T, typename... Args>
class ProducingProvider : public Provider<Any> {
 public:
  Any Get() override;

 private:
  void Initialize(const std::shared_ptr<cppdi::Injector> &injector) override;

  Any instance_;
  std::shared_ptr<cppdi::Injector> injector_;
};

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_PRODUCING_PROVIDER_H_
