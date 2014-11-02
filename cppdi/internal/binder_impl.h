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
#include <type_traits>

#include "../binder.h"
#include "../errors.h"
#include "concrete_provider_wrapper.h"
#include "instance_provider.h"

namespace cppdi {

template<typename T, typename ... Args>
void Binder::BindConstructor() throw (BindingError) {
  internal::Key key(typeid(T));

  AssertBindingNotExists(key);

  internal::Producer<T> producer = internal::make_producer<T, Args...>();

  producer_map_.emplace(key, producer);
}

template<typename F, typename T>
void Binder::BindTypes() throw (BindingError) {
  BindTypes<F, T>(std::string());
}

template<typename F, typename T>
void Binder::BindTypes(const std::string &name) throw (BindingError) {
  static_assert(std::is_base_of<F, T>::value,
      "T must be a descendant of F"
  );
  static_assert(!std::is_same<F, T>::value,
      "T cannot be same as F"
  );

  internal::Key source_key(typeid(F), name);
  AssertBindingNotExists(source_key);

  internal::Key target_key(typeid(T));

  linked_bindings_map_.emplace(source_key, target_key);
}

template<typename T>
void Binder::BindInstance(const std::shared_ptr<T> &instance)
    throw (BindingError) {
  Binder::BindInstance<T>(instance, std::string());
}

template<typename T>
void Binder::BindInstance(const std::shared_ptr<T> &instance,
                          const std::string &name) throw (BindingError) {
  internal::Key key(typeid(T), name);
  AssertBindingNotExists(key);

  std::shared_ptr<Provider<void>> provider(
      new internal::InstanceProvider<T>(instance));

  provider_map_.emplace(key, provider);
}

template<typename T, typename P>
void Binder::BindProvider() throw (BindingError) {
  BindProvider<T, P>(std::string());
}

template<typename T, typename P>
void Binder::BindProvider(const std::string &name) throw (BindingError) {
  static_assert(std::is_base_of<Provider<T>, P>::value,
      "P must implement Provider<T>"
  );

  internal::Key key(typeid(T), name);
  AssertBindingNotExists(key);

  std::shared_ptr<P> provider(new P());

  std::shared_ptr<Provider<void>> raw_provider(
      new internal::ConcreteProviderWrapper<T>(provider));

  provider_map_.emplace(key, raw_provider);
}

void Binder::AssertBindingNotExists(const internal::Key &key) {
  if (provider_map_.find(key) != provider_map_.end()
      || linked_bindings_map_.find(key) != linked_bindings_map_.end()
      || producer_map_.find(key) != producer_map_.end()) {
    throw BindingError(
        std::string("Binding already exists for " + key.GetFullName()));
  }
}

const std::unordered_map<internal::Key, std::shared_ptr<Provider<void>>>& Binder::GetProviderBindings() const {
  return provider_map_;
}

const std::unordered_map<internal::Key, internal::Key>& Binder::GetLinkedBindings() const {
  return linked_bindings_map_;
}

const std::unordered_map<internal::Key, internal::Producer<void>> &Binder::GetPoducerBindings() const {
  return producer_map_;
}

}  // namespace cppdi

#endif  // CPPDI_INTERNAL_BINDER_IMPL_H_
