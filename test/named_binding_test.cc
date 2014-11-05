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

TEST(named_binding, instance) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder){
    binder->BindInstance<int>(5, "a");
    binder->BindInstance<int>(10, "b");
  });
  DisposeGuard guard(injector);

  EXPECT_EQ(5, injector->GetInstance<int>("a"));
  EXPECT_EQ(10, injector->GetInstance<int>("b"));

  EXPECT_EQ(5, injector->GetInstance<shared_ptr<Provider<int>>>("a")->Get());
  EXPECT_EQ(10, injector->GetInstance<shared_ptr<Provider<int>>>("b")->Get());
}
