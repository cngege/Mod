# Minecraft  C++ Mod

### **功能更新**

- **当前已实现功能**
  - 抗击退（只有在长按**CTRL键**时抗击退）
  - 强制显示坐标
  - 瞬间破坏（按下 **SHIFT+CTRL** 时瞬间破坏）
  - 下落无伤害（目前仅本地房间）
  - 碰撞箱（**大写锁定**=开启，反之关闭）
  - ImGui绘制UI绘制文字 渲染面板 （**INSERT**）
  - 向前挺进一格 可穿过一层墙（**快捷键F2**）
  - 坐标点,可记录(**CTRL+F3**)、传送（**SHIFT+F3**）
  - 好友检查，（**F4**）开启后通过玩家名称判断是否是好友，来拦截攻击（防止误伤），HitBox也不对好友产生影响
  - 自动疾跑模块 (**F6**) 默认开启，开启后，玩家移动即会自动奔跑
  - HiveTreasurePos Hive起床中使用（**F7**）开启后在小宝箱附近下蹲,将小宝箱传送至玩家位置
  - FastViewPerspective 长按快捷键显示第二人称视野,松开恢复 （**（R）F9**）
  - 点击显示实体名称和血量（**CTRL+F10**）
  - 攻击距离 = 7 （**SHIFT+F10**）
- **将要更新的功能**
  - ImGui面板UI用中文
- **可能更新的功能**
  - 发送消息翻译成指定的语言(**缺少http的翻译api**)
  - 下载字体到本地 作为UI字体使用,UI字体可选择
  - ImGUi配置等选项信息能够保存本地
  - 命令系统
  - 配置保存本地,模块开启关闭状态保持本地
  
### **已经解决的和未解决的BUG**
- **已解决**
  - 日志乱码

- **未解决**
  - cpp-httplib 库无法进行Https请求, 因为定义`#define CPPHTTPLIB_OPENSSL_SUPPORT`宏后dll无法注入
  - 被断开连接时如果开启了`RenderHealth`模块,且正在显示玩家信息则会崩溃
  
### **项目开发**
- **工具**
  - Visual Studia 2022

### **技术参考（学习）**
- **Horion-Open-SRC**
  - https://github.com/NRGJobro/Horion-Open-SRC/
- **YT Coding Internal Client**
  - https://www.youtube.com/playlist?list=PL9mZGNEJquY77cTy1eNOLs12BsVK5zA-P

### **外部库**
- **nlohmann::json** （暂未用到）
  - https://github.com/nlohmann/json
  
- **cpp-httplib** （已加入代码中 暂未使用）
  - https://github.com/yhirose/cpp-httplib

- **minhook**
  - https://github.com/TsudaKageyu/minhook

- **Minecraft-DX12-Hook** (非常感谢)
  - https://github.com/NRGJobro/Minecraft-DX12-Hook
