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

#ifndef CPPDI_TESTS_TESTED_TYPES_H_
#define CPPDI_TESTS_TESTED_TYPES_H_

#include <memory>

#include "cppdi/cppdi.h"

struct NoDependency {

};

struct IntDependent {
  IntDependent(int x) {
    x_ = x;
  }

  int x_;
};

struct IntDependentWithNonArgCtor {
  IntDependentWithNonArgCtor() {
    x_ = 666;
  }

  IntDependentWithNonArgCtor(int x) {
    x_ = x;
  }

  int x_;
};

struct TypeDependency {
  TypeDependency(std::shared_ptr<NoDependency> x) {
    x_ = x;
  }

  std::shared_ptr<NoDependency> x_;
};

struct Interface {
  virtual ~Interface() {}
  virtual void interface_method() = 0;
};

struct Implementation : public Interface {
  void interface_method() override {}
};

struct Interface2 {
  virtual ~Interface2() {}
  virtual void interface2_method() = 0;
};

struct ImplementationWithDependency : public Interface2 {
  ImplementationWithDependency(std::shared_ptr<Interface> intf) {
    intf_ = intf;
  }

  void interface2_method() override {}
  std::shared_ptr<Interface> intf_;
};

struct ImplementationWithDependencyAndNonArgCtor : public Interface2 {
  ImplementationWithDependencyAndNonArgCtor() {}

  ImplementationWithDependencyAndNonArgCtor(std::shared_ptr<Interface> intf) {
    intf_ = intf;
  }

  void interface2_method() override {}
  std::shared_ptr<Interface> intf_;
};

struct ImplementationOfTwoInterfaces : public Interface, public Interface2 {
  void interface_method() override {}
  void interface2_method() override {}
};

struct InterfaceProvider : public cppdi::Provider<std::shared_ptr<Interface>> {
  std::shared_ptr<Interface> Get() override {
    return std::shared_ptr<Interface>(new Implementation());
  }
};

struct IntProvider : public cppdi::Provider<int> {
  int Get() override {
    return 10;
  }
};

inline int int_function() {
  return 18;
}

inline long long_function_with_int_arg(int a) {
  return a + 22;
}

struct CycleIntf1 {
  virtual ~CycleIntf1() {}
};

struct CycleIntf2 {
  virtual ~CycleIntf2() {}
};

struct CycleIntf3 {
  virtual ~CycleIntf3() {}
};

struct CycleImpl1 : public CycleIntf1 {
  CycleImpl1(std::shared_ptr<CycleIntf2> intf2) {
    intf2_ = intf2;
  }

  std::shared_ptr<CycleIntf2> intf2_;
};

struct CycleImpl2 : public CycleIntf2 {
  CycleImpl2(std::shared_ptr<CycleIntf3> intf3) {
    intf3_ = intf3;
  }

  std::shared_ptr<CycleIntf3> intf3_;
};

struct CycleImpl3 : public CycleIntf3 {
  CycleImpl3(std::shared_ptr<cppdi::Provider<std::shared_ptr<CycleIntf1>>> intf1p) {
    intf1p_ = intf1p;
  }

  std::shared_ptr<cppdi::Provider<std::shared_ptr<CycleIntf1>>> intf1p_;
};

#endif
