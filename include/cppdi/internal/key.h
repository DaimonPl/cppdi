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

#ifndef CPPDI_INTERNAL_KEY_H_
#define CPPDI_INTERNAL_KEY_H_

#include <string>
#include <typeindex>
#include <functional>

#include "cppdi/errors.h"

namespace cppdi {
namespace internal {

class Key {
 public:
  Key(const std::type_index &type, const std::string &name)
      : type_(type),
        name_(name) {
  }

  explicit Key(const std::type_index &type)
      : type_(type) {
  }

  const std::string& GetName() const {
    return name_;
  }

  const std::type_index& GetType() const {
    return type_;
  }

  const std::string GetFullName() const {
    return std::string("Key{") + type_.name() + ',' + name_ + '}';
  }

  bool operator==(const Key& other) const {
    return type_ == other.type_ && name_ == other.name_;
  }

  bool operator!=(const Key& other) const {
    return !operator==(other);
  }

 private:
  std::type_index type_;
  std::string name_;
};

}  // namespace internal
}  // namespace cppdi

namespace std {

template<> struct hash<cppdi::internal::Key> {
  size_t operator()(const cppdi::internal::Key & key) const {
    size_t h1 = hash<string>()(key.GetName());
    size_t h2 = hash<type_index>()(key.GetType());

    return h1 ^ (h2 << 1);
  }
};

}  // namespace std

#endif  // CPPDI_INTERNAL_KEY_H_
