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

#ifndef CPPDI_INTERNAL_PRODUCER_H_
#define CPPDI_INTERNAL_PRODUCER_H_

#include <string>
#include <typeindex>
#include <functional>

#include "cppdi/errors.h"

namespace cppdi {
namespace internal {

/**
 * Producer is function which is responsible for creation of new instances.
 * If certain type have constructor dependencies, they MUST be obtained from
 * passed Injector instance.
 */
template<typename T>
using Producer = std::function<std::shared_ptr<T>(const std::shared_ptr<Injector>&)>;

/**
 * Creates producer of type T using constructor with arguments Args
 */
template<typename T, typename ... Args>
Producer<T> make_producer();

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_PRODUCER_H_
