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

#include <functional>
#include <string>
#include <unordered_map>

#include "cppdi/errors.h"
#include "cppdi/provider.h"
#include "cppdi/internal/key.h"
#include "cppdi/internal/shared_any.h"
#include "cppdi/internal/types.h"

namespace cppdi {

class InjectorFactory;

/**
 * Binder is used to define (in declarative way) relations between injected
 * type and concrete instance which is going to be injected.
 *
 * Following bindings are supported:
 *  - constructor of Type
 *  - Type -> ConcreteType (ConcreteType must derive from Type)
 *  - Type -> instance
 *  - Type -> Provider<Type>
 *  - Type -> std::function (function must return T)
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
   * Binds constructor of type T, identified by arguments Args.
   *
   * This binding method can be used only for constructors with non-named
   * arguments.
   *
   * If more complicated constructor is required, BindProvider<T,P> should be
   * used instead.
   *
   * Types bound by this method have singleton behavior and can be injected as:
   *  - shared_ptr<T>
   *  - shared_ptr<Provider<shared_ptr<T>>>
   *
   * @throw BindingError if binding for shared_ptr<T> already exist
   */
  template<typename T, typename ... Args>
  void BindConstructor();

  /**
   * Has same behavior as BindConstructor<T, Args>() but creates named binding
   * of t.
   */
  template<typename T, typename ... Args>
  void BindConstructor(const std::string &name);

  /**
   * Creates shared_ptr<F> -> shared_ptr<T> binding - T must derive from F.
   *
   * Binding creates by this method shares behavior of T - i.e. if T is singleton
   * binding, then F would be singleton as well.
   *
   * Types bound by this method can be injected as:
   *  - shared_ptr<F>
   *  - shared_ptr<Provider<shared_ptr<F>>>
   *
   * @throw BindingError if binding for shared_ptr<F> already exist
   */
  template<typename F, typename T>
  void BindTypes();

  /**
   * Has same behavior as BindTypes<F, T>() but creates named binding of F to
   * named binding of T.
   */
  template<typename F, typename T>
  void BindTypes(const std::string &f_name, const std::string &t_name);

  /**
   * Creates T -> instance binding
   *
   * Types bound by this method can be injected as:
   *  - T
   *  - shared_ptr<Provider<T>>
   *
   * @throw BindingError if binding for T already exist
   */
  template<typename T>
  void BindInstance(const T &instance);

  /**
   * Has same behavior as BindInstance<T>(const T&) but creates named binding
   * of T.
   */
  template<typename T>
  void BindInstance(const T &instance, const std::string &name);

  /**
   * Creates T -> Provider<T> binding
   *
   * P must derive from Provider<T> and have public, non-argument constructor.
   *
   * Types bound by this method can be injected as:
   *  - T
   *  - shared_ptr<Provider<T>>
   *
   * @throw BindingError if binding for T already exist
   */
  template<typename T, typename P>
  void BindProvider();

  /**
   * Has same behavior as BindProvider<T, P>() but creates named binding of T.
   */
  template<typename T, typename P>
  void BindProvider(const std::string &name);

  /**
   * Creates binding between T and producing_fun. Provider function will be
   * called each time injection of T is requested. Singleton behavior is up to
   * internal function implementation.
   *
   * In case function has arguments, they'll be obtained directly from injector
   * and passed to function during invocation.
   *
   * Types bound by this method can be injected as:
   *  - T
   *  - shared_ptr<Provider<T>>
   *
   * @throw BindingError if binding for T already exist
   */
  template<typename T, typename ...Args>
  void BindFunction(const std::function<T(Args...)> &producing_func);

  /**
   * Has same behavior as BindFunction<T, Args...>() but creates named binding
   * of T.
   */
  template<typename T, typename ...Args>
  void BindFunction(const std::function<T(Args...)> &producing_func,
                    const std::string &name);

 private:
  Binder() {}

  void AssertBindingNotExists(const internal::Key &key);
  template<typename T>
  void CreateProviderBinding(const std::string &name,
                             const internal::SharedAnyProviderPtr &provider);
  template<typename T>
  void CreateProviderBinding(const std::string &name,
                             const internal::VoidPtrProviderPtr &provider);
  void CreateBinding(const internal::Key &key,
                     const internal::SharedAnyProviderPtr &provider);
  void CreateBinding(const internal::Key &key,
                     const internal::VoidPtrProviderPtr &provider);

  internal::BindingMap<internal::SharedAny> shared_any_provider_map_;
  internal::BindingMap<std::shared_ptr<void>> shared_ptr_provider_map_;

  // ensure binder is not passed by value
  Binder(const Binder &) = delete;
  Binder(Binder &&) = delete;
  void operator=(const Binder &) = delete;
  void operator=(Binder &&) = delete;

  friend InjectorFactory;
};

}  // namespace cppdi

#include "cppdi/internal/binder_impl.h"

#endif  // CPPDI_BINDER_H_
