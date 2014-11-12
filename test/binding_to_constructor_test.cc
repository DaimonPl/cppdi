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

TEST(binding_to_constructor, no_argument) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder) {
    binder->BindConstructor<NoDependency>();
  });
  DisposeGuard guard(injector);

  EXPECT_FALSE(!injector->GetInstance<shared_ptr<NoDependency>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<NoDependency>>(), injector->GetInstance<shared_ptr<NoDependency>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<NoDependency>>(), injector->GetInstance<shared_ptr<Provider<shared_ptr<NoDependency>>>>()->Get());
}

TEST(binding_to_constructor, with_primitive_argument) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder){
    binder->BindConstructor<IntDependent, int>();
    binder->BindInstance<int>(15);
  });
  DisposeGuard guard(injector);

  EXPECT_FALSE(!injector->GetInstance<shared_ptr<IntDependent>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<IntDependent>>(), injector->GetInstance<shared_ptr<IntDependent>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<IntDependent>>(), injector->GetInstance<shared_ptr<Provider<shared_ptr<IntDependent>>>>()->Get());
  EXPECT_EQ(15, injector->GetInstance<shared_ptr<IntDependent>>()->x_);
}

TEST(binding_to_constructor, with_type_argument) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder){
    binder->BindConstructor<NoDependency>();
    binder->BindConstructor<TypeDependency, shared_ptr<NoDependency>>();
  });
  DisposeGuard guard(injector);

  EXPECT_FALSE(!injector->GetInstance<shared_ptr<TypeDependency>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<TypeDependency>>(), injector->GetInstance<shared_ptr<TypeDependency>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<TypeDependency>>(), injector->GetInstance<shared_ptr<Provider<shared_ptr<TypeDependency>>>>()->Get());
  EXPECT_EQ(injector->GetInstance<shared_ptr<NoDependency>>(), injector->GetInstance<shared_ptr<TypeDependency>>()->x_);
}
