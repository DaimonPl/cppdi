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

#include "cppdi/errors.h"
#include "cppdi/provider.h"
#include "cppdi/internal/any.h"
#include "cppdi/internal/key.h"

namespace cppdi {

class Injector;
class InjectorFactory;

/**
 * Binder is used to define (in declarative way) relations between injected
 * type and concrete instance which is going to be injected.
 *
 * Following bindings are supported:
 *  - constructor of Type
 *  - Producer of type
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
   * Binds constructor of type T, identified by Args.
   *
   * This binding method can be used only for constructors with non-named
   * shared_ptr arguments.
   *
   * If more complicated constructor is required, BindProvider<T,P> should be
   * used instead
   *
   * \throw BindingError if producer is already registered
   */
  template<typename T, typename ... Args>
  void BindConstructor();

  /**
   * Creates F -> T binding - T must derive from F.
   *
   * Created binding has singleton behaviour - i.e. all injections of F will
   * return same instance of T
   */
  template<typename F, typename T>
  void BindTypes();

  /**
   * Creates F (name) -> T binding - T must derive from F
   *
   * Created binding has singleton behaviour - i.e. all injections of F will
   * return same instance of T
   */
  template<typename F, typename T>
  void BindTypes(const std::string &name);

  /**
   * Creates T -> instance binding
   */
  template<typename T>
  void BindInstance(const T &instance);

  /**
   * Creates T (name) -> instance binding
   */
  template<typename T>
  void BindInstance(const T &instance, const std::string &name);

  /**
   * Creates T -> Provider<T> binding
   *
   * P must derive from Provider<T>
   */
  template<typename T, typename P>
  void BindProvider();

  /**
   * Creates T (name) -> Provider<T> binding
   *
   * P must derive from Provider<T>
   */
  template<typename T, typename P>
  void BindProvider(const std::string &name);

  // ensure binder is not passed by value
  Binder(const Binder &) = delete;
  Binder(Binder &&) = delete;
  void operator=(const Binder &) = delete;
  void operator=(Binder &&) = delete;
 private:
  Binder() {};

  const std::unordered_map<internal::Key, std::shared_ptr<Provider<internal::Any>>>&GetProviderBindings() const;
  void AssertBindingNotExists(const internal::Key &key);

  void CreateBinding(const internal::Key &key, const std::shared_ptr<Provider<internal::Any>> &provider);

  std::unordered_map<internal::Key, std::shared_ptr<Provider<internal::Any>>> provider_map_;

  friend Injector;
  friend InjectorFactory;
};

}  // namespace cppdi

#include "cppdi/internal/binder_impl.h"

#endif  // CPPDI_BINDER_H_
