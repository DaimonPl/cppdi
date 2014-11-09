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

class A {

};

class B {
 public:
  B() {

  }

  B(shared_ptr<A> a) {
    a_ = a;
  }

  shared_ptr<A> a_;
};

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

TEST(named_binding, constructor) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder) {
    binder->BindConstructor<A>();
    binder->BindConstructor<B>();
    binder->BindConstructor<B, shared_ptr<A>>("with_argument");
  });
  DisposeGuard guard(injector);

  shared_ptr<B> normal_b = injector->GetInstance<shared_ptr<B>>();
  shared_ptr<B> named_b = injector->GetInstance<shared_ptr<B>>("with_argument");

  EXPECT_FALSE(!normal_b);
  EXPECT_FALSE(!named_b);
  EXPECT_FALSE(normal_b == named_b);

  EXPECT_FALSE(normal_b->a_);
  EXPECT_FALSE(!named_b->a_);

  EXPECT_EQ(normal_b, injector->GetInstance<shared_ptr<Provider<shared_ptr<B>>>>()->Get());
  EXPECT_EQ(named_b, injector->GetInstance<shared_ptr<Provider<shared_ptr<B>>>>("with_argument")->Get());
}
