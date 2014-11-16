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
#include "cppdi/internal/cycle_verifier.h"
#include "cppdi/internal/key.h"
#include "cppdi/internal/shared_any.h"
#include "cppdi/internal/traits.h"
#include "cppdi/internal/types.h"

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
   * Has same behavior as GetInstance<T>() but injects named binding of T.
   */
  template <typename T>
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

 private:
  enum State {
    UNINITIALIZED,
    INITIALIZED,
    DISPOSED
  };

  explicit Injector(internal::BindingMap<internal::SharedAny> &&shared_any_providers,
                    internal::BindingMap<std::shared_ptr<void>> &&shared_ptr_providers);
  std::shared_ptr<Provider<internal::SharedAny>> &GetAnyProvider(const internal::Key &key);
  std::shared_ptr<Provider<std::shared_ptr<void>>> &GetPtrProvider(const internal::Key &key);
  void AutoInitialize();

  template <typename T>
  auto GetNamedInstance(const std::string &name)
      -> typename std::enable_if<!internal::is_shared_ptr<T>{}, T>::type;

  template <typename T>
  auto GetNamedInstance(const std::string &name)
      -> typename std::enable_if<internal::is_shared_ptr<T>{}, T>::type;

  State state_;
  internal::BindingMap<internal::SharedAny> shared_any_provider_map_;
  internal::BindingMap<std::shared_ptr<void>> shared_ptr_provider_map_;

  internal::SharedAnyProviderPtr empty_any_provider_;
  internal::VoidPtrProviderPtr empty_ptr_provider_;

#ifdef _CPPDI_DEBUG_MODE_
  internal::CycleVerifier cycle_verifier_;
#endif

  // ensure injector is not passed by value
  Injector(const Injector &) = delete;
  Injector(Injector &&) = delete;
  void operator=(const Injector &) = delete;
  void operator=(Injector &&) = delete;

  friend InjectorFactory;
};

}  // namespace cppdi

#include "cppdi/internal/injector_impl.h"

//  moved here due to file dependency
#include "cppdi/internal/function_provider_impl.h"
#include "cppdi/internal/linking_provider_impl.h"
#include "cppdi/internal/producing_provider_impl.h"

#endif  // CPPDI_INJECTOR_H_
