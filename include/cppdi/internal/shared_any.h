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

#ifndef CPPDI_INTERNAL_SHARED_ANY_H_
#define CPPDI_INTERNAL_SHARED_ANY_H_

#include <algorithm>
#include <memory>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace cppdi {
namespace internal {

/*
 * This code is based on public-domain code obtained from:
 * http://codereview.stackexchange.com/questions/20058/a-c11-any-class
 */

template<class T>
using StorageType = typename std::decay<T>::type;

struct SharedAny {
  bool is_null() const {
    return !sptr;
  }

  SharedAny() {}

  template<typename U> SharedAny(U&& value)
      : sptr(std::make_shared<Derived<StorageType<U>>>(std::forward<U>(value))) {}

  template<class U>
  StorageType<U>& as() {
    typedef StorageType<U> T;

    auto derived = std::static_pointer_cast<Derived<T>>(sptr);

    return derived->value;
  }

  template<class U>
  operator U() {
    return as<StorageType<U>>();
  }

  SharedAny(SharedAny& that) : sptr(that.sptr) {}

  SharedAny(SharedAny&& that) : sptr(std::move(that.sptr)) {}

  SharedAny(const SharedAny& that) : sptr(that.sptr) {}

  SharedAny(const SharedAny&& that) : sptr(that.sptr) {}

  SharedAny& operator=(const SharedAny& a) {
    if (sptr == a.sptr) {
      return *this;
    }

    sptr = a.sptr;

    return *this;
  }

  SharedAny& operator=(SharedAny&& a) {
    if (sptr == a.sptr) {
      return *this;
    }

    sptr = std::move(a.sptr);

    return *this;
  }

 private:
  struct Base {
    virtual ~Base() {}
  };

  template<typename T>
  struct Derived : Base {
    template<typename U> Derived(U&& value) : value(std::forward<U>(value)) {}

    T value;
  };

  std::shared_ptr<Base> sptr;
};

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_SHARED_ANY_H_
