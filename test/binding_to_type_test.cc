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

class IA {

};

class A : public IA {

};

class IB {

};

class B : public IB {
 public:
  B(shared_ptr<IA> a) {a_=a;};

  shared_ptr<IA> a_;
};

TEST(binding_to_type, simple) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder){
    binder->BindConstructor<A>();
    binder->BindTypes<IA, A>();
  });

  EXPECT_FALSE(!injector->GetInstance<shared_ptr<A>>());
  EXPECT_FALSE(!injector->GetInstance<shared_ptr<IA>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<A>>(), injector->GetInstance<shared_ptr<IA>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<Provider<shared_ptr<A>>>>()->Get(), injector->GetInstance<shared_ptr<Provider<shared_ptr<IA>>>>()->Get());
}

TEST(binding_to_type, complex) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder){
    binder->BindConstructor<A>();
    binder->BindTypes<IA, A>();
    binder->BindConstructor<B, shared_ptr<IA>>();
    binder->BindTypes<IB, B>();
  });

  EXPECT_FALSE(!injector->GetInstance<shared_ptr<B>>());
  EXPECT_FALSE(!injector->GetInstance<shared_ptr<IB>>());
  EXPECT_FALSE(!injector->GetInstance<shared_ptr<A>>());
  EXPECT_FALSE(!injector->GetInstance<shared_ptr<IA>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<A>>(), injector->GetInstance<shared_ptr<IA>>());
  EXPECT_EQ(injector->GetInstance<shared_ptr<A>>(), injector->GetInstance<shared_ptr<B>>()->a_);
  EXPECT_EQ(injector->GetInstance<shared_ptr<Provider<shared_ptr<A>>>>()->Get(), injector->GetInstance<shared_ptr<Provider<shared_ptr<IA>>>>()->Get());
  EXPECT_EQ(injector->GetInstance<shared_ptr<Provider<shared_ptr<B>>>>()->Get(), injector->GetInstance<shared_ptr<Provider<shared_ptr<IB>>>>()->Get());
}
