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

TEST(complex_binding, cycle) {
  cppdi::InjectorFactory factory;

  shared_ptr<Injector> injector = factory.Create([](Binder *binder) {
    binder->BindConstructor<CycleImpl1, shared_ptr<CycleIntf2>>();
    binder->BindConstructor<CycleImpl2, shared_ptr<CycleIntf3>>();
    binder->BindConstructor<CycleImpl3, shared_ptr<Provider<shared_ptr<CycleIntf1>>>>();
    binder->BindTypes<CycleIntf1, CycleImpl1>();
    binder->BindTypes<CycleIntf2, CycleImpl2>();
    binder->BindTypes<CycleIntf3, CycleImpl3>();
  });
  DisposeGuard guard(injector);

  shared_ptr<CycleIntf1> if1 = injector->GetInstance<shared_ptr<CycleIntf1>>();
  shared_ptr<CycleIntf3> if3 = injector->GetInstance<shared_ptr<CycleIntf3>>();
  shared_ptr<CycleImpl3> impl3 = injector->GetInstance<shared_ptr<CycleImpl3>>();

  EXPECT_EQ(if3, impl3);
  EXPECT_EQ(if1, impl3->intf1p_->Get());
}
