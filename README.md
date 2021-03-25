## LAB 0

1.环境我用的是自己搭建的Ubuntu虚拟机，版本20，工具链搭建十分简单，比当时我搭建xv6的工具舒服了多少倍，不过也可能是我踩坑踩出来了。

2.除了lab的pdf之外，一定要看sponge的documentary，在里头可以找到要用的TCPsocket。

3.Lab本身很普通，也不难，但出了个非常让我预料不到的bug，当我开了代理的时候，vscode单步调试正常出结果，而vscode直接运行和bash直接执行都没有输出，简直玄幻。后来意外关掉代理后结果正常，我被整吐了🤮。

4.发现Sponge那个代码文档是自动生成的，发现了注释生成文档工具的强大。。。自己试了试生成了个文档，Doxygen很赞！

5.终于结束了，我是弱智，_container.cend() + n的错都能写的出来，吐了。

6.另外测试webget别忘了关vpn，不然GET不到网页内容，时好时坏，醉了。

---

## LAB 1

1.不知道为啥vscode调试的时候放的断点会自动移动到其他位置，不够凑合用吧，上一次捣鼓gdb把我整出阴影了，写完这个lab再看看lldb和日志这两个东西。

2.另外我用了半年github到现在终于会使用git merge了。。。还是因为commit没绿点才发现的，不管怎么样，git技术提升是可喜可贺的。

3.调试代码真的无敌了，我这是面向调试代码编程啊。。。我现在彻底懂了自动测试代码才是最重要的，开发人员也是人，不能肯定写的代码一定没有bug的。

---

## LAB 2

1.刚开始就发现之前lab1写的有bug，当_output这个字节流满的时候，没能写入却移动了_received和_unacceptable这两个边界，真刺激，感谢测试代码。

2.第一个部分没啥难度，就是总是把UINT32_MAX当成2^32,其实是2^32-1,别忘了换成uint64_t后+1。

3.在.vscode的c_cpp_properties.json文件(或者直接通过设置里面找)，来设置c或者c++的版本，自从把gnu++14变成gnu++17，终于没有一些旧版本的报错了

4.真就是面向测试编程咯，🤮，不过难度确实比lab1低了一些，不过感觉部分细节理解并不是很好，不过可能是我没看课程，直接看材料写代码的原因吧。

5.过程中卡壳的时候不乏参考了一下别人的实现，不过有点搞笑的是一个人一种实现，结果因为之前lab的实现并不同还被坑了。。。第一次遇到参考反作用。。。

---

## LAB 3

1.lab3难度比前面确实大了太多，不过最主要问题是太执着于细节导致无法开始和不注意细节就到处bug，感觉挺无语的，文档读完后仍然一脸愣逼，借鉴下别人的做法还因前面lab的实现不同被坑，不过最后过了。

2.感觉gdb配合在程序中cout输出无敌，实在不行跑下正确的代码验证下思路，有些地方处理方式不同，真不是我们想的问题。

---

## LAB 4

1.刚读完给的材料，看了看TCP的FSM图的时候，自信慢慢，三次握手四次挥手已经不能阻挡我的。然后就开始自闭了，完全没有思路，而且发现理解的FSM状态图怎么还有一堆非典型变化，🤮。

2.花了6h，目前做到一半，为了简化确实没全按照FSM图(说实话我也没这水平)，吐了。

3.github还是ssh好用，不用输密码。

4.终于因为编译产生的东西太大而强制学习.gitignore了。。。羞耻🤮

5.我不应该小看四次挥手的，实现有点难。。。另外，零窗口探测的情况我之前也没发现。。。

6.修改了好一段时间改到就剩一个测试没过，然后修改好了这个测试过了又多了三个timeout，不过那三个测试和之前的测试不一样，是实际连网络的，也不大好调试，也许是网络原因(应该不是，逃XD），不过已经过了98%，而且tcp也可以用用了，已经满足了。

7.不用担心那不知道发生在上面边角上的死锁情况，完全没必要，lab本身是好lab，只不过没TA容易有误区没办法，另外用CS144TCPSocket是完全没问题的，make check_webget通过了。

8.一会webget改用我的tcp实现，再找个http网页，用用给的socket，加点文件读写，正式完成c++课设。

9.终于完成了tcp的5个lab，哈哈，总共用时6天，终于不用那么肝lab了，作为课设也赶上了时间。至于剩下的两个lab看了看发现都是独立的而且相对lab4很简单，打算过两天随心情再做，之后会伴随着一段时间的自顶向下的大部分习题，把计网和xv6的操作系统和cs61b的java lab收尾，估计是一段比较清闲但又有点无聊的时光。

---

## 下面是原LAB README.md

    $ make -j$(nproc)

To test (after building; make sure you've got the [build prereqs](https://web.stanford.edu/class/cs144/vm_howto) installed!)

    $ make check

The first time you run `make check`, it will run `sudo` to configure two
[TUN](https://www.kernel.org/doc/Documentation/networking/tuntap.txt) devices for use during
testing.

### build options

You can specify a different compiler when you run cmake:

    $ CC=clang CXX=clang++ cmake ..

You can also specify `CLANG_TIDY=` or `CLANG_FORMAT=` (see "other useful targets", below).

Sponge's build system supports several different build targets. By default, cmake chooses the `Release`
target, which enables the usual optimizations. The `Debug` target enables debugging and reduces the
level of optimization. To choose the `Debug` target:

    $ cmake .. -DCMAKE_BUILD_TYPE=Debug

The following targets are supported:

- `Release` - optimizations
- `Debug` - debug symbols and `-Og`
- `RelASan` - release build with [ASan](https://en.wikipedia.org/wiki/AddressSanitizer) and
  [UBSan](https://developers.redhat.com/blog/2014/10/16/gcc-undefined-behavior-sanitizer-ubsan/)
- `RelTSan` - release build with
  [ThreadSan](https://developer.mozilla.org/en-US/docs/Mozilla/Projects/Thread_Sanitizer)
- `DebugASan` - debug build with ASan and UBSan
- `DebugTSan` - debug build with ThreadSan

Of course, you can combine all of the above, e.g.,

    $ CLANG_TIDY=clang-tidy-6.0 CXX=clang++-6.0 .. -DCMAKE_BUILD_TYPE=Debug

**Note:** if you want to change `CC`, `CXX`, `CLANG_TIDY`, or `CLANG_FORMAT`, you need to remove
`build/CMakeCache.txt` and re-run cmake. (This isn't necessary for `CMAKE_BUILD_TYPE`.)

### other useful targets

To generate documentation (you'll need `doxygen`; output will be in `build/doc/`):

    $ make doc

To lint (you'll need `clang-tidy`):
