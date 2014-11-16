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
#include <type_traits>
#include <utility>

#include "cppdi/injector.h"
#include "cppdi/provider.h"
#include "cppdi/internal/raw_provider_wrapper.h"
#include "cppdi/internal/linking_provider.h"
#include "cppdi/internal/producing_provider.h"
#include "cppdi/internal/traits.h"

namespace cppdi {

inline Injector::Injector(internal::BindingMap<internal::SharedAny> &&any_providers,
                          internal::BindingMap<std::shared_ptr<void>> &&ptr_providers) {
  shared_any_provider_map_ = std::move(any_providers);
  shared_ptr_provider_map_ = std::move(ptr_providers);
  state_ = UNINITIALIZED;
}

inline void Injector::AutoInitialize() {
  if (state_ == UNINITIALIZED) {
    state_ = INITIALIZED;

    std::shared_ptr<Injector> this_ptr = shared_from_this();

    for (auto &provider_binding : shared_any_provider_map_) {
      provider_binding.second->Initialize(this_ptr);
    }

    for (auto &provider_binding : shared_ptr_provider_map_) {
      provider_binding.second->Initialize(this_ptr);
    }
  }
}

inline internal::SharedAnyProviderPtr &Injector::GetAnyProvider(
    const internal::Key &key) {
  auto provider_it = shared_any_provider_map_.find(key);

  if (provider_it == shared_any_provider_map_.end()) {
    return empty_any_provider_;
  }

  return provider_it->second;
}

inline internal::VoidPtrProviderPtr &Injector::GetPtrProvider(
    const internal::Key &key) {
  auto provider_it = shared_ptr_provider_map_.find(key);

  if (provider_it == shared_ptr_provider_map_.end()) {
    return empty_ptr_provider_;
  }

  return provider_it->second;
}

template<typename T>
T Injector::GetInstance() {
  return GetInstance<T>(std::string());
}

template <typename T>
T Injector::GetInstance(const std::string &name) {
  static_assert(!internal::is_cr<T>::value,
                "GetInstance<T>() - T must not be constant nor reference");

  return GetNamedInstance<T>(name);
}

template<typename T>
auto Injector::GetNamedInstance(const std::string &name)
    -> typename std::enable_if<!internal::is_shared_ptr<T>{}, T>::type {
  if (state_ == DISPOSED) {
    throw InjectionError("Injector has been disposed!");
  }

  AutoInitialize();

  internal::Key key(typeid(T), name);

#ifdef _CPPDI_DEBUG_MODE_
  internal::CycleCheckGuard cycleCheckGuard(&cycle_verifier_, key);
#endif

  auto &provider = GetAnyProvider(key);

  if (!provider) {
    throw InjectionError(std::string("No binding for " + key.GetFullName()));
  }

  return provider->Get().as<T>();
}

template<typename T>
auto Injector::GetNamedInstance(const std::string &name)
    -> typename std::enable_if<internal::is_shared_ptr<T>{}, T>::type {
  using U = typename std::decay<T>::type::element_type;

  if (state_ == DISPOSED) {
    throw InjectionError("Injector has been disposed!");
  }

  AutoInitialize();

  internal::Key key(typeid(std::shared_ptr<U>), name);

#ifdef _CPPDI_DEBUG_MODE_
  internal::CycleCheckGuard cycleCheckGuard(&cycle_verifier_, key);
#endif

  auto &ptr_provider = GetPtrProvider(key);

  if (ptr_provider) {
    return std::static_pointer_cast<U>(ptr_provider->Get());
  }

  auto &any_provider = GetAnyProvider(key);

  if (!any_provider) {
    throw InjectionError(std::string("No binding for " + key.GetFullName()));
  }

  return any_provider->Get().as<std::shared_ptr<U>>();
}

inline void Injector::Dispose() {
  if (state_ != DISPOSED) {
    state_ = DISPOSED;

    // clearing collections, to cut cyclic dependency between Injector and
    // various Provider's
    shared_any_provider_map_.clear();
    shared_ptr_provider_map_.clear();
  }
}

}  // namespace cppdi

#endif  // CPPDI_INTERNAL_INJECTOR_IMPL_H_
