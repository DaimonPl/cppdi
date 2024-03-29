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
#include <typeinfo>

#include "cppdi/binder.h"
#include "cppdi/errors.h"
#include "cppdi/internal/concrete_provider_wrapper.h"
#include "cppdi/internal/function_provider.h"
#include "cppdi/internal/instance_provider.h"
#include "cppdi/internal/linking_provider.h"
#include "cppdi/internal/producing_provider.h"
#include "cppdi/internal/ptr_instance_provider.h"
#include "cppdi/internal/raw_provider_wrapper.h"

namespace cppdi {

template<typename T, typename ... Args>
void Binder::BindConstructor(const std::string &name) {
  static_assert(!internal::is_crp<T>::value, "BindConstructor<T, Args...>()"
      " - T must not be constant, reference nor pointer");

  internal::Key key(typeid(std::shared_ptr<T>), name);

  internal::VoidPtrProviderPtr provider =
      std::make_shared<internal::ProducingProvider<T, Args...>>();

  CreateBinding(key, provider);
  CreateProviderBinding<std::shared_ptr<T>>(name, provider);
}

template<typename T, typename ... Args>
void Binder::BindConstructor() {
  BindConstructor<T, Args...>(std::string());
}

template<typename F, typename T>
void Binder::BindTypes() {
  BindTypes<F, T>(std::string(), std::string());
}

template<typename F, typename T>
void Binder::BindTypes(const std::string &f_name, const std::string &t_name) {
  static_assert(std::is_base_of<F, T>::value,
                "BindTypes<F, T>() - T must be a descendant of F");
  static_assert(!std::is_same<F, T>::value,
                "BindTypes<F, T>() - T cannot be same as F");
  static_assert(!internal::is_crp<T>::value, "BindTypes<F, T>()"
        " - T must not be constant, reference nor pointer");
  static_assert(!internal::is_crp<F>::value, "BindTypes<F, T>()"
          " - F must not be constant, reference nor pointer");

  internal::Key source_key(typeid(std::shared_ptr<F>), f_name);

  internal::VoidPtrProviderPtr provider =
      std::make_shared<internal::LinkingProvider<F, T>>(t_name);

  CreateBinding(source_key, provider);
  CreateProviderBinding<std::shared_ptr<F>>(f_name, provider);
}

template<typename T>
void Binder::BindInstance(const T &instance) {
  Binder::BindInstance<T>(instance, std::string());
}

template<typename T>
void Binder::BindInstance(const T &instance, const std::string &name) {
  static_assert(!internal::is_cr<T>::value,
                "BindInstance<T>() - T must not be constant nor reference");

  internal::Key key(typeid(T), name);

  internal::SharedAnyProviderPtr provider =
      std::make_shared<internal::InstanceProvider>(internal::SharedAny(instance));

  CreateBinding(key, provider);
  CreateProviderBinding<T>(name, provider);
}

template<typename T, typename P>
void Binder::BindProvider() {
  BindProvider<T, P>(std::string());
}

template<typename T, typename P>
void Binder::BindProvider(const std::string &name) {
  static_assert(!internal::is_crp<T>::value, "BindProvider<T, P>()"
          " - T must not be constant, reference nor pointer");
  static_assert(std::is_base_of<Provider<T>, P>::value,
                "BindProvider<T, P>() - P must implement Provider<T>");
  static_assert(std::is_default_constructible<P>::value,
                "BindProvider<T, P>() - P must be default constructible");

  internal::Key key(typeid(T), name);
  std::shared_ptr<Provider<T>> provider = std::make_shared<P>();
  internal::SharedAnyProviderPtr any_provider =
      std::make_shared<internal::ConcreteProviderWrapper<T>>(provider);

  CreateBinding(key, any_provider);
  CreateProviderBinding<T>(name, any_provider);
}

template<typename T, typename ...Args>
void Binder::BindFunction(const std::function<T(Args...)> &producing_func) {
  BindFunction<T, Args...>(producing_func, std::string());
}

template<typename T, typename ...Args>
void Binder::BindFunction(const std::function<T(Args...)> &producing_func,
                          const std::string &name) {
  static_assert(!internal::is_crp<T>::value, "BindFunction<T, Args...>()"
            " - T must not be constant, reference nor pointer");

  internal::Key key(typeid(T), name);

  internal::SharedAnyProviderPtr provider =
      std::make_shared<internal::FunctionProvider<T, Args...>>(producing_func);

  CreateBinding(key, provider);
  CreateProviderBinding<T>(name, provider);
}

inline void Binder::CreateBinding(
    const internal::Key &key,
    const std::shared_ptr<Provider<internal::SharedAny>> &provider) {
  AssertBindingNotExists(key);

  shared_any_provider_map_.emplace(key, provider);
}

inline void Binder::CreateBinding(
    const internal::Key &key,
    const std::shared_ptr<Provider<std::shared_ptr<void>>> &provider) {
  AssertBindingNotExists(key);

  shared_ptr_provider_map_.emplace(key, provider);
}

template<typename T>
void Binder::CreateProviderBinding(
    const std::string &name,
    const internal::SharedAnyProviderPtr &provider) {
  internal::Key provider_key(typeid(std::shared_ptr<Provider<T>>), name);

  std::shared_ptr<Provider<T>> concrete_provider =
      std::make_shared<internal::RawProviderWrapper<T>>(provider);
  internal::VoidPtrProviderPtr provider_of_provider =
      std::make_shared<internal::PtrInstanceProvider>(concrete_provider);

  CreateBinding(provider_key, provider_of_provider);
}

template<typename T>
void Binder::CreateProviderBinding(
    const std::string &name,
    const internal::VoidPtrProviderPtr &provider) {
  internal::Key provider_key(typeid(std::shared_ptr<Provider<T>>), name);

  internal::VoidPtrProviderPtr provider_of_provider =
        std::make_shared<internal::PtrInstanceProvider>(provider);

  CreateBinding(provider_key, provider_of_provider);
}

inline void Binder::AssertBindingNotExists(const internal::Key &key) {
  if (shared_any_provider_map_.find(key) != shared_any_provider_map_.end() ||
      shared_ptr_provider_map_.find(key) != shared_ptr_provider_map_.end()) {
    throw BindingError(
        std::string("Binding already exists for " + key.GetFullName()));
  }
}

}  // namespace cppdi

#endif  // CPPDI_INTERNAL_BINDER_IMPL_H_
