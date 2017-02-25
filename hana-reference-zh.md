# Hana参考文档

如用户手册中所述,参考文档按逻辑模块进行了区分.

|                       |                          |                            |
|:--------------------:|:------------------------:|:----------------------------|
|     Concepts          |                          | 由库提供的（约束语义）Concept            |
|                       | Applicative              | 表示具有提升值和组合计算的能力的函数  |
|                       | Comonad                  | 表示上下文相关的计算和数值                |
|                       | Comparable               | 表示相等和不相等的比较                    |
|                       | Constant                 | 表示数据需要在编译时可操作                 |
|                       | Euclidean Ring           | 表示也可以被赋予分割算法的交换“环”          |
|                       | Foldable                 | 表示该数据结构可被推导为单个值              |
|                       | Functor                  | 表示可映射类型                          |
|                       | Group                    |                                       |
|                       | Hashable                 |                                       |
|                       | Iterable                 |                                       |
|                       | Logical                  |                                       |
|                       | Metafunction             | 表示一个函数，它接收hana::type作为输入，并输出一个hana::type      |
|                       | Monad                    |                                       |
|                       | MonadPlus                |                                       |
|                       | Orderable                | 表示可排序的序列                        |
|                       | Product                  | 表示有2个类型元素的容器                   |
|                       | Ring                     |                                       |
|                       | Searchable               | 表示可进行查找操作的结构                  |
|                       | Sequence                 | 表示基于索引的序列                      |
|                       | Struct                   | 表示类struct的用户自定义类型             |
|     Data types        |                          | 库提供的通用数据类型                      |
|     Functional        |                          | 通用函数对象                             |
|     Core              |                          | 库的核心功能                               |
| Experimental features |                          | 实验性功能，可能或不会选入库。 这些功能是不稳定的 |
| External adapters     |                          | 外部库适配器                               |
|                       | Boost.Fusion adapters    | Boost.Fusion适配器                      |
|                       | Boost.MPL adapters       | Boost.MPL适配器                  |
|                       | Other Boost adapters     | 其它Boost适配器                          |
|                       | Standard library adapters| 标准库适配器                            |
| Configuration options |                          | 配置选项，用来调整库的全局行为                  |
|     Assertions        |                          | 宏，用于执行不同各类的断言                      |
|     Details           |                          | 实现细节，即使它已经文档化了，也不要依赖于此       |


## Concepts

Hana库支持的约束

>* 模块
>
>1. Applicative
>1. Conmonad
>1. Comparable
>1. Constant
>1. Euclidean Ring
>1. Foldable
>1. Functor
>1. Group
>1. Hashable
>1. Iterable
>1. Logical
>1. Metafunction
>1. Monad
>1. MonadPlus
>1. Monoid
>1. Orderable
>1. Product
>1. Ring
>1. Searchable
>1. Struct
>
>* 类
>
>`boost::hana::IntegralConstant<C>`

### Applicative

#### 说明

Applicative约束表示具有提升值和组合计算能力的Functor。

`Functor`只能采用普通函数并将其映射到包含值的结构上，以获取包含值的新结构。 `Applicative`更直观地取值并将其提升到结构中。 另外，`Applicative`可以采用包含函数的结构，并将其应用于包含值的结构以获得包含值的新结构。 通过在结构内部调用函数，可以将`n`元(`n-ary`)函数应用于包含值的`n`个结构。

>* 注意
>
>本文档没有详细介绍`Applicative`的本质。 但是，[Typeclassopedia](https://wiki.haskell.org/Typeclassopedia#Applicative)是一个很好的`Haskell-oriented`的资源，可以找到这样的信息。

##### 最小完全定义

`lift`和`ap`满足以下规则。一个`Applicative`必须是一个`Functor`。

##### 规则

给定一个`Applicative`--`F`,它必须满足以下规则：

>1. 特性
>
>    对于所有`xs`对象的`tag` `F(A)`,有：
>
>```c++
>ap(lift<F>(id),xs) == xs
>```
>
>1. 组成
>
>    对于所有`xs`对象的`tag`--`F(A)`及一个`Applicative`中的`function`,有：fs:F(B -> C),gs:F(A -> B),
>
>```c++
>ap(ap(lift<F>(compose),fs,gs),xs) == ap(fs,ap(gs,xs))
>```
>
>1. 同形
>
>    对于所有`x`对象的`tag`--`A`及`function`,有：f:A -> B,
>
>```c++
>ap(ap(lift<F>(f),lift<F>(f(x)) == lift<F>(f(x))
>```
>
>1. 互换
>
>    对于所有`x`对象的`tag`--`A`及一个`Applicative`中的`function`,有：fs:F(A -> B),
>
>```c++
>ap(fs,lift<F>(x)) == ap(lift<F>(apply(-,x)),fs)
>```
>其中`apply(-,x)`表示`apply`函数从[Functional](http://boostorg.github.io/hana/group__group-functional.html)模块到`x`参数的部分应用。

作为这些规则的结果，`F`的`Functor`的模型将满足所有对象`xs`的标签`F(A)`和函数：`f:A -> B`：

```c++
transform(xs,f) == ap(lift<F>(f),xs)
```

##### 精练

>1. Fuctor (自由模式)
>
>    作为规则的结果，任何`Applicative`--`F`可以通过设置作为`Functor`
>
>```c++
>transform(xs,f) = ap(lift<F>(f),xs)
>```
>

##### 具体模型

hana::lazy,hana::optional,hana::tuple

##### 结构保留function

#### Variables

#### Variable 文档