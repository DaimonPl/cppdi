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

#ifndef CPPDI_BINDER_H_
#define CPPDI_BINDER_H_

#include <string>
#include <unordered_map>

#include "errors.h"
#include "provider.h"
#include "internal/key.h"

namespace cppdi {

class Injector;

/**
 * Binder is used to define (in declarative way) relations between injected
 * type and concrete instance which is going to be injected.
 *
 * Following bindings are supported:
 *  - Type -> ConcreteType (ConcreteType must derive from Type)
 *  - Type -> instance
 *  - Type -> Provider<Type>
 *
 *  Additionally named bindings are supported. Named bindings allow to define
 *  several bindings for same type for exemple:
 *
 *  Type ("normal") -> NormalImplOfType
 *  Type ("debug")  -> DebugImplOfType
 */
class Binder {
 public:
  /**
   * Creates F -> T binding - T must derive from F.
   *
   * Created binding has singleton behaviour - i.e. all injections of F will
   * return same instance of T
   */
  template<typename F, typename T>
  void Bind() throw(BindingError);

  /**
   * Creates F (name) -> T binding - T must derive from F
   *
   * Created binding has singleton behaviour - i.e. all injections of F will
   * return same instance of T
   */
  template<typename F, typename T>
  void Bind(const std::string &name) throw(BindingError);

  /**
   * Creates T -> instance binding
   */
  template<typename T>
  void Bind(const std::shared_ptr<T> &instance) throw(BindingError);

  /**
   * Creates T (name) -> instance binding
   */
  template<typename T>
  void Bind(const std::shared_ptr<T> &instance, const std::string &name)
                throw(BindingError);

  /**
   * Creates T -> Provider<T> binding
   *
   * P must derive from Provider<T>
   */
  template<typename T, typename P>
  void BindProvider() throw(BindingError);

  /**
   * Creates T (name) -> Provider<T> binding
   *
   * P must derive from Provider<T>
   */
  template<typename T, typename P>
  void BindProvider(const std::string &name) throw(BindingError);

 private:
  const std::unordered_map<internal::Key, std::shared_ptr<Provider<void>>> &GetProviderBindings() const;
  const std::unordered_map<internal::Key, internal::Key> &GetLinkedBindings() const;

  void AssertBindingNotExists(const internal::Key &key);

  std::unordered_map<internal::Key, std::shared_ptr<Provider<void>>> provider_map_;
  std::unordered_map<internal::Key, internal::Key> linked_bindings_map_;

  friend Injector;
};

}
  // namespace cppdi

#include "internal/binder_impl.h"

#endif  // CPPDI_BINDER_H_
