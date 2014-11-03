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

#ifndef CPPDI_ERRORS_H_
#define CPPDI_ERRORS_H_

#include <string>

namespace cppdi {

/**
 * Base error type from cppdi.
 *
 * cppdi errors should not be handled manually - they indicate bugs in program
 * setup / logic
 */
struct Error {
  /**
   * Information about error reason
   */
  std::string message;

 protected:
  Error(const std::string &msg)
      : message(msg) {
  }
};

/**
 * Error during injection phase.
 */
struct InjectionError : public Error {
  InjectionError(const std::string &msg)
      : Error(msg) {
  }
};

/**
 * Error during binding phase.
 */
struct BindingError : public Error {
  BindingError(const std::string &msg)
      : Error(msg) {
  }
};

}  // namespace cppdi

#endif  // CPPDI_ERRORS_H_
