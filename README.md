基于 libuv C++ 轮子封装 再进行 CLI 封装的, 适合用 C# 开发服务器端的基础框架

运行需要 vs2017, 打开 all.sln, 同时设置两个 test 为启动项目

技术讨论群: 125081098


目录大结构:
1. 生成器
2. 模板
3. 依赖库
4. 测试项目

生成器通过对模板的读取解析, 产生具体项目代码, 向指定生成目录覆盖写入

目录结构:

db
数据库操作代码生成器的输出目录项目

dbgen
数据库操作代码生成器, 生成目标路径当前为 server_db 项目下

dbgen_template_DB
数据库模板, 用于填写所有用到的 SQL 语句, 以及描述结果集的长相

gen_library
各种生成器所依赖的库

pkg
通信包生成器的输出目录项目

pkggen
通信包生成器, 生成目标路径当前为 pkg 项目下

pkggen_template_PKG
通信包结构描述模板

test_cpp1
chat room 测试客户端 C++ 实现

test_csharp1
chat room 测试服务器端 C# 实现

xxlib_csharp
为 c# 项目服务的库, 命名空间为 xx

xxuvlib_csharp
为 c# 项目服务的库, 基于 xxqp\xxlib_cpp 的 uv 相关代码, 用 cli/c++ 封装而成

xxuvlibex_csharp
为 c# 项目服务的库, 引用 xxuvlib_csharp, 继承 Peer 相关类以提供 send pkg 等便利功能


