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

#ifndef CPPDI_INTERNAL_BINDER_IMPL_H_
#define CPPDI_INTERNAL_BINDER_IMPL_H_

#include <memory>
#include <string>
#include <type_traits>

#include "cppdi/binder.h"
#include "cppdi/errors.h"
#include "cppdi/internal/concrete_provider_wrapper.h"
#include "cppdi/internal/instance_provider.h"
#include "cppdi/internal/linking_provider.h"
#include "cppdi/internal/producing_provider.h"
#include "cppdi/internal/raw_provider_wrapper.h"

namespace cppdi {

template<typename T, typename ... Args>
void Binder::BindConstructor() {
  internal::Key key(typeid(std::shared_ptr<T>));

  std::shared_ptr<Provider<internal::Any>> provider(
      new internal::ProducingProvider<T, Args...>());

  CreateBinding(key, provider);

  //provider binding
  internal::Key provider_of_provider_key(
      typeid(std::shared_ptr<Provider<std::shared_ptr<T>>>));
  std::shared_ptr<Provider<std::shared_ptr<T>>> concrete_provider(new internal::RawProviderWrapper<std::shared_ptr<T>>(provider));
  std::shared_ptr<Provider<internal::Any>> provider_of_provider(
      new internal::InstanceProvider(internal::Any(concrete_provider)));

  CreateBinding(provider_of_provider_key, provider_of_provider);
}

template<typename F, typename T>
void Binder::BindTypes() {
  BindTypes<F, T>(std::string());
}

template<typename F, typename T>
void Binder::BindTypes(const std::string &name) {
  static_assert(std::is_base_of<F, T>::value, "BindTypes<F, T>() - T must be a descendant of F");
  static_assert(!std::is_same<F, T>::value, "BindTypes<F, T>() - T cannot be same as F");

  internal::Key source_key(typeid(std::shared_ptr<F>), name);

  std::shared_ptr<Provider<internal::Any>> provider(
      new internal::LinkingProvider<F, T>());

  CreateBinding(source_key, provider);

  //bind provider
  internal::Key provider_of_provider_key(
      typeid(std::shared_ptr<Provider<std::shared_ptr<F>>>), name);
  std::shared_ptr<Provider<std::shared_ptr<F>>> concrete_provider(new internal::RawProviderWrapper<std::shared_ptr<F>>(provider));
  std::shared_ptr<Provider<internal::Any>> provider_of_provider(
      new internal::InstanceProvider(internal::Any(concrete_provider)));

  CreateBinding(provider_of_provider_key, provider_of_provider);
}

template<typename T>
void Binder::BindInstance(const T &instance) {
  Binder::BindInstance<T>(instance, std::string());
}

template<typename T>
void Binder::BindInstance(const T &instance, const std::string &name) {
  internal::Key key(typeid(T), name);

  std::shared_ptr<Provider<internal::Any>> provider(
      new internal::InstanceProvider(internal::Any(instance)));
  CreateBinding(key, provider);

  //provider binding
  internal::Key provider_of_provider_key(typeid(std::shared_ptr<Provider<T>>), name);
  std::shared_ptr<Provider<T>> concrete_provider(
      new internal::RawProviderWrapper<T>(provider));
  std::shared_ptr<Provider<internal::Any>> provider_of_provider(
      new internal::InstanceProvider(internal::Any(concrete_provider)));

  CreateBinding(provider_of_provider_key, provider_of_provider);
}

template<typename T, typename P>
void Binder::BindProvider() {
  BindProvider<T, P>(std::string());
}

template<typename T, typename P>
void Binder::BindProvider(const std::string &name) {
  static_assert(std::is_base_of<Provider<T>, P>::value, "BindProvider<T, P>() - P must implement Provider<T>");

  internal::Key key(typeid(T), name);
  std::shared_ptr<Provider<T>> provider(new P());
  std::shared_ptr<Provider<internal::Any>> any_provider(
      new internal::ConcreteProviderWrapper<T>(provider));

  CreateBinding(key, any_provider);

  //bind provider
  internal::Key provider_of_provider_key(typeid(std::shared_ptr<Provider<T>>), name);
  std::shared_ptr<Provider<internal::Any>> provider_of_provider(
      new internal::InstanceProvider(internal::Any(provider)));

  CreateBinding(provider_of_provider_key, provider_of_provider);
}

inline void Binder::CreateBinding(
    const internal::Key &key,
    const std::shared_ptr<Provider<internal::Any>> &provider) {
  AssertBindingNotExists(key);

  provider_map_.emplace(key, provider);
}

inline void Binder::AssertBindingNotExists(const internal::Key &key) {
  if (provider_map_.find(key) != provider_map_.end()) {
    throw BindingError(
        std::string("Binding already exists for " + key.GetFullName()));
  }
}

inline const std::unordered_map<internal::Key,
    std::shared_ptr<Provider<internal::Any>>>&Binder::GetProviderBindings() const {
  return provider_map_;
}

}
  // namespace cppdi

#endif  // CPPDI_INTERNAL_BINDER_IMPL_H_
