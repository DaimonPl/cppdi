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
    B(int x) {x_ = x;}

    int x_;
  };

class C {
   public:
    C(shared_ptr<A> a) {a_ = a;}

    shared_ptr<A> a_;
  };

TEST(binding_to_constructor, no_argument) {
  class TestModule : public Module {
   public:
    void Configure(Binder *binder) const override {
      binder->BindConstructor<A>();
    }
  };

  TestModule module;
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create(module);

  EXPECT_FALSE(!injector->GetInstance<shared_ptr<A>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<A>>(), injector->GetInstance<shared_ptr<A>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<A>>(), injector->GetInstance<shared_ptr<Provider<shared_ptr<A>>>>()->Get());
}

TEST(binding_to_constructor, with_primitive_argument) {
  class TestModule : public Module {
   public:
    void Configure(Binder *binder) const override {
      binder->BindConstructor<B, int>();
      binder->BindInstance<int>(15);
    }
  };

  TestModule module;
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create(module);

  EXPECT_FALSE(!injector->GetInstance<shared_ptr<B>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<B>>(), injector->GetInstance<shared_ptr<B>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<B>>(), injector->GetInstance<shared_ptr<Provider<shared_ptr<B>>>>()->Get());
  EXPECT_EQ(15, injector->GetInstance<shared_ptr<B>>()->x_);
}

TEST(binding_to_constructor, with_type_argument) {
  class TestModule : public Module {
   public:
    void Configure(Binder *binder) const override {
      binder->BindConstructor<A>();
      binder->BindConstructor<C, shared_ptr<A>>();
    }
  };

  TestModule module;
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create(module);

  EXPECT_FALSE(!injector->GetInstance<shared_ptr<C>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<C>>(), injector->GetInstance<shared_ptr<C>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<C>>(), injector->GetInstance<shared_ptr<Provider<shared_ptr<C>>>>()->Get());
  EXPECT_EQ(injector->GetInstance<shared_ptr<A>>(), injector->GetInstance<shared_ptr<C>>()->a_);
}
