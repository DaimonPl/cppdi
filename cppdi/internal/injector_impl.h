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
#include "../producer_repository.h"
#include "raw_provider_wrapper.h"
#include "linking_provider.h"
#include "producing_provider.h"

namespace cppdi {

Injector::Injector(const Binder &binder) {
  provider_map_ = binder.GetProviderBindings();
  linked_bindings_map_ = binder.GetLinkedBindings();
  state_ = UNINITIALIZED;
}

std::shared_ptr<void> Injector::GetInstance(const internal::Key &key)
    throw (InjectionError) {
  return GetProvider(key)->Get();
}

std::shared_ptr<Provider<void>> Injector::GetProvider(const internal::Key &key)
    throw (InjectionError) {
  if (state_ == DISPOSED) {
    throw InjectionError("Injector has been disposed!");
  } else if (state_ == UNINITIALIZED) {
    state_ = INITIALIZED;

    for (auto &linked_binding : linked_bindings_map_) {
      std::shared_ptr<Provider<void>> provider(
          new internal::LinkingProvider(linked_binding.second));

      provider_map_.emplace(linked_binding.first, provider);
    }

    //providers were created, linked bindings map can be cleared
    linked_bindings_map_.clear();

    std::shared_ptr<Injector> this_ptr = shared_from_this();

    for (auto &provider_binding : provider_map_) {
      provider_binding.second->Initialize(this_ptr);
    }
  }

  auto provider_it = provider_map_.find(key);

  if (provider_it != provider_map_.end()) {
    return provider_it->second;
  }

  //binding does not exist, try to create producer-based one
  Producer<void> producer = ProducerRepository::Instance().Get(key.GetType());
  std::shared_ptr<Provider<void>> provider(
      new internal::ProducingProvider(producer));
  provider->Initialize(shared_from_this());

  provider_map_.emplace(key, provider);

  return provider;
}

template<typename T>
std::shared_ptr<T> Injector::GetInstance() throw (InjectionError) {
  return GetInstance<T>(std::string());
}

template<typename T>
std::shared_ptr<T> Injector::GetInstance(const std::string &name)
    throw (InjectionError) {
  internal::Key key(typeid(T), name);

  return std::static_pointer_cast<T>(GetInstance(key));
}

template<typename T>
std::shared_ptr<Provider<T>> Injector::GetProvider() throw (InjectionError) {
  return GetProvider<T>(std::string());
}

template<typename T>
std::shared_ptr<Provider<T>> Injector::GetProvider(const std::string &name)
    throw (InjectionError) {
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
