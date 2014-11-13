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

TEST(illegal_injection_test, missing_binding) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder) {

  });
  DisposeGuard guard(injector);

  EXPECT_THROW(injector->GetInstance<int>(), InjectionError);
  EXPECT_THROW(injector->GetInstance<Interface*>(), InjectionError);
  EXPECT_THROW(injector->GetInstance<shared_ptr<Interface>>(), InjectionError);
  EXPECT_THROW(injector->GetInstance<shared_ptr<Provider<shared_ptr<Interface>>>>(), InjectionError);
}

TEST(illegal_injection_test, injection_after_disposal) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder) {
    binder->BindInstance<int>(10);
    binder->BindConstructor<Implementation>();
    binder->BindTypes<Interface, Implementation>();
  });
  DisposeGuard guard(injector);

  shared_ptr<Provider<shared_ptr<Interface>>> b_provider = injector
      ->GetInstance<shared_ptr<Provider<shared_ptr<Interface>>>>();

  injector->Dispose();

  EXPECT_THROW(injector->GetInstance<int>(), InjectionError);
  EXPECT_THROW(b_provider->Get(), InjectionError);
}

TEST(illegal_injection_test, cyclic_binding_self) {
  cppdi::InjectorFactory factory(true);

  shared_ptr<Injector> injector = factory.Create([](Binder *binder) {
    binder->BindConstructor<SelfCyclic, shared_ptr<SelfCyclic>>();
  });
  DisposeGuard guard(injector);

  EXPECT_THROW(injector->GetInstance<shared_ptr<SelfCyclic>>(), InjectionCycleError);
}

TEST(illegal_injection_test, cyclic_binding_non_direct) {
  cppdi::InjectorFactory factory(true);

  shared_ptr<Injector> injector = factory.Create([](Binder *binder) {
    binder->BindConstructor<CycleA, shared_ptr<IfCycleB>>();
    binder->BindConstructor<CycleB, shared_ptr<IfCycleC>>();
    binder->BindConstructor<CycleC, shared_ptr<IfCycleA>>();
    binder->BindTypes<IfCycleA, CycleA>();
    binder->BindTypes<IfCycleB, CycleB>();
    binder->BindTypes<IfCycleC, CycleC>();
  });
  DisposeGuard guard(injector);

  EXPECT_THROW(injector->GetInstance<shared_ptr<IfCycleA>>(), InjectionCycleError);
}
