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

class IaP : public Provider<shared_ptr<IA>> {
 public:
  shared_ptr<IA> Get() override {
    return shared_ptr<IA>(new IA());
  }
};

class IntP : public Provider<int> {
 public:
  int Get() override {
    return 7;
  }
};


TEST(binding_to_provider, primitive) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder){
    binder->BindProvider<int, IntP>();
  });

  EXPECT_EQ(7, injector->GetInstance<int>());
  EXPECT_EQ(7, injector->GetInstance<shared_ptr<Provider<int>>>()->Get());
}

TEST(binding_to_provider, type) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder){
    binder->BindProvider<shared_ptr<IA>, IaP>();
  });

  EXPECT_FALSE(!injector->GetInstance<shared_ptr<IA>>());
  EXPECT_FALSE(!injector->GetInstance<shared_ptr<Provider<shared_ptr<IA>>>>()->Get());
}
