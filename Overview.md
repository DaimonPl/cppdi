## Code organization ##

cppdi is header-only library organized into 2 namespaces:

  * cppdi - public API
  * cppdi::internal - internal namesace for cppdi implementation - this namespace SHOULD NOT be used as API compatibility is not guaranteed even between minor versions

## Dependency injection flow ##

**1. Binding Definition**
> Initial step in which dependencies are defined in declarative way. No instances are created in this part.

> Check [Binding](Binding.md) for information about supported bindings.

**2. Injection**
> Instantiation of object graph. Instances are created per-request - i.e. until user explicitely requests instance of particular type, no memory allocation / constructor calls take place. Requesting instance of concrete type will also automatically create instances of its dependencies (if they were not existing yet).

> Instance requests are called directly on Injector using `Injector::GetInstance<T>()` method. Injector can be obtained from `InjectorFactory`

> Check [BasicUsage](BasicUsage.md) page for example `GetInstance` calls

**3. Disposal**

> cppdi internals create circular dependencies between `shared_ptr<Injector>` and internal collection of `shared_ptr<Provider<T>>`. To ensure memory is released, each injector MUST be disposed either by direct `Injector::Dispose()` call or usage of `DisposeGuard`.

> Disposal should be performed when it's sure that injection won't be required any more. If `Provider` class is being used directly, disposal should be performed at program exit.

> Check [BasicUsage](BasicUsage.md) page for example `DisposeGuard` usage