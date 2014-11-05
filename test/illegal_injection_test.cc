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

#include "gtest/gtest.h"
#include "cppdi/cppdi.h"

using namespace cppdi;
using namespace std;

class I {

};

TEST(illegal_injection_test, missing_binding) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder){

  });

  EXPECT_THROW(injector->GetInstance<int>(), InjectionError);
  EXPECT_THROW(injector->GetInstance<I>(), InjectionError);
  EXPECT_THROW(injector->GetInstance<shared_ptr<I>>(), InjectionError);
  EXPECT_THROW(injector->GetInstance<shared_ptr<Provider<shared_ptr<I>>>>(), InjectionError);
}