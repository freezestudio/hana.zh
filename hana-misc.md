起步
======

hana的核心部分有两个概念：tag和concept

tag
------

tag是为了统一异构类型，比如一系列的integral_constant<int,...>{}，虽然它们是不同的类型，但是在概念上它们是一致的，这时候我们用integral_constant_tag来表示它们。

concept
--------

可以理解为在模板中对模板参数加以约束。让我想想...有什么例子呢。

比如有一个操作，我们想要它只能应用于整型,换句话说，有一个类型T，我们要判断`std::is_integral<T>::value==true`：

```C++
template<typename Integral,where<Integral is Integer>>
void print_integral(Integral v)
{
    //print integral...
}
```

该怎么做呢？为了实现这个约束，我们先打一些基础。

```C++

//表示是否为默认实现的语义
//我们的设想是：如果某个约束是默认实现的，那么它不符合此约束的要求。
struct _default{};

//先声明一个类型，表达想要约束什么的语义
template<bool condition>
struct where;

//表示约束语义
template<typename T,typename = void>
struct concept;

//先给出基本的实现
//基本实现继承了它自身的一个特化模板，作为默认实现
template<typename T,typename>
struct concept : concept<T,where<true>>
{

};

//表示此约束是默认实现的，显然，它不符合约束的要求。
template<typename T,bool condition>
struct concept<T,where<condition>> : _default
{

};

//表示具体的约束，
//如果约束成立，则此偏特化会优先于默认实现，从而被编译器选中
// e.g. xxx=std::is_integral<T>::value 表示整型约束
template<typename T>
struct concept<T,where<hana::integral_constant<bool,xxx>::value>>
{

};
```


有了以上基础，我们试着实现这个例子：
