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

#ifndef CPPDI_INTERNAL_TRAITS_H_
#define CPPDI_INTERNAL_TRAITS_H_

#include <memory>
#include <type_traits>

namespace cppdi {
namespace internal {

/**
 * Code is based on answer for SO question from
 * http://stackoverflow.com/questions/26915718
 */

template <typename T>
struct is_shared_ptr_impl : std::false_type {};
template <typename T>
struct is_shared_ptr_impl<std::shared_ptr<T>> : std::true_type {};

/**
 * SFINAE trait which allows to enable special handling for shared_ptr<T>.
 */
template <typename T>
using is_shared_ptr =
    typename is_shared_ptr_impl<typename std::decay<T>::type>::type;

/**
 * Trait which checks if type is constant or reference
 */
template <typename T>
struct is_cr {
  constexpr static bool value = !std::is_same<T, typename std::decay<T>::type>::value;
};

/**
 * Trait which checks if type is constant, reference or pointer
 */
template <typename T>
struct is_crp {
  constexpr static bool value = is_cr<T>::value || std::is_pointer<T>::value;
};

}  // namespace internal
}  // namespace cppdi

#endif  // CPPDI_INTERNAL_TRAITS_H_
