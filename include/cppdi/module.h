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

#ifndef CPPDI_MODULE_H_
#define CPPDI_MODULE_H_

#include "cppdi/binder.h"
#include "cppdi/errors.h"

namespace cppdi {

/**
 * Module represents information about bindings - i.e. mapping between
 * interfaces and their target types / providers / instance.
 *
 * It's meant to create re-usable code modules which can be reused in different
 * parts/projects without need to re-define bindings (especially internal ones).
 *
 * For example Interface -> Concrete (where Concrete implements Interface)
 * represents type binding - all requests for Interface would evaluate to
 * Concrete type.
 *
 * Module implementations MUST be stateless.
 */
class Module {
 public:
  virtual ~Module() {}

  /**
   * Method called by InjectorFactory during Injector creation.
   * Method is called only once for particular Binder. It can be called multiple
   * times (with different binders as parameters).
   *
   * Implementation should create all corresponding bindings on provider Binder
   * instance.
   *
   * \param binder reference to binder on which bindings should be defined
   */
  virtual void Configure(Binder *binder) const = 0;
};

}  // namespace cppdi

#endif  // CPPDI_MODULE_H_
