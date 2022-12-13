# Minecraft  C++ Mod

### **功能更新**

- **当前已实现功能**
  - 下落无伤害（目前仅本地房间）
  - 碰撞箱（**大写锁定**=开启，反之关闭）
  - 空气跳（可以在空中无限跳跃）
  - 瞬间破坏（按下 **SHIFT+CTRL** 时再破坏将瞬间破坏）
  - 向前挺进一格 可穿过一层墙（**F2**开启关闭,**鼠标中键**触发）
  - 强制显示坐标
  - 坐标点,可记录(**CTRL+F3**)、传送（**SHIFT+F3**）
  - ImGui绘制UI绘制文字 渲染面板 （**INSERT**）
  - 抗击退（只有在开启后长按**CTRL键**时抗击退,开关快捷键:**CTRL+F4**）
  - 好友检查，（**SHIFT+F4**）开启后通过玩家名称判断是否是好友，来拦截攻击（防止误伤），HitBox也不对好友产生影响
  - 自动疾跑模块 (**Ctrl+F6**) 默认开启，开启后，玩家移动即会自动奔跑
  - 玩家雷达（**SHIST+F6**）默认开启,开启后在右下角显示一个Box,以对应颜色显示所有玩家（能够触发Tick的玩家）
  - HiveTreasurePos Hive起床中使用（**F7**）开启后在小宝箱附近下蹲,将小宝箱传送至玩家位置
  - FastViewPerspective 长按快捷键显示第二人称视野,松开恢复 （**（鼠标侧键）F9**）
  - 攻击距离 = 7 （**SHIFT+F10**）
  - 点击显示实体名称和血量（**CTRL+F10**）
- **已经实现的其他功能**
  - ImGui面板UI用中文
  - ImGUi配置等选项信息能够保存本地
  - 配置保存本地,模块开启关闭状态保持本地
  - 模块的参数（比如攻击距离，碰撞箱大小）可以在UI中调节,且可保存到本地配置
  - 下载字体到本地 作为UI字体使用,UI字体暂不可选择
- **将要更新的功能**
  - NoFound
- **可能更新的功能**
  - 发送消息翻译成指定的语言(**缺少可翻译api**)
  - 命令系统（**等先实现在本地客户端上显示聊天消息的功能**）
### **已经解决的和未解决的BUG**
- **已解决**
  - 日志乱码
  - 被断开连接时如果开启了`RenderHealth`模块,且正在显示玩家信息则会崩溃
- **未解决**
  - 玩家雷达功能可能由于不正常的退出存档、房间导致不再显示
### **项目开发**
- **工具**
  - Visual Studia 2022

### **技术参考（学习）**
- **Horion-Open-SRC**
  - https://github.com/NRGJobro/Horion-Open-SRC/
- **YT Coding Internal Client**
  - https://www.youtube.com/playlist?list=PL9mZGNEJquY77cTy1eNOLs12BsVK5zA-P

### **外部库**
- **nlohmann::json**
  - https://github.com/nlohmann/json

- **minhook**
  - https://github.com/TsudaKageyu/minhook

- **Minecraft-DX12-Hook** (非常感谢)
  - https://github.com/NRGJobro/Minecraft-DX12-Hook

- **Dear ImGui**
  - https://github.com/ocornut/imgui
