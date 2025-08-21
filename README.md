# 前置要求

- **编译器**: 支持C++17的编译器 (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake**: 3.14或更高版本
- **Git**: 用于克隆子模块

# 克隆项目

由于项目包含Git子模块，请使用以下命令进行完整克隆：

```bash
# 方法1: 递归克隆（推荐）
git clone --recursive https://github.com/your-username/Convention-CPP.git
cd Convention-CPP

# 方法2: 先克隆主项目，再初始化子模块
git clone https://github.com/your-username/Convention-CPP.git
cd Convention-CPP
git submodule update --init --recursive
```

# 更新子模块

如果子模块有更新，使用以下命令：

```bash
# 更新所有子模块到最新版本
git submodule update --remote

# 或者更新特定子模块
git submodule update --remote Convention/[nlohmann]
git submodule update --remote Convention/[Static]
```
