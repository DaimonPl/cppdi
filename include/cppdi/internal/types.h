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

#ifndef CPPDI_INTERNAL_TYPES_H_
#define CPPDI_INTERNAL_TYPES_H_

#include <memory>
#include <unordered_map>

#include "cppdi/provider.h"
#include "cppdi/internal/key.h"
#include "cppdi/internal/shared_any.h"

namespace cppdi {
namespace internal {

template<typename T>
using BindingMap = std::unordered_map<internal::Key, std::shared_ptr<Provider<T>>>;

typedef Provider<internal::SharedAny> SharedAnyProvider;
typedef Provider<std::shared_ptr<void>> VoidPtrProvider;

typedef std::shared_ptr<Provider<internal::SharedAny>> SharedAnyProviderPtr;
typedef std::shared_ptr<Provider<std::shared_ptr<void>>> VoidPtrProviderPtr;

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_TYPES_H_
