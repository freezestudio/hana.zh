起步
======

hana的核心部分有两个概念：`tag`和`concept`

tag
---

`tag`是为了统一异构类型，比如一系列的`integral_constant<int,n>`，虽然它们是不同的类型，但是在概念上它们是一致的，这时候我们用`integral_constant_tag`来表示它们。

`tag`只是为了区别类型，事实上，我们通常只声明它，而不用定义它。就像这样：

```c++
struct xxx_tag;
```

`xxx`代表实际类型，添加了`_tag`后缀只是一种习惯作法。我们看到了`tag`只是作为不同的类型而存在，而且编译器通常会优化掉它。`tag`的主要作用是用来“标签分派”的。

Tag dispatching
---------------



concept
-------


