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

TEST(binding_to_instance, primitives) {
  class TestModule : public Module {
   public:
    void Configure(Binder *binder) const override {
      binder->BindInstance<int>(5);
      binder->BindInstance<long>(100);
    }
  };

  TestModule module;
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create(module);

  EXPECT_EQ(5, injector->GetInstance<int>());
  EXPECT_EQ(100, injector->GetInstance<long>());

  EXPECT_EQ(5, injector->GetInstance<shared_ptr<Provider<int>>>()->Get());
  EXPECT_EQ(100, injector->GetInstance<shared_ptr<Provider<long>>>()->Get());
}

TEST(binding_to_instance, strings) {
  class TestModule : public Module {
   public:
    void Configure(Binder *binder) const override {
      binder->BindInstance<string>(string("xyz"));
    }
  };

  TestModule module;
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create(module);

  EXPECT_EQ(string("xyz"), injector->GetInstance<string>());
  EXPECT_EQ(string("xyz"), injector->GetInstance<shared_ptr<Provider<string>>>()->Get());
}

TEST(binding_to_instance, pointers) {
  static int a = 7;
  static long b = 9;

  class TestModule : public Module {
   public:
    void Configure(Binder *binder) const override {
      binder->BindInstance<int*>(&a);
      binder->BindInstance<long*>(&b);
    }
  };

  TestModule module;
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create(module);

  EXPECT_EQ(&a, injector->GetInstance<int*>());
  EXPECT_EQ(&b, injector->GetInstance<long*>());
  EXPECT_EQ(&a, injector->GetInstance<shared_ptr<Provider<int*>>>()->Get());
  EXPECT_EQ(&b, injector->GetInstance<shared_ptr<Provider<long*>>>()->Get());
}
