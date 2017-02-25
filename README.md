# Boost.Hana-zh <a target="_blank" href="http://semver.org">![Version][badge.version]</a> <a target="_blank" href="https://travis-ci.org/boostorg/hana">![Travis status][badge.Travis]</a> <a target="_blank" href="https://ci.appveyor.com/project/ldionne/hana">![Appveyor status][badge.Appveyor]</a> <a target="_blank" href="http://melpon.org/wandbox/permlink/g4ozIK33ITDtyGa3">![Try it online][badge.wandbox]</a> <a target="_blank" href="https://gitter.im/boostorg/hana">![Gitter Chat][badge.Gitter]</a>

[Boost.Hana](https://github.com/boostorg/hana) 中文文档，翻译自 <http://boostorg.github.io/hana/>
版本: Boost.Hana 1.0.2

## **元编程标准库中文文档**

* Hana 是一个元编程库，提供异构容器和算法，用于类型和值的计算。用户手册在[这里](./hana-zh.md)

  * [起步](./hana-zh.md/#起步)
  * [先决条件和安装](./hana-zh.md/#先决条件和安装)
  * [技术支持](./hana-zh.md/#技术支持)
  * [简介](./hana-zh.md/#简介)
  * [快速入门](./hana-zh.md/#快速入门)
  * [备忘表](./hana-zh.md/#备忘表)
  * [断言](./hana-zh.md/#断言)
  * [编译期数值](./hana-zh.md/#编译期数值)
  * [类型计算](./hana-zh.md/#类型计算)
  * [内省](./hana-zh.md/#内省)
  * [容器的一般性操作](./hana-zh.md/#容器的一般性操作)
  * [算法的一般性操作](./hana-zh.md/#算法的一般性操作)
  * [性能注意事项](./hana-zh.md/#性能注意事项)
  * [与外部库集成](./hana-zh.md/#与外部库集成)
  * [Hana内核](./hana-zh.md/#hana内核)
  * [头文件的组织结构](./hana-zh.md/#头文件的组织结构)
  * [结束语](./hana-zh.md/#结束语)
  * [使用本参考](./hana-zh.md/#使用本参考)
  * [致谢](./hana-zh.md/#致谢)
  * [词汇表](./hana-zh.md/#词汇表)
  * [快速问答](./hana-zh.md/#快速问答)
  * [附录一：高级constexpr](./hana-zh.md/#附录一高级constexpr)
  * [附录二：一个MiniMPL库](./hana-zh.md/#附录二一个minimpl库)
  * [参考文档](./hana-reference-zh.md)
  * [按字母顺序索引](./hana-zh.md/#按字母顺序索引)
  * [头文件](./hana-header-zh.md)
  * [TODO列表](./hana-zh.md/#todo列表)
  * [Deprecated列表](./hana-zh.md/#deprecated列表)
  * [Bug列表](./hana-zh.md/#bug列表)

* Hana 提供了异构容器和算法，合并了异构计算和编译期计算。参考文档在[这里](./hana-reference-zh.md)

* Hana 是一个头文件库，利用C++14技术和惯用法编写，不依赖于其它库。头文件文档在[这里](./hana-header-zh.md)

* 应用Hana库是很容易的，它背后的构建与技术也可能有必要加以了解。这些想法在[这里](./hana-misc.md)

快速访问：<https://coding.net/u/freezestudio/p/Boost.Hana-zh/git>

## 概览

<!-- Important: keep this in sync with example/overview.cpp -->

```cpp
#include <boost/hana.hpp>
#include <cassert>
#include <string>
namespace hana = boost::hana;
using namespace hana::literals;

struct Fish { std::string name; };
struct Cat  { std::string name; };
struct Dog  { std::string name; };

int main() {
  // 持有异构对象的序列和操纵它们的算法。
  auto animals = hana::make_tuple(Fish{"Nemo"}, Cat{"Garfield"}, Dog{"Snoopy"});
  auto names = hana::transform(animals, [](auto a) {
    return a.name;
  });
  assert(hana::reverse(names) == hana::make_tuple("Snoopy", "Garfield", "Nemo"));

  // `animals`承载的字符串不是常量表达式，即便如此，因为长度是`constexpr`的，故此不会丢失编译时信息。
  static_assert(hana::length(animals) == 3u, "");

  //类型的计算可以使用与普通C++相同的语法来执行。 不管相信与否，一切都在编译时完成。
  auto animal_types = hana::make_tuple(hana::type_c<Fish*>, hana::type_c<Cat&>, hana::type_c<Dog*>);
  auto animal_ptrs = hana::filter(animal_types, [](auto a) {
    return hana::traits::is_pointer(a);
  });
  static_assert(animal_ptrs == hana::make_tuple(hana::type_c<Fish*>, hana::type_c<Dog*>), "");

  // 还有更多可轻松获取的好处，包括：
  // 1. 访问元组的语法更清晰
  static_assert(animal_ptrs[0_c] == hana::type_c<Fish*>, "");
  static_assert(animal_ptrs[1_c] == hana::type_c<Dog*>, "");

  // 2. 很轻松地在编译时展开循环
  std::string s;
  hana::int_c<10>.times([&]{ s += "x"; });
  // 相当于 s += "x"; s += "x"; ... s += "x";

  // 3. 检查表达式的有效性也很方便
  //    原先为此通常需要基于复杂的SFINAE技巧才行
  auto has_name = hana::is_valid([](auto&& x) -> decltype((void)x.name) { });
  static_assert(has_name(animals[0_c]), "");
  static_assert(!has_name(1), "");
}
```

## 文档

可以在线浏览 <http://boostorg.github.io/hana>。也可以从`gh-pages`分支检出一份离线文档。为避免覆盖当前目录，最好clone`gh-pages`分支到一个像`doc/html`这样的一个子目录中：

```shell
git clone http://github.com/boostorg/hana --branch=gh-pages --depth=1 doc/html
```

发布后，`doc/html`将包含与[可在线使用] [Hana.docs]的完全相同的静态网站。 注意，`doc/html`会自动被Git忽略，所以更新文档不会搞乱你的索引。


## 独立使用Hana

很容易将Hana设置为只为你所用。首先，需要安装[CMake][]。而后，切换(`cd`)到项目根目录并新建`build`子目录：

```shell
mkdir build
cd build
cmake ..
```

通常，系统自带的编译器过于陈旧，你可能需要指定的编译器：

```shell
cmake .. -DCMAKE_CXX_COMPILER=/path/to/compiler
```

一般来说，这样就能很好地工作了。但是，在一些系统上，标准库和/或编译器默认不支持`C++14`。这样的话查看[wiki][Hana.wiki]，那里有更多针对不同系统的设置信息。

通常，`Hana`会尝试在你的系统上查找`Boost`头文件，没有`Boost`头文件`Hana`仍然能够工作，只不过将会禁用少数几个依赖于`Boost`的测试。但是如果你想要`Hana`使用自定义安装的`Boost`,你需要指定自定义安装`Boost`的位置：

```shell
cmake .. -DCMAKE_CXX_COMPILER=/path/to/compiler -DBOOST_ROOT=/path/to/boost
```

现在可以构建与运行单元测试及示例了：

```shell
cmake --build . --target check
```

先说清楚，编译单元测试是相耗时的也需要大量的内存占用，特别是对外部适配器的测试。 这是由于`Hana`的单元测试是非常彻底的，而且其他库中的异构序列往往具有更巨量的编译消耗。

一些可选的目标仅在你的系统上安装有相应的软件才会启用。比如，生成文件需要系统安装了[Doxygen][]。`CMake`在生成期间会打印出哪些目标没有启用。你可以根据这些信息安装相应的软件后再重新`CMake`。

> ### 提示
> 用 `help` 选项可列出需要的软件。

若想添加单元测试或者示例，将源文件添加到`test/`或`example/`目录下重新CMake即可。假设相对路径从项目到新源文件的根目录是“path/to/file.cpp”。 重新运行CMake生成步骤后，将会创建一个名为`path.to.file'的新目标，并且还将创建相同名称的测试。像这样，

```shell
cd build # Go back to the build directory
cmake --build . --target path.to.file # Builds the program associated to path/to/file.cpp
ctest -R path.to.file # Runs the program as a test
```

> ### 给`Sublime Text`用户的提示
> 使用此[hana.sublime-project](hana.sublime-project)支持文件,选择"[Hana] Build current file" 构建系统，当看到目标被关联到(like a test or an example), 按 ⌘B键编译, 或者按⇧⌘B键编译并运行。


## 项目结构

项目有几个子目录。

* [benchmark](benchmark)目录存放编译时和运行时的基准测试内容，以确保库的速度与所说的那快。 基准测试中的代码主要以[eRuby][]模板的形式编写。 模板用于生成`C++`文件，以进行编译编译和执行统计。
* [cmake](cmake)目录存放用于编译系统所用的CMake模块及其它脚本。
* [doc](doc)目录存放生成文件所需的配置文件。Git会自动忽略`doc/html`子目录；如上所述，您可以方便地将`gh-pages`分支克隆到该目录以存储本地文档副本。
* [example](example)目录存放所有教程与参考示例的源代码。
* [experimental](experimental)目录存放的实验性功能有可能在适当的时候加入到库中。
* [include](include)目录存放库的源代码，它们只有头文件。
* [test](test)目录存放单元测试源代码。

## 相关资料

* [CppCon][] 2015 有关元编程和Hana的讨论 ([幻灯片](http://ldionne.com/hana-cppcon-2015)/[视频](https://youtu.be/cg1wOINjV9U))
* [C++Now][] 2015  有关元编程和Hana的讨论 ([幻灯片](http://ldionne.com/hana-cppnow-2015))
* [CppCon][] 2014 有关Hana的讨论 ([幻灯片](http://ldionne.com/hana-cppcon-2014)/[视频](https://youtu.be/L2SktfaJPuU))
* [MPL11][] 库，Hana的起源
* [C++Now][] 2014 有关MPL11的讨论 ([幻灯片](http://ldionne.com/mpl11-cppnow-2014)/[视频](https://youtu.be/8c0aWLuEO0Y))
* Louis Dionne的学士学位论文经由类别来形式化C++元编程(a formalization of C++ metaprogramming through
  category)理论。 论文在[这里](https://github.com/ldionne/hana-thesis/blob/gh-pages/main.pdf),
  相关演示文稿的幻灯片在[这里](http://ldionne.com/hana-thesis)。很不幸，都是法语的。


## 贡献

参见[CONTRIBUTING.md](https://github.com/boostorg/hana/blob/master/CONTRIBUTING.md).

## 许可

参见[LICENSE.md](https://github.com/boostorg/hana/blob/master/LICENSE.md).


## 发布

本节所述仅与Hana开发者有关，记录创建新版本的几个简单步骤。为发布一个新版本，使用`git tag -a`创建新标签，在`include/boost/hana/version.hpp`中改变相应的版本号。

<!-- Links -->
[badge.Appveyor]: https://ci.appveyor.com/api/projects/status/github/boostorg/hana?svg=true&branch=master
[badge.Gitter]: https://img.shields.io/badge/gitter-join%20chat-blue.svg
[badge.Travis]: https://travis-ci.org/boostorg/hana.svg?branch=master
[badge.version]: https://badge.fury.io/gh/boostorg%2Fhana.svg
[badge.Wandbox]: https://img.shields.io/badge/try%20it-online-blue.svg
[C++Now]: http://cppnow.org
[CMake]: http://www.cmake.org
[CppCon]: http://cppcon.org
[Doxygen]: http://www.doxygen.org
[eRuby]: http://en.wikipedia.org/wiki/ERuby
[Hana.docs]: http://boostorg.github.io/hana
[Hana.wiki]: https://github.com/boostorg/hana/wiki
[MPL11]: http://github.com/ldionne/mpl11