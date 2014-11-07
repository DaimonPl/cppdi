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

int producing_fun() {
  return 18;
}

long producing_fun2(int a) {
  return a + 22;
}

TEST(binding_to_function, no_argument) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([=](Binder *binder) {
    binder->BindFunction<int>(std::function<int()>(producing_fun));
  });
  DisposeGuard guard(injector);

  EXPECT_EQ(18, injector->GetInstance<int>());
  EXPECT_EQ(18, injector->GetInstance<shared_ptr<Provider<int>>>()->Get());
}

TEST(binding_to_function, with_argument) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([=](Binder *binder) {
    binder->BindFunction<int>(std::function<int()>(producing_fun));
    binder->BindFunction<long>(std::function<long(int)>(producing_fun2));
  });
  DisposeGuard guard(injector);

  EXPECT_EQ(40, injector->GetInstance<long>());
  EXPECT_EQ(40, injector->GetInstance<shared_ptr<Provider<long>>>()->Get());
}
