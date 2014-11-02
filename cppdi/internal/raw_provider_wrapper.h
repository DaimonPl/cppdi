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

#ifndef CPPDI_INTERNAL_RAW_PROVIDER_WRAPPER_H_
#define CPPDI_INTERNAL_RAW_PROVIDER_WRAPPER_H_

#include <memory>

namespace cppdi {
namespace internal {

template<typename T>
class RawProviderWrapper : public Provider<T> {
 public:
  explicit RawProviderWrapper(const std::shared_ptr<Provider<void>> &raw_provider)
      : raw_provider_(raw_provider) {
  }

  virtual ~RawProviderWrapper() = default;

  virtual std::shared_ptr<T> Get() override {
    return std::static_pointer_cast<T>(raw_provider_->Get());
  }
 private:
  std::shared_ptr<Provider<void>> raw_provider_;
};

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_RAW_PROVIDER_WRAPPER_H_
