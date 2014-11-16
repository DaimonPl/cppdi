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
#include <exception>

namespace cppdi {

/**
 * Base error type from cppdi.
 *
 * cppdi errors should not be handled manually - they indicate bugs in program
 * setup / logic.
 */
class Error : public std::exception {
 public:
  /**
   * Returns reason message
   */
  const char* what() const noexcept override {
    return message.c_str();
  }

 protected:
  /**
   * Creates Error
   *
   * @param msg message describing reason of error
   */
  explicit Error(const std::string &msg) : message(msg) {}

 private:
  std::string message;
};

/**
 * Error during injection phase.
 */
struct InjectionError : public Error {
  /**
   * Creates InjectionError
   *
   * @param msg message describing reason of error
   */
  explicit InjectionError(const std::string &msg) : Error(msg) {}
};

/**
 * Error caused by cycle during injection.
 *
 * It's thrown only during debug mode (_CPPDI_DEBUG_MODE_ flag must be enabled)
 *
 * If debug mode is not enabled, injection cycle will lead to stack overflow.
 */
struct InjectionCycleError : public InjectionError {
  /**
   * Creates InjectionCycleError
   *
   * @param msg message describing reason of error
   */
  explicit InjectionCycleError(const std::string &msg) : InjectionError(msg) {}
};

/**
 * Error during binding phase.
 */
struct BindingError : public Error {
  /**
   * Creates BindingError
   *
   * @param msg message describing reason of error
   */
  explicit BindingError(const std::string &msg) : Error(msg) {}
};

}  // namespace cppdi

#endif  // CPPDI_ERRORS_H_
