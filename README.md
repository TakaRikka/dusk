Dusklight 是对《塞尔达传说：黄昏公主》的逆向工程重实现，力求在还原原版体验的同时提供全新的画质选项、增强功能和自定义工具。

---

## 本仓库特性

本仓库（[snnh/dusk](https://github.com/snnh/dusk)）在 [TwilitRealm/dusklight](https://github.com/TwilitRealm/dusklight) 基础上增加了以下增强功能。当前分支 `cn-enhancements` 不只包含 i18n 改动，也包含中文输入、便携模式、TPHD 资源适配和面向中文发行包的构建修复。欢迎star本项目

### 🌐 国际化与中文本地化

| 改进 | 说明 |
|------|------|
| 完整 i18n 框架 | 基于 XML 的多语言翻译系统，支持 en / fr / ja / zh-cn |
| CJK 字体集成 | 内置 HarmonyOS Sans 字体，完美显示中日韩文字 |
| 中文翻译 | 设置、控制器配置、成就、图形调谐器等界面已全面中文化 |
| 中文姓名键盘 | 新增中文姓名输入键盘，支持中文存档名输入 |
| 翻译检查工具 | `tools/check_i18n_tokens.py` 自动检测缺失的翻译 token |
| UI 适配 | 菜单栏、叠加层、预启动界面、传送界面、存档编辑器等均已接入翻译系统 |

### 📦 便携模式

| 改进 | 说明 |
|------|------|
| `--portable` 参数 | 启动时添加该参数，设置/存档/日志/缓存保存在可执行文件旁 |
| `portable.txt` 标记 | 在可执行文件旁创建此文件可自动启用便携模式 |
| 数据目录隔离 | 所有用户数据存储在 `data/` 目录，不污染系统路径 |
| 初始管线缓存 | 便携模式会在首次启动时尝试复制 `initial_pipeline_cache.db` 到 `data/pipeline_cache.db` |

### 🎨 TPHD 资源适配

> [!WARNING]
> TPHD 资源加载仍属于实验性功能。旧版本在 Windows 下遇到包含中文的 HD `content` 路径可能会启动后闪退；如果路径已经保存到配置里，见下方“TPHD 闪退恢复”。

| 改进 | 说明 |
|------|------|
| HD 内容目录 | 设置中可指定 TPHD `content` 目录，用于尝试加载高清资源 |
| `pack.gz` 纹理包 | 支持读取 Wii U 版 `pack.gz` 中的 GTX 纹理并注册为替换纹理 |
| 字体纹理包 | 支持 `.bfn` 字体对应的 `.bfn.gtx`，用于高清字体纹理替换 |
| 布局资源处理 | 对 `res/Layout` 与 `res/LayoutRevo` 的资源路径做兼容处理 |
| LOS 表 | 支持读取高清内容中的 LOS 数据，改善 HD 分支相关资源行为 |

### 🛠 构建与发行修复

| 改进 | 说明 |
|------|------|
| GitHub Actions | 修复中文分支合并 HD 改动后的 CI 编译问题 |
| Aurora 适配 | 同步并修复 Aurora/RmlUi 翻译回调相关补丁 |
| Windows 发行包 | 面向中文便携发行包整理数据目录和缓存初始化逻辑 |

---

## 中文增强版使用说明

### 切换中文界面

在设置中将界面语言设置为 **简体中文**。配置文件中对应字段为：

```json
"backend.uiLanguage": "zh-cn"
```

### 启用便携模式

任选其一：

- 启动时传入 `--portable`
- 在可执行文件同目录创建 `portable.txt`

启用后，用户数据会写入可执行文件旁的 `data/` 目录。

### 使用 TPHD 高清资源

将 TPHD 的 `content` 目录路径填入设置中的高清内容目录。启用后，程序会尝试从该目录加载可用的 HD archive、`pack.gz` 纹理包、字体纹理包和相关资源。

建议先用可恢复的方式测试资源目录：不要同时启用跳过预启动界面，确认可以稳定进入游戏后再作为常用配置。

### TPHD 闪退恢复

如果配置里保存了有问题的 TPHD 路径，启动时可能会再次自动加载并闪退。无需删除整个配置文件，可以任选一种方式临时禁用 TPHD：

- 启动时传入 `--cvar backend.hdContentPath=`
- 启动前设置环境变量 `DUSK_DISABLE_TPHD=1`

成功进入程序后，在设置中清空或改正高清内容目录。

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

### 1. Dump your game

You must dump your own copy of the game, please see [this article](https://wiki.dolphin-emu.org/index.php?title=Ripping_Games) for instructions. After dumping, you can use a program like [Dolphin](https://dolphin-emu.org/) or [nodtool](https://github.com/encounter/nod/releases) to convert the `.iso` to a `.rvz` to save space.

Currently, only the GameCube USA and EUR releases are supported. Support for other versions of the game is planned in the future.

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
