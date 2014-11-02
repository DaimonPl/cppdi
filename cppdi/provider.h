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

#ifndef CPPDI_PROVIDER_H_
#define CPPDI_PROVIDER_H_

#include <memory>

#include "errors.h"

namespace cppdi {

class Injector;

/**
 * Provider is responsible of providing instances of particular type.
 *
 * Concrete implementation may implement singleton or non-singleton instance
 * providing behavior.
 *
 * Custom provider implementations MUST use no-argument constructor. In case
 * initialization is required, Initialize method can be used for that purpose.
 */
template<typename T>
class Provider {
 public:
  virtual ~Provider() = default;

  /**
   * Returns instance.
   */
  virtual std::shared_ptr<T> Get() = 0;
 private:
  /**
   * Called once by cppdi internals. It's ensured that Initialize is called
   * always before first Get call.
   *
   * \param injector injector which shares same scope as this Provider instance.
   *        can be used to initialize provider with other dependencies.
   */
  virtual void Initialize(const std::shared_ptr<Injector> &injector) {}

  friend Injector;
};

}  // namespace cppdi

#endif  // CPPDI_PROVIDER_H_
