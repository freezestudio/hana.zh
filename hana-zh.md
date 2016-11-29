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

如果在项目中使用CMake，可以使用Hana提供的[FindHana.cmake](https://github.com/boostorg/hana/blob/master/cmake/FindHana.cmake)模块将Hana设置为外部CMake项目.
该模块还允许将Hana本地安装到该项目，而无需按照上述说明在系统上安装Hana.最后，如果你想对Hana做出贡献，你可以看到如何在[README](https://github.com/boostorg/hana/blob/master/README.md#hacking-on-hana)中更好地设置你的开发环境.

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
或者提交一个新的issue.[StackOverflow]()的[boost-hana]()标签处是提出问题的首选.
如果您发现了bug,请提交issue.

# 简介
