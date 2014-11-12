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

#ifndef CPPDI_INTERNAL_INJECTOR_FACTORY_IMPL_H_
#define CPPDI_INTERNAL_INJECTOR_FACTORY_IMPL_H_

#include <memory>
#include <utility>

#include "cppdi/injector_factory.h"
#include "cppdi/binder.h"

namespace cppdi {

inline std::shared_ptr<Injector> InjectorFactory::Create(const Module &module) const {
  return Create({ &module });
}

inline std::shared_ptr<Injector> InjectorFactory::Create(
    const std::initializer_list<const Module*> &modules) const {
  Binder binder;

  for (auto module : modules) {
    module->Configure(&binder);
  }

  return std::shared_ptr<Injector>(new Injector(std::move(binder.provider_map_)));
}

inline std::shared_ptr<Injector> InjectorFactory::Create(
      const std::function<void(Binder *binder)> &binding_function) {
  Binder binder;

  binding_function(&binder);

  return std::shared_ptr<Injector>(new Injector(std::move(binder.provider_map_)));
}

}  // namespace cppdi

#endif  // CPPDI_INTERNAL_INJECTOR_FACTORY_IMPL_H_
