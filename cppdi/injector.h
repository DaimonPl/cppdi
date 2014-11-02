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
#include <unordered_map>

#include "binder.h"
#include "errors.h"
#include "provider.h"
#include "internal/key.h"
#include "internal/producer.h"

namespace cppdi {

namespace internal {
class LinkingProvider;
}  // namespace internal

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
  std::shared_ptr<T> GetInstance() throw (InjectionError);

  /**
   * Gets instance of named type T (name)
   */
  template<typename T>
  std::shared_ptr<T> GetInstance(const std::string &name) throw (InjectionError);

  /**
   * Gets provider of type T
   */
  template<typename T>
  std::shared_ptr<Provider<T>> GetProvider() throw (InjectionError);

  /**
   * Gets provider of named type T (name)
   */
  template<typename T>
  std::shared_ptr<Provider<T>> GetProvider(const std::string &name)
      throw (InjectionError);

  /**
   * Disposes injector to ensure that circular shared_ptr's between Injector
   * and internal Providers are cut.
   *
   * Disposed injector (and providers which are using that injector instance)
   * cannot be used for further injections. All attempts will throw
   * InjectionError
   *
   * If called on already disposed injector its behavior is no-operation.
   */
  void Dispose();

 private:
  enum State {
    UNINITIALIZED,
    INITIALIZED,
    DISPOSED
  };

  Injector(const Binder &binder);
  std::shared_ptr<void> GetInstance(const internal::Key &key)
      throw (InjectionError);
  std::shared_ptr<Provider<void>> GetProvider(const internal::Key &key)
      throw (InjectionError);

  State state_;
  std::unordered_map<internal::Key, std::shared_ptr<Provider<void>>>provider_map_;
  std::unordered_map<internal::Key, internal::Key> linked_bindings_map_;
  std::unordered_map<internal::Key, internal::Producer<void>> producer_map_;

  friend InjectorFactory;
  friend internal::LinkingProvider;
};

} // namespace cppdi

#include "internal/injector_impl.h"

//  moved here due to file dependency
#include "internal/linking_provider_impl.h"
#include "internal/producer_impl.h"

#endif  // CPPDI_INJECTOR_H_
