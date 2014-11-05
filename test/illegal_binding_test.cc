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

class MultiConstructor {
 public:
  MultiConstructor() {}
  MultiConstructor(int a) {}
};

class I {

};

class A : public I {

};

class B : public I {

};

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
      binder->BindConstructor<MultiConstructor>();
      binder->BindConstructor<MultiConstructor, int>();
    });
  , BindingError);
}

TEST(illegal_binding_test, types) {
  cppdi::InjectorFactory factory;

  EXPECT_THROW(
    factory.Create([](Binder *binder){
      binder->BindConstructor<A>();
      binder->BindConstructor<B>();
      binder->BindTypes<I, A>();
      binder->BindTypes<I, B>();
    });
  , BindingError);
}
