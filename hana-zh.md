Boost.Hana 用户手册
------------------

# 起步

Hana 是一个头文件库,用于计算类型和值的C++元编程套件.它提供的功能是Boost.MPL和Boost.Fusion这两个库的超集.
利用C++11/14实现技术和惯用法,以前的库相比,Hana拥有更快的编译速度和运行时性能,同时也显著提高了编程体验.
Hana很容易以ad-hoc方式扩展，它提供了与Boost.Fusion，Boost.MPL和标准库的开箱即用的交互操作.

# 先决条件和安装

Hana是一个没有外部依赖的头文件库(甚至不依赖Boost).因此,在您自己的项目中使用Hana非常容易.基本上，只需添加
`include/`目录到你的编译器的头文件搜索路径即可.然而，如果你想在你的系统上干净地安装Hana，你有几个选项.
首先，您可以安装Boost 1.61.0或更高版本，因为从该版本开始，Hana包含在Boost中.如果你不想安装所有的Boost，也可
以只安装Hana.为此，您可以从官方[GitHub存储库](https://github.com/boostorg/hana)下载代码，并通过从项目根目
录发出以下命令来手动安装库(需要[CMake](http://www.cmake.org/)).

``` bat
> mkdir build && cd build
> cmake ..
> cmake --build . --target install
```

这样,将在您的平台上安装Hana到缺省的安装目录(Unix下为`/usr/local`,Windows下为`C:\\Program Files`).如果想要
安装到指定位置,可以这样做:

``` bat
> cmake .. -DCMAKE_INSTALL_PREFIX=/custom/install/prefix 
```

**注意**
* 手动安装还将安装一个与[pkg-config](http://www.freedesktop.org/wiki/Software/pkg-config/)一起使用的
`hana.pc`文件.
* 如果您已经安装了Boost，请不要再安装Hana，因为新安装将覆盖Boost附带的安装.

如果在项目中使用CMake，可以使用Hana提供的
[FindHana.cmake](https://github.com/boostorg/hana/blob/master/cmake/FindHana.cmake)模块将Hana设置为外部CMake项目.
该模块还允许将Hana本地安装到该项目，而无需按照上述说明在系统上安装Hana.最后，如果你想对Hana做出贡献，你可以看到如何在
[README](https://github.com/boostorg/hana/blob/master/README.md#hacking-on-hana)中更好地设置你的开发环境.

## 编译器要求

Hana依赖于C++14编译器和标准库,除此之外再无要求.以下为当前支持Hana的C++14编译器/工具链列表:

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

如果您有什么问题,请查看[FAQ]()及[wiki](https://github.com/boostorg/hana/wiki).
或者查找[问题列表](https://github.com/boostorg/hana/issues)也是个不错的主意.
如果这些没有解决您的问题,您可以随时与我们在[Gitter](https://gitter.im/boostorg/hana)聊天,
或者提交一个新的issue.[StackOverflow](http://stackoverflow.com/)的
[boost-hana](http://stackoverflow.com/questions/tagged/boost-hana)标签处是提出问题的首选.
如果您发现了bug,请提交issue.

# 简介

自Boost.MPL首发以来,它通过提供大量的模板接口为C++程序员进行元编程提供了便利.这个突破极大地促进
了C++模板元编程的主流化,如今模板元编程已经深深扎根于各种项目中.最近以来,C++11和C++14对语言带来
了许多重大变化,其中一些使元编程更加容易,其它一些也大大拓宽了库的设计空间.因此,一些问题自然而来:
是否仍然希望有元编程的抽象?如果是,有哪些?在考察了不同选择,如MPL11之后,最终答案是Hana库.对Hana的
关键洞察是,类型和值的计算是一体两面的.通过统一这两个概念,元编程变得更为容易,新的令人兴奋的可能出现
在我们面前.

## C++计算象限

但是要真正了解Hana是什么，有必要理解C++中的不同类型的计算. 先不管可能的更细粒度的区分,我们将把注意
力集中在四种不同类型的计算上. 首先，是运行时计算，这是我们在C++中使用的通常的计算.在运行时世界中，
我们有运行时容器，运行时函数和运行时算法：

```C++
auto f=[](int i)->std::string {
    return std::to_string(i*i);
};

std::vector<int> ints{1,2,3,4};
std::vector<std::string> strings;
std::transform(ints.begin(),ints.end(),std::back_inserter(strings),f);

assert((string==std::vector<std::string>{"1","4","9","16"}));
```
此象限的计算,通常以C++标准库为工具,C++标准库提供运行时可重用的算法和容器.
自C++11以来,第二种计算成为可能:`constexpr`计算.此种计算,我们用`constexpr`
容器,`constexpr`函数及`constexpr`算法:

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

若使以上代码可执行,需要确保std::array的operator==操作符标记为`constexpr`,
在C++14下,这不是问题.

基本上,`constexpr`计算与运行时计算的不同之处在于它足够简单,可以被编译器
解析执行.一般来说,任何不对编译器的求值程序过于不友好的函数(像抛出异常或者
分配内存等),都可以标记为`constexpr`,而无需作出修改.`constexpr`计算与运行时
计算类似,除了`constexpr`计算更受限制,并需要获得编译时执行的能力.不幸的是,没有
常用于`constexpr`计算的工具框,即没有广泛采用的用于`constexpr`编程的标准库.也许,
对`constexpr`编程感兴趣的人可以去了解一下[Sprout](https://github.com/bolero-MURAKAMI/Sprout)库.

第三种计算是异构计算. 异构计算不同于平常计算，因为不是使用存储同类对象（所有对象具有相同类型）容器,
异构容器可以保存具有不同类型的对象. 此外，在这个计算象限中的函数是异构函数，这是一种讨论模板函数的复杂方式.
 类似地，我们有异构算法操作异构容器和函数：

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

如果你觉得操作异构容器很奇怪的话,不妨把它相像成操作`std::tuple`.在C++03的
世界中,用于进行此计算的库是[Boost.Fusion](http://www.boost.org/doc/libs/release/libs/fusion/doc/html/index.html),
它提供了几个操作异构数据的结构和算法的集合.
我们将考察的第四个计算象限的计算是类型计算.在这个象限中,我们有类型级容器,
类型级函数(通常称为元函数)和类型级算法.在此,任何类型操作:容器存储类型和元函数
接受类型作为参数并返回类型结果.

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

类型计算的领域已经被相当广泛地探索了，并且C++03中类型计算的事实解决方案是一个名为[Boost.MPL](http://www.boost.org/doc/libs/release/libs/mpl/doc/index.html)的库，它提供类型容器和算法.
对于低级类型转换，C++11中也可以使用<type_traits>标准头提供的元函数。

## Hana库是干什么的?

以上所有计算都作都很好了,那么,Hana库又是干什么的?现在我们已经知道了C++的各种计算类型,回答这个问题可以会很简单了.
**Hana的目的是合并第三和第四象限的计算**,具体来说,Hana是一个经过长期构建而证明出,异构计算比类型计算更强大,因此我们可以
通过等效的异构计算来表达任何类型计算.这种构造在两个步骤中完成.
首先,Hana是一个功能齐全的异构算法和容器库,有点像现代化的Boost.Fusion.
其次,Hana提供了一种将任何类型计算转换为其等效的异构计算的方法.这允许异构计算的全部机制被重用于类型计算,而没有任何代码重复.
当然,这种统一的最大优点是用户能看到的.

# 快速起步






