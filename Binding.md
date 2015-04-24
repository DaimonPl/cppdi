# Binding #

## Singletons ##

cppdi by default treats all bindings as singletons within single `Injector`. If non-singleton behaviour is required, user can define custom `Provider<t>` or `std::function<T(Args)>`to create new instance each time it's requested.

If several instances of same class graph are required, separate `Injector` should be used for each of them.

## Binding definition ##

Bindings can be defined either in `std::function<void(cppdi::Binder *binder)` (including lambdas):

```
[](cppdi::Binder *binder) {
  binder->BindConstructor<A>();
}
```

or `cppdi::Module` implementation:

```
class MyModule : public cppdi::Module {
  void Configure(cppdi::Binder *binder) const override {
    binder->BindConstructor<A>();
  }
}
```


`InjectorFactory` is used to create `Injector` from bindings (see [BasicUsage](BasicUsage.md) for example);

## Constructor binding ##

Constructor binding of type `T` defines that each request for `shared_ptr<T>` or `shared_ptr<Provider<shared_ptr<T>>>` should use bound constructor to create instance.

Instances created by this binding are singletons.

```
[](cppdi::Binder *binder) {
  // bind non-argument constructor of A
  binder->BindConstructor<A>();

  // bind single argument constructor of B (shared_ptr<A> is argument)
  binder->BindConstructor<B, shared_ptr<A>>();
}
```

## Type binding ##

Type binding is mostly used for interface -> implementation binding but in general can be applied to any base\_class -> sub\_class hierarchy.

```
[](cppdi::Binder *binder) {
  // bind non-argument constructor of Impl
  binder->BindConstructor<Impl>();

  // Interface -> Impl binding - Interface must derive from Impl
  binder->BindTypes<Interface, Impl>();
}
```

## Instance binding ##

Simple type -> instance binding. It can be applied to both pointers and values.

**WARNING!** if type is bound to value, its copy constructor will be executed during injection process. To avoid performance problems, value binding should only used for primitives or other fast-copy types.

```
[](cppdi::Binder *binder) {
  binder->BindInstance<int>(6);
  binder->BindInstance<string>(string("test"));
}
```

Instance binding can be also used to bind pointers to external components:

```
[](cppdi::Binder *binder) {
  binder->BindInstance<Other *>(/* address of other here */);
}
```

## Provider binding ##

Provider binding can be used if custom object creation or non-singleton behaviour is required. Provider will be called each time bound type is requested.

```
struct X {}

class ProviderOfXPtr : public cppdi::Provider<X*> {
 public:
  X *Get() override {
    return new X();
  }

 private:
  void Initialize(const shared_ptr<cppdi::Injector> &injector) override {
    //optional initialization
  }
}

[](cppdi::Binder *binder) {
  // ProviderOfXPtr must have non-argument constructor
  binder->BindProvider<X*, ProviderOfXPtr>();
}
```

## Function binding ##

Function binding can be used to bind existing factory function to desired type. Function will be called each time bound type is requested.

Function MAY have arguments. Instances of arguments will be automatically injected during function call.

```
int producing_fun() {
  return 18;
}

long producing_fun2(int a) {
  return a + 22;
}

[](cppdi::Binder *binder) {
  binder->BindFunction<int>(std::function<int()>(producing_fun));
  binder->BindFunction<long>(std::function<long(int)>(producing_fun2));
}
```

## Mixing bindings ##

Different binding ways may be used together:

```
[](cppdi::Binder *binder) {
  binder->BindConstructor<Impl, int, X*>();
  binder->BindTypes<Interface, Impl>();
  binder->BindProvider<X*, ProviderOfXPtr>();
  binder->BindFunction<int>(std::function<int()>(producing_fun));
}
```

## Named bindings ##

Named bindings can be used with any type of previously described bindings (constructor, type, instance, provider, function).

Named bindings are used to create different bindings of same type:
```
[](cppdi::Binder *binder) {
  // non-named instance
  binder->BindInstance<int>(6);

  // named instance
  binder->BindInstance<int>(20, "foo");
}
```