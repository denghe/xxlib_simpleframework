基于 libuv, 适合用 C# 开发服务器端的基础框架

开发需要 vs2017( 升级到最新 ) 并勾安 CLI/C++ 旧crt 支持和 win81 sdk

技术讨论群: 125081098


目录大结构:
1. 生成器
2. 模板
3. 依赖库
4. 示例

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

xxuvlib
为 libuv 封装本体, 可跨平台编译为各种 dll so a 啥的, 支持.net core

xxlib_csharp
libuv 的 C# 端, 含各种周边工具类, 命名空间为 xx

rpc_xxxxxxx
最新服务端项目实用示例


