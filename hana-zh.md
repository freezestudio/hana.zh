Boost.Hana 用户手册
------------------

# 起步

Hana 是一个头文件库,是用于计算类型和值的C++元编程套件.它提供的功能是[Boost.MPL](http://www.boost.org/doc/libs/release/libs/mpl/doc/index.html)和[Boost.Fusion](http://www.boost.org/doc/libs/release/libs/fusion/doc/html/index.html)库的超集.利用C++11/14技术和惯用法,与之前的库相比,Hana拥有更快的编译速度和运行时性能,同时也显著提高了编程体验.Hana很容易以ad-hoc方式扩展，它提供了与`Boost.Fusion`，`Boost.MPL`和`标准库`的开箱即用的交互操作.

# 先决条件和安装

Hana是一个没有外部依赖的头文件库(甚至不依赖Boost).因此,在您自己的项目中使用Hana非常容易.基本上，只需添加`include/`目录到你的编译器的头文件搜索路径即可.以下方式可以干净地安装Hana.首先，您可以安装Boost 1.61.0或更高版本，因为从该版本开始，Hana包含在Boost中.如果你不想安装所有的Boost，也可以只安装Hana.为此，您可以从官方[GitHub存储库](https://github.com/boostorg/hana)下载代码，并通过从项目根目录执行以下命令来手动安装库(需要[CMake](http://www.cmake.org/)).

``` bat
> mkdir build && cd build
> cmake ..
> cmake --build . --target install
```

这样,将在您的平台上安装Hana到缺省的安装目录(Unix为`/usr/local`,Windows为`C:\\Program Files`).如果想要安装到指定位置,可以这样做:

``` bat
> cmake .. -DCMAKE_INSTALL_PREFIX=/custom/install/prefix 
```

**注意**
* 手动安装还将安装一个与[pkg-config](http://www.freedesktop.org/wiki/Software/pkg-config/)一起使用的`hana.pc`文件.
* 如果您已经安装了Boost，请不要再安装Hana，因为新安装将覆盖Boost附带的安装.

如果在项目中使用CMake，可以使用Hana提供的[FindHana.cmake](https://github.com/boostorg/hana/blob/master/cmake/FindHana.cmake)模块将Hana设置为外部CMake项目.该模块还允许将Hana本地安装到该项目，而无需按照上述说明在系统上安装Hana.最后，如果你想对Hana做出贡献，[README](https://github.com/boostorg/hana/blob/master/README.md#hacking-on-hana)文档中有相关设置你的开发环境的描述.

## 编译器要求

Hana仅依赖于C++14编译器和标准库,除此之外再无其它要求了.以下为当前支持Hana的C++14编译器/工具链列表:

| 编译器/工具链   |               状态            |
-----------------|-------------------------------
| Clang >= 3.5.0 | 完整支持;每次向GitHub推送前测试 |
| Xcode >= 6.3   | 完整支持;每次向GitHub推送前测试 |
| GCC   >= 6.0.0 | 完整支持;每次向GitHub推送前测试 |

更具体地说，Hana需要一个支持以下C++14特性的编译器/标准库（非穷举）:

* 泛型 `lambda`
* 通用 `constexpr`
* 自动推导的返回类型
* 所有支持C++14的`<type_traits>`头文件中的类型特性

# 技术支持

如果您有什么问题,请查看[FAQ](#Faq)及[wiki](https://github.com/boostorg/hana/wiki).或者查找[问题列表](https://github.com/boostorg/hana/issues)也是个不错的主意.如果这些没有解决您的问题,您可以随时与我们在[Gitter](https://gitter.im/boostorg/hana)聊天,或者提交一个新的issue.[StackOverflow](http://stackoverflow.com/)的
[boost-hana](http://stackoverflow.com/questions/tagged/boost-hana)标签处是提出问题的首选.如果您发现了bug,请提交issue.

# 简介

自Boost.MPL首发以来,它通过提供大量的模板接口为C++程序员进行元编程提供了便利.这个突破极大地促进了C++模板元编程的主流化,如今模板元编程已经深植于各种项目中了.近期以来,C++11和C++14对语言带来了许多重大变化,其中一些使元编程更加容易,其它一些也大大拓宽了库的设计空间.因此,一些问题自然而来:是否仍然希望有元编程的抽象?如果是,有哪些?在考察了不同选择,如MPL11之后,最终答案是Hana库.对Hana的关键洞察是,类型和值的计算是一体两面的.通过统一这两个概念,元编程变得更为容易,新的令人兴奋的可能出现在我们面前了.

## C++计算分类(四个象限)

但是要真正了解Hana是什么，有必要理解C++中的不同类型的计算. 先不管可能的更细粒度的区分,我们将把注意力集中在四种不同类型的计算上. 首先，是运行时计算，这是我们通常在C++中使用的计算方式.在运行时世界中，我们有运行时容器，运行时函数和运行时算法：

```C++
auto f=[](int i)->std::string {
    return std::to_string(i*i);
};

std::vector<int> ints{1,2,3,4};
std::vector<std::string> strings;
std::transform(ints.begin(),ints.end(),std::back_inserter(strings),f);

assert((string==std::vector<std::string>{"1","4","9","16"}));
```
这个象限中的计算,通常以C++标准库为工具,C++标准库提供运行时可重用的算法和容器.自C++11以来,第二种计算成为可能:`constexpr`计算.这种计算中,我们用`constexpr`容器,`constexpr`函数及`constexpr`算法:

```C++
constexpr int factorial(int n){
    return n==0?1:n*factorial(n-1);
}

template<typename T,std::size_t N,typename F>
constexpr std::array<std::result_of_t<F(T),N> transform(std::array<T,N> arr,F f){
    // ...
}

constexpr std::array<int,4> ints{{1,2,3,4}};
constexpr std::array<int,4> facts=transform(ints,factorial);

static_assert(facts==std::array<int,4>{{1,2,6,24}},"");
```

**注意**

* 若使以上代码可执行,需要确保`std::array`的`operator==`操作符标记为`constexpr`,在C++14下,这不是问题.

基本上,`constexpr`计算与运行时计算的不同之处在于它足够简单,可以被编译器解析执行.一般来说,任何不对编译器的求值程序过于不友好的函数(像抛出异常或者
分配内存等),都可以标记为`constexpr`,而无需作出修改.`constexpr`计算与运行时计算类似,除了`constexpr`计算更受限制,并需要获得编译时执行的能力.不幸的是,没有常用于`constexpr`计算的工具集,即没有广泛采用的用于`constexpr`编程的标准库.也许,对`constexpr`编程感兴趣的人可以去了解一下[Sprout](https://github.com/bolero-MURAKAMI/Sprout)库.

第三种计算是异构计算. 异构计算不同于普通的计算方式，因为异构计算不使用存储同类对象（所有对象具有相同类型）的容器,而是使用异构容器.异构容器可以保存具有不同类型的对象. 此外，在这个计算象限中的函数是异构函数，这是一种讨论模板函数的复杂方式.类似地，我们由异构算法操作异构容器和函数：

```C++
auto to_string=[](auto t){
    std::stringstream ss;
    ss<<t;
    return ss.str();
};

fusion::vector<int,std::string,float> seq{1,"abc",3.4f};
fusion::vector<std::string,std::string,std::string> strings=fusion::transform(seq,to_string);

assert(strings==funsion::make_vector("1"s,"abc"s,"3.4"s));
```

如果你觉得操作异构容器很奇怪的话,不妨把它相像成操作`std::tuple`.在C++03的世界中,用于进行此计算的库是[Boost.Fusion](http://www.boost.org/doc/libs/release/libs/fusion/doc/html/index.html),它提供了几个操作异构数据的结构和算法的集合.我们将考察的第四个计算象限的计算是类型计算.在这个象限中,我们有类型容器,类型函数(通常称为元函数)和类型算法.在这里,任何类型操作:容器存储类型和元函数接受类型作为参数并返回类型结果.

```C++
template<typename T>
struct add_const_pointer{
    using type=T const*;
};

using types=mpl::vector<int,char,float,void>;
using pointers=mpl::transform<types,add_const_pointer<mpl::_1>>::type;

static_assert(mpl::equal<pointers,
    mpl::vecotr<int const*,char const*,float const*,void const*>::value,"");
```

类型计算的领域已经被相当广泛地探索了，并且C++03中类型计算的事实解决方案是一个名为[Boost.MPL](http://www.boost.org/doc/libs/release/libs/mpl/doc/index.html)的库，它提供类型容器和算法.对于低级类型转换，C++11中也可以使用<type_traits>标准头提供的元函数。

## Hana库是干什么的?

以上所有计算都作地很好了,那么,Hana库又是干什么的?现在我们已经知道了C++的各种计算类型,回答这个问题可以会很简单了.**Hana的目的是合并第三和第四象限的计算**,具体来说,Hana经过长期构建证明,异构计算比类型计算更强大.我们可以通过等效的异构计算来表达任何类型计算.这种构造在两个步骤中完成.首先,Hana是一个功能齐全的异构算法和容器库,有点像现代化的Boost.Fusion.其次,Hana提供了一种将任何类型计算转换为其等效的异构计算的方法.这允许异构计算的全部机制被重用于类型计算,而没有任何代码重复.当然,这种统一的最大优点是用户能看到的.

# 快速起步

本节的目的是从非常高的层次快速介绍Hana库的主要概念; 不用担心看不明白一股脑仍给你的东西. 但是，本教程要求读者已经至少熟悉基本元编程和C++14标准.首先,需要包含以下库:

```C++
#include <boost/hana.hpp>
namespace hana=boost::hana;
```

除非另行说明,本文档假定示例和代码片断都在之前添加了以上代码.还要注意更详细的头文件包含将在[头文件的组织](http://boostorg.github.io/hana/index.html#tutorial-header_organization)节详述.为了快速起步,现在我们再包含一些头文件,并定义一些动物类型:

```C++
#include <cassert>
#include <iostream>
#include <string>

struct Fish{std::string name;};
struct Cat {std::string name;};
struct Dog {std::string name;};
```

如果你正在阅读这个文档,你可能已经知道`std::tuple`和`std::make_tuple`.Hana也提供了自己的`tuple`和`make_tuple`:

```C++
auto animals=hana::make_tuple(Fish{"Nemo"},Cat{"Garfield"},Dog{"Snoopy"});
```

创建一个元组,除了可以存储不同类型的区别外,它就像是一个数组.像这样能够存储不同类型元素的容器称为异构容器.C++标准库只提供了操作`std::tuple`的少数支持.而Hana对自己的`tuple`的操作支持要多一些:

```C++
using namespace hana::literals;

//Access tuple elements with operator[] instead of std::get.
Cat grafield=animals[1_c];

//Perform high level algorithms on tuples (this is like std::transform)
auto names=hana::transform(animals,[](auto a){
    return a.name;
});

assert(hana::reverse(names)==hana::make_tuple("Snoopy","Garfield","Nemo"));
```

**注意**

* `1_c`是一个用[C++14用户自定义字面量](http://en.wikipedia.org/wiki/C%2B%2B11#User-defined_literals)创建的[编译时数值]().此自定义字面量位于`boost::hana::literals`名字空间,故此using了该名字空间.

注意我们是如何将[C++14泛型lambda]传递到`transform`的;必须要这样做是因为lambda首先用`Fish`来调用的,接着用`Cat`,最后用`Dog`来调用,它们都是类型不同的.Hana提供了C++标准提供的大多数算法,除了它们工作在元组和异构容器上而不是在`std::tuple`等之上的之外.除了使用异构值之外,Hana还使用自然语法执行类型计算,所有这些都在编译期完成,没有任何开销:

```C++
auto animal_types=hana::make_tuple(hana::type_c<Fish*>,hana::type_c<Cat&>,hana::type_c<Dog>);

auto no_pointers=hana::remove_if(animal_types,[](auto a){
    return hana::traits::is_pointer(a);
});

static_assert(no_pointers==hana::make_tuple(hana::type_c<Cat&>,hana::type_c<Dog>),"");
```

**注意**

* `type_c<...>`不是一个类型!它是一个[C++14变量模板](http://en.wikipedia.org/wiki/C%2B%2B14#Variable_templates)生成的Hana类型对象.更多详情参见[类型计算]().

除了用于异构和编译时序列外,Hana还提供一些特性使您的元编程恶梦成为过去.举例来说,你可以简单使用一行代码来检查结构的成员是否存在,而不再依赖于笨重的SFINAE:

```C++
auto has_name=hana::is_vaild([](auto&& x)->decltype((void)x.name){});

static_assert(has_name(garfield),"");
static_assert(!has_name(1),"");
```

想编写一个序列化库?不要着急,我们给你准备.反射机制可以很容易地添加到用户定义的类型中.这允许遍历用户定义类型的成员,使用编程接口查询成员等等,而且没有运行时开销:

```C++
// 1. Give introspection capabilities to 'Person'
struct Person{
    BOOST_HANA_DEFINE_STRUCT(Person,
        (std::string,name),
        (int,age)
    );
};

// 2. Write a generic serializer (bear with std::ostream for the example)
auto serialize=[](std::ostream& os,auto const& object){
    hana::for_each(hana::members(objec),[&](auto member){
        os<<member<<std::endl;
    });
};

// 3. Use it
Person john{"John",30};
serialize(std::cout,john);

// output:
// John
// 30
```

酷,但是我已经听到你的抱怨了,编译器给出不可理解的错误消息.我们是故意搞砸的,这表明构建Hana的家伙是一般人而不是专业的元编程程序员.让我们先看看错误情况:

``` C++
auto serialize = [](std::ostream& os, auto const& object) {
  hana::for_each(os, [&](auto member) {
    //           ^^ oopsie daisy!
    os << member << std::endl;
  });
};
```

详情:

``` C++
error: static_assert failed "hana::for_each(xs, f) requires 'xs' to be Foldable"
        static_assert(Foldable<S>::value,
        ^             ~~~~~~~~~~~~~~~~~~
note: in instantiation of function template specialization
      'boost::hana::for_each_t::operator()<
        std::__1::basic_ostream<char> &, (lambda at [snip])>' requested here
  hana::for_each(os, [&](auto member) {
  ^
note: in instantiation of function template specialization
    'main()::(anonymous class)::operator()<Person>' requested here
serialize(std::cout, john);
```

不是那么坏,对吧? 小例子非常容易展示但没有什么实际意义,让我们来一个真实世界的例子.

## 一个真实世界的例子

本节,我们的目标是实现一种能够处理`boost::any'的`switch`语句.给定一个`boost::any`,目标是分发any的动态类型到关联的函数:

``` C++
boost::any a='x';
std::string r=switch_(a)(
    case_<int>([](auto i){return "int: "s+std::to_string(i);}),
    case_<char>([](auto c){return "char: "s+std::string{c};}),
    default_([]{return "unknown"s;})
);

assert(r=="char: x"s);
```

**注意**

* 此文档中,我们将经常在字符串字面量上使用`s`后缀来创建`std::string`(而没有语法上的开销),这是个[C++14用户自定义字面量](http://en.wikipedia.org/wiki/C%2B%2B11#User-defined_literals)的标准定义.


因为any中保存有一个`char`,因此第二个函数被调用.如果any保存的是`int`,第一个函数将被调用.当any保存的动态类型不匹配任何一个case时,`default_`函数会被调用.最后,`switch`返回值为与any动态类型关联的函数的返回值.返回值的类型被推导为所有关联函数的返回类型的公共类型:

``` C++
boost::any a='x';
auto r=switch_(a)(
    case_<int>([](auto)->int{return 1;}),
    case_<char>([](auto)->long{return 2l;}),
    default_([]()->long long{return 3ll;})
);

//r is inferred to be a long long
static_assert(std::is_same<decltype(r),long long>{},"");
assert(r==2ll);
```

现在,我们看看如何用Hana来实现这个实用程序.第一步是将每个类型关联到一个函数.为此,我们将每个`case_`表示为`hana::pair`,`hana::pair`的第一个元素是类型,第二个元素是函数.另外,我们(arbitrarily)决定将`default_`表示为一个映射一个虚拟的类型到一个函数的`hana::pair`.

``` C++
template<typename T>
auto case_=[](auto f){
    return hana::make_pair(hana::type_c<T>,f);
}

struct default_t;
auto default_=case_<default_t>;
```




