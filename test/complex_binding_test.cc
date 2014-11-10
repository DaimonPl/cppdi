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
 public:
  virtual ~IA() {}
  virtual void a() = 0;
};

class IB {
 public:
  virtual ~IB() {}
  virtual void b() = 0;
};

class IC {
 public:
  virtual ~IC() {}
  virtual void c() = 0;
};

class A : public IA {
 public:
  A(shared_ptr<IB> ib) {
    ib_ = ib;
  }

  void a() override {}

  shared_ptr<IB> ib_;
};

class B : public IB {
 public:
  B(shared_ptr<IC> ic) {
    ic_ = ic;
  }

  void b() override {}

  shared_ptr<IC> ic_;
};

class C : public IC {
 public:
  C(shared_ptr<Provider<shared_ptr<IA>>> iap) {
    iap_ = iap;
  }

  void c() override {}

  shared_ptr<Provider<shared_ptr<IA>>> iap_;
};

TEST(complex_binding, cycle) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder) {
    binder->BindConstructor<A, shared_ptr<IB>>();
    binder->BindConstructor<B, shared_ptr<IC>>();
    binder->BindConstructor<C, shared_ptr<Provider<shared_ptr<IA>>>>();
    binder->BindTypes<IA, A>();
    binder->BindTypes<IB, B>();
    binder->BindTypes<IC, C>();
  });
  DisposeGuard guard(injector);

  shared_ptr<IA> ia = injector->GetInstance<shared_ptr<IA>>();
  shared_ptr<IC> ic = injector->GetInstance<shared_ptr<IC>>();
  shared_ptr<C> c = static_pointer_cast<C>(ic);

  EXPECT_EQ(ia, c->iap_->Get());
}
