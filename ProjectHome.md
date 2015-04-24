# cppdi #


## Description ##

cppdi is simple C++11 dependency injection, header-only library licensed under Mozilla Public License 2.0 (MPL). RTTI, `"Any"` and static casts are used internally to provide type-safe, template-based API. Library is pure C++11 (does not require additional libraries) and lightweight (~950 code lines).

## Features ##
  * non-invasive - does not require modifications to plain C++ classes
  * typesafe interface
  * binding module support
  * named binding and injection
  * automatic cycle detection (in debug mode)
  * type-to-constructor binding
  * type-to-type binding (aka interface to implementation)
  * type-to-provider binding (including user-defined providers)
  * type-to-instance binding (including external pointers, primitives, values)
  * type-to-function binding (including lambdas)
  * covered with gtest integration tests

## Read more ##
  * [Overview](Overview.md)
  * [BasicUsage](BasicUsage.md)
  * [Binding](Binding.md)
  * [DependencyCycles](DependencyCycles.md)

## Limitations ##
  * instances created by cppdi (i.e. not created by user-defined provider, nor bound to instance) can be only injected as `shared_ptr<T>` or `shared_ptr<Provider<shared_ptr<T>>>`