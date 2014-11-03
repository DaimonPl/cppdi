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

#ifndef CPPDI_INTERNAL_ANY_H_
#define CPPDI_INTERNAL_ANY_H_

#include <algorithm>
#include <type_traits>
#include <utility>
#include <typeinfo>
#include <string>
#include <cassert>

namespace cppdi {
namespace internal {

/*
 * This code is based on public-domain code obtained from:
 * http://codereview.stackexchange.com/questions/20058/a-c11-any-class
 */

template<class T>
using StorageType = typename std::decay<T>::type;

struct Any {
  bool is_null() const {
    return !ptr;
  }

  bool not_null() const {
    return ptr;
  }

  template<typename U> Any(U&& value)
      : ptr(new Derived<StorageType<U>>(forward<U>(value))) {}

  template<class U> bool is() const {
    typedef StorageType<U> T;

    auto derived = dynamic_cast<Derived<T>*>(ptr);

    return derived;
  }

  template<class U>
  StorageType<U>& as() {
    typedef StorageType<U> T;

    auto derived = dynamic_cast<Derived<T>*>(ptr);

    if (!derived) {
      throw std::bad_cast();
    }

    return derived->value;
  }

  template<class U>
  operator U() {
    return as<StorageType<U>>();
  }

  Any() : ptr(nullptr) {}

  Any(Any& that) : ptr(that.clone()) {}

  Any(Any&& that) : ptr(that.ptr) {
    that.ptr = nullptr;
  }

  Any(const Any& that) : ptr(that.clone()) {}

  Any(const Any&& that) : ptr(that.clone()) {}

  Any& operator=(const Any& a) {
    if (ptr == a.ptr) {
      return *this;
    }

    auto old_ptr = ptr;

    ptr = a.clone();

    if (old_ptr) {
      delete old_ptr;
    }

    return *this;
  }

  Any& operator=(Any&& a) {
    if (ptr == a.ptr) {
      return *this;
    }

    std::swap(ptr, a.ptr);

    return *this;
  }

  ~Any() {
    if (ptr) {
      delete ptr;
    }
  }

 private:
  struct Base {
    virtual ~Base() {}

    virtual Base* clone() const = 0;
  };

  template<typename T>
  struct Derived : Base {
    template<typename U> Derived(U&& value) : value(forward<U>(value)) {}

    T value;

    Base* clone() const {
      return new Derived<T>(value);
    }
  };

  Base* clone() const {
    if (ptr) {
      return ptr->clone();
    } else {
      return nullptr;
    }
  }

  Base* ptr;
};

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_ANY_H_
