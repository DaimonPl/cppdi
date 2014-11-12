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

#include "tested_types.h"

using namespace cppdi;
using namespace std;

TEST(binding_to_function, no_argument) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([=](Binder *binder) {
    binder->BindFunction<int>(std::function<int()>(int_function));
  });
  DisposeGuard guard(injector);

  EXPECT_EQ(int_function(), injector->GetInstance<int>());
  EXPECT_EQ(int_function(), injector->GetInstance<shared_ptr<Provider<int>>>()->Get());
}

TEST(binding_to_function, with_argument) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([=](Binder *binder) {
    binder->BindFunction<int>(std::function<int()>(int_function));
    binder->BindFunction<long>(std::function<long(int)>(long_function_with_int_arg));
  });
  DisposeGuard guard(injector);

  EXPECT_EQ(long_function_with_int_arg(int_function()), injector->GetInstance<long>());
  EXPECT_EQ(long_function_with_int_arg(int_function()), injector->GetInstance<shared_ptr<Provider<long>>>()->Get());
}
