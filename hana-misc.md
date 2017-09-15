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

tag分派其实就是为了获得在编译进行分支的能力，可类比于运行时的`if`。但是不能直接在编译时运用`if`来使代码分支，原因是编译器会检查每一个分支，编译时的分支是互斥的，也就是一个分支通过了，其它的分支肯定通不过（编译）。但编译器不管哪个分支都要检查，这就导致编译失败。

解决问题的最好办法是使用常量`if`,即`if constexpr(...) ... else ...`，但这需要`c++17`标准的支持。退一步的办法是使用`sfinae`，标准库提供了`enable_if`和`void_t`(`c++17`)用来简化`SFINAE`的使用。还有一个办法就是定义具有分支能力的元函数，再有一个办法就是`tag dispatching`了。它最旱可能使用于标准库的迭代器。

基本用法是,把不同的类型用于函数重载，比如：

```c++

struct tag_a;
struct tag_b;

void foo(int a,tag_a)
{
    //...
}

void foo(int b,tag_b)
{
    //...
}

void foo(int a)
{
    //tag is typeof tag_a or tag b;
    foo(a,tag);
}
```

concept
-------


