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

#ifndef CPPDI_INTERNAL_INSTANCE_PROVIDER_H_
#define CPPDI_INTERNAL_INSTANCE_PROVIDER_H_

#include "cppdi/provider.h"
#include "cppdi/internal/shared_any.h"

namespace cppdi {
namespace internal {

class InstanceProvider : public Provider<SharedAny> {
 public:
  explicit InstanceProvider(const SharedAny &instance)
      : instance_(instance) {
  }

  SharedAny Get() override {
    return instance_;
  }
 private:
  SharedAny instance_;
};

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_INSTANCE_PROVIDER_H_
