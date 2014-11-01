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

#ifndef CPPDI_INTERNAL_LINKING_PROVIDER_H_
#define CPPDI_INTERNAL_LINKING_PROVIDER_H_

#include <memory>

#include "../injector.h"
#include "key.h"

namespace cppdi {
namespace internal {

class LinkingProvider : public Provider<void> {
 public:
  explicit LinkingProvider(const Key &target);

  virtual ~LinkingProvider() = default;

  virtual std::shared_ptr<void> Get() throw (InjectionError) override;

 private:
  virtual void Initialize(const std::shared_ptr<cppdi::Injector> &injector)
      override;

  Key target_;
  std::shared_ptr<cppdi::Injector> injector_;
};

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_LINKING_PROVIDER_H_
