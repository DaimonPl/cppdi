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

#include "cppdi/binder.h"
#include "cppdi/errors.h"
#include "cppdi/provider.h"
#include "cppdi/internal/any.h"
#include "cppdi/internal/key.h"

namespace cppdi {

class InjectorFactory;

/**
 * Main role of Injector is creation of object graphs. Type dependencies
 * (constructor arguments) are obtained basing on bindings defined in Module.
 *
 * Injector is obtained from InjectorFactory.
 */
class Injector : public std::enable_shared_from_this<Injector> {
 public:
  /**
   * Gets instance of type T
   */
  template<typename T>
  T GetInstance();

  /**
   * Gets instance of named type T (name)
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

  explicit Injector(const Binder &binder);
  internal::Any GetInstanceByKey(const internal::Key &key);
  std::shared_ptr<Provider<internal::Any>> GetProvider(const internal::Key &key);
  void AutoInitialize();

  State state_;
  std::unordered_map<internal::Key, std::shared_ptr<Provider<internal::Any>>>provider_map_;

  friend InjectorFactory;
};

}  // namespace cppdi

#include "cppdi/internal/injector_impl.h"

//  moved here due to file dependency
#include "cppdi/internal/linking_provider_impl.h"
#include "cppdi/internal/producing_provider_impl.h"

#endif  // CPPDI_INJECTOR_H_
