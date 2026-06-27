# GXDE SNI Server

## Introduction | 介绍

This is GXDE SNI Server, its source code is adapted from Plasma workspace v6.3.4, commit id `c46ef35d7976e3d70eca56e506469cd132493a14`. This software provides `StatusNotifierWatcher` support that provide the tray icons.



这是GXDE SNI Server，基于Plasma workspace v6.3.4，Commit ID`c46ef35d7976e3d70eca56e506469cd132493a14`。本程序提供`StatusNotifierWatcher` 支持以让托盘图标工作。



## Note | 一些笔记

Please note that this shim only supports Wayland because `deepin-daemon` handles SNI server correctly under X11.



请注意本程序仅支持Wayland会话，因为在X11上`deepin-daemon`能正常工作，不需要干预。



Original code has been modified so the this package is no longer depending on `KDED`.



原Plasma Workspace的代码已被修改，移除了对`KDED`的依赖。



## Compiling | 编译

```bash 
# Generate package · 生成DEB包
$ chmod a+x ./build-deb
$ ./build-deb -d

# Cleanup when you copied away the artifacts · 备份好DEB包后执行清理
$ ./build-deb -c
```





## Original README | 原版README

Original README could be found [here](./README.original.md).



原版README可以在[这里](./README.original.md)找到。



## Copying | 许可证

Copyright (C) 2026 CharOfString

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
