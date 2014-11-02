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

#include <memory>

namespace cppdi {
namespace internal {

template<typename T>
class InstanceProvider : public Provider<void> {
 public:
  explicit InstanceProvider(const std::shared_ptr<T> &instance)
      : instance_(instance) {
  }

  virtual ~InstanceProvider() = default;

  virtual std::shared_ptr<void> Get() override {
    return std::static_pointer_cast<T>(instance_);
  }
 private:
  std::shared_ptr<T> instance_;
};

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_INSTANCE_PROVIDER_H_
