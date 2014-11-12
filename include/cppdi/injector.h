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

#ifndef CPPDI_INJECTOR_H_
#define CPPDI_INJECTOR_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "cppdi/errors.h"
#include "cppdi/provider.h"
#include "cppdi/internal/any.h"
#include "cppdi/internal/key.h"

namespace cppdi {

class InjectorFactory;

/**
 * Main role of Injector is creation of object graphs. It acts as entry-point to
 * cppdi and maintains its own scope (singleton instances created by different
 * injectors won't have same address).
 *
 * In case injected type has constructor arguments, Injector will automatically
 * (and recursively) obtain instances from its scope.
 *
 * Injector can be obtained only from InjectorFactory as shared_ptr<Injector>.
 */
class Injector : public std::enable_shared_from_this<Injector> {
 public:
  /**
   * Gets instance of type T
   *
   * @throw InjectionError if injector has been disposed or bindings are missing
   *        for requested type and/or its dependencies
   */
  template<typename T>
  T GetInstance();

  /**
   * Has same behavor as GetInstance<T> but returns named instance of T.
   */
  template<typename T>
  T GetInstance(const std::string &name);

  /**
   * Disposes injector to ensure that circular shared_ptr's between Injector
   * and internal Providers are cut.
   *
   * Disposed injector (and providers which are using that injector instance)
   * cannot be used for further injections. All attempts will throw
   * InjectionError.
   *
   * If called on already disposed injector its behavior is no-operation.
   */
  void Dispose();

  // ensure injector is not passed by value
  Injector(const Injector &) = delete;
  Injector(Injector &&) = delete;
  void operator=(const Injector &) = delete;
  void operator=(Injector &&) = delete;

 private:
  enum State {
    UNINITIALIZED,
    INITIALIZED,
    DISPOSED
  };

  explicit Injector(std::unordered_map<internal::Key, std::shared_ptr<Provider<internal::Any>>> &&providers);
  std::shared_ptr<Provider<internal::Any>> &GetProvider(const internal::Key &key);
  void AutoInitialize();

  State state_;
  std::unordered_map<internal::Key, std::shared_ptr<Provider<internal::Any>>>provider_map_;

  friend InjectorFactory;
};

}  // namespace cppdi

#include "cppdi/internal/injector_impl.h"

//  moved here due to file dependency
#include "cppdi/internal/function_provider_impl.h"
#include "cppdi/internal/linking_provider_impl.h"
#include "cppdi/internal/producing_provider_impl.h"

#endif  // CPPDI_INJECTOR_H_
