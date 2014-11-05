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

#ifndef CPPDI_INTERNAL_INJECTOR_IMPL_H_
#define CPPDI_INTERNAL_INJECTOR_IMPL_H_

#include <memory>
#include <string>
#include <typeinfo>

#include "cppdi/injector.h"
#include "cppdi/provider.h"
#include "cppdi/internal/raw_provider_wrapper.h"
#include "cppdi/internal/linking_provider.h"
#include "cppdi/internal/producing_provider.h"

namespace cppdi {

inline Injector::Injector(const Binder &binder) {
  provider_map_ = binder.GetProviderBindings();
  state_ = UNINITIALIZED;
}

inline internal::Any Injector::GetInstanceByKey(const internal::Key &key) {
  return GetProvider(key)->Get();
}

inline void Injector::AutoInitialize() {
  if (state_ == UNINITIALIZED) {
    state_ = INITIALIZED;

    std::shared_ptr<Injector> this_ptr = shared_from_this();

    for (auto &provider_binding : provider_map_) {
      provider_binding.second->Initialize(this_ptr);
    }
  }
}

inline std::shared_ptr<Provider<internal::Any>> Injector::GetProvider(
    const internal::Key &key) {
  if (state_ == DISPOSED) {
    throw InjectionError("Injector has been disposed!");
  }

  AutoInitialize();

  auto provider_it = provider_map_.find(key);

  if (provider_it == provider_map_.end()) {
    throw InjectionError(std::string("No binding for ") + key.GetFullName());
  }

  return provider_it->second;
}

template<typename T>
T Injector::GetInstance() {
  return GetInstance<T>(std::string());
}

template<typename T>
T Injector::GetInstance(const std::string &name) {
  internal::Key key(typeid(T), name);

  internal::Any instance = GetInstanceByKey(key);

  return instance.as<T>();
}

inline void Injector::Dispose() {
  if (state_ != DISPOSED) {
    state_ = DISPOSED;

    // clearing collections, to cut cyclic dependency between Injector and
    // various Provider's
    provider_map_.clear();
  }
}

}  // namespace cppdi

#endif  // CPPDI_INTERNAL_INJECTOR_IMPL_H_
