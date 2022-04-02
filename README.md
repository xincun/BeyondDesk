# BeyondDesk
Desktop gadget collections / clock calendar yellow calendar memo weather time tray clock replacement translation

桌面小工具集合/时钟日历黄历便签相框天气时间/托盘时钟

详情参见我的CSND博客：https://blog.csdn.net/startl

技术特点：

主要功能皆开放源码，C++开发，支持winxp及以上的所有windows平台，也可以wine兼容的方式运行在linux桌面上。

UI采用了二次开发的Duilib，架构采用接口方式的插件架构，所有子功能皆为插件，可以动态删减。

观察者模式的异步事件架构，可以方便的注册C++类成员函数作为监听的回调通知。

Facade模式，unify统一管理各个子模块，各子模块唯一可见unify，隔离依赖，减少记忆成本。

功能：
主要在桌面上显示各种小工具，每种小工具皆为插件实现，灵活，伸缩性强。

包含桌面轮播相框、日历、天气、日期/时间倒计时、节日、便签、各种样式的时钟。

一个软件集合了多个软件的功能。

其它功能：
托盘时钟替换(支持显示天气）：
今日信息：
时间日期计算工具：
时间同步工具：
世界时间查看：

其它：
历史上的今天、多媒体工具、桌面小工具管理、网络代理等

协议：
本代码基于GPL协议，你可以自由使用本代码，但如果使用了代码，请在发行版中做出声明。
