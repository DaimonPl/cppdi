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

#include "../errors.h"

namespace cppdi {
namespace internal {

template<typename T>
Producer<T> make_producer() {
  return [](const std::shared_ptr<Injector> &injector)->std::shared_ptr<T> {
    return std::shared_ptr<T>(new T());
  };
}

template<typename T, typename A>
Producer<T> make_producer() {
  return [](const std::shared_ptr<Injector> &injector)->std::shared_ptr<T> {
    std::shared_ptr<A> a = injector->GetInstance<A>();

    return std::shared_ptr<T>(new T(a));
  };
}

template<typename T, typename A, typename B>
Producer<T> make_producer() {
  return [](const std::shared_ptr<Injector> &injector)->std::shared_ptr<T> {
    std::shared_ptr<A> a = injector->GetInstance<A>();
    std::shared_ptr<B> b = injector->GetInstance<B>();

    return std::shared_ptr<T>(new T(a, b));
  };
}

template<typename T, typename A, typename B, typename C>
Producer<T> make_producer() {
  return [](const std::shared_ptr<Injector> &injector)->std::shared_ptr<T> {
    std::shared_ptr<A> a = injector->GetInstance<A>();
    std::shared_ptr<B> b = injector->GetInstance<B>();
    std::shared_ptr<C> c = injector->GetInstance<C>();

    return std::shared_ptr<T>(new T(a, b, c));
  };
}

template<typename T, typename A, typename B, typename C, typename D>
Producer<T> make_producer() {
  return [](const std::shared_ptr<Injector> &injector)->std::shared_ptr<T> {
    std::shared_ptr<A> a = injector->GetInstance<A>();
    std::shared_ptr<B> b = injector->GetInstance<B>();
    std::shared_ptr<C> c = injector->GetInstance<C>();
    std::shared_ptr<D> d = injector->GetInstance<D>();

    return std::shared_ptr<T>(new T(a, b, c, d));
  };
}

template<typename T, typename A, typename B, typename C, typename D, typename E>
Producer<T> make_producer() {
  return [](const std::shared_ptr<Injector> &injector)->std::shared_ptr<T> {
    std::shared_ptr<A> a = injector->GetInstance<A>();
    std::shared_ptr<B> b = injector->GetInstance<B>();
    std::shared_ptr<C> c = injector->GetInstance<C>();
    std::shared_ptr<D> d = injector->GetInstance<D>();
    std::shared_ptr<E> e = injector->GetInstance<E>();

    return std::shared_ptr<T>(new T(a, b, c, d, e));
  };
}

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_PRODUCER_IMPL_H_