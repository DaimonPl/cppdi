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

#ifndef CPPDI_CPPDI_H_
#define CPPDI_CPPDI_H_

/**
 * Single header file which includes entire public cppdi API
 *
 * User of cppdi should include only this file
 */

/**
 * Compile time flags:
 * _CPPDI_DEBUG_MODE_ - enables additional runtime checks (including automatic
 *                      cycle detection)
 */

#include "cppdi/dispose_guard.h"
#include "cppdi/binder.h"
#include "cppdi/errors.h"
#include "cppdi/injector.h"
#include "cppdi/injector_factory.h"
#include "cppdi/module.h"
#include "cppdi/provider.h"

#endif  // CPPDI_CPPDI_H_
