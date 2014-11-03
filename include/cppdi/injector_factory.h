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

#ifndef CPPDI_INJECTOR_FACTORY_H_
#define CPPDI_INJECTOR_FACTORY_H_

#include <initializer_list>
#include <memory>

#include "cppdi/injector.h"
#include "cppdi/module.h"

namespace cppdi {

/**
 * Injector factory. Each created injector has its own scope (see Injector)
 */
class InjectorFactory {
 public:
  /**
   * Creates injector from single module
   *
   * \param module module to create injector from
   */
  std::shared_ptr<Injector> Create(const Module &module) const;

  /**
   * Creates injector from module initialization list
   *
   * \param modules initialization list of modules
   */
  std::shared_ptr<Injector> Create(
      const std::initializer_list<const Module*> &modules) const;
};

}  // namespace cppdi

#include "internal/injector_factory_impl.h"

#endif  // CPPDI_INJECTOR_FACTORY_H_
