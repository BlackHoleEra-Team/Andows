# Andows

> A user-space compatibility layer, run lightweight Android APKs on Windows, without virtualization.

Andows 是 **Android 界的 Wine**:像 Wine 在 Linux 上运行 Windows 程序一样,Andows 在 Windows 上以**纯用户态**方式运行 Android APK——不依赖 Hyper-V、不启动虚拟机,整个 Android 用户态栈(ART、System Server、SurfaceFlinger 等)作为普通 Windows 进程运行。

## 与 WSL2 / WSA 的区别

| | WSL2 / WSA | Andows |
|---|---|---|
| 底层 | Hyper-V 虚拟机 + 定制 Linux 内核 | 纯用户态 API 翻译 |
| 内核依赖 | 需要虚拟化 | 完全不需要 |
| 指令 | 虚拟机内同架构 | 用户态(起步仅支持 x86_64 APK) |
| 模式 | 跑整套 Android 镜像 | 把 Android 用户态移植为 Windows 原生进程 |

## 核心思想

```
APK
 ↓
Android 用户态栈 (ART · System Server · Zygote 替代 · Binder 模拟)
 ↓
Andows 翻译层 (HAL → Win32)
   ├─ 图形  SurfaceFlinger → DXGI / ANGLE (OpenGL ES → D3D11)
   ├─ 音频  AudioFlinger   → WASAPI
   ├─ 输入  InputManager   → Win32 消息 / RawInput
   └─ 媒体  MediaCodec     → Media Foundation / ffmpeg
 ↓
Windows API (Win32 · DXGI · WASAPI · NTFS)
```

窗口化不是难点:Android 12L+ 本身支持 freeform / 桌面窗口模式,可借鉴其窗口逻辑。真正的难点在系统底座——指令架构、Zygote fork 替代、Binder 用户态模拟、HAL 设备映射。

## 技术栈

| 层 | 选型 |
|---|---|
| 核心语言 | C++(移植 AOSP 用户态)+ Java(AOSP 自带 framework,非手写) |
| 运行时 | ART host(x86_64) |
| 系统服务 | AOSP 的 System Server / SurfaceFlinger / AudioFlinger |
| IPC | libbinder + 用户态 binder 模拟 |
| 图形 | ANGLE(OpenGL ES → D3D11)+ DXGI 交换链 |
| 音频 | WASAPI |
| 宿主壳 | 纯 Win32 窗口 + 原生控件(壳原生,Android 画面整块贴图) |
| 构建 | Docker 交叉编译 AOSP → Windows 原生 DLL/EXE |

## 目录结构

```
src/
  host/        Windows 宿主壳(Win32 窗口 + 原生控件)
  andows/      Android 用户态移植层(规划中)
```

## 构建

需要 CMake 3.16+ 与 MSVC(Visual Studio 2022)。

```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Debug
```

构建产物: `build/src/host/Debug/andows_host.exe`

## 状态

项目处于起步阶段。路线图见 [TODO.md](TODO.md)。

## License

见 [LICENSE](LICENSE)。
