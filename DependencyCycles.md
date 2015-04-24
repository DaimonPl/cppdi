## What is dependency cycle? ##

Dependency cycle happens if object graph with type `A` contains `A` as it's dependency (**including non-direct dependencies!**).

Trivial example (direct dependency):

```
class A {
 public:
  A(shared_ptr<A> a_) {
  } 
}
```

Non-trivial example (non-direct dependency):

```
struct IA {};
struct IB {};
struct IC {};

struct A : public IA {
  A(shared_ptr<IB> ib) {
  } 
};

struct B : public IB {
  B(shared_ptr<IC> ic) {
  } 
};

struct C : public IC {
  // dependency cycle! injection of A, requires A as non-direct dependency!
  C(shared_ptr<IA> ia) {
  } 
};
```

Cyclic dependencies are bad from design perspective and should be always avoided. Unfortunately in big projects it's easy to create non-direct cyclic dependency by accident.

## cppdi status ##

cppdi supports automatic cycle detection in "debug" mode. Debug mode is enabled during compile time by setting `_CPPDI_DEBUG_MODE_` flag. This way it's very easy to enable cycle debugging in debug builds and at the same time maintain better performance in release builds.

`InjectionCycleError` is thrown if cycle is detected during debug-mode injection.

If cycle happens in code compiled without `_CPPDI_DEBUG_MODE_`, injection will lead to stack overflow.

## Workaround ##

As workaround, cyclic dependency can be "cut" by changing one class to use `Provider<shared_ptr<IA>>` instead of `shared_ptr<IA>` directly:

```
struct C : public IC {
  // OK, provider can be injected even though A instance does not exist yet
  C(shared_ptr<Provider<shared_ptr<IA>>> ia_provider) {
    // ia_provider->Get() MUST NOT be called until C's constructor exits
    // calling ia_provider->Get() here would create exactly same cyclic dependency as before
    // ia_provider should be stored as member and invoked when required later
  } 
}
```