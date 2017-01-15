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
* 变量模板
* 自动推导的返回类型
* 所有支持C++14的`<type_traits>`头文件中的类型特性

查看[wiki](https://github.com/boostorg/hana/wiki)以获取更多平台相关信息.

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
constexpr std::array<std::result_of_t<F(T)>,N> transform(std::array<T,N> arr,F f){
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

# 快速入门

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

注意我们是如何将[C++14泛型lambda](http://en.wikipedia.org/wiki/C%2B%2B14#Generic_lambdas)传递到`transform`的;必须要这样做是因为lambda首先用`Fish`来调用的,接着用`Cat`,最后用`Dog`来调用,它们都是类型不同的.Hana提供了C++标准提供的大多数算法,除了它们工作在元组和异构容器上而不是在`std::tuple`等之上的之外.除了使用异构值之外,Hana还使用自然语法执行类型计算,所有这些都在编译期完成,没有任何开销:

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

为支持上述接口，`switch_`必须返回一个case分支的函数,另外,`switch_(a)`还需要接受任意数量的的case(它们都是`haha::pair`),并能以正确的逻辑执行某个case的分派函数.可以通过返回C++14泛型lambda来实现:

``` C++
template<typename Any>
auto switch_(Any& a){
    return [&a](auto... case_){
        // ...
    };
}
```

参数包不是太灵活,我们把它转为`tuple`好利用操作:

``` C++
template<typename Any>
auto switch_(Any& a){
    return [&a](auto... cases_){
        auto cases=haha::make_tuple(cases_...);
        // ...
    };
}
```

注意在定义`cases`时是怎样使用`auto`关键字的;这通常更容易让编译器推断出tuple的类型，并使用make_tuple而不是手动处理类型.下一步要做的是区分出`default case`与其它case.为此,我们使用Hana的`find_if`算法,它在原理上类似于`std::find_if`:

``` C++
template<typename Any>
auto switch_(Any& a){
    return [&a](auto... cases_){
        auto cases=hana::make_tuple(cases_...);

        auto default_=hana::find_if(cases,[](auto const& c){
            return hana::first(c)==hana::type_c<default_t>;
        });
    };

    // ...
}
```

`find_if`接受一个元组和一个谓词,返回元组中满足谓词条件的第一个元素.返回结果是一个`hana::optional`,它类似于`std::optional`,除了可选值为empty或不是编译时已知的.如果元组的元素不满足谓词条件,`find_if`不返回任何值(空值).否则,返回`just(x)`(非空值),其中`x`是满足谓词的第一个元素.与STL算法中使用的谓词不同,此处使用的谓词必须是泛型的,因为元组中的元素是异构的.此外,该谓词必须返回Hana调用的IntegeralConstant,这意味着谓词的结果必须是编译时已知的.更多细节请参见[交叉相位算法]().在谓词内部,我们只需将cases的第一个元素的类型与`type_c<default_t>`比较.如果还记得我们使用`hana::pair`来对case进行编码的话,这里的意思即为我们在所有提供的case中找到default case.但是,如果没有提供default case时会怎样呢?当然是编译失败!

``` C++
template<typename Any>
auto switch_(Any& a){
    return [&a](auto... cases_){
        auto cases=hana::make_tuple(cases_...);

        auto default_=hana::find_if(cases,[](auto const& c){
            return haha::first(c)==hana::type_c<default_t>;
        });
        static_assert(default_!=hana::nothing,"switch is missing a default_ case");

        // ...
    };
}
```

注意我们是怎样用`static_assert`来处理`nothing`结果的.担心`default_`是非constexpr对象吗?不用.Hana能确保非编译期已知的信息传递到运行时.这显然能保证`default_`必须存在.下一步该处理非default的case了,我们这里用`filter`算法,它可以使序列仅保留满足谓词的元素:

``` C++
template<typename Any>
auto switch_(Any& a){
    return [&a](auto... cases_){
        auto cases=hana::make_tuple(cases_...);

        auto default_=hana::find_if(cases,[](auto const& c){
            return haha::first(c)==hana::type_c<default_t>;
        });
        static_assert(default_!=hana::nothing,"switch is missing a default_ case");

        auto rest=hana::filter(cases,[](auto const& c){
            return hana::first(c)!=hana::type_c<default_t>;
        });

        // ...
    };
```

接下来就该查找哪一个case匹配`any`的动态类型了,找到后要调用与此case关联的函数.简单处理的方法是使用递归,传入参数包.当然,也可以复杂一点,用hana算法来实现.有时最好的办法就是用最基础的技术从头开始编写.故此,我们将用`unpack`函数来实现,这个函数需要一个元组,元组中的元素就是这些case(不含default_):

``` C++
template<typename Any>
auto switch_(Any& a){
    return [&a](auto... cases_){
        auto cases=hana::make_tuple(cases_...);

        auto default_=hana::find_if(cases,[](auto const& c){
            return haha::first(c)==hana::type_c<default_t>;
        });
        static_assert(default_!=hana::nothing,"switch is missing a default_ case");

        auto rest=hana::filter(cases,[](auto const& c){
            return hana::first(c)!=hana::type_c<default_t>;
        });

        return hana::unpack(rest,[&](auto&... rests){
            return process(a,a.type(),hana::second(*default_),rests...);
        });
    };
```

`unpack`接受一个元组和一个函数,并以元组的内容作为参数调用函数.解包的结果是调用该函数的结果.此例,函数是一个泛型lambda,lambda调用了`process`函数.在这里使用`unpack`的原因是将rest元组转换为一个参数包更容易递归(相对于tuple来说).在继续处理`process`函数之前,先对参数`second(*default_)`作以解释.如前所述,`default_`是一个可选值.像`std::optional`一样,这个可选值重载了`dereference`运算符(和箭头运算符)以允许访问`optional`内部的值.如果`optional`为空(nothing),则引发编译错误.因为我们知道`default_`不为空(上面代码中有检查),我们只须简将与default相关联的函数传递给`process`函数.接下来进行最后一步的处理,实现`process`函数:

``` C++
template<typename Any,typename Default>
auto process(Any&,std::type_index const&,Default& default_){
    return default_();
}

template<typename Any,typename Default,typename Case,typename... Rest>
auto process(Any& a,std::type_index const& t,Default default_,Case& case_,Rest&... rest){
    using T=typename decltype(+hana::first(case_))::type;
    return t==typeid(T)?hana::second(case_)(*boost::unsafe_any_cast<T>(&a)):
        process(a,t,default_,rest...);
}
```

这个函数有两个重载版本:一个重载用于至少有一个case,一个重载用于仅有`default_` case.与我们期望的一样,仅有`default_` case的重载简单调用default函数并返回该结果.另一个重载才更有趣.首先,我们检索与与该case相关联的类型并将其保存到`T`变量.这里`decltype(...)::type`看起来挺复杂的,其实很简单.详情参见[类型计算]().然后,我们比较`any`的动态类型是否匹配这个case,如果匹配就调用关联函数,将`any`转换为正确的类型,否则,用其余的case再次递归.是不是很简单?以下是完整的代码:

``` C++
#include <boost/hana.hpp>
#include <boost/any.hpp>
#include <cassert>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <utility>
namespace hana = boost::hana;

//! [cases]
template <typename T>
auto case_ = [](auto f) {
  return hana::make_pair(hana::type_c<T>, f);
};
struct default_t;
auto default_ = case_<default_t>;
//! [cases]

//! [process]
template <typename Any, typename Default>
auto process(Any&, std::type_index const&, Default& default_) {
  return default_();
}
template <typename Any, typename Default, typename Case, typename ...Rest>
auto process(Any& a, std::type_index const& t, Default& default_,
             Case& case_, Rest& ...rest)
{
  using T = typename decltype(+hana::first(case_))::type;
  return t == typeid(T) ? hana::second(case_)(*boost::unsafe_any_cast<T>(&a))
                        : process(a, t, default_, rest...);
}
//! [process]

//! [switch_]
template <typename Any>
auto switch_(Any& a) {
  return [&a](auto ...cases_) {
    auto cases = hana::make_tuple(cases_...);
    auto default_ = hana::find_if(cases, [](auto const& c) {
      return hana::first(c) == hana::type_c<default_t>;
    });
    static_assert(default_ != hana::nothing,
      "switch is missing a default_ case");
    auto rest = hana::filter(cases, [](auto const& c) {
      return hana::first(c) != hana::type_c<default_t>;
    });
    return hana::unpack(rest, [&](auto& ...rest) {
      return process(a, a.type(), hana::second(*default_), rest...);
    });
  };
}
//! [switch_]
```

以上就是我们的快速入门了.这个例子只介绍了几个有用的算法(`find_if`,`filter`,`unpack`)和异构容器(`tuple`,`optional`),放心,还有更多!本教程的后续部分将以友好的方式逐步介绍与Hana有关的概念.如果你想立即着手编写代码,可以用以下备忘表作为快速参考.这个备忘表囊括了最常用的算法和容器,还提供了简短的说明.

# 备忘表

**备注**

* 大多数算法同时处理类型和值(参见[类型计算]())
* 算法通常返回一个新容器作为它们的返回结果;不会更改原容器(参见[算法]())
* 所有的算法都是`constexpr`函数对象

| 容器           | 说明                                                     |
-----------------|----------------------------------------------------------
| tuple          | 固定长度的基于索引的泛型异构序列,可将它视为异构对象中的`std::vector`|
| optional       | 表示可选值,即可以为空的值.它有点像`std::optional`,除了空值也是编译时已知的外|
| map            | 无序关联数组映射(唯一)编译期实体到任意对象.好比异构对象的`std::unordered_map`|
| set            | 无序容器,包含必须是编译期实体的唯一键.好比异构对象的`std::unordered_set`|
| range          | 表示编译期的数值范围,好比更好的`std::integer_sequence`|
| pair           | 持有两种异构对象的容器.好比`std::pair`,但压缩空类型的存储|
| string         | 编译期字符串|
| type           | 表示一个C++类型的容器.是类型与值相统一的根本所在.比MPL式的计算更有趣[类型计算]()|
| integral_constant| 表示编译期数值.非常类似于`std::integral_constant`,不同之处在于`hana::integral_constant`还定义了一些操作和语法糖|
| lazy           | 封装一个懒惰值或计算.|
| basic_tuple    | 简装版`hana::tuple`.不符合标准,但更有编译时效率|

| 函数                     | 说明                                                          |
--------------------------|----------------------------------------------------------------
| adjust(sequence,value,f)| 将函数应用到序列的每个元素上,将值与元素比较,返回调整后的序列|
| adjust_if(sequence,predicate,f)| 将函数应用到序列的每个元素上,调整满足谓词的每个元素,返回调整后的序号|
| {all,any,none}(sequence) | 返回是否 所有/任意/无 序列的元素都是真值|
| {all,any,none}(sequence) | 返回是否 所有/任意/无 序列的元素满足谓词|
| append(sequence,value)   | 附加元素到序列|
| at(sequence,index)       | 返回序列第n个元素,index必须是一个IntegralConstant|
| back(sequence)           | 返回非空序列的最后一个元素|
| concat(sequence1,sequence2)| 合并两个序列|
| contains(sequence,value) | 序列中是否包含给定的值|
| count(sequence,value)    | 序列中给定值的数量|
| count_if(sequence,predicate)| 序列中给定符合谓词条件的元素的数量|
| drop_front(sequence[,n]) | 从序列中删除前`n`个元素,若`length(sequence)<=n`,则删除整个序列.`n`必须是`IntegralConstant`,未提供`n`时,``默认为`1`|
| drop_front_exactly(sequence[,n])|从序列中删除前`n`个元素,`n`必须是`IntegralConstant`而且序列中至少要有`n`个元素,未提供`n`时,``默认为`1`|
| drop_back(sequence[,n])  | 从序列结尾删除`n`个元素,若`length(sequence)<=n`,则删除整个序列.`n`必须是`IntegralConstant`,未提供`n`时,``默认为`1`|
| drop_while(sequence,predicate) | 从序列删除满足谓词的元素.谓词必须返回一个`IntegralConstant`|
| fill(sequence,value) | 用`value`重置序列中的所有元素|
| filter(sequence,predicate) | 移除序列中所有不满足谓词的元素,谓词必须返回一个`IntegralConstant`|
| find(sequence,value) | 查找序列中第一个与`value`相等的元素,并返回`just`或`nothing`,参见`hana::optional`|
| find_if(sequence,predicate) | 查找序列中第一个与满足谓词的元素,并返回`just`或`nothing`,参见`hana::optional`|
| flatten(sequence) | 连接序列,类似于`std::tuple_cat`|
| fold_left(sequence[,state],f) | 从左侧累积序列的元素，可选地使用提供的初始状态|
| fold_right(sequence[,state],f) | 从右侧累积序列的元素，可选地使用提供的初始状态|
| fold(sequence[,state],f) | 相当于`fold_left`,与`Boost.MPL`和`Boost.Fusion`保持一致|
| for_each(sequence,f) | 在序列的每个元素上调用`f`函数.返回`void`|
| front(sequence) | 返回非空序列的第一个元素|
| group(sequence[,predicate])| 将所有满足（或全部不满足）谓词的序列的相邻元素组。 谓词默认为相等，在这种情况下，元素必须是`Comparable`(可比性)的|
| insert(sequence,index,element) | 在指定索引处插入元素,`index`必须是`IntegralConstant`|
| insert_range(sequence,index,elements) | 在指定索引处插入多个元素,`index`必须是`IntegralConstant`|
| is_empty(sequence)| 序列是否为空,返回值作为`IntegralConstant`|
| length(sequence) | 序列的长度,返回值作为`IntegralConstant`|
| lexicographical_compare(sequence1,sequence2[,predicate])| 执行两个序列的词典比较，可选地使用自定义谓词，默认情况下使用`hana::less`|
| maximum(sequence[,predicate])| 返回序列的最大元素，可选地根据谓词。 如果没有提供谓词，那么元素必须是`Orderable`(可排序)的|
| minimum(sequence[,predicate])|返回序列的最小元素，可选地根据谓词。 如果没有提供谓词，那么元素必须是`Orderable`(可排序)的|
| partition(sequence,predicate) | 将序列分成满足和不满足谓词的元素对 |
| prepend(sequence,value) | 将元素预置到序列|
| remove(sequence,value) | 移除所有与`value`相等的元素|
| remove_at(sequence,index) | 移除给定索引处的元素,`index`必须是`IntegralConstant`|
| remove_if(sequence,predicate)| 移除所有满足谓词的元素,谓词必须返回`IntegralConstant`|
| remove_range(sequence,from,to) | 移除半开区间[from,to)的元素,这些索引必须都是`IntegralConstant`|
| replace(sequence,oldval,newval) | 将序列中所有元素与oldval相等的值替换为newval|
| replace_if(sequence,predicate,newval)|用newval替换序列中满足某些谓词的元素.|
| reverse(sequence)|逆序排列|
| reverse_fold(sequence[,state],f)|相当于`fold_right`.为与Boost.MPL和Boost.Fusion保持一致.|
| size(sequence) |相当于`length`,为与C++标准库保持一致.|
| slice(sequence,indices)|返回包含原始序列的给定索引处的元素的新序列.|
| slice_c<from,to>(sequence)|返回包含原始序列的半开区间`[from，to)`中包含的索引处的元素的新序列.|
| sort(sequence[,predicate]) |可选地根据谓词对序列的元素进行（稳定）排序. 如果没有提供谓词，那么元素必须是可排序的.|
| take_back(sequence,number) |取序列的最后`n`个元素，如果`length（sequence）<= n`，则取整个序列。 `n`必须是`IntegralConstant`.|
| take_front(sequence,number) |取序列的前`n`个元素，如果`length（sequence）<= n`，则取整个序列. `n`必须是`IntegralConstant`.|
| take_while(sequence,predicate) |当一些谓词满足时，获取序列的元素，并返回.|
| transform(sequence,f) |对序列的每个元素应用函数并返回结果.|
| unique(sequence[,predicate]) |从序列中删除所有连续的重复项.谓词默认为相等，在这种情况下，元素必须是Comparable(可比较的).|
| unpack(sequence,f) |用序列的内容调用`f`,等价于`f(x1,...,xN)`|
| zip(s1,...,sN) | 将`N`个序列压缩成元组序列.所有序列必须具有相同的长度.|
| zip_shortest(s1,...,sN) | 将N个序列压缩成元组序列. 返回的序列具有最短输入序列的长度. |
| zip_with(f,s1,...,sN) | 用N元函数压缩N个序列.所有序列必须具有相同的长度.|
| zip_shortest_with(f,s1,...,sN) |用N元函数压缩N个序列.返回的序列具有最短输入序列的长度.|

# 断言

在本教程的其余部分，您将看到使用不同类型的断言（如`BOOST_HANA_RUNTIME_CHECK`和`BOOST_HANA_CONSTANT_CHECK`）的代码段。 像任何合理的断言宏，它们基本上检查它们给出的条件是否满足。 然而，在异构编程的上下文中，一些信息在编译时是已知的，而其他信息仅在运行时可知。 在上下文中使用的断言的确切类型告诉您所声明的条件是否可以在编译时可知，或者是否必须在运行时计算，这是非常宝贵的信息。 下面是教程中使用的不同类型的断言，以及它们的特殊的简述。 有关更多详细信息，您应该检查[对断言的引用]().

| 断言 | 说明|
------|------
|BOOST_HANA_RUNTIME_CHECK|在直到运行时才可知的条件上断言.这种断言提供了最弱的保证形式.|
|BOOST_HANA_CONSTEXPR_CHECK|如果在常量表达式中允许lambda,则在`constexpr`的条件上断言.换句话说,它不是一个`static_assert`的唯一原因是语言限制了lambda不能出现在常量表达式中,这个限制可能会在c++17中解除.|
|static_assert|在`constexpr`条件上断言.强度上大于`BOOST_HANA_CONSTEXPR_CHECK`,因为它要求条件是一个`constexpr`表达式,因此它保证了表达式中使用的算法是`constexpr`友好的.|
|BOOST_HANA_CONSTANT_CHECK|在一个布尔型`IntegralConstant`条件上断言.这种断言提供了最强的保证形式.因为`IntegralConstant`可以转换为`constexpr`值,即使它本身不是`constexpr`.|

# 编译期数值

本节介绍`IntegralConstant`的重要概念和Hana的元编程范式背后的哲学.让我们从一个奇怪的问题开始. 什么是`integral_constant`?

``` C++
template<class T, T v>
struct integral_constant{
    static constexpr T value=v;
    typedef T value_type;
    typedef integral_constant type;
    constexpr operator value_type() const noexcept{return value;}
    constexpr value_type operator()() const noexcept{return value;}
};
```

**注意**
* 如果你觉得这些很新奇，你可以需要看看`std::integral_constant`[文档](http://en.cppreference.com/w/cpp/types/integral_constant)

一个有效的回答是,`integral_constant`表示数值的类型编码,或者更一般地表示为任何整型对象.比如,我们可以使用模板别名很容易地在该表示中的数值上定义后继函数：

``` C++
template<typename N>
using succ=integral_constant<int,N::value+1>;

using one=integral_constant<int,1>;
using two=succ<one>;
using three=succ<two>;
// ...
```

通常将这种使用`integral_constant`的方式用于模板元编程的类型实体.我们还会看到另一种`integral_constant`使用方式是作为一个运行时对象,代表一个整型的`constexpr`值:

``` C++
auto one=integral_constant<int,1>{};
```

这里,虽然`one`没有标记为`constexpr`,它所拥有的抽象值(一个`constexpr`的 1)在编译期仍然可用,因为该值被编码到`one`类型中,事实上，即使`one`不是`constexpr`,我们也可以用`decltype`检索它表示的编译期值:

``` C++
auto one=integral_constant<int,1>{};
constexpr int one_constexpr=decltype(one)::value;
```

但是为什么我们会想把`integral_constant`当作对象而不是类型实体呢?为了看是为什么,考虑我们现在如果实现之前同样的后继函数:

``` C++
template<typename N>
auto succ(N){
    return integral_constant<int,N::value+1>{};
}

auto one=integral_constant<int,1>{};
auto two=succ(one);
auto three=succ(two);
```

您注意到了什么新东西吗? 区别在于，不是在类型级别使用模板别名实现`succ`，我们现在使用模板函数在值级别实现它. 此外，我们现在可以使用与普通C++相同的语法执行编译时算术.这种将编译期实体看作对象而不是类型的方式是Hana的表达力的关键。

##编译期计算##

MPL定义了多个[算术运算符](http://www.boost.org/doc/libs/release/libs/mpl/doc/refmanual/arithmetic-operations.html)以支持使用`integral_constant`做编译期计算.一个典型的例子是`plus`运算符,其大致实现如下：

``` C++
template<typename X,typename Y>
struct plus{
    using type=integral_constant<decltype(X::value+Y::value),X::value+Y::value>;
};

using three=plus<integral_constant<int,1>,integral_constant<int,2>>::type;
```

通过将`integral_constant`作为对象而不是类型来看待，从元函数到函数的转换非常简单：

``` C++
template<typename V,V v,typename U,U u>
constexpr auto operator+(integral_constant<V,v>,integral_constant<U,u>){
    return integral_constant<decltype(v+u>),v+u>{};
}

auto three=integral_constant<int,1>{}+integral_constant<int,2>{};
```

强调这个操作符不返回正常整数的事实是非常重要. 相反，它返回一个值初始化的对象，其类型包含加法的结果。 该对象中包含的唯一有用的信息实际上是在它的类型，我们正在创建一个对象，因为它允许我们使用这个漂亮的值级语法。 事实证明，我们可以通过使用[C++14变量模板](http://en.wikipedia.org/wiki/C%2B%2B14#Variable_templates)来简化`integral_constant`建，从而使这种语法更好：

``` C++
template<int i>
constexpr integral_constant<int,i> int_c{};

auto three=int_c<1>+int_c<2>;
```

现在我们谈论的是在初始类型层面方法中表现出的增强体验，不是吗？ 但还有更多; 我们还可以使用C ++ 14用户定义的文字使这个过程更简单：

``` C++
template<char... digits>
constexpr auto operator"" _c(){
    //parse the digits and return an integral_constant
}

auto three=1_c+3_c;
```

Hana提供了自己的`integral_constants`,它定义了算术运算符,就像我们上面显示的一样, Hana还提供了变量模板，可以轻松创建不同类型的`integral_constants`：`int_c，long_c，bool_c`等.这允许你省略后面的`{}`大括号，否则需要值来初始化这些对象。 当然，也提供`_c`后缀; 它是`hana :: literals`命名空间的一部分，您必须在使用它之前将其导入到命名空间中：

``` C++
using namesapce hana::literals;

auto three=1_c+3_c;
```

这样,你可以做编译期计算,而不必尴尬的与类型级别的特性斗争,你的同事现在将能够了解发生了什么.

##示例：距离公式

为了说明它是多么的好用,让我们实现一个函数在编译期计算一个2-D欧氏距离.作为提醒,2-D平面中的两个点的欧几里得距离由下式给出

![distance](./res/20161210133037.jpg)

先看看用类型计算的样子(使用MPL):

``` C++
template<typename P1,typename P2>
struct distance{
    using xs=typename mpl::minus<typename P1::x,typename P2::x>::type;
    using ys=typename mpl::minus<typename p1::y,typename P2::y>::type;

    using type=typename sqrt<
        typename mpl::plus<
            typename mpl::multiplies<xs,xs>::type,
            typename mpl::multiplies<ys,ys>::type
        >::type
    >::type;
};

static_assert(mpl::equal_to<
    distance<point<mpl::int_<3>,mpl::int_<5>>,point<<mpl::int_<7>,mpl::int_<2>>>::type,
    mpl::int_<5>
>::value);
```

嗯...现在,让我们用上面提到的值级方法来实现它：

``` C++
template<typename P1,typename P2>
constexpr auto distance(P1 p1,P2 p2){
    auto xs=p1.x-p2.x;
    auto ys=p1.y-p2.y;
    return sqrt(xs*xs+ys*ys);
}

BOOST_HANA_CONSTANT_CHECK(distance(point(3_c,5_c),point(7_c,2_c))==5_c);
```

这个版本看起来简洁多了.然而,这还没完。注意到`distance`函数看起来和你为计算动态值的欧几里德距离所写的一样吗？ 事实上，因为我们在动态值和编译期计算使用了相同的语法,为其编写的通用函数将能同时工作于编译期和运行期！

``` C++
auto p1=point(3,5); // dynamic values now
auto p2=point(7,2); //
BOOST_HANA_RUNTIME_CHECK(distance(p1,p2)==5); //same function works!
```
*不用改变任何代码*,我们可以在运行时使`distance`函数正确地工作。

##编译期分发##

现在我们有了编译期计算，下一步需要解决编译期分发问题，元编程时，如果一些条件为真则编译一段代码，否则编译另一段代码是很有用的。就好像是[static_if](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4461.html)一样.还没搞清楚为什么需要编译期分发?先考察下面的代码(改编自[N4461](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4461.html)):

``` C++
template<typename T,typename... Args>
std::enable_if_t<std::is_constructible<T,Args...>::value,std::unique_ptr<T>>
make_unique(Args&&... args){
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<typename T,typename... Args>
std::enable_if_t<!std::is_constructible<T,Args...>::value,std::unique_ptr<T>>
make_unique(Args&&... args){
    return std::unique_ptr<T>(new T{std::forward<Args>(args)...});
}
```

以上代码使用构造函数正规的语法形式创建`std::unique_ptr`.为此,它利用`SFINAE`实现两个不同的重载.现在,每个看到这些代码的人都不仅会问,为什么不能更简单一点:

``` C++
template <typename T, typename ...Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  if (std::is_constructible<T, Args...>::value)
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  else
    return std::unique_ptr<T>(new T{std::forward<Args>(args)...});
}
```

原因是编译器需要编译`if`语句的两个分支，而不考虑条件（即使它在编译时是已知的）.但是当`T`不能从`Args...`构造时，第二个分支将无法编译，这将导致硬编译错误。 我们真正需要的是找一种方法告诉编译器，当条件为真时，`不要编译`第二个分支第一个分支。

为了模拟这一点，Hana提供了一个`if_`函数，它像一个普通的`if`语句一样工作，除了它需要一个可以是IntegralConstant的条件，并返回由条件选择的两个值之一（可能有不同的类型）之外. 如果条件为真，则返回第一个值，否则返回第二个值。 一个有点空洞的例子如下：

``` C++
auto one_two_three=hana::if_(hana::true_c,123,"hello");
auto hello=hana::if_(hana::false_c,123,"hello");
```

**注意**

* `hana::true_c`和`hana::false_c`为编译期`IntegralConstant`布尔值.分别表示编译期真值和假值.

`one_two_three`等于`123`,`hello`等于`"hello"`.从另一个角度看,`if_`很像`?:`运算符,除了`:`分割符两边可以有不同类型外:

``` C++
//这两条语句都失败了,因为分支有不兼容的类型.
auto one_two_three=hana::true_c ? 123 : "hello";
auto hello=hana::false_c ? 123 : "hello";
```

好吧，这样的代码看起来非常简洁，但是编译器不支持这个偷懒的办法。那么，如何实现类似`if`的分支呢?我们决定在分支中使用泛型`lambda`,借助`hana::if_`来执行我们想要的分支.以下重写`make_unique`:

``` C++
template<typename T,typename... Args>
std::unique_ptr<T> make_unique(Args&&... args){
    return hana::if_(std::is_constructible<T,Args...>{},
        [](auto&&... x){return std::unique_ptr<T>(new T(std::forward<Args>(x)...));},
        [](auto&&... x){return std::unique_ptr<T>(new T{std::forward<Args>(x)...});}
    )(std::forward<Args>(args)...);
}
```

如果条件为真,`hana::if_`执行第一个泛型lambda分支,为假则执行第二个分支.`hana::if_`仅简单返回某分支,我们传入了`(std::forward<Args>(args)...)`参数以便返回的lambda立即执行,这里，预期的泛型lambda将参数`x...`用`args...`实参执行并返回结果.

这样做（立即传参数）的原因是因为每个分支的主体只能在所有`x...`类型已知时才被实例化.事实上,由于分支是泛型lambda,在它被调用之前,参数的类型是未知的,编译器必须在检查lambda函数体内类型之前等待`x...`的类型变为已知.因为当条件不满足（`hana::if_`忽略了它）时，错误的lambda从不被调用，所以失败的lambda的函数体从不被类型检查，因此不会发生编译错误。

**注意**

* `if_`的分支是lambda,因此，它们从不同的途径构造了`make_unique`函数.在这些分支中出现的变量必须被lambdas捕获或作为参数传递给它们，因此它们受到捕获或传递的方式（通过值，引用等）的影响。

由于这种将分支表达为lambda类型然后调用它们的模式是非常常见的，Hana提供了一个`eval_if`函数，其目的是使编译时分支更容易。 `eval_if`来自于一个事实，在lambda中，可以接收输入数据作为参数或从上下文中捕获它。 然而，为了模拟语言级`if`语句，隐含地从封闭范围捕获变量通常更自然. 因此，我们更喜欢这样写:

``` C++
template <typename T, typename ...Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return hana::if_(std::is_constructible<T, Args...>{},
    [&] { return std::unique_ptr<T>(new T(std::forward<Args>(args)...)); },
    [&] { return std::unique_ptr<T>(new T{std::forward<Args>(args)...}); }
  );
}
```

这里，我们捕获了来自闭包范围的`args ...`变量，这就不需要我们引入新的`x ...`变量并将它们作为参数传递给分支。 然而，还两个问题. 首先，这样做将不会实现正确的结果，因为`hana::if_`将最终返回一个lambda，而不是返回调用该lambda的结果. 要解决这个问题，我们可以使用`hana::eval_if`而不是`hana::if_`：

``` C++
template <typename T, typename ...Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return hana::eval_if(std::is_constructible<T, Args...>{},
    [&] { return std::unique_ptr<T>(new T(std::forward<Args>(args)...)); },
    [&] { return std::unique_ptr<T>(new T{std::forward<Args>(args)...}); }
  );
}
```

这里，我们使用`[&]`通过引用捕获闭包的`args ...`，我们不需要接收任何参数. 此外,`hana::eval_if`假定其参数是可以被调用的分支，它将负责调用由条件选择的分支. 然而，这仍然会导致编译失败，因为lambda的主体不再有任何依赖，因而将对两个分支进行语义分析，即使只有一个将被使用. 这个问题的解决方案是使lambda的主体人为地依赖于某些东西，以防止编译器在lambda被实际使用之前执行语义分析. 为了使这一点成为可能，`hana::eval_if`将使用标识函数（一个函数无改变地返回其参数）调用所选的分支，如果分支接受这样的参数：

``` C++
template <typename T, typename ...Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return hana::eval_if(std::is_constructible<T, Args...>{},
    [&](auto _) { return std::unique_ptr<T>(new T(std::forward<Args>(_(args))...)); },
    [&](auto _) { return std::unique_ptr<T>(new T{std::forward<Args>(_(args))...}); }
  );
}
```

这里，分支的主体采用约定的称为`_`的附加参数.这个参数将由`hana::eval_if`提供给所选择的分支. 然后，我们使用`_`作为我们想要在每个分支的主体内依赖的变量的函数.使用`_`会发生什么呢?`_`是一个直接原样返回其参数的函数.但是，编译器在lambda实际被调用之前不可能知道它，因此它不能知道`_(args)`的类型.这样一来,会阻止编译器执行语义分析，并且不会发生编译错误.另外，由于`_(x)`保证等于`x`，我们知道我们实际上没有通过使用这个技巧改变分支的语义.

虽然使用这个技巧可能看起来很麻烦，但当处理分支中的许多变量时，它可能非常有用。此外，不需要用`_`来包装所有变量; 只有那些包装类型检查必须延迟的表达式中涉及的变量才需要使用它.在Hana中还有一些需要了解的编译时分支，参见`hana::eval_if`,`hana::if_`和`hana::lazy`来深入了解它们.

##为什么停到这里了?##

为什么我们应该限制算术运算和分支?当您开始将IntegralConstants视为对象时，使用更多通常有用的函数来增加其接口更为明智。 例如，Hana的IntegralConstants定义了一个`times`成员函数，可用于调用函数一定次数，这对于循环展开尤其有用：

``` C++
__attribute__((noinline)) void f() { }
int main() {
  hana::int_c<10>.times(f);
}
```

以上代码在编译会展开为调用10次`f`.相当于如下操作:

``` C++
f();f();...f();//10 times
```

**注意**

* 通常[要小心](https://youtu.be/qkzaZumt_uk?t=4478)手动展开循环或手动执行其他此类优化.在大多数情况下,你的编译器在优化时可能比你更好.

`IntegralConstant`的另一个很好的用途是定义更好的运算符来索引异构序列.`std::tuple`必须使用`std::get`访问,`hana::tuple`可以使用用于标准库容器的熟悉的`operator []`来访问：

``` C++
auto values=hana::make_tuple(1,'x',3.4f);
char x=values[1_c];
```

这是怎样让工作变得简单的呢. 基本上,`hana::tuple`定义一个使用一个整数常量的运算符`[]`而不是一个通常常的整数,类似于:

``` C++
template<typename N>
constexpr decltype(auto) operator[](N const&){
    return std::get<N::value>(*this);
}
```

本节,`IntegralConstant`部分结束了,本节介绍 了Hana的新的元编程方法.如果你喜欢你所看到的.本教程的其余部分应该会感到更加熟悉.

# 类型计算

在这一点上,如果你有兴趣像MPL一样进行类型计算,你可能会想知道Hana如何帮助你.不用担心,Hana提供了一种通过将类型表示为值来执行具有大量表达性的类型计算的方法,就像我们将编译时数字表示为值一样. 这是一种全新的接近元编程的方法,如果你想熟练使用Hana,你应该尝试将你的旧MPL习惯放在一边.

但是,请注意,现代C ++的功能,如[自动推导返回类型](http://en.wikipedia.org/wiki/C%2B%2B14#Function_return_type_deduction),在许多情况下不需要类型计算. 因此,在考虑做一个类型计算之前,你应该问自己是否有一个更简单的方法来实现你想要实现的.在大多数情况下,答案是肯定的. 然而,当答案是否定的时候,Hana将为你提供核力量设施来做需要做的事情.

## 类型作为对象

Hana中类型计算的关键点基本上与编译时计算的方法相同.基本想法是将编译时实体表示为对象,将它们包装到某种容器中. 对于`IntegralConstant`,编译时实体是整型的常量表达式,我们使用的包装器是`integral_constant`. 在本节中，编译时实体将是类型,我们将使用的包装器称为`type`,就像我们对`IntegralConstant`做的一样,让我们开始定义一个可以用来表示类型的虚拟模板：

``` C++
template<typename T>
struct basic_type{
    //empty (for now)
};

basic_type<int> Int{};
basic_type<char> Char{};
```

**注意**

* 在这里我们使用`basic_type`名字，是因为我们仅构建一个hana提供版本的最简版本。

虽然这看起来完全没用，但实际上足以开始编写看起来像函数的元函数了。 让我们考虑以下`std::add_pointer`和`std::is_pointer`的替代实现：

``` C++
template<typename T>
constexpr basic_type<T*> add_pointer(basic_type<T> const&)
{ return {}; }

template<typename T>
constexpr auto is_pointer(basic_type<T> const&)
{ return hana::bool_c<false>; }

template<typename T>
constexpr auto is_pointer(basic_type<T*> const&)
{ return hana::bool_c<true>; }
```

我们刚刚编写了看起来像函数的元函数，就像我们在上一节中将编译时算术元函数编写为异构C++操作符一样. 以下是我们如何使用它们：

``` C++
basic_type<int> t{};
auto p=add_pointer(t);
BOOST_HANA_CONSTANT_CHECK(is_pointer(p));
```

注意到我们现在如何使用正常的函数调用语法来执行类型级别的计算了吗? 这类似于使用编译时数值如何使用正常的C++操作符来执行编译时计算. 像我们对integral_constant所做的一样,我们还可以进一步使用C++ 14变量模板为创建类型提供语法糖：

``` c++
template<typename T>
constexpr basic_type<T> type_c{};

auto t=type_c<int>;
auto p=add_pointer(t);
BOOST_HANA_CONSTANT_CHECK(is_pointer(p));
```

**注意**

* 这不是`hana::type_c`变量模板完整实现，因为有些细微不同之处;仅用于解释的目的.把它仍到一边,参考`hana::type`的实现，以确切知道您可以从`hana::type_c<...>`中获得什么.

## 优势

这样做有什么好处呢？因为`type_c<...>`是一个对象,我们可以将它存储到像`tuple`这样的异构容器中,我们可以移动它,也可以将它传递(或者返回)到函数中,而且我们可以用在任何需要对象的地方:

``` C++
auto types=hana::make_tuple(hana::type_c<int*>,hana::type_c<char&>,hana::type_c<void>);
auto char_ref=types[1_c];

BOOST_HANA_CONSTANT_CHECK(char_ref==hana::type_c<char&>);
```

**注意**

* 当需要多个类型时,编写`make_tuple(type_c<T>...)`可能会觉得比较繁嗦,为此,Hana提供了变量模板`tuple_t<T...>`,它是`make_tuple(type_c<T>...)`的语法糖.

另外,请注意,由于上面的元组实际上只是一个正常的异构序列,就像ints的元组上一样,我们可以对该序列应用异构算法.此外,由于我们只是操作对象,我们现在可以使用完整的语言支持,而不仅仅是在类型级别提供的小子集.例如,考虑删除不是引用的所有类型或来自类型序列的指针的任务.如果用MPL,我们必须使用占位符表达式来表达谓词,看起来较为笨重:

```C++
using types = mpl::vector<int, char&, void*>;
using ts = mpl::copy_if<types, mpl::or_<std::is_pointer<mpl::_1>,
                                        std::is_reference<mpl::_1>>>::type;
//                             ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//                             placeholder expression
static_assert(mpl::equal<ts, mpl::vector<char&, void*>>::value, "");
```

现在,由于我们在操作对象,我们可以使用完整的语言支持,并使用一个通用的lambda,这样的代码有更好的可读性：

```C++
auto types = hana::tuple_t<int*, char&, void>;
auto ts = hana::filter(types, [](auto t) {
  return is_pointer(t) || is_reference(t);
});
BOOST_HANA_CONSTANT_CHECK(ts == hana::tuple_t<int*, char&>);
```

由于Hana对所有异构容器进行统一处理,这种将类型表示为值的方法也具有以下优点：异构计算和类型计算现在只需要单个库即可. 事实上，虽然我们通常需要两个不同的库来执行几乎相同的任务，但我们现在想要一个单独的库来完成它们.再次,考虑使用谓词过滤序列的任务.如果使用MPL和Fusion，我们必须这样做：

```C++
// types (MPL)
using types = mpl::vector<int*, char&, void>;
using ts = mpl::copy_if<types, mpl::or_<std::is_pointer<mpl::_1>,
                                        std::is_reference<mpl::_1>>>::type;
// values (Fusion)
auto values = fusion::make_vector(1, 'c', nullptr, 3.5);
auto vs = fusion::filter_if<std::is_integral<mpl::_1>>(values);
```

但用Hana,只要一个库就行了.注意看我们怎么使用相似的算法和容器的，并且只调整谓词，以便它可以对值进行操作：

```C++
// types
auto types = hana::tuple_t<int*, char&, void>;
auto ts = hana::filter(types, [](auto t) {
  return is_pointer(t) || is_reference(t);
});

// values
auto values = hana::make_tuple(1, 'c', nullptr, 3.5);
auto vs = hana::filter(values, [](auto const& t) {
  return is_integral(hana::typeid_(t));
});
```

但这不是全部。 实际上，具有用于类型和值计算的统一语法允许我们在异构容器的接口中实现更大的一致性。 例如，考虑创建将类型与值相关联，然后访问它的元素的异构映射的简单任务。 使用Fusion,您那未经训练的眼睛不会一看到就能理解的：

```C++
auto map = fusion::make_map<char, int, long, float, double, void>(
  "char", "int", "long", "float", "double", "void"
);
std::string Int = fusion::at_key<int>(map);
BOOST_HANA_RUNTIME_CHECK(Int == "int");
```

但是，对于类型和值统一语法，同样的事情变得更加清楚：

```C++
auto map = hana::make_map(
  hana::make_pair(hana::type_c<char>,   "char"),
  hana::make_pair(hana::type_c<int>,    "int"),
  hana::make_pair(hana::type_c<long>,   "long"),
  hana::make_pair(hana::type_c<float>,  "float"),
  hana::make_pair(hana::type_c<double>, "double")
);
std::string Int = map[hana::type_c<int>];
BOOST_HANA_RUNTIME_CHECK(Int == "int");
```

虽然以Hana的方式需要更多的代码行，但它更可读，更接近我们期望的初始化方式。

## 使用此方式工作

到目前为止，我们可以将类型表示为值，并使用通常的C++语法对这些对象执行类型级计算。 这很好，但它不是非常有用，因为我们没有办法从对象表示推导出正常的C++类型。 例如，我们如何声明一个从类型计算的结果获取的变量类型？

```C++
auto t = add_pointer(hana::type_c<int>); // could be a complex type computation
using T = the-type-represented-by-t;
T var = ...;
```

现在，没有简单的方法去做以上工作。 为了更容易实现，我们丰富了上面定义的basic_type容器的接口。 而不是一个空的结构，我们现在定义为：

```C++
template <typename T>
struct basic_type {
  using type = T;
};
```

**注意**

* 这相当于使basic_type是MPL意义上的元函数。

这样，我们可以使用`decltype`来容易地访问由`type_c<...>`对象表示的实际C++类型：

```C++
auto t = add_pointer(hana::type_c<int>);
using T = decltype(t)::type; // fetches basic_type<T>::type
T var = ...;
```

一般来说，使用Hana进行类型级元编程需要三步：

1. 使用`hana::type_c<...>`将对象表示为对象
2. 使用值语法执行类型转换
3. 使用decltype(...)::type解用类型

现在，你一定认为这是令人难以置信的繁琐。 在现实中，它有以下几个原因。 首先，这种包装和解包只需要在一些非常薄的边界发生。

```C++
auto t = hana::type_c<T>;
auto result = huge_type_computation(t);
using Result = decltype(result)::type;
```

此外，由于您在计算中获得处理对象（无需包装/解开）的优势，因此包装和解包的成本将摊销在整个计算上。 因此，对于复杂类型的计算，根据在该计算内的值的工作的表达性增益，该三步骤过程的句法噪声很快变得可忽略。 另外，使用值而不是类型意味着我们可以避免在整个地方键入typename和template，这在经典元编程中占了很多句法噪声。

另一点是，并不总是需要三个完整的步骤。 事实上，有时候只需要一个类型的计算并查询结果，而不必将结果作为一个普通的C++类型：

```C++
auto t = hana::type_c<T>;
auto result = type_computation(t);
BOOST_HANA_CONSTANT_CHECK(is_pointer(result)); // third step skipped
```

在这种情况下，我们可以跳过第三步，因为我们不需要访问由result表示的实际类型。 在其他情况下，可以避免第一步，就像使用`tuple_t`，它没有比任何其他纯类型级别方法更多的语法噪声：

```C++
auto types = hana::tuple_t<int*, char&, void>; // first step skipped
auto pointers = hana::transform(types, [](auto t) {
  return add_pointer(t);
});
```

对此持怀疑态度的读者，让我们考虑一个找到类型序列中最小类型的任务。 这是一个很短的类型计算的一个很好的例子，这是我们期望新的范式受到最大的影响的地方。 正如你将看到的，即使对于小计算，事情仍然可管理。 首先，让我们用MPL实现它：

```C++
template <typename ...T>
struct smallest
  : mpl::deref<
    typename mpl::min_element<
      mpl::vector<T...>,
      mpl::less<mpl::sizeof_<mpl::_1>, mpl::sizeof_<mpl::_2>>
    >::type
  >
{ };
template <typename ...T>
using smallest_t = typename smallest<T...>::type;
static_assert(std::is_same<
  smallest_t<char, long, long double>,
  char
>::value, "");
```

结果是很有可读性（对于任何熟悉MPL的人）。 现在让我们使用Hana实现相同的事情：

```C++
template <typename ...T>
auto smallest = hana::minimum(hana::make_tuple(hana::type_c<T>...), [](auto t, auto u) {
  return hana::sizeof_(t) < hana::sizeof_(u);
});
template <typename ...T>
using smallest_t = typename decltype(smallest<T...>)::type;
static_assert(std::is_same<
  smallest_t<char, long, long double>, char
>::value, "");
```

正如你所看到的，3步过程的句法噪声几乎完全被其余的计算所掩盖。

## The generic lifting process

我们以函数形式引入的第一个类型计算如下：

```C++
template <typename T>
constexpr auto add_pointer(hana::basic_type<T> const&) {
  return hana::type<T*>;
}
```

我们需要将它写成如下样子，虽然看起来复杂了一些：

```C++
template <typename T>
constexpr auto add_pointer(hana::basic_type<T> const&) {
  return hana::type_c<typename std::add_pointer<T>::type>;
}
```

然而，这个实现强调的事实，我们真的模拟一个现有的元函数，并简单地表示为一个函数。 换句话说，我们通过创建我们自己的add_pointer函数来提取一个元函数（std :: add_pointer）到值的世界。 事实证明，这个提升过程是一个通用的过程。 事实上，给定任何元函数，我们可以写几乎相同的事情：

```C++
template <typename T>
constexpr auto add_const(hana::basic_type<T> const&)
{ return hana::type_c<typename std::add_const<T>::type>; }
template <typename T>
constexpr auto add_volatile(hana::basic_type<T> const&)
{ return hana::type_c<typename std::add_volatile<T>::type>; }
template <typename T>
constexpr auto add_lvalue_reference(hana::basic_type<T> const&)
{ return hana::type_c<typename std::add_lvalue_reference<T>::type>; }
// etc...
```

这种机械变换很容易被抽象成可以处理任何[MPL元函数](http://www.boost.org/doc/libs/release/libs/mpl/doc/refmanual/metafunction.html)的通用提升器，如下所示：

```C++
template <template <typename> class F, typename T>
constexpr auto metafunction(hana::basic_type<T> const&)
{ return hana::type_c<typename F<T>::type>; }
auto t = hana::type_c<int>;
BOOST_HANA_CONSTANT_CHECK(metafunction<std::add_pointer>(t) == hana::type_c<int*>);
```

更一般地，我们将允许具有任何数量的参数的元函数，这带来了以下稍微呆板的实现：

```C++
template <template <typename ...> class F, typename ...T>
constexpr auto metafunction(hana::basic_type<T> const& ...)
{ return hana::type_c<typename F<T...>::type>; }
BOOST_HANA_CONSTANT_CHECK(
  metafunction<std::common_type>(hana::type_c<int>, hana::type_c<long>) == hana::type_c<long>
);
```

Hana提供了一个类似的通用元函数升级器`hana::metafunction`。 一个小小的改进是`hana::metafunction<F>`是一个函数对象，而不是一个重载的函数，所以可以把它传递给更高阶的算法。 它也是一个稍微更强大的Metafunction概念的模型，但是现在可以安全地忽略它。 我们在本节中探讨的过程不仅适用于元函数; 它也适用于模板。 事实上，我们可以定义：

```C++
template <template <typename ...> class F, typename ...T>
constexpr auto template_(hana::basic_type<T> const& ...)
{ return hana::type_c<F<T...>>; }
BOOST_HANA_CONSTANT_CHECK(
  template_<std::vector>(hana::type_c<int>) == hana::type_c<std::vector<int>>
);
```

Hana为名为`hana::template_`的模板提供了一个通用的提升器，它还为名为`hana::metafunction_class`的MPL元函数类提供了一个通用的提升器。 这为我们提供了一种将“传统”类型计算统一表示为函数的方法，以便使用经典类型元编程库编写的任何代码几乎可以与Hana一起使用。 例如，假设你有一大块基于MPL的代码，你想与Hana接口。 这样做的过程不会比用Hana提供的提升器包装你的元函数更难：

```C++
template <typename T>
struct legacy {
  using type = ...; // something you really don't want to mess with
};
auto types = hana::make_tuple(...);
auto use = hana::transform(types, hana::metafunction<legacy>);
```

但是，请注意，并非所有类型级别的计算都可以使用Hana提供的工具提升。 例如，不能提升`std::extent`，因为它需要非类型模板参数。 因为没有办法在C ++中统一处理非类型模板参数，所以必须使用特定于该类型级别计算的手写函数对象：

```C++
auto extent = [](auto t, auto n) {
  return std::extent<typename decltype(t)::type, hana::value(n)>{};
};
BOOST_HANA_CONSTANT_CHECK(extent(hana::type_c<char>, hana::int_c<1>) == hana::size_c<0>);
BOOST_HANA_CONSTANT_CHECK(extent(hana::type_c<char[1][2]>, hana::int_c<1>) == hana::size_c<2>);
```

**注意**

* 当从头文件<type_traits>中使用type traits时，不要忘记包含桥接文件：`std::integral_constrant`(<boost/hana/ext/std/integral_constant.hpp>)

然而，在实践中，这不应该是一个问题，因为绝大多数类型级计算可以很容易地提升。 最后，由于<type_traits>头提供的metafunctions频繁使用，Hana为其中每一个提供一个提升的版本。 那些解除的traits在hana::traits命名空间，他们在<boost/hana/traits.hpp>头文件中：

```C++
BOOST_HANA_CONSTANT_CHECK(hana::traits::add_pointer(hana::type_c<int>) == hana::type_c<int*>);
BOOST_HANA_CONSTANT_CHECK(hana::traits::common_type(hana::type_c<int>, hana::type_c<long>) == hana::type_c<long>);
BOOST_HANA_CONSTANT_CHECK(hana::traits::is_integral(hana::type_c<int>));
auto types = hana::tuple_t<int, char, long>;
BOOST_HANA_CONSTANT_CHECK(hana::all_of(types, hana::traits::is_integral));
```

到这里类型计算部分就结束了。 虽然这种用于类型编程的新范例可能很难在最初使用，但随着使用它越来越有意义。 你也会来欣赏它如何模糊类型和值之间的界限，开辟新的令人兴奋的可能性和简化许多任务。

# 内省

## 表达式有效性检查

### 非静态数值

### 静态数值

### 嵌套类型名

### 嵌套模板

## SFINAE 控制

## 内省用户定义类型

## 示例：生成JSON

# 容器的一般性操作

## 创建容器

## 容器类型

## 容器元素

# 算法的一般性操作

## 传值

## (非)惰性

## 生成什么？

## 副作用和纯度

## 正交算法

# 与外部库集成

# Hana 核心

## Tag

## Tag 分发

## 模拟C++约束

# 头文件组织

# 结语

## Fair warning: functional programming ahead

# 使用此参考

## 函数签名

# 致谢

# 词汇表

# 快速问答

# 附录一：高级constexpr

## Constexpr剥离

## Constexpr保存

## 副作用

# 附录二：一个Mini MPL库

# 参考文档

# 按字母顺序索引

# 头文件

# TODO 列表

# Deprecated 列表

# Bug 列表




