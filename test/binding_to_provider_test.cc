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

TEST(binding_to_provider, primitive) {
  cppdi::InjectorFactory factory;
  IntProvider provider;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder){
    binder->BindProvider<int, IntProvider>();
  });
  DisposeGuard guard(injector);

  EXPECT_EQ(provider.Get(), injector->GetInstance<int>());
  EXPECT_EQ(provider.Get(), injector->GetInstance<shared_ptr<Provider<int>>>()->Get());
}

TEST(binding_to_provider, type) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder){
    binder->BindProvider<shared_ptr<Interface>, InterfaceProvider>();
  });
  DisposeGuard guard(injector);

  EXPECT_FALSE(!injector->GetInstance<shared_ptr<Interface>>());
  EXPECT_FALSE(!injector->GetInstance<shared_ptr<Provider<shared_ptr<Interface>>>>()->Get());
}
