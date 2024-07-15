# 浅谈QString(2024新版)

[TOC]



## 1. 简介

- `QString`是Qt中的一个类，用于存储字符串，且没有父类。
- 存储字符串的每一个字符是一个`QChar`类型，`QChar`使用的是`UTF-16`编码，也就是每一个字符占两字节`2B`，`16bit`。
- `UTF-16`是一种`Unicode`编码，可以表示汉字，汉字在`QString`中是一个字符、一个`QChar`。
- 对于编码值大于*65535*（`16bit`）的字符集，`Unicode`会使用代理编码对(*surrogate pairs*)来储存，例如用两个`QChars`表示[古希腊文字](https://en.wikipedia.org/wiki/Ancient_Greek_Numbers_(Unicode_block))或补充私人使用区(*Supplementary Private Use Area*)等。

------

### 1.1 编码与字符集

- **ASCII**
  - `ASCII`是基本的字符集，用1字节编码。
  - 其中编码值`0~127`包含英语中的大小写字母、数字0~9、标点符号、换行符、制表符、退格符等
- **Latin1**
  - `Latin1`字符集是对`ASCII`字符集的扩展，也是用一字节编码，它用`128~255`表示拉丁字母表中特殊语言字符；
- **Unicode**
  - `ASCII`和`Latin1`都是用1字节编码的，最多只有256个字符，无法表示汉语、日语等其他语言里的字符，因此又出现了`Unicode`编码。
  - `Unicode`增加一个或多个高位字节对`Latin1`进行扩展。当这些高位字节为0时，低字节数据就是`Latin1`字符。
  - `Unicode`支持大多数国家和地区语言文字，所以被广泛使用。
  - 官网：[Unicode – The World Standard for Text and Emoji](https://home.unicode.org/)
- `Unicode`**存储方案**
  - `Unicode`有多种存储方案，其中`UTF-8`最少用1字节编码，可以使用1~4字节编码。
  - `UTF-16`最少使用2字节编码，可以使用2字节或4字节编码。
  - `UTF-8`可以兼容`Latin1`编码，所以被广泛使用，Qt Creator存储的C++头文件和源程序文件都默认使用`UTF-8`的存储方案（或叫成UTF-8编码）。

[^Extensibility]: 在UCS 编码中有一个叫做 "Zero Width No-Break Space" ，中文译名作“零宽无间断间隔”的字符，它的编码是 FEFF。而 FEFF 在 UCS 中是不存在的字符，所以不应该出现在实际传输中。UCS 规范建议我们在传输字节流前，先传输字符 "Zero Width No-Break Space"。这样如果接收者收到 FEFF，就表明这个字节流是 Big-Endian 的；如果收到FFFE，就表明这个字节流是 Little- Endian 的。

------

### 1.2 QString的隐式共享

<a id="implicitShared">implicitShared</a>

- 简介

  - <!--Many C++ classes in Qt use implicit data sharing to maximize resource usage and minimize copying. Implicitly shared classes are both safe and efficient when passed as arguments, because only a pointer to the data is passed around, and the data is copied only if and when a function writes to it-->
  - 隐式共享又称**回写复制**（*copy-on-write or value semantics*）。当两个对象共享同一份数据（*通过浅拷贝实现数据块共享*）时，如果数据不改变，则不进行数据的复制。
  - 而当某个对象需要**改变数据**时，则执行深拷贝。

- 深浅拷贝

  - 在处理共享对象时，使用深拷贝和浅拷贝两种方式复制对象。
  - 深拷贝指生成对象的完整的复制品，所占空间、资源等。
  - 而浅拷贝仅复制指向共享对象的**指针**。
  - 执行一个深拷贝的代价比较昂贵，要占用更多的内存和CPU资源，而浅拷贝的效率很高，它仅需设置一个指向共享数据块的指针及修改**引用计数**（*reference count*）的值。

- 举个例子（栈空间内）

  ```c++
  QString str1_t = "expend";
  QString str2_t = str1_t;	//1
  str2_t[2] = 't';			//2
  str1_t = str2_t;			//3
  ```

  1. `QString str2_t = str1_t;`
     - 将字符串对象`str1_t`赋值给另一个字符串`str2_t`，由QString的拷贝构造函数完成`str2_t`的初始化。
     - 在对`str2_t`赋值的时候，将发生一次**浅拷贝**，导致两个QString对象都指向同一个数据结构。
     - 该数据结构除了保存字符串`"expend"`外，还保存了一个引用计数器，用来记录字符串数据的引用次数。
     - 这里`str1_t`和`str2_t`指向同一个数据结构，所以*reference count*的值为2。
  2. `str2_t[2] = 't';`
     - 对QString对象`str2_t`的修改会导致一次深拷贝，使得`str2_t`指向一个新的、不同于`str1_t`的数据结构，保存的字符串为`"extend"`。
     - 该数据结构的*reference count*为1，因为只有`str2_t`指向这个数据结构。
     - 同时原来的数据结构会修改*reference count*为1，这意味着该数据结构没有被共享。
  3. `str1_t = str2_t;`
     - 将`str2_t`赋值给`str1_t`，此时`str1_t`将它指向的数据结构的*reference count*修改为0，也就是说，没有QString对象再使用这个数据结构了。
     - 因此，`str1_t`指向的数据结构将会从内存中释放掉，`str2_t`指向的数据结构的*reference count*为2。

------

### 1.3 QString的内存分配机制

<a id="preallocate">Preallocates</a>

- QString在一个连续的内存块中保存字符串数据，当字符串长度不断增加时，QString需要重新分配内存空间，以便有足够的空间保存增加的字符串。

- > If you are building a QString gradually and know in advance approximately how many characters the QString will contain, you can call [reserve](https://doc.qt.io/qt-5/qstring.html#reserve)(), asking QString to preallocate a certain amount of memory. You can also call [capacity](https://doc.qt.io/qt-5/qstring.html#capacity)() to find out how much memory QString actually allocated.

- Qt官方文档[Container Classes | Qt Core 6.7.1](https://doc.qt.io/qt-6/containers.html)中**分配策略**（*Growth Strategies*）如下：

  - $$
    a_n=2^(n+3)-8
    $$

  - 其中`n`为**重新分配**的次数，注意分配的是多少个`QChar`字符。一个字符占**2字节**(*byte*)，16位(*bit*)。QString内部使用`Unicode`(UTF-16)编码。

  - 注意实际分配是受**32位还是64位系统**影响，具体可以参考例子自己试一试。

- 例如`QString::append()`函数源码段：

  - ```c++
    {
        if (d->ref.isShared() || uint(d->size + str.d->size) + 1u > d->alloc)
        	reallocData(uint(d->size + str.d->size) + 1u, true);
    	memcpy(d->data() + d->size, str.d->data(), str.d->size * sizeof(QChar));
    	d->size += str.d->size;
    	d->data()[d->size] = '\0';
    }
    ```

  - 可以看到，用了`reallocData()`函数重新分配内存空间。

  - 其中有个`allocOptions`选项会与上(`|=`)`QArrayData::Grow`，也就是`0x8`。

  - 最后会通过`qCalculateGrowingBlockSize()`函数去计算需要重新分配内存空间的大小。

  - ```c++
    if (Q_UNLIKELY(int(morebytes) < 0)) {
            // catches morebytes == 2GB
            // grow by half the difference between bytes and morebytes
            bytes += (morebytes - bytes) / 2;
        } else {
            bytes = morebytes;
        }
    ```

- 举个具体的例子：

  - ```c++
    void TestQStringCapacity::test_m()
    {
        QString testStr_t = "";
        qint64 recordCapacity_t = testStr_t.capacity();
        qDebug() << "Start!\tCapacity:" << testStr_t.capacity() << "Size:" << testStr_t.size();
        for (int i = 0; i < 10000; ++i) {
            testStr_t.append("a");
            if (Q_UNLIKELY(!!(testStr_t.capacity() - recordCapacity_t))) {
                qDebug() << "test Str has been expanded!" << QString("Capacity:%1 Size:%2 At %3 Cycle.").arg(testStr_t.capacity()).arg(testStr_t.size()).arg(i);
                recordCapacity_t = testStr_t.capacity();
            }
        }
        qDebug() << "End!\tCapacity:" << testStr_t.capacity() << "Size:" << testStr_t.size();
    }
    ```

  - 根据QString的内存分配策略，这个循环操作将导致**11**次内存重新分配：*8, 24, 56, 120, 248, 504, 1016, 2040, 4088, 8184, 16376*。

  - 最后一次内存重新分配操作后，QString对象`testStr_t`具有一个*16376*个`Unicode`字符大小的内存块（32752字节），其中有*10000*个字符空间被使用（*20000B*）。

  - 实际打印的`Capacity`跟32位还是64位**编译环境**、字符后的`\0`、Qt版本等其他因素有关。

  - 例如下面是`Qt5.15.2 MSVC2019 32bit`编译的结果：

    ```C++
    Start!	Capacity: 0 Size: 0
    test Str has been expanded! "Capacity:7 Size:1 At 0 Cycle."
    test Str has been expanded! "Capacity:23 Size:8 At 7 Cycle."
    test Str has been expanded! "Capacity:55 Size:24 At 23 Cycle."
    。。。
    test Str has been expanded! "Capacity:4087 Size:2040 At 2039 Cycle."
    test Str has been expanded! "Capacity:8183 Size:4088 At 4087 Cycle."
    test Str has been expanded! "Capacity:16375 Size:8184 At 8183 Cycle."
    End!	Capacity: 16375 Size: 10000
    ```

  - 下面是`Qt5.15.2 MinGW 64bit`编译的结果：

    ```C++
    Start!	Capacity: 0 Size: 0
    test Str has been expanded! "Capacity:3 Size:1 At 0 Cycle."
    test Str has been expanded! "Capacity:19 Size:4 At 3 Cycle."
    test Str has been expanded! "Capacity:51 Size:20 At 19 Cycle."
    test Str has been expanded! "Capacity:115 Size:52 At 51 Cycle."
    。。。
    test Str has been expanded! "Capacity:16371 Size:8180 At 8179 Cycle."
    End!	Capacity: 16371 Size: 10000
    ```




------

## 2. QChar

### 2.1 中文帮助文档：

- [QChar 类 | Qt Core 6.4.1 - 中文文档编制,帮助,手册,教程 (digitser.top)](http://qt6.digitser.top/6.4/zh-CN/qchar.html)
- [Qt - QChar (class) (runebook.dev)](https://runebook.dev/zh/docs/qt/qchar)

### 2.2 常用的函数

|            函数名称            | 功能                                                         |        返回值        |
| :----------------------------: | :----------------------------------------------------------- | :------------------: |
|       `isDigit() const`        | 判断该字符是否为*0~9的数字*                                  |         bool         |
|       `isLetter() const`       | 判断该字符是否是字母，**注意，汉字也是Letter**               |         bool         |
|   `isLetterOrNumber() const`   | 是否为*字母或数字*                                           |         bool         |
|       `isLower() const`        | 是否为*小写*字母                                             |         bool         |
|       `isUpper() const`        | 是否为*大写*字母                                             |         bool         |
|        `isMark() const`        | 是否为*记号*                                                 |         bool         |
|    `isNonCharacter() const`    | 判断是否是*非文字字符*，例如Unicode编码0xfffe~0xffff之间     |         bool         |
|        `isNull() const`        | 判断字符编码是否为0x0000，也就是'\0'                         |         bool         |
|       `isNumber() const`       | 判断该字符是否为*一个数*，不仅包括0~9，还包括字符`①、⑨`等    |         bool         |
|       `isPrint() const`        | 是否为*可打印字符*                                           |         bool         |
|       `isPunct() const`        | 是否为*标点符号*                                             |         bool         |
|       `isSpace() const`        | 是否为**空白符**，包括空格、制表符                           |         bool         |
|       `isSymbol() const`       | 是否为*符号*，例如`♡`                                        |         bool         |
|       `toLatin1() const`       | 返回于`Qchar`字符等效的`Latin1`字符，如果无等效字符则返回'\0' |       **char**       |
|       `toLower() const`        | 返回字母的*小写*形式，如果该字符不是字母，则返回其本身       |        QChar         |
|       `toUpper() const`        | 返回字母的*大写*形式，如果该字符不是字母，则返回其本身       |        Qchar         |
| `unicode() const`、`unicode()` | 返回该字符的16位Unicode*编码数值*                            | char16_t、char16_t & |

### 2.3 QChar字符与Latin1字符互相转换

- `QChar`中有个函数`toLatin1()`将该字符转换位`Latin1`字符，也就是将UTF-16编码的字符（2字节）转换为1字节的`Latin1`编码的字符；
- 只有当字符的编码为**0~255**时，该函数才有意义。
- 同样的，可以将`Latin1`字符转换为`QChar`字符；
  - **静态**函数：`QChar QChar::fromLatin1(char c)`；
  - `QChar`构造函数：`QChar::QChar(QLatin1Char ch)`、`QChar::QChar(char ch)`（当宏`QT_NO_CAST_FROM_ASCII`被定义的时候不可用）、`QChar::QChar(uchar ch)`（当宏`QT_NO_CAST_FROM_ASCII`、`QT_RESTRICTED_CAST_FROM_ASCII`被定义的时候不可用）;

### 2.4 Qchar字符的Unicode编码

- 可以通过`unicode()`函数获取任何一个字符的`UTF-16`编码数值；

- 也可以通过`QChar QChar::fromUcs2(char16_t c)`这个**静态**函数来从`UTF-16`编码得到一个字符（**注意，该函数是Qt6.0以后才有的**）；

- 或者通过`QChar`的构造函数`QChar::QChar(char16_t ch)`构造一个字符；

- 例如：

  - ```c++
    QString str_t("你好,世界");
    str_t[3] = QChar(0x4e2d);
    str_t[4] = QChar::fromUcs2(0x56fd);
    ```

  - 该字符串的内容从"你好世界"变成了"你好中国"；

- **注意**！

- 如果字符的UTF-16编码超过了`Latin1`的编码范围（也就是超过了255），就不能直接传递字符用于构造`QChar`对象，例如：

- `str_t[4] = QChar('国'); //错误的`

- Qt源程序采用UTF-8编码储存文件，源代码中的"国"是2字节的UTF-8编码，`QChar`没有这种类型参数的构造函数；

> 参考链接：
>
> - [Unicode Character Categories (fileformat.info)](https://www.fileformat.info/info/unicode/category/index.htm)
> - [Unicode Character Categories (compart.com)](https://www.compart.com/en/unicode/category)

## 3. QString常用功能函数

> 所用版本：<mark>Qt 6.5 LTS</mark>
>
> Qt6.5废弃的一些函数：[Obsolete Members for QString | Qt Core 6.5.5](https://doc.qt.io/qt-6.5/qstring-obsolete.html)
>
> 在QString中的所有函数都是可重入且线程安全的[Reentrancy and Thread-Safety](https://doc.qt.io/qt-6.5/threads-reentrancy.html)；

### 3.1 **增**

#### append

`QString &QString::append(const QString &str)`
*作用：在末尾追加字符/字符串*

- *除此之外还有**7**个重载函数*
- 追加函数`append()`是非常快的（时间复杂度为O(1)常量级别），因为`QString`用了[预分配内存分配机制](#preallocate)（空间换时间）
- 有**2**个重载函数是QT6之后引入的：
  - `QString &QString::append(QStringView v)` <mark>[since 6.0]</mark>
  - `QString &QString::append(QUtf8StringView str)` <mark>[since 6.5]</mark>

#### insert

`QString &QString::insert(qsizetype position, const QString &str)`

*作用：在给定位置的前面插入字符串*

- *除此之外还有**7**个重载函数*
- **注意**：如果给定插入的位置`position`比原字符串还大，则从原字符串末尾一直到`position`的位置会用空格代替；
- 该函数返回的是`*this`，所以会修改原字符串的内容；
- 有**2**个重载函数是QT6之后引入的：
  - `QString &QString::insert(qsizetype position, QStringView str)` <mark>[since 6.0]</mark>
  - `QString &QString::insert(qsizetype position, QUtf8StringView str)` <mark>[since 6.5]</mark>

#### prepend

`QString &QString::prepend(const QString &str)`

*作用：在最前面追加字符/字符串*

- *除此之外还有**7**个重载函数*
- 描述和`append()`相同
- 有**2**个重载函数是QT6之后引入的：
  - `QString &QString::prepend(QStringView str)` <mark>[since 6.0]</mark>
  - `QString &QString::prepend(QUtf8StringView str)` <mark>[since 6.5]</mark>

#### push_back

`void QString::push_back(const QString &other)`

- *除此之外还有**1**个重载函数*
- 这个函数是为了兼容STL而提供的；
- 该函数和`append()`完全一致；

#### push_front

`void QString::push_front(const QString &other)`

- *除此之外还有**1**个重载函数*
- 这个函数是为了兼容STL而提供的；
- 该函数和`prepend()`完全一致；

#### repeated

`QString QString::repeated(qsizetype times) const`

*作用：返回原字符串拷贝的字符串，返回后的字符串由重复原字符串组成*

- 如果times<1，则会返回一个**空串**；



### 3.2 **删**(截)

#### chop

`void QString::chop(qsizetype n)`
*作用：删除该字符串最后的n个字符*

- 如果n>=字符串大小，则删完；
- 如果n为负数，则不删；

#### chopped

1. `QString QString::chopped(qsizetype len) const`
   *作用：返回该字符串从最左边开始长度为len的字符串*
   - len为负或者大于字符串长度会产生不可预知；

#### clear

`void QString::clear()`
*作用：清空字符串*

- isEmpty()和isNull都是true；

#### erase

`QString::iterator QString::erase(QString::const_iterator first, QString::const_iterator last)` <mark>[since 6.1]</mark>

*作用：删除从迭代器first到迭代器last位置的字符串，并且返回删除字符串紧接着之后位置的迭代器*

- *除此之外还有**1**个重载函数*

- 注意，删除区间是半开式的，可以理解为：`[first, last)`，也就是删除first所在位置，但不删除`last`；

- 删除之后，返回的迭代器其实也就是`last`；

- 两个函数都是**QT6新加入**的函数，还有一个重载函数为：

  - `QString::iterator QString::erase(QString::const_iterator it)` <mark>[since 6.5]</mark>

  - ```c++
    QString c = "abc";
    auto it = c.erase(c.cbegin());
    //删除后c字符串为："bc"
    //迭代器it所指的位置为'b'
    ```

#### first / last

`QString QString::first(qsizetype n) const` <mark>[since 6.0]</mark>

*作用：返回前n个字符的字符串*

- 该函数自从Qt6.0之后引入；
- 如果n<0或n>size就会出现不可预知的问题；

`QString QString::last(qsizetype n) const` <mark>[since 6.0]</mark>

*作用：返回后n个字符的字符串*

- 同`first()`；

*引入该函数是在预先直到需要截取的字符串不会超过原始字符串，用这两个函数会比用`left()`和`right()`快*

#### left* / right*

共有**4**个截取字符串头尾的函数：

> 如果预先知道`n`不会超过边界，则请使用`first/left()`，比这些函数要快；

`QString QString::left(qsizetype n) const`

- *作用：返回原始字符串左边的n个字符串*

- 如果`n`超过边界则会返回原始字符串（`n`大于原始字符串大小或`n`小于0的时候）；

`QString QString::leftJustified(qsizetype width, QChar fill = u' ', bool truncate = false) const`

- *作用：返回是否有填充的左边部分字符串*
- 如果给定的`width`大于原字符串的长度，则多余的部分全部使用`fill`的字符来填充；
- `truncate`代表是否截取字符串，默认设置为不截取，如果要类似`left()`的效果，则要设置为`true`；

`QString QString::right(qsizetype n) const`

- *作用：返回原始字符串右边的n个字符串*
- 描述同`left()`类似；

`QString QString::rightJustified(qsizetype width, QChar fill = u' ', bool truncate = false) const`

- *作用：返回是否有填充的右边部分字符串*
- 描述同`leftJustified()`类似，会在字符串最前面填充；

#### mid / sliced

> 有两个函数可以从字符串中间截取指定长度的字符串；

`QString QString::mid(qsizetype position, qsizetype n = -1) const`

- *作用：截取从position开始，包含position，长度为n的字符串*
- 如果预先直到截取的长度不会超过原字符串的边界，则使用`sliced()`函数更快；
- 如果`position`超过了原字符串的边界，则会返回空字符串；
- 如果给定的`n`大于原字符串剩余的大小，则`n`等同于原字符串剩余的大小，即返回剩下所有的字符串；
- 如果`n`未给定（n = -1），或者`n<0`，则也会返回`position`后的所有字符串；

`QString QString::sliced(qsizetype pos, qsizetype n) const` <mark>[since 6.0]</mark>

- 作用和`mid`相同；
- *除此之外还有**1**个重载函数*，两个函数都是自Qt6.0之后新加的；
  - `QString QString::sliced(qsizetype pos) const` <mark>[since 6.0]</mark>
- 注意，以下情况可能会出现不可预知（源码其实也是有做断言ASSERT的）：
  - 如果`pos`超过原字符串的边界；
  - `n`小于0；
  - `n` + `pos` > `size()`；
- 该函数自Qt6.0后引入，目的是取消复杂的判断使得返回字符串更快；

#### remove*

> 共有**9**个remove系列的函数，返回值都是`*this`，也就是会更改原字符串的内容。

1. `QString &QString::remove(qsizetype position, qsizetype n)`
   - *作用：删除源字符串从position开始，包括position的后n个字符*
   - 如果`position + n`超出了原字符串的长度，则会把position后的字符全删掉；如果`n<0`则直接返回；
   - 如果`position<0`，则会从字符串最后开始删除，最后一位算-1，以此类推；
   - **注意**：字符串中字符的删除不会释放实际预申请的空间，这点可以调用`capacity()`查看到，如果想释放多余的内存空间，可以在remove后使用`squeeze()`函数；
2. `QString &QString::remove(QChar ch, Qt::CaseSensitivity cs = Qt::CaseSensitive)`
   - *作用：删除源字符串中出现的每一个ch字符/串*
   - *除此之外还有**3**个重载函数*
   - 同样的也不会释放预申请的空间，该函数等同于`replace(str, "", cs)`；
   - 重载函数中同样也有使用正则表达式的形式；
3. `QString &QString::removeAt(qsizetype pos)` <mark>[since 6.5]</mark>
   - *作用：删除pos所在位置的字符*
   - 如果pos超出了原字符串的边界（pos<0 || pos >= size()），则不会进行任何操作；
   - 该函数返回的是`*this`；
4. `QString &QString::removeFirst()` <mark>[since 6.5]</mark>
   - *作用：删除原字符串的第一个字符*
   - 如果原字符串为空，则不会发生任何事；
5. `template <typename Predicate> QString &QString::removeIf(Predicate pred)` <mark>[since 6.1]</mark>
   - 从原字符串中删除所有pred返回为true的字符；
6. `QString &QString::removeLast()` <mark>[since 6.5]</mark>
   - *作用：删除原字符串的最后一个字符*
   - 如果原字符串为空，则不会发生任何事；

#### section

`QString QString::section(QChar sep, qsizetype start, qsizetype end = -1, QString::SectionFlags flags = SectionDefault) const`

*作用：返回原字符串的一部分*

- *除此之外还有**2**个重载函数*
- `section()`函数可以用**字符、字符串、正则表达式**作为分隔符去得到子串，使用正则表达式会比前面两种方式慢得多；
- 如果`sep`为空，则无论后面`start`和`end`是多少，返回的都是一个空串；
- `start`和`end`都可以是负数，`0`代表第一个字符串，`-1`代表最后一个子串，`-2`代表倒数第二个子串，以此类推；
- `start`和`end`都是闭区间，`[start, end]`，返回的子串会包含这两个；
- `end`默认为`-1`，也就是说，如果没有指定，则会返回从`start`到原字符串最后；
- 可以指定分割符的风格，例如`flags  = QString::SectionSkipEmpty`，则会跳过分割出来的空串（详细例子里有）；
- 枚举`enum QString::SectionFlag`及含义（后面例子有详细用法）：
  - `QString::SectionDefault`：默认值为0x0；
  - `QString::SectionSkipEmpty`：如果头尾有分隔符存在，则跳过头尾分割出来的空串；
  - `QString::SectionIncludeLeadingSep`：返回的子串包含前面的分隔符；
  - `QString::SectionIncludeTrailingSep`：返回的子串包含后面的分隔符；
  - `QString::SectionCaseInsensitiveSeps`：分隔符不区分大小写，默认区分大小写；
- 默认风格：
  - 不会跳过空串，也就是说，如果`sep`在头和尾都有出现，则分割的子串至少有两个空串（头和尾各一个）；
  - 如果`start`和`end`是连续的，则最后的子串中间会有分隔符；
  - 默认返回的子串不会带头和尾的分隔符；

#### shrink_to_fit

`void QString::shrink_to_fit()`

- 该函数功能和`squeeze()`完全一样，只是提供和STL兼容的写法；

#### simplified / trimmed

`QString QString::simplified() const`

*作用：返回一个去掉开头和结尾的空字符，且把中间多个空字符替换为一个的原字符串的拷贝字符串*

- 该函数会把字符串中的多个空白字符替换为一个**空格**字符；
- 空白字符包括`\t`、空格、`\n`、`\r`、`\f`、`\v`等，判断是否为空白字符可以通过` QChar::isSpace()`函数；
- 详细参考：[List of Unicode Characters of Category “Control” (compart.com)](https://www.compart.com/en/unicode/category/Cc)

`QString QString::trimmed() const`

- 只去掉开头和结尾的空白字符，**中间空白字符不会变**；

#### squeeze

`void QString::squeeze()`

*作用：提供一种微调QString内存的方法，让QStirng申请的内存空间压缩*

#### truncate

`void QString::truncate(qsizetype position)`

*作用：从包括position开始删除原字符串一直到最后为止*

- 如果`position`超出边界，则什么都不会发生；

- 如果`position`是负数，则`position`等同于0，就是相当于全删完；

- 和`chop()`的区别和演示：

- ```c++
  QString str1_QS_t("qt for everything");
  QString str2_QS_t("qt for everything");
  str1_QS_t.chop(5); //"qt for every" size: 12 capacity: 17
  str2_QS_t.truncate(5); //"qt fo" size: 5 capacity: 17
  str2_QS_t.truncate(-5); //"" size: 0 capacity: 17
  ```



### 3.3 **改**

#### fill

`QString &QString::fill(QChar ch, qsizetype size = -1)`

*作用：把字符串的每一个字符都更改为ch*

- 默认按照原来的字符串长度填充，如果指定长度，则按照指定长度填充；
- 如果`size`小于0，则按照原来的字符串长度填充；

#### replace

> 该函数共有**12**种形式，其中**11**个重载函数，以下详细说几个常用的形式

1. `QString &QString::replace(qsizetype position, qsizetype n, const QString &after)`

   - 从`positon`开始，使用字符串`after`，替换原字符串包含`position`位置共`n`个字符；
   - 如果`n`超过了剩余的原字符串的长度，则会把从`position`到原字符串末尾的字符全部替换为`after`；
   - 如果`n`为0，则相当于在`position`的位置前面**插入**字符串`after`；
   - 如果`position`为负数，则什么都不会发生，并不会像通常那样负数指的原字符串末尾；
   - 如果`n`为负数，则会出现问题，并不会在`position`位置增加字符串`after`；

2. `QString &QString::replace(qsizetype position, qsizetype n, QChar after)`

   - 同第一点的描述；

3. `QString &QString::replace(qsizetype position, qsizetype n, const QChar *unicode, qsizetype size)`

   - 同第一点的描述；

4. `QString &QString::replace(QChar before, QChar after, Qt::CaseSensitivity cs = Qt::CaseSensitive)`

   - 会把原字符串中的所有`before`字符替换为`after`字符；

5. `QString &QString::replace(const QChar *before, qsizetype blen, const QChar *after, qsizetype alen, Qt::CaseSensitivity cs = Qt::CaseSensitive)`

   - 可以指定替换和被替换的字符串的长度，不会造成**重复替换**；
   - 如果`blen`小于`before`的长度，则按照`before`中`blen`的长度来替换；如果大于，则不替换直接返回；
   - 如果`alen`小于`after`的长度，则按照`after`中`alen`的长度来替换；如果大于，则多余的**使用空字符(u+0000)填充**；

6. `QString &QString::replace(QLatin1StringView before, QLatin1StringView after, Qt::CaseSensitivity cs = Qt::CaseSensitive)`

7. `QString &QString::replace(QLatin1StringView before, const QString &after, Qt::CaseSensitivity cs = Qt::CaseSensitive)`

8. `QString &QString::replace(const QString &before, QLatin1StringView after, Qt::CaseSensitivity cs = Qt::CaseSensitive)`

9. `QString &QString::replace(const QString &before, const QString &after, Qt::CaseSensitivity cs = Qt::CaseSensitive)`

   - 顺序替换完成后，不会重新扫描字符串然后继续替换，例如：

   - ```c++
     QString equis = "xxxxxx";
     equis.replace("xx", "x"); // equis == "xxx"
     ```

10. `QString &QString::replace(QChar ch, const QString &after, Qt::CaseSensitivity cs = Qt::CaseSensitive)`

11. `QString &QString::replace(QChar c, QLatin1StringView after, Qt::CaseSensitivity cs = Qt::CaseSensitive)`

12. `QString &QString::replace(const QRegularExpression &re, const QString &after)`

    - 可以使用正则表达式查询需要被替换的字符串；

    - **常用的方式**：可以使用`\1、\2...`等来代替前面正则表达式捕捉到的值，正则表达是里面需要用`()`来写子表达式（参考：[正则表达式 – 语法](https://www.runoob.com/regexp/regexp-syntax.html)）；

    - 举个例子：

    - ```c++
      QString str4_QS_t("Qt qt qqt qqttqq");
      static QRegularExpression express_QRE_s("([^Q ]*)([t]+)([q]?)");
      //"Q() (q) (qq) (qqt)q"
      str4_QS_t.replace(express_QRE_s, QString("(\\1)"));
      str4_QS_t = "Qt qt qqt qqttqq";
      //"Q(t) (t) (t) (t)q"
      str4_QS_t.replace(express_QRE_s, QString("(\\2)"));
      str4_QS_t = "Qt qt qqt qqttqq";
      //"Q() () () (q)q"
      str4_QS_t.replace(express_QRE_s, QString("(\\3)"));
      str4_QS_t = "Qt qt qqt qqttqq";
      //"Q(\\4) (\\4) (\\4) (\\4)q"
      str4_QS_t.replace(express_QRE_s, QString("(\\4)"));
      str4_QS_t = "Qt qt qqt qqttqq";
      //"Q(t\\4) (qt\\4) (qqt\\4) (qqttq\\4)q"
      str4_QS_t.replace(express_QRE_s, QString("(\\1\\2\\3\\4)"));
      ```

#### reserve

`void QString::reserve(qsizetype size)`

*作用：确保字符串中有size大小的空间*

- 如果能预先知道字符串大致要使用多少空间，建议使用该函数，因为可以避免多次重复申请空间而造成的内存碎片，提高整体性能；

- 如果后续还要多次向字符串中添加，使用该函数预先估一个值也能避免添加操作的最后一次操作申请一个非常大的空间，具体可查看 [QString的内存分配机制](#preallocate)；

- **注意**：使用`reserve()`函数预留内存并不会更改字符串本身的大小；

  - 如果`reserve()`的大小比字符串的大小大，在这种情况下访问超出字符串末尾边界会造成不可预知的行为；
  - 如果确实需要访问末尾边界后，可以使用`resize()`重新分配大小；

- 测试的几种现象：

- ```c++
  QString str1_QS_t;
  str1_QS_t.reserve(17); //17
  str1_QS_t.push_back("Qt");
  str1_QS_t.reserve(7); //17
  str1_QS_t.reserve(20); //20
  str1_QS_t.clear(); //0
  str1_QS_t.reserve(5); //5
  str1_QS_t = ""; //5
  str1_QS_t.reserve(3); //5
  
  str1_QS_t = "Qt For Everything";
  str1_QS_t.reserve(5); //17
  ```

- 最长见的使用情况是：对于一个会往后面添加很多次字符的字符串，大致知道最后这个很长的字符串有多大，这个时候可以按照下面来使用。

- ```c++
  QString result;
  qsizetype maxSize;
  
  result.reserve(maxSize);
  while (condition) {xxxx/*比如在这里添加文件数据流*/}
  result.squeeze(); //完成之后压缩大小
  ```

#### resize

`void QString::resize(qsizetype size)`

*作用：重新设置该字符串的大小*

- *除此之外还有**1**个重载函数*

- 其实底层都会重新构造一个QString的`DataPointer`类，然后把数值按照给定的大小`::memcpy()`过去；

- 如果`size`比原来的字符串大小要大，则后面多扩充的空间是没有初始化的；

- 如果`size`比原来的字符串大小要小，则会删掉大于`size`的字符串；

- ```c++
  QString s = "Hello world";
  s.resize(5);// s == "Hello"
  s.resize(8);// s == "Hello???"后面的字符是未初始化的
  ```

- 通过`resize()`函数重设后的字符串，也有动态增长空间的功能；

- 如果`size`为负，则为空串（isEmpty:true, isNull:false），跟`clear()`不同；

- 还有一个重载函数可以设置重新设置大小后填充的字符：

  - `void QString::resize(qsizetype newSize, QChar fillChar)`
  - 该函数可以和`leftJustified()`类比使用：

- ```c++
  QString str2_QS_t("Qt");
  str2_QS_t = str2_QS_t.leftJustified(10, u'Q'); //"QtQQQQQQQQ"
  str2_QS_t = "Qt";
  str2_QS_t.resize(10, u'T'); //"QtTTTTTTTT"
  ```



### 3.4 **查**

#### at

`const QChar QString::at(qsizetype position) const`
*作用：返回指定位置的字符*

- 在不对字符进行修改的情况下建议使用`at()`，因为它不会发生深拷贝，比`operator[]`快。

#### back / front

1. `QChar QString::back() const`
   *作用：返回字符串最后的字符*

   - 这里还有一个*类似重载*的函数可以修改最后的值：
     - `QChar &QString::back()`
     - 在Qt6以前是用`QCharRef`重载`QChar`来实现的，Qt6丢弃了`QCharRef`这个类。

   - 该函数兼容STL，建议不要对空字符串（`isNull()`）使用，不然会出现未定义行为。

2. `QChar QString::front() const`
   *作用：返回字符串最前面的字符*

   - 这里还有一个*类似重载*的函数可以修改最前面的值：
     - `QChar &QString::front()`

   - 其他描述参照`back()`；

3. Example：

   - ```c++
     QString str1_QS_t("peak");
     str1_QS_t.front() = 'b';
     str1_QS_t.back().setCell(0x74u);//after modified str1_QS_t == "beat"
     ```

#### capacity

`qsizetype QString::capacity() const`
*作用：返回实际开辟的内存空间容量*

- 注意，无论静态(static)的字符串多大，返回结果都为0，也就是说该函数返回的是**堆**上的空间大小。

#### *compare

> 有两中关于字符串比较的函数，其中一个是基于数值比较，非常快；另一个是基于平台和地区比较，可读性较好，常用于界面显示的字符串比较；

`static int QString::compare(const QString &s1, const QString &s2, Qt::CaseSensitivity cs = Qt::CaseSensitive)`
*作用：比较两个字符串的大小*

- *除此之外还有**8**个重载函数*，其中有4个静态函数；
- 逐个字符进行比较，如果相等返回0，前小于后返回正的差值，前大雨后返回负的差值；
- 区分大小写的比较完全基于字符的Unicode数值，并且非常快；
- 注：区分大小写的比较可能会不按照期望的那样（因为按照Unicode编码数值进行比较的），所以当出现汉字、其他语言等字符串的比较可以使用静态函数：`localeAwareCompare()`

`static int QString::localeAwareCompare(const QString &s1, const QString &s2)`

*作用：按照Locale设定的进行字符串比较*

- *除此之外还有**3**个重载函数*，其中有1个静态函数，功能类似`compare()`函数；
- 其中有**2**个重载函数是在Qt6.0之后引入的：
  - `int QString::localeAwareCompare(QStringView other) const` <mark>[since 6.0]</mark>
  - `static int QString::localeAwareCompare(QStringView s1, QStringView s2)` <mark>[since 6.0]</mark>
- 关于比较的参考：[Comparing Strings](https://doc.qt.io/qt-6.5/qstring.html#comparing-strings)



#### constData

`const QChar *QString::constData() const`

*作用：返回字符串中只读的原始数据*

- 注意，返回的指针只在字符串没有被修改时有效；
- 注意，返回的字符串可能不是以'\0'结尾的，可以使用`size()`确定字符数组的长度；

#### contains

`bool QString::contains(const QString &str, Qt::CaseSensitivity cs = Qt::CaseSensitive) const`
*作用：返回是否包含字符串str*

- *除此之外该函数还有**4**个重载函数*
- 其中有单独查询一个`QChar`的重载函数；
- 还有一种重载函数可以用正则表达式：
- `bool QString::contains(const QRegularExpression &re, QRegularExpressionMatch *rmatch = nullptr) const`
- 如果匹配成功并且`rmatch`不是`nullptr`，它还将匹配结果写入`rmatch`所指向的`QRegularExpressionMatch`对象。

#### count

`qsizetype QString::count(const QString &str, Qt::CaseSensitivity cs = Qt::CaseSensitive) const`
*作用：返回该字符串中出现字符或串`str`的次数*

- *除此之外还有**3**个重载函数*
- 其中有一个重载函数可以使用正则表达式，返回正则表达式匹配后的字符串出现的次数。
- 其中有一个重载函数是Qt6.0之后出现的：
  - `qsizetype QString::count(QStringView str, Qt::CaseSensitivity cs = Qt::CaseSensitive) const` <mark>[since 6.0]</mark>
- **注意**：有一个同名的重载函数已经在<mark>Qt 6.4</mark>中废弃使用了；所以这里没有算在上面重载函数中；

#### data

`QChar *QString::data()`

*作用：返回指向该字符串数据的指针，可以通过该指针访问和修改该字符串数据*

- *除此之外还有**1**个重载函数*，作用和`constData()`一样；
- 注意，返回的字符数组**始终**以'\0'结尾；
- 只有当字符串没有被其他方式修改时，指针才有效；
- 当使用指针修改字符串时(++[data] = 'x')，会导致深拷贝的发生；

#### endsWith / startsWith

`bool QString::endsWith(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const`

*作用：查询字符串是否以给出的字符/串结尾*

- *除此之外还有**3**个重载函数*

`bool QString::startsWith(const QString &s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const`

*作用：返回原字符串是否以s开始的*

- *除此之外还有**3**个重载函数*

#### indexOf / lastIndexOf

`qsizetype QString::indexOf(const QString &str, qsizetype from = 0, Qt::CaseSensitivity cs = Qt::CaseSensitive) const`

*作用：返回指定字符串在该字符串中第一次出现的位置*

- *除此之外还有**4**个重载函数*

- 可以使用`from`指定从哪个位置开始查找，也可以指定大小写敏感，如果没有找到则返回`-1`；

- 如果`from`是负数，则代表从最后开始找，例如`from = -1`代表从最后一个字符开始找；

- 重载函数中可以使用正则表达式；

- Example：

  ```c++
  QString str1_QS_t("Reference by Taiga");
  QString str2_QS_t("re");
  qsizetype index_qst_t = str1_QS_t.indexOf(str2_QS_t);           //4
  index_qst_t = str1_QS_t.indexOf("Re", 1, Qt::CaseInsensitive);  //4
  index_qst_t = str1_QS_t.indexOf("ga", -1);                      //-1
  index_qst_t = str1_QS_t.indexOf("ga", -2);                      //16
  index_qst_t = str1_QS_t.indexOf(QChar('a'), -5);                //14
  
  QRegularExpressionMatch match_QREM_pt;
  static QRegularExpression expression_QRE_s("[aeiou]+");
  //index_qst_t:14    //match_QREM_pt.captured():"ai"
  index_qst_t = str1_QS_t.indexOf(expression_QRE_s, -5, &match_QREM_pt);
  expression_QRE_s.setPattern("e[^aeiou]");
  index_qst_t = str1_QS_t.indexOf(expression_QRE_s);              //1
  ```

- 所有函数的返回值都是`[[nodiscard]]`；

`qsizetype QString::lastIndexOf(const QString &str, qsizetype from, Qt::CaseSensitivity cs = Qt::CaseSensitive) const`

*作用：返回指定字符串在该字符串中最后出现的位置*

- *除此之外还有**9**个重载函数*

- `from`指定从该位置开始（**包括该位置**）**向前搜索**最后一次出现的字符串，没找到则返回`-1`；

- 因为从最后搜索是包含指定位置的，所以当指定搜索的字符串为空串且使用默认搜索位置时，返回的是最后一位的后一位（具体例子有）；

- 有**5**个重载函数是QT6之后引入的：

  - `qsizetype QString::lastIndexOf(QChar c, Qt::CaseSensitivity cs = Qt::CaseSensitive) const` <mark>[since 6.3]</mark>
  - `qsizetype QString::lastIndexOf(QLatin1StringView str, Qt::CaseSensitivity cs = Qt::CaseSensitive) const` <mark>[since 6.2]</mark>
  - `qsizetype QString::lastIndexOf(const QString &str, Qt::CaseSensitivity cs = Qt::CaseSensitive) const` <mark>[since 6.2]</mark>
  - `qsizetype QString::lastIndexOf(QStringView str, Qt::CaseSensitivity cs = Qt::CaseSensitive) const` <mark>[since 6.2]</mark>
  - `qsizetype QString::lastIndexOf(const QRegularExpression &re, QRegularExpressionMatch *rmatch = nullptr) const` <mark>[since 6.2]</mark>

- 同样也能用正则表达式，但是算法和查找字符串不太一样；由于正则表达式匹配算法的工作方式，这个函数实际上会从字符串的开始重复匹配，直到到达字符串的末尾。

- 例子：

- ```c++
  QString str1_QS_t("Reference by Taiga");
  qsizetype index_qst_t;
  index_qst_t = str1_QS_t.lastIndexOf("AI", Qt::CaseInsensitive); //14
  
  index_qst_t = str1_QS_t.lastIndexOf(QString("e")); //8
  index_qst_t = str1_QS_t.lastIndexOf(QString("e"), 0); //-1
  index_qst_t = str1_QS_t.lastIndexOf(QString("e"), -1); //8
  
  index_qst_t = str1_QS_t.lastIndexOf(QString("re"), 4); //4
  index_qst_t = str1_QS_t.lastIndexOf(QString("re"), 5); //4
  index_qst_t = str1_QS_t.lastIndexOf(QString("re"), 3, Qt::CaseInsensitive); //0
  
  index_qst_t = str1_QS_t.lastIndexOf(QString("")); //18
  index_qst_t = str1_QS_t.lastIndexOf(QString(""), -1); //17
  index_qst_t = str1_QS_t.lastIndexOf(QString(""), 0); //0
  
  static QRegularExpression expression_QRE_s("[eiou](g)?");
  QRegularExpressionMatch match_QREM_pt;
  index_qst_t = str1_QS_t.lastIndexOf(expression_QRE_s, &match_QREM_pt); //15 "ig"
  index_qst_t = str1_QS_t.lastIndexOf(expression_QRE_s, -3, &match_QREM_pt); //15 "ig"
  index_qst_t = str1_QS_t.lastIndexOf(expression_QRE_s, -4, &match_QREM_pt); //8 "e"
  ```

#### is*

有**6**的判断字符串的函数；

- *判空*
  *Qt makes a distinction between null strings and empty strings for historical reasons.*

  - `bool QString::isEmpty() const`

    - 判断字符串是否是个空字符串，和`isNull()`有区别；

  - `bool QString::isNull() const`

    - 判断QString里面是否为空，和`isEmpty()`有区别；

    - ```c++
      QString().isEmpty();   // returns true
      QString("").isEmpty(); // returns true
      QString().isNull();    // returns true
      QString("").isNull();  // returns false
      ```

- *判断大小写*

  - `bool QString::isLower() const`
    - 注意：如果返回true并不代表该字符串不包含**大写字符**，因为某些大写字符并没有小写映射；
    - 通常用于判断标准英文字母的大小写；
    - 参考：[Glossary::folding(unicode.org)](https://www.unicode.org/glossary/#folding)
  - `bool QString::isUpper() const`
    - 判断是否为全大写字符串，参考`isLower()`；

- `bool QString::isRightToLeft() const`

  - 表示标准规范中双向字符类型中的方向，不是书写方向；
  - 参考：[UAX #9: Unicode Bidirectional Algorithm](https://www.unicode.org/reports/tr9/tr9-35.html#Table_Bidirectional_Character_Types)

- `bool QString::isValidUtf16() const`

  - 验证是否包含有效的UTF-16编码数据；
  - 注意，该函数不会做额外的数据验证，仅仅是判断是否能以UTF-16解译；
  - 该函数默认主机字节序，在本函数字节序标记（*BOM*）没意义；

#### length / size

两个函数是一样的，都是返回字符串中字符的个数；

- `qsizetype QString::length() const`
- `qsizetype QString::size() const`
- 注意：有一个同样功能的函数在<mark>Qt 6.4</mark>及以上版本被废弃使用了，所以这里不列出了（虽然为了向下兼容还是可以用的），如果在之前的旧的版本可以使用，但是还是建议使用`length()/size()`；



### 3.5 **变**

#### arg

`QString QString::arg(const QString &a, int fieldWidth = 0, QChar fillChar = u' ') const`
*作用：返回一个被格式控制字符替换后的新字符串*

- *除此之外还有**14**个重载函数*

- 可以指定**额外填充**的字符和方向，例如：

- ```c++
  QString result_QS_t;
  QString padded_QS_t("lie");
  QChar fillChar_QC_t = u'e';
  //result_QS_t结果："believe"
  result_QS_t = QString("be%1%2").arg(padded_QS_t).arg("ve");
  //result_QS_t结果："belieeeevvvve"
  result_QS_t = QString("be%1%2").arg(padded_QS_t, -6, fillChar_QC_t).arg("ve", 5u, QChar::fromLatin1('v'));
  ```

- 与`asprintf()`函数相比，该函数有以下有优点：

  - 不需要关心数字的顺序问题；
  - 数字可以重复代替；

- ```c++
  //result_QS_t结果："belielielieve"
  result_QS_t = QString("be%2%2%2%1").arg("ve").arg(padded_QS_t);
  ```

- 关于`arg()`函数的规范用法说明：[clazy/docs/checks/README-qstring-arg.md at 1.11 · KDE/clazy (github.com)](https://github.com/KDE/clazy/blob/1.11/docs/checks/README-qstring-arg.md)

- 其中还有一个模板重载函数：`template <typename Args> QString QString::arg(Args &&... args) const`

  - 用多个`.arg()`的方式会导致不必要的拷贝，因为该函数会返回一个拷贝的字符串(*Returns a copy of this string*)；

  - 会有警告：*Use multi-arg instead [clazy-qstring-arg]*

  - 所以有多个替换的时候尽量用多参数来代替，例如：

  - ```c++
    //result_QS_t结果："believe"
    result_QS_t = QString("be%1%2%3").arg(padded_QS_t, QChar('v'), "e"_L1);
    ```

- 当填充为**整数**数字时：

  - 可以指定填充的*进制(2~36)*，默认为10进制填充；

  - 如果填充的是`int`和`long`类型，且在10进制的情况下，还可以快捷指定*定位符（例如`,`等）*，默认为当前使用语言和地区。

  - ```c++
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
    str = QString("%1 %L2 %L3")
            .arg(12345)
            .arg(12345)
            .arg(12345, 0, 16);//不是10进制则忽略
    // str == "12345 12,345 3039"，这里的,就是定位符
    ```

- 当填充的数字为**实数**时：

  - 可以用*浮点数格式字符*来控制显示格式，默认*format*是`'g'`；

  - 例如：

    ```c++
    double PI_d_t = 31415926.535897932384626;
    QString str_QS_t = QString(QStringLiteral("π：%1")).arg(PI_d_t, 0, 'e', 4);
    //结果："π：3.1416e+07"
    ```

    - | *format* | 格式字符的含义                |       *precision*含义        |
      | :------: | ----------------------------- | :--------------------------: |
      |   'e'    | 科学计数法，如`3.14e+2`       |   基数的小数点后的有效位数   |
      |   'E'    | 科学计数法，如`3.14E-3`       |   基数的小数点后的有效位数   |
      |   'f'    | 自然计数法，如`3.14`、`-3.14` |      小数点后的有效位数      |
      |   'g'    | 使用'e'或'f'，哪种简洁用哪种  | 小数点前后的数字位数**之和** |
      |   'G'    | 使用'E'或'f'，哪种简洁用哪种  | 小数点前后的数字位数**之和** |

#### asprintf / vasprintf

> 有两个使用格式控制字符构造Qstring的静态函数

`static QString QString::asprintf(const char *cformat, ...)`
*作用：根据格式化字符串和参数列表安全的构建一个字符串*

- 该静态函数用于构造格式化输出各种数据的字符串，类似于标准C语言中的`printf()`，可以使用**转义序列**(*escape sequence*)，也可以指定数据宽度和小数位数等。

- **注意**，虽然`cformat`格式化字符串中支持汉字，但是使用`%s`替换`cformat`中的字符串只能用`UTF-8`编码的字符串，并且只能是`char *`类型。例如：

- ```c++
  QString strReplaced_QS_t("世界");
  QString strResult_QS_t(QString::asprintf("Hello, World.(你好，%s。)", "世界")); //错误的，这样写跟源程序代码编码有关，如果源文件编码为UTF-8则能显示否则显示乱码
  strResult_QS_t = QString::asprintf("Hello, World.(你好，%s。)", strReplaced_QS_t.toLocal8Bit().data()); //错误的，会得到乱码
  strResult_QS_t = QString::asprintf("Hello, World.(你好，%s。)", strReplaced_QS_t.data()); //错误的，会得到乱码
  
  strResult_QS_t = QString::asprintf("hello, world %.10f", 3.1415); //正确的，结果为"hello, world 3.1415000000"
  ```

- 关于转义序列：

  - `%lc`：`char16_t`或者`ushort`类型，或者`QChar::unicode()`的返回值也行；
  - `%ls`：以字符`'\0'`结尾的`char16_t`或者`ushort`类型的指针或地址，或者`QString::utf16()`的返回值也行；
  - 这两个与标准C++库中的`printf()`不一致，标准C++库定义`%lc`输出`wchar_t`类型,`%ls`输出`wchar_t *`类型，还可能在`sizeof(wchar_t)`不是16位的平台上产生编译器警告。

- 官方的**建议**不要使用`asprintf()`函数：

  - *We do not recommend using QString::asprintf() in new Qt code. Instead, consider using QTextStream or arg(), both of which support Unicode strings seamlessly and are type-safe*.

  - *Here is an example that uses QTextStream:*

  - ```c++
    QString result;
    QTextStream(&result) << "pi = " << 3.14;
    // result == "pi = 3.14"
    ```

- 对于需要多国翻译的字符串，建议使用`arg()`函数，因为被替代的序列也可以被Qt的翻译器（类似`tr()`）所控制。

`static QString QString::vasprintf(const char *cformat, va_list ap)`

- 该函数作用和`asprintf()`一致，但是使用了`va_list`宏来代替了可变参数列表`...`；
- 关于格式控制字符和`asprintf()`函数完全一致；
- 具体参考：[std::va_list - cppreference.com](https://en.cppreference.com/w/cpp/utility/variadic/va_list)
- **注意**：该函数**不会清空**va_list可变参数列表，所以需要在外面手动清空`ap`如：`va_end(ap)`；

#### number

`static QString QString::number(long n, int base = 10)`

*作用：返回指定进制和数字的数值一致的字符串*

- *除此之外还有**6**个重载函数，全部都是静态函数*
- 基本常用的格式都包含进去了，对于浮点型还能指定**格式**和**精度**，参考`arg()`函数的格式表；
- 例子：`QString t = QString::number(a, 16).toUpper(); //t == "3F"`

#### set*

共有**4**种把其他类型转变为`QString`的方法。

1. `QString &QString::setNum(int n, int base = 10)`
   *作用：把数字n转变为字符串*
   - *除此之外还有**9**个重载函数*
   - 该函数返回的字符串默认用`QLocale::C`标准（English/United States），如果需要返回本地区域的字符串，可以用`QLocale::toString()`；
   - 注意返回的是`*this`指针，会修改源字符串的值；
   - 也可以把实数设置为字符串，可以设置格式控制字符和精度；参考`number()`函数；
2. `QString &QString::setRawData(const QChar *unicode, qsizetype size)`
   - 使用`QChar`数组设置字符串，必须保证源数据不会被修改或删除；
   - 设置的QString不是用源数据拷贝生成的，一旦需要修改QString的内容，则会发生深拷贝；
3. `QString &QString::setUnicode(const QChar *unicode, qsizetype size)`
   - 与`setRawData()`函数不同的是该函数会把源数据拷贝一份；
   - **注意：**如果`unicode`是个空串，设置的QString也会按照`size`来设置空间大小；
4. `QString &QString::setUtf16(const ushort *unicode, qsizetype size)`
   - 设置该字符串为使用UTF-16标准的字符串，设置大小为`size`；
   - **注意**：如果`unicode`是个空串，设置的QString也会按照`size`来设置空间大小；
   - **注意**：该函数和`fromUtf16()`不一样，该函数不考虑BOM和可能出现不同的字节顺序；

#### split

`QStringList QString::split(const QString &sep, Qt::SplitBehavior behavior = Qt::KeepEmptyParts, Qt::CaseSensitivity cs = Qt::CaseSensitive) const`

*作用：返回以sep分割但不包括的子字符串列表*

- *除此之外还有**2**个重载函数*

- 如果没有找到分隔符，则会返回一个包含一个字符串的列表；

- 可以指定大小写敏感和子字符串列表格式；

  - `Qt::KeepEmptyParts`：如果子字符串有空，则保留到列表中；
  - `Qt::SkipEmptyParts`:如果子字符串有空，则不会保留到列表中；

- **注意**：如果`sep`为空，则会返回一个前后都为空串，中间是每一个QString中的字符构造的字符串；其实也就相当于原始字符串中每个字符中间（包含头尾）都有一个空串，（详细例子有）；

- 同样的，也可以使用正则表达式作为`sep`，可以实现很多方便的功能；

- 例子：

- ```c++
  
  QString str1_QS_t("qt for,everything reference,");
  QString str2_QS_t("Q t");
  QStringList result_QSL_t;
  result_QSL_t = str1_QS_t.split(";"); //["qt for,everything reference,"]
  result_QSL_t = str1_QS_t.split(","); //["qt for", "everything reference", ""]
  result_QSL_t = str1_QS_t.split(",", Qt::SkipEmptyParts); //["qt for", "everything reference"]
  result_QSL_t = str2_QS_t.split(QString(), Qt::KeepEmptyParts); //["", "Q", " ", "t", ""]
  result_QSL_t = str2_QS_t.split(QString(), Qt::SkipEmptyParts); //["Q", " ", "t"]
  const QRegularExpression expr_QRE_c("\\b[^a-z^A-Z]");
  result_QSL_t = str1_QS_t.split(expr_QRE_c, Qt::SkipEmptyParts); //["qt", "for", "everything", "reference"]
  ```

#### swap

`void QString::swap(QString &other)`

*作用：和其他字符串交换*

- 该函数的字符串交换操作非常快（其实就是交换数据），并且从来不会失败；

#### to*

> QString提供了一系列的转换其他类型的函数，一共**25**个

##### OS-String

*有**2**个转换为苹果API中String的函数，仅在macOS和iOS系统下可用*

1. `CFStringRef QString::toCFString() const`
   - 转换后调用者需要去**主动释放**该字符串内容；
   - 参考`fromCFString()`；
2. `NSString *QString::toNSString() const`
   - 转换的`NSString`调用者**不需要主动释放**；
   - 参考`fromNSString()`；

##### Case

*有**3**个大小写转换相关的函数*

1. `QString QString::toCaseFolded() const`
   - 通常会转换为小写字符；
2. `QString QString::toLower() const`
   - 将原字符串转换为小写字符然后返回一个拷贝的字符串；
   - 使用的是标准`'C' locale`，如果要转换为本地地区的小写，可以使用`QLocale::toLower()`；
3. `QString QString::toUpper() const`
   - 转换为大写形式，对于某些Unicode字符可能只有大写没有小写；

##### number

*有**10**个转换为数字数值相关的函数*

1. `double QString::toDouble(bool *ok = nullptr) const`

   - 如果转换溢出上边界，则返回无穷大，如果转换失败（例如溢出下边界），则会返回`0.0`；

   - 关于**无穷大**QT也有个宏定义（`Q_INFINITY`），是基于`ISO C99 macro INFINITY`的；参考：[Infinity and NaN (The GNU C Library)](https://www.gnu.org/software/libc/manual/html_node/Infinity-and-NaN.html)

   - 可以设置是否成功转换的标志`ok`；

   - 被转换的字符串内容只能包含有效的数字字符，包括正负号、科学记数法中使用的字符`e`和小数点。包含单位（$等）或附加字符会导致转换错误。

   - 该函数的转换是默认用`QLocale::C`标准（*A simplified English locale*）；

     - 如果使用其他国家地区的数字转换请用`QLocale::toDouble()`函数；

     - 例如：

     - ```c++
       double d;
       d = QString( "1234,56" ).toDouble(&ok); // ok == false
       d = QString( "1234.56" ).toDouble(&ok); // ok == true, d == 1234.56
       
       QLocale german(QLocale::German);
       d = german.toDouble("1234,56", &ok);  // ok == true,  d == 1234.56
       d = german.toDouble("1.234,56", &ok); // ok == true,  d == 1234.56
       d = german.toDouble("1234.56", &ok);  // ok == false, d == 0
       d = german.toDouble("1.234", &ok);    // ok == true,  d == 1234.0
       ```

   - 因为历史原因，该函数也不会处理千位分隔符，如果需要带分隔符一起转换，可以使用`QLocale::toDouble()`函数；

     - 例如：

     - ```c++
       d = QString( "1,234,567.89" ).toDouble(&ok); // ok == false
       
       QLocale c(QLocale::C);
       d = c.toDouble("1,234.56", &ok); // ok == true,  d == 1234.56
       ```

   - **注意**：该函数还会忽略字符串首尾的空白符；

2. `float QString::toFloat(bool *ok = nullptr) const`

   - 参考`toDouble()`；

3. `int QString::toInt(bool *ok = nullptr, int base = 10) const`

   - 将字符串按照指定的进制转换为int类型数据，可以传递转换成功与否标志；
   - 可指定进制在`2~36`之间，默认为10进制；
   - 如果失败，ok设为`false`且返回`0`；
   - **注意**：如果指定`base`为`0`，则该字符串会参考C语言的习惯转换，例如：
     - 以`0x`开头的字符串会被认为是16进制的；
     - 以`0b`开头的字符串会被认为是2进制的（这个是自从<mark>Qt6.4</mark>之后才支持的）；
     - 以`0`开头的字符串会被认为是8进制的；
     - 其他则默认为10进制的；
   - 同样的，该函数默认使用`QLocale::C`标准，如果需要用本地格式转换可以用`QLocale::toInt()`函数；
   - **注意**：该函数会忽略前导空格和尾随空格；

4. `long QString::toLong(bool *ok = nullptr, int base = 10) const`

   - 参考`toInt()`；

5. `qlonglong QString::toLongLong(bool *ok = nullptr, int base = 10) const`

   - 参考`toInt()`；

6. `short QString::toShort(bool *ok = nullptr, int base = 10) const`

   - 参考`toInt()`；

7. `uint QString::toUInt(bool *ok = nullptr, int base = 10) const`

   - 参考`toInt()`；
   - 如果有负数，则也会返回失败，返回值为0；

8. `ulong QString::toULong(bool *ok = nullptr, int base = 10) const`

   - 参考`toUInt()`；

9. `qulonglong QString::toULongLong(bool *ok = nullptr, int base = 10) const`

   - 参考`toUInt()`；

10. `ushort QString::toUShort(bool *ok = nullptr, int base = 10) const`

    - 参考`toUInt()`；

##### HTML

*有**1**个转换为HTML语言格式的函数*

1. `QString QString::toHtmlEscaped() const`

   - *作用：该函数会把纯文本字符串转换为HTML字符串*

   - 会把`< > & "`等符号用HTML的方式代替；例如：

   - ```c++
     QString plain = "#include <QtCore>"
     QString html = plain.toHtmlEscaped();
     // html == "#include &lt;QtCore&gt;"
     ```

##### 8-bit

*有**3**个转换为8bit字符串相关的函数*

1. `QByteArray QString::toLatin1() const`
   - 将源字符串转换为`Latin1`字符集的字符串；
   - 如果字符串包含非Latin1字符，则返回的字节数组中对应的位置可能会用`'?'`来代替；
2. `QByteArray QString::toLocal8Bit() const`
   - 将源字符串转换为本地8bit的字符数组；
   - 如果是在`Unix`系统上，该函数相当于`toUtf8()`，如果是在Windows系统上，则按照系统正在使用的字符集转换；
   - 如果源字符串中有不是8-bit编码的字符，那么转换到字符数组中这些字符可能不显示或者用其他字符代替了； 
3. `QByteArray QString::toUtf8() const`
   - 将源字符串转换为UTF-8字符集字符串；
   - UTF-8是Unicode编解码器，可以表示Unicode字符串(如QString)中的所有字符。

##### std*

*有**4**个转换为标准`std`字符串相关的函数*

1. `std::string QString::toStdString() const`
   - 将原字符串转换为标准的字符串`std::string`；
   - 因为QString是Unicode编码的，所以会用`toUtf8()`函数转为8-bit的字符串；
   - 该函数常用于将QSting传递出去然后用标准字符串接收；
2. `std::u16string QString::toStdU16String() const`
   - 将原字符串转为标准字符串`std::u16string`；
   - 其中的Unicode编码源数据和`utf16()`函数返回的一致；
3. `std::u32string QString::toStdU32String() const`
   - 将原字符串转为标准字符串`std::u32string`；
   - 其中的Unicode编码源数据和`toUcs4()`函数返回的一致；
4. `std::wstring QString::toStdWString() const`
   - 将原字符串转为标准字符串`std::wstring`，即是宽字符串类型；
   - 补充说明：在类型`wchar_t`为**2**字节的平台(例如 windows)上使用的是`utf16`编码，类型`wchar_t`为**4**字节的平台(大多数Unix系统)上使用的是`ucs4`编码。

##### UCS4

*有**1**个转换为UCS4标准的字符串函数*

1. `QList<uint> QString::toUcs4() const`
   - 转换为`UCS-4/UTF-32`标准的字符列表；
   - 返回值是一个`QList`；
   - 补充说明：
     - `UCS-4`是Unicode编解码器，因此它是无损的；
     - 该字符串中的所有字符将用`UCS-4`进行编码；
     - 如果出现任何不合法的字符，则会用`QChar::ReplacementCharacter`来代替，也就是`U+FFFD(0xfffd)`详细描述：[enum QChar::SpecialCharacter](https://doc.qt.io/qt-6.5/qchar.html#SpecialCharacter-enum)
   - **注意**：转换后的字符串（列表）不会以`'\0'`结尾；
   - 可以参考`fromUcs4()`函数，但是表示UCS4的类型不太一样；

##### wchar_t

*有**1**个转换为C++标准中wchar_t类型的字符串函数*

1. `qsizetype QString::toWCharArray(wchar_t *array) const`
   - 转换后的数组`array`在`wchar_t`为**2**字节的平台(例如windows)上用UTF-16编码，在`wchar_t`为**4**字节的平台(大多数Unix系统)上用UCS-4编码。
   - **注意**：传入的数组`array`必须事先就分配好足够的空间，可以使用该字符串的`length()`来分配空间，这样空间肯定是足够的；
   - 该函数会返回转换后的数组`array`的实际长度；
   - 该函数不会在末尾对数组`array`追加空字符('\0')；

#### unicode

`const QChar *QString::unicode() const`

*作用：返回源字符串的Unicode编码源数据*

- 返回的指针在源字符串未被修改前一直有效；
- **注意**：返回的字符串数据**可能**不会以`'\0'`结尾，请使用`size()`函数来确定数组的长度；

#### utf16

`const ushort *QString::utf16() const`

*作用：将源字符串以'\0'结尾的无符号短整类型返回*

- 返回的指针在源字符串未被修改前一直有效；
- 返回的源数据**一定**会以`'\0'`结尾；
- 返回的字符串按主机字节顺序（*host byte order*）排列；



### 3.6 **得**

#### fromCFString / NSString

1. `static QString QString::fromCFString(CFStringRef string)`
   *作用：从苹果系统API中定义的常量字符串CFStringRef类获得一个QString*

   - 该函数只在`macOS`和`iOS`系统环境上可用；

   - API参考：[CFString | Apple Developer Documentation](https://developer.apple.com/documentation/corefoundation/cfstring-rfh)

2. `static QString QString::fromNSString(const NSString *string)`
   *作用：从苹果系统API中定义的Unicode字符串NSString类构造一个QString*

   - 该函数只在`macOS`和`iOS`系统环境上可用；

   - API参考：[NSString | Apple Developer Documentation](https://developer.apple.com/documentation/foundation/nsstring)

#### fromLatin1

`static QString QString::fromLatin1(const char *str, qsizetype size)`

*作用：从Latin1字符集字符串获得长度为size的QString*

- *除此之外还有**2**个重载函数*
- 该静态函数构造字符串的长度是从最开始一直到长度为size为止；
- 如果size为负数，则会按照`strlen(str)`来代替；
- 如果size大于字符串本身的长度，会按照设置的size构造，但是构造的字符串大于`strlen(str)`的字符是不确定的；
- 其中有**1**个重载函数是Qt6.0之后引入的
  - `static QString QString::fromLatin1(QByteArrayView str)` <mark>[since 6.0]</mark>
  - `str`中所有的`'\0'`字符都会转化为Unicode的空字符（U+0000）；
- 其中还有**1**个重载函数是在Qt6.0之后有所改动：
  - `static QString QString::fromLatin1(const QByteArray &str)`
  - `str`中所有的`'\0'`字符都会转化为Unicode的空字符（U+0000），这点在**Qt5**中的处理方式不一样；

#### fromLocal8Bit

`static QString QString::fromLocal8Bit(const char *str, qsizetype size)`

*作用：从8-bit字符串获得长度为size的QString字符串*

- *除此之外还有**2**个重载函数*
- 该静态函数和`fromLatin1()`一样有**1**个是QT6.0之后引入的，有**1**个是QT6.0之后修改过的；
- 在`Unix`系统上，该函数相当于`fromUtf8()`，在`Windows`系统上，会根据源文件的编码格式判断，如果是UTF-8编码格式，则直接用字符串常量会显示乱码。

#### fromRawData

`static QString QString::fromRawData(const QChar *unicode, qsizetype size)`

*作用：使用原始的unicode数据构造一个QString*

- 构造的`QString`是没有通过**深拷贝**的，也就是说，如果原始数据修改或删除了，那么构造的`QString`也没有意义；

- 该函数可以用来**读写二进制Unicode字符串文件**，如果不需要修改文件内容，但为了方便又要用到`QString`的一些查询相关的接口，就可以使用这种方法构造一个`QString`；

- 如果修改或拷贝构造出来的字符串，则会导致深拷贝的发生，可以参考[`QString`的隐式共享](#implicitShared)；

- 该静态函数构造的`QString`字符串常常被用来只读和查询，因为这样非常快；

- 例如，利用正则表达式匹配内存中的字符串：

- ```c++
  QString str1_QS_t;
  //QT6以上不再支持隐式转换
  QChar unicode_QC_t[] = {
      (QChar)0x0071u, (QChar)0x0074u, (QChar)0x0020u,
      (QChar)0x0066u, (QChar)0x006fu, (QChar)0x0072u,
      (QChar)0x0020u, (QChar)0x0065u, (QChar)0x0076u,
      (QChar)0x0065u, (QChar)0x0072u, (QChar)0x0079u,
      (QChar)0x0074u, (QChar)0x0068u, (QChar)0x0069u,
      (QChar)0x006eu, (QChar)0x0067u
  };
  qsizetype size_qst_t = sizeof(unicode_QC_t) / sizeof(QChar);
  str1_QS_t = QString::fromRawData(unicode_QC_t, size_qst_t); //str1_QS_t === "qt for everything"
  static QRegularExpression pattern_QRE_s("\u0020");
  if (str1_QS_t.contains(pattern_QRE_s)) {
      qDebug() << "matched";
  }
  ```

- 如果原始数据被删除或者修改了，则不能保证`QString`更改，这样做危险！

- ```c++
  QChar *unicode_QC_p_t = new QChar[2];
  unicode_QC_p_t[0] = (QChar)0x0071u, unicode_QC_p_t[1] = (QChar)0x0074u;
  str2_QS_t = QString::fromRawData(unicode_QC_p_t, 2);
  qDebug() << "before deleted:" << str2_QS_t;
  delete[] unicode_QC_p_t; //don't do this;
  unicode_QC_p_t = Q_NULLPTR;
  qDebug() << "after deleted:" << str2_QS_t;
  ```

- 注意：如果原始数据最后不是以'\0'结尾，则创建的字符串也不会以'\0'结尾；

  - 如果调用`unicode()`函数就不会返回一个以'\0'结尾的字符串；
  - 如果调用`utf16()`函数则会发生深拷贝，然后返回以'\0'结尾的字符串；


#### fromStd*

有**4**个从C++标准字符串拷贝构造的字符串接口：

- `static QString QString::fromStdString(const std::string &str)`
- `static QString QString::fromStdU16String(const std::u16string &str)`
- `static QString QString::fromStdU32String(const std::u32string &str)`
- `static QString QString::fromStdWString(const std::wstring &str)`
- 以上函数都会返回一个源数据的拷贝副本；

#### fromUcs/Utf*

有**3**个静态函数从ISO-10646标准字符协议中构造`QString`：

- `static QString QString::fromUcs4(const char32_t *unicode, qsizetype size = -1)`
- `static QString QString::fromUtf8(const char *str, qsizetype size)`
  - *除此之外还有**4**个重载函数*
  - 其中有3个重载函数是在Qt6.0之后引入的：
    - `static QString QString::fromUtf8(QByteArrayView str)` <mark>[since 6.0]</mark>
    - `static QString QString::fromUtf8(const char8_t *str)` <mark>[since 6.1]</mark>
      - *This overload is only available when compiling in C++20 mode.*
    - `static QString QString::fromUtf8(const char8_t *str, qsizetype size)` <mark>[since 6.0]</mark>
      - *This overload is only available when compiling in C++20 mode.*
- `static QString QString::fromUtf16(const char16_t *unicode, qsizetype size = -1)`
  - 该函数会检查<u>字节序标记</u>（*Byte Order Mark*），如果没有，则会用<u>主机字节序</u>；
  - 与其他Unicode转换相比，此函数速度较慢，该函数会深拷贝一份unicode数据；
- 在以上函数中，某些函数如果没有指定字符串的大小，则都需要指定字符串结尾的符号；
- 在以上函数中，某些函数有些函数如果`size`小于0，则会使用strlen(str)代替`size`；
- 在以上函数中，如果源数据更改或者删除，则会发生深拷贝，参考[1.2 QString的隐式共享](#implicitShared)；

#### fromWCharArray

`static QString QString::fromWCharArray(const wchar_t *string, qsizetype size = -1)`

*作用：根据数据的格式自动判断编码并拷贝构造一个QString*

- 具体哪种编码方式得看`sizeof(wchar_t)`：
  - 如果为4字节，则`string`被解释为`UCS-4`；
  - 如果为2字节，则`string`被解释为`UTF-16`；
- 如果`size`未指定或者小于0，则需要字符串末尾有'\0'；

#### normalized

`QString QString::normalized(QString::NormalizationForm mode, QChar::UnicodeVersion version = QChar::Unicode_Unassigned) const`

*作用：根据给定的Unicode标准，返回用该标准规范化的字符串*

- `Unicode`最新标准：[Unicode 15.1.0](https://www.unicode.org/versions/Unicode15.1.0/)



## 4. 关于QString的宏

### 4.1 QStringLiteral

- `QStringLiteral(str)`宏的作用可以在编译期间通过指定的常量字符串`str`构建一个QString；

- 这样就不用在运行期间创建临时的QString，而且通过这个宏生成的QString会被存储在已编译的对象文件（`.obj/.o`文件）的**只读片段**中；

- 例如：

- ```c++
  //这是传入某个形参为只读的QString引用的函数
  void thisIsFunction(const QString &str) {
      xxxxx;
  }
  
  bool otherFunction() {
  	//1：
      if (thisIsFunction("abcde...中间很长很长...xyz")) {
          xxxxxx;
      }
      //2:
      if (thisIsFunction(QStringLiteral("abcde...中间很长很长...xyz"))) {
          xxxxxx;
      }
  }
  ```

- **第一种写法**会在运行的时候使用这一串原始字符串（字符串在常量区）生成一个QString，这可能消耗内存和时间，因为它涉及到内存分配和将数据复制/转换为QString的内部编码；

- 而**第二种写法**QString的内部数据将在**编译时**生成; 在运行时不会发生任何转换或分配。

- 使用`QStringLiteral`宏来代替双引号的纯C++字符串，可以显著加快从编译时已知的数据创建QString实例的速度。

- 其实源码也有体现：

- ```c++
  #define QT_UNICODE_LITERAL(str) u"" str
  
  #define QStringLiteral(str) \
      (QString(QtPrivate::qMakeStringPrivate(QT_UNICODE_LITERAL(str)))) \
      /**/
  ```

- 注释：有些编译器对包含`US-ASCII`字符集之外的字符的字符串进行编码时存在 bug。在这些情况下，确保字符串的前缀是`u`，除此之外就不需要带`u`；

- 可以参考`QByteArrayLiteral`也是同样的原理；



*Extensibility（扩展）*

1. 还有个**自定义字面量后缀**和该宏类似；

- `QString operator""_s(const char16_t *str, size_t size)` <mark>[since 6.4]</mark>
- 该`QString`是在编译时创建的，生成的字符串数据存储在已编译对象文件的只读段中。
- **重复**的文字可能具有**相同的只读内存**。
- 这个功能可以与`QStringLiteral`互换，但是当代码中出现许多字符串文字时可以节省输入。

关于一些类似的用法请参考：

- [StringLiterals Namespace | Qt Core 6.5.5](https://doc.qt.io/qt-6.5/qt-literals-stringliterals.html)



2. 可以用`QLatin1StringView`避免通过常量字符串构造出`QString`；

- 其实如果避免构造出一个QString（因为相比消耗大），可以用`QLatin1StringView`，它只是`const char*`的一个**非常薄**的包装器，不会有额外的开销；

- 如果强迫症或者内存优化爱好者（比如我）可以经常使用，因为自从Qt6后大多数都提供了`QLation1StringView`的重载，可以极度减少开销优化内存利用空间；

- Example：

- ```c++
  if (thisIsAString_QS_m == "taiga") { //通过const char*构造QString
      if (thisIsAString_QS_m == "taiga"_L1) { //没有额外开销
          xxxxx;
      }
  }
  ```

- 这样就避免了通过常量字符串构造出QString而占用额外开销了；



### 4.2 RestrictCast To/From

*QString提供了**3**个限制关于QString字符/字符串的自动转换的宏，保证类型安全*

#### QT_NO_CAST_FROM_ASCII

- 描述：

  - 禁用从8-bit字符串(`char *`)到`QString`(Unicode)的自动转换；
  - 禁用从8-bit字符类型(`char`和`unsigned char`)到`QChar`的自动转换；

- 其实也就是禁用隐式的自动转换，优化对程序员来说透明（*transparency*）的一些东西；

- 例如：

- ```c++
  //transparency
  if (str == "auto" || str == "extern" || str == "static" || str == "register") {
      xxxxx;
  }
  
  //opacity
  if (str == "auto"_L1 || str == "extern"_L1 || str == "static"_L1 || str == "register"_L1 {
      xxxxx;
  }
  ```

#### QT_NO_CAST_TO_ASCII

- 禁用从`QString`到8-bit字符串(`char *`)的自动转换；
- 同理参考`QT_NO_CAST_FROM_ASCII`；

#### QT_RESTRICTED_CAST_FROM_ASCII

- 禁用**大多数**从*源字符串*和*8-bit数据*到`QString`(Unicode)的自动转换；
- 但允许下面这样的自动转换 ；
  - 构造字符：`QChar(char)`
  - 以常量引用数组方式构造字符串：`QString(const char (&ch)[N])`
  - 重载`=`操作符：`QString::operator=(const char (&ch)[N])`
- 如果用这个宏，则**既能**保证像`QT_NO_CAST_FROM_ASCII`大多是都禁掉保证类型安全，又**不需要**用户代码用`QLatin1Char`、`QLatin1StringView`或类似的方法包装字符和字符串文字。
- 其实就是更方便使用，例如上面的例子如果全禁了虽然不透明，但是不方便使用；
- 注释：*Using this macro together with source strings outside the 7-bit range, non-literals, or literals with embedded NUL characters is undefined.*



### 4.3 Printable

*有**3**个用于临时将QString转换为C/C++风格的字符串*

**注意**：

- 返回的指针都是临时指向源字符串的，仅在当前语句块内有效；

  - 不能用于浅拷贝或者语句块外使用；

  - 例如：

  - ```c++
    QString str1_QS_t("abcd");
    //这里会有警告：Returning data of temporary QByteArray
    auto str2_c_tP = qPrintable(str1_QS_t);
    qDebug() << "1:" << str2_c_tP; //1: 错误的，该指针指向未知空间
    qDebug() << "2:" << qPrintable(str1_QS_t); //2: abcd
    ```

- 在多线程中，由于多线程可能会同时访问同一个字符串，因此需要注意使用临时指针的**线程安全**问题。

- 这三个宏也加入到`<QtGlobal>`头文件中了，可以全局使用；

#### qPrintable

- `const char *qPrintable(const QString &str)`
- 返回一个临时的指向源字符串数据的指针，等同于`str.toLocal8Bit().constData()`；
- 可以在` qDebug()，qInfo(), qWarning(), qCritical(), qFatal() `等用`%s`格式控制字符所需求`UTF-8`编码的时候使用；
- 注意该宏转换出来的字符串是用本地的8-bit编码，如果需要记录字符串可以使用`qUtf8Printable`宏；

#### qUtf16Printable

- `const wchar_t *qUtf16Printable(const QString &str)`

- 将str作为`const ushort *`返回，但将其转换为`const wchar *`以避免警告；

- 该宏相当于`str.utf16()`加上一些类型转换；

- 基本上该宏唯一的作用就是在`QString::asprintf()`中使用`%ls`的时候转换一下；

- 注意：该宏返回的不是一个严格合法的`const wchar*`；

- 同样返回的也是临时指针，用法：

- ```c++
  qWarning("%ls: %ls", qUtf16Printable(key), qUtf16Printable(value));
  ```

#### qUtf8Printable

- `const char *qUtf8Printable(const QString &str)`
- 参考`qPrintable`宏，该宏等同于`str.toUtf8().constData()`



------

该文章的参考链接：

> - [QString Class | Qt Core 6.5.5](https://doc.qt.io/qt-6.5/qstring.html)
> - [Implicit Sharing | Qt Core 5.15.16](https://doc.qt.io/qt-5/implicit-sharing.html)
> - [Reentrancy and Thread-Safety | Qt 6.5](https://doc.qt.io/qt-6.5/threads-reentrancy.html)
> - [Unicode - Wikipedia](https://en.wikipedia.org/wiki/Unicode)
> - [Ancient Greek Numbers (Unicode block) - Wikipedia](https://en.wikipedia.org/wiki/Ancient_Greek_Numbers_(Unicode_block))
> - [Container Classes | Qt Core 6.7.1](https://doc.qt.io/qt-6/containers.html)
> - [Container Classes | Qt 4.8](https://doc.qt.io/archives/qt-4.8/containers.html)
> - [QChar Class | Qt Core 6.7.1](https://doc.qt.io/qt-6/qchar.html)
> - [Unicode Character Categories (fileformat.info)](https://www.fileformat.info/info/unicode/category/index.htm)
> - [Unicode Character Categories (compart.com)](https://www.compart.com/en/unicode/category)
