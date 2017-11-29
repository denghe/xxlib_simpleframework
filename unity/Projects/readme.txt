所有源代码都在 Sources 下, 几个平台的项目只是引用它

Android 生成办法: 命令行进 Android 目录, 执行 ndk-build  后于 libs 目录找到 lib***********.so

iOS 生成办法: 
macos 下进 iOS 用 xcode 打开项目文件. 先 WIN + B 生成, 再选 Archive 打包
显示 success 后并没有任何 "包管理" 窗口弹出, 此时需要去生成目录中找出来.
左侧菜单 Products 中定位到 lib***********.a , 从它的属性面板中点击定位文件图标, 
在与 .........Products 同级的 Intermediates.noindex/ArchiveIntermediates/***********lib/Interm........FilesPath/UninstalledProducts/iphones/ 下面可找到  lib***********.a
复制出来即可

Windows 生成办法: 用 vs2017 15.4+ 打开并选择 Release 编译. 后于 x64/Release 下找到 ***********.dll  