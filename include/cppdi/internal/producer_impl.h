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

#ifndef CPPDI_INTERNAL_PRODUCER_IMPL_H_
#define CPPDI_INTERNAL_PRODUCER_IMPL_H_

#include <string>
#include <typeindex>
#include <functional>

#include "cppdi/errors.h"

namespace cppdi {
namespace internal {

template<typename T, typename... Args>
Producer<T> make_producer() {
  return [](const std::shared_ptr<Injector> &injector)->std::shared_ptr<T> {
    T *t = new T(injector->GetInstance<Args>()...);

    return std::shared_ptr<T>(t);
  };
}

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_PRODUCER_IMPL_H_
