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

#include "producer.h"

namespace cppdi {

class Injector;

namespace internal {

class ProducingProvider : public Provider<void> {
 public:
  explicit ProducingProvider(const Producer<void> &producer)
      : producer_(producer) {
  }

  virtual ~ProducingProvider() = default;

  virtual std::shared_ptr<void> Get() override {
    if (!instance_) {
      instance_ = producer_(injector_);

      //instance was created, pointer to injector can be removed
      injector_.reset();
    }

    return instance_;
  }

 private:
  virtual void Initialize(const std::shared_ptr<cppdi::Injector> &injector) override {
    injector_ = injector;
  }


  Producer<void> producer_;
  std::shared_ptr<void> instance_;
  std::shared_ptr<cppdi::Injector> injector_;
};

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_PRODUCING_PROVIDER_H_
