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

#ifndef CPPDI_INTERNAL_PRODUCER_REPOSITORY_IMPL_H_
#define CPPDI_INTERNAL_PRODUCER_REPOSITORY_IMPL_H_

#include <typeindex>

#include "../producer_repository.h"

namespace cppdi {

ProducerRepository &ProducerRepository::Instance() {
  static ProducerRepository instance;

  return instance;
}

template<typename T>
void ProducerRepository::Register() throw (BindingError) {
  static_assert(std::is_default_constructible<T>::value,
      "T must be default constructible"
  );

  Register<T>([](const std::shared_ptr<Injector>&)->std::shared_ptr<T> {
    return std::shared_ptr<T>(new T());
  });
}

template<typename T>
void ProducerRepository::Register(const Producer<T> &producer)
    throw (BindingError) {
  std::type_index type = typeid(T);

  auto producer_it = producers_.find(type);

  if (producers_.end() != producer_it) {
    throw BindingError(
        std::string("Producer already exists for ") + type.name());
  }

  producers_.emplace(type, producer);
}

template<typename T>
void ProducerRepository::Unregister() {
  producers_.erase(typeid(T));
}

const Producer<void> ProducerRepository::Get(const std::type_index &type) const
    throw (InjectionError) {
  auto producer_it = producers_.find(type);

  if (producers_.end() == producer_it) {
    throw InjectionError(
        std::string("Producer does not exist for ") + type.name());
  }

  return producer_it->second;
}

}  // namespace cppdi

#endif  // CPPDI_INTERNAL_PRODUCER_REPOSITORY_IMPL_H_
