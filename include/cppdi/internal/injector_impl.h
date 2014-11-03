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

#include "../injector.h"
#include "../provider.h"
#include "raw_provider_wrapper.h"
#include "linking_provider.h"
#include "producing_provider.h"

namespace cppdi {

Injector::Injector(const Binder &binder) {
  provider_map_ = binder.GetProviderBindings();
  linked_bindings_map_ = binder.GetLinkedBindings();
  producer_map_ = binder.GetPoducerBindings();
  state_ = UNINITIALIZED;
}

std::shared_ptr<void> Injector::GetInstance(const internal::Key &key) {
  return GetProvider(key)->Get();
}

void Injector::AutoInitialize() {
  if (state_ == UNINITIALIZED) {
    state_ = INITIALIZED;

    for (auto &linked_binding : linked_bindings_map_) {
      std::shared_ptr<Provider<void>> provider(
          new internal::LinkingProvider(linked_binding.second));

      provider_map_.emplace(linked_binding.first, provider);
    }

    linked_bindings_map_.clear();

    for (auto &producer_binding : producer_map_) {
      std::shared_ptr<Provider<void>> provider(
          new internal::ProducingProvider(producer_binding.second));

      provider_map_.emplace(producer_binding.first, provider);
    }

    producer_map_.clear();

    std::shared_ptr<Injector> this_ptr = shared_from_this();

    for (auto &provider_binding : provider_map_) {
      provider_binding.second->Initialize(this_ptr);
    }
  }
}

std::shared_ptr<Provider<void>> Injector::GetProvider(const internal::Key &key) {
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
std::shared_ptr<T> Injector::GetInstance() {
  return GetInstance<T>(std::string());
}

template<typename T>
std::shared_ptr<T> Injector::GetInstance(const std::string &name) {
  internal::Key key(typeid(T), name);

  return std::static_pointer_cast<T>(GetInstance(key));
}

template<typename T>
std::shared_ptr<Provider<T>> Injector::GetProvider() {
  return GetProvider<T>(std::string());
}

template<typename T>
std::shared_ptr<Provider<T>> Injector::GetProvider(const std::string &name) {
  internal::Key key(typeid(T), name);

  std::shared_ptr<Provider<T>> provider(
      new internal::RawProviderWrapper<T>(GetProvider(key)));

  return provider;
}

void Injector::Dispose() {
  if (state_ != DISPOSED) {
    state_ = DISPOSED;

    //clearing collections, to cut cyclic dependency between Injector and
    //various Provider's
    provider_map_.clear();
    linked_bindings_map_.clear();
  }
}

}  // namespace cppdi

#endif  // CPPDI_INTERNAL_INJECTOR_IMPL_H_
