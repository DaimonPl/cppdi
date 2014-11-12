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
    binder->BindConstructor<Implementation>();
    binder->BindTypes<Interface, Implementation>();
    binder->BindConstructor<ImplementationWithDependencyAndNonArgCtor>();
    binder->BindConstructor<ImplementationWithDependencyAndNonArgCtor, shared_ptr<Interface>>("with_argument");
  });
  DisposeGuard guard(injector);

  shared_ptr<ImplementationWithDependencyAndNonArgCtor> normal = injector->GetInstance<shared_ptr<ImplementationWithDependencyAndNonArgCtor>>();
  shared_ptr<ImplementationWithDependencyAndNonArgCtor> named = injector->GetInstance<shared_ptr<ImplementationWithDependencyAndNonArgCtor>>("with_argument");

  EXPECT_FALSE(!normal);
  EXPECT_FALSE(!named);
  EXPECT_FALSE(normal == named);

  EXPECT_FALSE(normal->intf_);
  EXPECT_FALSE(!named->intf_);

  EXPECT_EQ(normal, injector->GetInstance<shared_ptr<Provider<shared_ptr<ImplementationWithDependencyAndNonArgCtor>>>>()->Get());
  EXPECT_EQ(named, injector->GetInstance<shared_ptr<Provider<shared_ptr<ImplementationWithDependencyAndNonArgCtor>>>>("with_argument")->Get());
}
