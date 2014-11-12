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

TEST(illegal_binding_test, instances) {
  cppdi::InjectorFactory factory;

  EXPECT_THROW(
    factory.Create([](Binder *binder){
      binder->BindInstance<int>(6);
      binder->BindInstance<int>(7);
    });
  , BindingError);
}

TEST(illegal_binding_test, constructors) {
  cppdi::InjectorFactory factory;

  EXPECT_THROW(
    factory.Create([](Binder *binder){
      binder->BindConstructor<IntDependentWithNonArgCtor>();
      binder->BindConstructor<IntDependentWithNonArgCtor, int>();
    });
  , BindingError);
}

TEST(illegal_binding_test, types) {
  cppdi::InjectorFactory factory;

  EXPECT_THROW(
    factory.Create([](Binder *binder){
      binder->BindConstructor<Implementation>();
      binder->BindConstructor<ImplementationOfTwoInterfaces>();
      binder->BindTypes<Interface, Implementation>();
      binder->BindTypes<Interface, ImplementationOfTwoInterfaces>();
    });
  , BindingError);
}
