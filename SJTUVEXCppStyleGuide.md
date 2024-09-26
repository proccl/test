# SJTUVEXCppStyleGuide

SJTU VEX C++代码风格指南

参考 Google C++ Style Guide，并作了一定修改与简化

https://google.github.io/styleguide/cppguide.html



[TOC]

## 0. Background

VEX Code基于C++

VEX Code V5官方文档 https://api.vexcode.cloud/v5/

所有队员上传代码前需自查是否符合此代码风格文件要求



## 1. C++ Version

目前，代码应该基于C++17，即不应使用C++2x的特性。

在使用C++14和C++17特性之前，考虑对于其他环境的可迁移性。



## 2. 头文件

通常来说,每个`.cpp`文件都应关联到一个`.h`头文件。

正确的使用头文件能够让你代码的可读性。

### 2.1. 独立的头文件

头文件应该是独立的（可被单独编译）并命名为`.h`文件。

所有的头文件应该是独立的。用户和重构工具不必遵循特定条件以包含这些头文件，即一个头文件应该有**头文件保护**(head guard)并且包含所有其他所需的头文件。

### 2.2. `#define`保护

所有的头文件应有`#define`保护以避免多次include。命名格式应为`<PROJECT>_<PATH>_<FILE>_H_`

为了保证唯一性，这一保护相应基于项目结构树的完整路径。例如在`foo`项目中的`foo/include/bar.h`文件应该有下述保护行:

```C++
#ifndef FOO_BAR_H_
#define FOO_BAR_H_

...

#endif  // FOO_BAR_H_
```

### 2.3. 仅包含使用的头文件

如果一个源文件或者头文件引用了其他地方定义的符号，那么这一文件就应直接包含提供该符号声明或定义的头文件。除此之外不应以任何理由包含头文件。

即：**不要include没有使用的头文件**。

### 2.4. 前向声明(Forward Declarations)

如果可能的话，避免使用前向声明

**定义:**

前向声明是声明一个实体而不关联其定义

```C++
// In a C++ source file:
class B;
void FuncInB();
extern int variable_in_b;
ABSL_DECLARE_FLAG(flag_in_b);
```

### 2.5. 内联函数(inline functions)

只在内联函数很小，也就是10行以内时才定义内联函数。

如果你不知道这是什么，或者不知道某处是否应该使用内联函数，不要使用内联函数。

### 2.6. 包含的名称和顺序

请按照下述顺序包含头文件:

+ 相关头文件
+ C系统头文件
+ C++标准库头文件
+ 其他库文件
+ 你的项目库头文件

所有的项目头文件应该项目根目录的子目录而非使用目录别名`.`(当前目录)或者`..`(上级目录).例如`google-awesome-project/src/base/logging.h`应当以下述方式被包含:

```C++
#include "base/logging.h"
```

在`dir/foo.cc`或`dir/foo_test.cc`(用于实现或者测试`dir2/foo2.h`中内容)中,包含目录应如下排序:

+ `dir2/foo2.h`
+ 一行空行
+ C系统头文件(更具体而言:带`<>`的`.h`拓展) 例如`<unistd.h>`,`<stdlib.h>`.
+ 一行空行
+ C++标准库头文件(无文件拓展名)例如`<algorithm>`,`<cstddef>`.
+ 一行空行
+ 其他库的`.h`头文件
+ 你项目的`.h`头文件

例如,`google-awesome-project/src/foo/internal/fooserver.cc`的包含列表应如下:

```C++
#include "foo/server/fooserver.h"

#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/commandlineflags.h"
#include "foo/server/bar.h"
```

**例外情况:**

有时,指定系统的代码需要条件化包含.这样的代码可能会在其他包含项后添加条件包含.当然请确保你指定系统的代码小而且本地化,例如:

```C++
#include "foo/public/fooserver.h"

#include "base/port.h"  // For LANG_CXX11.

#ifdef LANG_CXX11
#include <initializer_list>
#endif  // LANG_CXX11
```

## 3. 作用域

### 3.1. 局部变量

将函数的变量限制在尽可能小的作用域中,并在声明中初始化变量.

C++允许你在函数的任何地方声明变量.我们建议您尽可能在局部范围内声明这些变量,尽可能靠近首次使用它的位置.这会让使用者更容易找到声明并发现这个变量的类型是什么以及被初始化成了什么.尤其是应该使用初始化而不是声明再赋值,例如:

```C++
int i;
i = f();      // Bad -- initialization separate from declaration.
```

```C++
int j = g();  // Good -- declaration has initialization.
```

```C++
std::vector<int> v;
v.push_back(1);  // Prefer initializing using brace initialization.
v.push_back(2);
```

```C++
std::vector<int> v = {1, 2};  // Good -- v starts initialized.
```

`if`,`while`和`for`语句等所需的变量通常在这些语句中声明,所有这些变量被限制在这些作用域内,例如:

```C++
while (const char* p = strchr(str, '/')) str = p + 1;
```

有一个需要注意的事情:如果一个变量是一个对象,它的构造器每次它进入作用域和被创建时都被调用,而析构器则在其每次离开作用于都被调用.

```C++
// Inefficient implementation:
for (int i = 0; i < 1000000; ++i) {
  Foo f;  // My ctor and dtor get called 1000000 times each.
  f.DoSomething(i);
}
```

```C++
Foo f;  // My ctor and dtor get called once each.
for (int i = 0; i < 1000000; ++i) {
  f.DoSomething(i);
}
```

### 3.2. 静态和全局变量

除非一个对象是**可平凡析构的(trivially destructible)**，否则不要将其声明为**静态或全局对象**。

**可平凡析构的(trivially destructible)**，简单来说即用户没有自定义析构函数或者虚拟析构函数，可以自动析构的对象。

#### 关于析构的建议

当析构器比较琐碎时,这些析构器的执行完全不受顺序限制(它们实际上是不"运行"的).否则我们就会面临在对象的生命周期结束后访问对象的风险.因此我们只允许可平凡析构的有静态存储周期的对象.基础类型(例如指针和`int`)是平凡析构的,而可平凡析构类型的数组也是一样.注意标明`constexpr`的变量也是可平凡析构的.

```C++
const int kNum = 10;  // allowed

struct X { int n; };
const X kX[] = {{1}, {2}, {3}};  // allowed

void foo() {
  static const char* const kMessages[] = {"hello", "world"};  // allowed
}

// allowed: constexpr guarantees trivial destructor
constexpr std::array<int, 3> kArray = {{1, 2, 3}};
```

```C++
// bad: non-trivial destructor
const std::string kFoo = "foo";

// bad for the same reason, even though kBar is a reference (the
// rule also applies to lifetime-extended temporary objects)
const std::string& kBar = StrCat("a", "b", "c");

void bar() {
  // bad: non-trivial destructor
  static std::map<int, int> kData = {{1, 0}, {2, 0}, {3, 0}};
}
```

注意引用不是对象,因此他们不收析构性限制.但是动态初始化的限制仍然存在特卫视一个形如`staticT& t = *new T`的函数局部静态引用是允许的.

#### 关于初始化的建议

初始化是一个更复杂的话题.因为我们不惜不只是考虑到一个类构造器是否执行,也需要考虑初始化器的执行:

```C++
int n = 5;    // fine
int m = f();  // ? (depends on f)
Foo x;        // ? (depends on Foo::Foo)
Bar y = g();  // ? (depends on g and on Bar::Bar)
```

除了首条语句以外,我们都面临着不确定的初始化顺序.

我们所寻求的概念在正式的C++语言标准中称为*常量初始化*.这意味着初始化表达式是一个常量表达式,而且如果对象是通过构造器调用初始化的,那么构造器也必须指定为`constexpr`:

```C++
struct Foo { constexpr Foo(int) {} };

int n = 5;  // fine, 5 is a constant expression
Foo x(2);   // fine, 2 is a constant expression and the chosen constructor is constexpr
Foo a[] = { Foo(1), Foo(2), Foo(3) };  // fine
```

常量初始化总是允许的.有静态存储周期的变量的常量初始化应该被标为`constexpr`或者在有`[ABSL_CONST_INIT](https://github.com/abseil/abseil-cpp/blob/03c1513538584f4a04d666be5eb469e3979febba/absl/base/attributes.h#L540)`属性的地方.任何未按上述要求标明的非局部的静态存储周期变量都应假定为动态初始化,并且仔细检查.

相比之下呢,下述初始化是有问题的:

```C++
// Some declarations used below.
time_t time(time_t*);      // not constexpr!
int f();                   // not constexpr!
struct Bar { Bar() {} };

// Problematic initializations.
time_t m = time(nullptr);  // initializing expression not a constant expression
Foo y(f());                // ditto
Bar b;                     // chosen constructor Bar::Bar() not constexpr
```

不鼓励使用非局部变量的动态初始化,通常情况下是禁止这样的.但我们也在程序的任一方面都不依赖于此初始化相对于其他初始化的顺序时允许这种行为.在这些限制条件下,初始化的顺序不会有明显差别.例如:

```C++
int p = getpid();  // allowed, as long as no other static variable
                   // uses p in its own initialization
```

静态局部变量的动态初始化是允许(且普遍的).

#### 普遍模式

+ 全局字符串: 如果你需要一个全局或静态字符串常量,考虑使用简单字符串数组或者指向字符串文本首字符元素的指针.字符串文本具有静态存储周期已经也通常来说足够了.
+ 映射,集合和其他动态容器: 如果你需要一个静态的,固定的容器,例如一个一个用于搜索或查找表的集合,则不能(cannot)使用标准库中的动态容器作为静态变量.因为他们有非平凡析构器.相对的,考虑平凡类型的简单数组例如一个整数数组的数组(对于整数到整数的映射).或者一个`pair`对的数组(例如由`int`和`const char*`的对组成).对于小容器,线性搜索效率已经足够(而且由于内存局部性的原因,效率很高).请考虑使用[absl/algorithm/container.h](https://github.com/abseil/abseil-cpp/blob/master/absl/algorithm/container.h)中的工具用于标准操作.如果必要的话,保持容器有序并使用二分搜索.如果你真的需要使用标准库中的动态容器.考虑使用函数局部景泰直镇,如下文所述.
+ 智能指针(`unique_ptr`和`shared_ptr`): 智能指针在析构期间执行清理故而禁止使用.考虑你使用的清晰是否符合本节所属的其它模式.一个简单的解决方案是使用指向动态分配对象的普通指针并且永不删除它.(参见后述条目).
+ 自定义类型的静态变量: 如果你需要静态的某一个你需要自定义的类的常量数据,为该类添加一个平凡析构器和`constexpr`构造器.
+ 如果所有其他方法都不行,那么你可以通过使用函数局部静态指针或引用(例如`static const auto& impl = *new T(args...);`)来动态创建一个对象并且从不删除它.



## 4. 类(class)



## 5. 命名



### 5.1. 基本命名规则

使用**描述性英文**命名。不要使用拼音。不要使用**非常用缩写**。保持命名的清晰易懂。

对于变量的具体命名应取决于其作用域。例如变量`n`在一个只有几行的函数里是一个很好的命名，在一个很大的类里就是一个不够清晰的糟糕命名。



### 5.2. 类型名

类型名采用**大驼峰命名法**，即首字母大写，之后每个单词首字母大写，无下划线分割。例如`MyExcitingClass`, `MyExcitingEnum`。

```c++
// classes and structs
class UrlTable { ...
class UrlTableTester { ...
struct UrlTableProperties { ...

// typedefs
typedef hash_map<UrlTableProperties *, std::string> PropertiesMap;

// using aliases
using PropertiesMap = hash_map<UrlTableProperties *, std::string>;

// enums
enum UrlTableError { ...
```



### 5.3. 变量名

变量名统一采用蛇形命名法(`snake_case`)，即全小写，单词间下划线分割。



### 5.4. 常量名

所有定义为`const`或`constexpr`的常量遵循此命名规则。命名方式为以`k`起始的混合命名法，即以`k`为首字母的小驼峰。特别的，对于无法驼峰分割的可以使用下划线分割。

例如：

```c++
const int kDaysInAWeek = 7;
const int kAndroid8_0_0 = 24;  // Android 8.0.0
```



### 5.5. 函数名

采用小驼峰

### 5.6. 枚举名

枚举（内部）名称应采用与常量相同的命名方式，以避免与宏定义冲突。

例如：

```c++
enum class UrlTableError {
	kOk = 0,
	kOutOfMemory,
	kMalformedInput,
};
```



### 5.7. 宏

**不要使用宏定义**

除COMPETITION和DEBUG等外。



## 6. 代码格式



### 6.1. 行长度

每行长度尽量不要超过80个字符。

若函数头太长则进行换行缩进，例如

```c++
ReturnType LongClassName::ReallyReallyReallyLongFunctionName(
    Type par_name1,  // 4 space indent
    Type par_name2,
    Type par_name3) {
  	DoSomething();
  	...
}

bool result = DoSomething(averyveryveryverylongargument1,
                          argument2, argument3); // aligned with the first argument
```

### 6.2. 非ASCII字符

不要使用任何非ASCII字符。



### 6.3. 空格与Tabs缩进

仅使用空格缩进。使用4个空格作为一个缩进。

不要在代码中使用Tabs (`\t`)作为缩进。在你的代码编辑器中将Tab键设置为空格缩进。



