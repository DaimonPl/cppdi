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

TEST(binding_to_type, simple) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder) {
    binder->BindConstructor<Implementation>();
    binder->BindTypes<Interface, Implementation>();
  });
  DisposeGuard guard(injector);

  EXPECT_FALSE(!injector->GetInstance<shared_ptr<Implementation>>());
  EXPECT_FALSE(!injector->GetInstance<shared_ptr<Interface>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<Implementation>>(), injector->GetInstance<shared_ptr<Interface>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<Provider<shared_ptr<Implementation>>>>()->Get(), injector->GetInstance<shared_ptr<Provider<shared_ptr<Interface>>>>()->Get());
}

TEST(binding_to_type, multi_inheritance) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder) {
    binder->BindConstructor<ImplementationOfTwoInterfaces>();
    binder->BindTypes<Interface, ImplementationOfTwoInterfaces>();
    binder->BindTypes<Interface2, ImplementationOfTwoInterfaces>();
  });
  DisposeGuard guard(injector);

  EXPECT_FALSE(!injector->GetInstance<shared_ptr<ImplementationOfTwoInterfaces>>());
  EXPECT_FALSE(!injector->GetInstance<shared_ptr<Interface>>());
  EXPECT_FALSE(!injector->GetInstance<shared_ptr<Interface2>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<Interface>>(), injector->GetInstance<shared_ptr<ImplementationOfTwoInterfaces>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<Interface2>>(), injector->GetInstance<shared_ptr<ImplementationOfTwoInterfaces>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<Provider<shared_ptr<Interface>>>>()->Get(), injector->GetInstance<shared_ptr<Provider<shared_ptr<ImplementationOfTwoInterfaces>>>>()->Get());
  EXPECT_EQ(injector->GetInstance<shared_ptr<Provider<shared_ptr<Interface2>>>>()->Get(), injector->GetInstance<shared_ptr<Provider<shared_ptr<ImplementationOfTwoInterfaces>>>>()->Get());
}

TEST(binding_to_type, complex) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder) {
    binder->BindConstructor<Implementation>();
    binder->BindTypes<Interface, Implementation>();
    binder->BindConstructor<ImplementationWithDependency, shared_ptr<Interface>>();
    binder->BindTypes<Interface2, ImplementationWithDependency>();
  });
  DisposeGuard guard(injector);

  EXPECT_FALSE(!injector->GetInstance<shared_ptr<ImplementationWithDependency>>());
  EXPECT_FALSE(!injector->GetInstance<shared_ptr<Interface2>>());
  EXPECT_FALSE(!injector->GetInstance<shared_ptr<Implementation>>());
  EXPECT_FALSE(!injector->GetInstance<shared_ptr<Interface>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<Implementation>>(), injector->GetInstance<shared_ptr<Interface>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<Implementation>>(), injector->GetInstance<shared_ptr<ImplementationWithDependency>>()->intf_);
  EXPECT_EQ(injector->GetInstance<shared_ptr<Provider<shared_ptr<Implementation>>>>()->Get(), injector->GetInstance<shared_ptr<Provider<shared_ptr<Interface>>>>()->Get());
  EXPECT_EQ(injector->GetInstance<shared_ptr<Provider<shared_ptr<ImplementationWithDependency>>>>()->Get(), injector->GetInstance<shared_ptr<Provider<shared_ptr<Interface2>>>>()->Get());
}
