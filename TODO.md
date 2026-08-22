# Andows TODO

> Android 界的 Wine:纯用户态把 Android APK 跑在 Windows 上。
> 起步阶段仅支持 x86_64 APK,路线按里程碑推进。

## 里程碑 0 — 项目骨架 (进行中)

- [x] 初始化仓库结构
- [x] Win32 宿主壳最小 demo(窗口 + 原生菜单 + 占位画布)
- [x] CI 基础配置(GitHub Actions:远程构建 ART host,产物下载回本地)
- [ ] 用 CI 产物跑通 ART host 基线(验证 dalvikvm / dex2oat 可用)

## 里程碑 1 — 纯 Java 应用

> 目标:能在 Windows 上跑不依赖 native 库、只用系统内置组件的 APK。

- [ ] 拉取并构建 AOSP 的 ART host(x86_64)
- [ ] 配置 Android framework(java)随 ART host 编译
- [ ] 最小 APK 安装器(解析 manifest、签名)
- [ ] Activity 启动器:从宿主壳拉起 Activity
- [ ] 宿主壳接入:Android 画面经 DXGI 贴进 Win32 窗口客户区
- [ ] 输入转发:Win32 键盘/鼠标 → Android InputManager

## 里程碑 2 — x86_64 APK 常见应用

> 目标:支持带 native 库(x86_64)的主流 APK,Andows 成为"真货"。

- [ ] 用户态 binder 模拟(libbinder 可用)
- [ ] Zygote fork 替代方案(Windows 无 fork)
- [ ] System Server 作为 Windows 原生进程运行
- [ ] SurfaceFlinger 移植 + 输出到 DXGI
- [ ] AudioFlinger 移植 + WASAPI 输出
- [ ] 文件系统映射:APK 数据目录 → NTFS
- [ ] 系统属性 / 权限框架适配

## 里程碑 3 — GPU 与多媒体应用

> 目标:游戏、视频、相机类应用可跑。

- [ ] ANGLE(OpenGL ES → D3D11)接入
- [ ] Vulkan → D3D12 方案评估(zink / 自研)
- [ ] MediaCodec → Media Foundation / ffmpeg
- [ ] 传感器 / 相机 HAL 映射(WinRT)
- [ ] 多窗口 / 桌面模式(借鉴 Android 12L 窗口逻辑)

## 里程碑 4 — ARM APK 全兼容

> 目标:任何 .so 无需重编译直接跑。

- [ ] ARM 指令翻译层评估(QEMU 用户态模式 / 自研)
- [ ] ARM → x86 的 syscall / HAL 联动
- [ ] 性能优化

## 长期 / 开放问题

- [ ] 应用商店 / 包源管理
- [ ] 与 Windows 剪贴板、拖拽、通知集成
- [ ] 兼容性矩阵(按 APK 类型标注可运行性)
