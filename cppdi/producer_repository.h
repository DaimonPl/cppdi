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

#ifndef CPPDI_PRODUCER_REPOSITORY_H_
#define CPPDI_PRODUCER_REPOSITORY_H_

#include <memory>
#include <functional>
#include <unordered_map>

#include "errors.h"

namespace cppdi {

class Injector;

/**
 * Producer is function which is responsible for creation of new instances.
 * If certain type have constructor dependencies, they MUST be obtained from
 * passed Injector instance.
 *
 * There should be only one producer for certain type regardless of number of
 * used injectors.
 */
template<typename T>
using Producer = std::function<std::shared_ptr<T>(const std::shared_ptr<Injector>&)>;

/**
 * Repository of producers. It implements classic singleton pattern to ensure
 * single repository is used for entire program. Allowance of multiple
 * repository instances would make cppdi usage prone to bugs.
 */
class ProducerRepository {
 public:
  /**
   * Returns repository instance
   */
  static ProducerRepository &Instance();

  /**
   * Registers type T with constructor arguments Args.
   *
   * This registration method can be used only for constructors up to 5
   * parameters with non-named shared_ptr arguments.
   *
   * If more complicated constructor is required, Register<T>(Producer<T>)
   * should be used instead
   *
   * \throw BindingError if producer is already registered
   */
  template<typename T, typename... Args>
  void Register() throw(BindingError);

  /**
   * Registers producer in repository
   *
   * \throw BindingError if producer is already registered
   */
  template<typename T>
  void Register(const Producer<T> &producer) throw (BindingError);

  /**
   * Unregisters producer from repository.
   *
   * If producer is not registered, this method has no-operation behavior.
   */
  template<typename T>
  void Unregister();
 private:
  const Producer<void> Get(const std::type_index &type) const
      throw (InjectionError);

  std::unordered_map<std::type_index, Producer<void>> producers_;

  ProducerRepository() = default;

  friend Injector;
};

}  // namespace cppdi

#include "internal/producer_repository_impl.h"

#endif  // CPPDI_PRODUCER_REPOSITORY_H_
