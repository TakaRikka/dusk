Dusklight 是对《塞尔达传说：黄昏公主》的逆向工程重实现，力求在还原原版体验的同时提供全新的画质选项、增强功能和自定义工具。

---

## 本仓库特性

本仓库（[snnh/dusk](https://github.com/snnh/dusk)）在 [TwilitRealm/dusklight](https://github.com/TwilitRealm/dusklight) 基础上增加了以下增强功能：

### 🌐 国际化与中文本地化

| 改进 | 说明 |
|------|------|
| 完整 i18n 框架 | 基于 XML 的多语言翻译系统，支持 en / fr / ja / zh-cn |
| CJK 字体集成 | 内置 HarmonyOS Sans 字体，完美显示中日韩文字 |
| 中文翻译 | 设置、控制器配置、成就、图形调谐器等界面已全面中文化 |
| 中文姓名键盘 | 新增中文姓名输入键盘，支持 PAL 区域输入 |
| 翻译检查工具 | `tools/check_i18n_tokens.py` 自动检测缺失的翻译 token |
| UI 适配 | 菜单栏、叠加层、预启动界面、传送界面等均已接入翻译系统 |

### 📦 便携模式

| 改进 | 说明 |
|------|------|
| `--portable` 参数 | 启动时添加该参数，设置/存档/日志/缓存保存在可执行文件旁 |
| `portable.txt` 标记 | 在可执行文件旁创建此文件可自动启用便携模式 |
| 数据目录隔离 | 所有用户数据存储在 `data/` 目录，不污染系统路径 |

---

# **原仓库 README**

<div align="center">
  <img src="res/logo.png" alt="Logo" width="640">

  <p align="center">
    <a href="https://twilitrealm.dev">Official Website</a>
    •
    <a href="https://discord.gg/6NpMhefCK9">Discord</a>
  </p>
</div>

# Overview

Dusklight is a reverse-engineered reimplementation of Twilight Princess.

It aims to be as accurate as possible to the original while also providing new options, enhancements, and tools to customize your experience.

# Setup

> [!IMPORTANT]
> Dusklight does *not* provide any copyrighted assets. You must provide your own copy of the original game.

> [!IMPORTANT]
> At a minimum, Dusklight requires a GPU with support for either D3D12, Vulkan, or Metal. Your experience with specific hardware, operating systems, and drivers may vary. In particular, older Intel iGPUs have a high likelihood of incompatibility. We are also aware of a number of issues on devices with Adreno GPUs and are working to resolve them.

### 1. Verify your dump

First, make sure your dump of the game is clean and supported by Dusklight. You can do this by checking the SHA-1 hash of your dump against this list of supported versions:

| Version      | SHA-1 hash                                 |
|--------------| ------------------------------------------ |
| GameCube USA | `75edd3ddff41f125d1b4ce1a40378f1b565519e7` |
| GameCube EUR | `2601822a488eeb86fb89db16ca8f29c2c953e1ca` |

*Support for other versions of the game is planned in the future.

### 2. Download [Dusklight](https://github.com/TwilitRealm/dusklight/releases)

### 3. Setup the game
**Windows / macOS / Linux**
- Extract the .zip file
- Launch Dusklight
- Press **Select Disc Image** and provide the path to your supported game dump
- Press **Play**!

**iOS**
- Follow the [iOS setup guide](docs/ios-install-altstore.md)

**Android**
- Install the Dusklight APK
- Launch Dusklight
- Press **Select Disc Image** and provide the path to your supported game dump
- Press **Play**!

# Building

If you'd like to build Dusklight from source, please read the [build instructions](docs/building.md).

Pull requests are welcomed! Note that we do not accept contributions that are primarily AI-generated and will close your PR if we suspect as much. Please also see the [code conventions](docs/code-conventions.md).

# Credits

Special thanks to the [TP decompilation](https://github.com/zeldaret/tp) team, the GC/Wii decompilation community, the [Aurora](https://github.com/encounter/aurora) developers, the [TP speedrunning community](https://zsrtp.link), and all [contributors](https://github.com/TwilitRealm/dusklight/graphs/contributors).

<br/>
<div align="center">
    <a href="https://github.com/encounter/aurora">
        <img src="assets/aurora-powered.png" alt="Powered by Aurora" width="800">
    </a>
</div>
