## Example class hierarchy ##

Following classes will be used in example:
```c++

class InterfaceA {
public:
virtual ~InterfaceA() {}
virtual void a() = 0;
};

class InterfaceB {
public:
virtual ~InterfaceB() {}
virtual void b() = 0;
};

class InterfaceC {
public:
virtual ~InterfaceC() {}
virtual void c() = 0;
};

class A : public InterfaceA {
public:
A(shared_ptr<InterfaceB> b, shared_ptr<InterfaceC> c)
: b_(b), c_(c) {}

virtual void a() {
cout << "a" << endl;

b_->b();
c_->c();
}

shared_ptr<InterfaceB> b_;
shared_ptr<InterfaceC> c_;
};

class B : public InterfaceB {
public:
virtual void b() {
cout << "b" << endl;
}
};

class C : public InterfaceC {
public:
virtual void c() {
cout << "c" << endl;
}
};
```

## cppdi usage ##

```c++

#include <iostream>
#include <memory>
#include <cassert>

#include "cppdi/cppdi.h" //no linking is required since cppdi is header-only library

using namespace std;

int main() {
cppdi::InjectorFactory factory;

// creating injector
shared_ptr<cppdi::Injector> injector = factory.Create([](cppdi::Binder *binder) {
binder->BindConstructor<A, shared_ptr<InterfaceB>, shared_ptr<InterfaceC>>();
binder->BindConstructor<B>();
binder->BindConstructor<C>();
binder->BindTypes<InterfaceA, A>();
binder->BindTypes<InterfaceB, B>();
binder->BindTypes<InterfaceC, C>();
});

// guard will ensure that injector releases memory when stack unwinds
cppdi::DisposeGuard guard(injector);

// obtaining first instance (none were created until this call)
shared_ptr<InterfaceA> ia1 = injector->GetInstance<shared_ptr<InterfaceA>>();
ia1->a();

// injector maintains scope - same singleton instance is returned within its scope
shared_ptr<InterfaceA> ia2 = injector->GetInstance<shared_ptr<InterfaceA>>();
assert(ia1 == ia2);

// because of binding, both A and InterfaceA are exactly same instance
shared_ptr<A> a = injector->GetInstance<shared_ptr<A>>();
assert(ia1 == a);

// ib is exactly same instance as in A
shared_ptr<InterfaceB> ib = injector->GetInstance<shared_ptr<InterfaceB>>();
assert(ib == a->b_);

return 0;
}
```