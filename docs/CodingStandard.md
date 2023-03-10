# Coding Standard

This is a live document, and is subject to change/additions as necessary.

## Style Standards

Brackets go on a new line. 

if/while/for must have brackets, even for one liners.

Includes are sorted in three groups: glibby headers, thirdparty headers, and STL headers. Includes are sorted alphabetically within their group.

### Naming Conventions

```
namespace NamespaceName
{
  int variableName;
  bool bIsBoolean;
  void FunctionName();
  class ClassName
  {};
  struct StructName
  {};
  enum class EnumName
  {
    ONE,
    TWO,
    THREE,
    ENUM_NAME_MAX
  }
}
```

Defer to the [UE5 coding standard](https://docs.unrealengine.com/4.26/en-US/ProductionPipelines/DevelopmentSetup/CodingStandard/) or project lead for questions/special cases

## Best-Practices

Prefer stack allocation to heap allocation.

Heap allocation should use smart pointers.

Prefer C++ style parameter passing (by const reference instead of pointer).

Avoid including headers inside of headers, prefer forward declarations.

## Documentation

Coming soon.
