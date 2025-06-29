# Convention-CPP Runtime 模块说明

本文档描述了基于 `Convention-Template` 为 `Convention-CPP` 实现的Runtime模块。

## 📁 模块结构

### 完整实现的模块

#### 1. **File.hpp** - 文件操作工具
- **ToolFile类**: 面向对象的文件系统操作
- **完整功能**: 文件读写、路径操作、目录管理
- **链式操作**: 支持流畅的API调用
- **跨平台**: 基于std::filesystem

**核心功能**:
- 文件存在性检查、类型判断
- 文本/二进制文件读写
- 目录遍历和管理
- 路径组合操作符 `|`
- 文件对话框接口（需平台实现）

#### 2. **GlobalConfig.hpp** - 全局配置管理
- **GlobalConfig类**: 配置文件管理和日志系统
- **ProjectConfig类**: 项目级配置特化
- **JSON支持**: 基于nlohmann/json
- **完整功能**: 配置读写、日志记录

**核心功能**:
- 键值对配置管理
- 自动配置文件生成
- 时间戳日志系统
- 迭代器支持（foreach循环）
- 文件操作集成

#### 3. **Math.hpp** - 数学工具库
- **完整实现**: 常用数学函数和工具
- **模板化**: 支持多种数值类型
- **高性能**: 基于标准库优化

**核心功能**:
- 数学常量和基础运算
- 三角函数、指数对数函数
- 插值函数（Lerp, InverseLerp）
- 随机数生成器
- 浮点数比较工具

### 接口实现的模块

#### 4. **Web.hpp** - 网络工具模块
- **ToolURL类**: URL解析和操作
- **HttpClient类**: HTTP客户端接口
- **接口状态**: 完整API设计，需HTTP库实现

**设计功能**:
- URL验证和属性解析
- 文件类型检测
- HTTP GET/POST方法
- 异步下载功能
- URL编码/解码

**实现要求**: 需要libcurl或类似HTTP库

#### 5. **Plugins.hpp** - 插件系统
- **PriorityQueue类**: 完整的优先队列实现
- **IPlugin接口**: 插件基类定义
- **PluginManager类**: 插件管理器接口

**设计功能**:
- 动态插件加载/卸载
- 插件依赖管理
- 平台特定插件支持
- 事件系统集成
- 优先级队列调度

**实现要求**: 需要动态库加载机制

#### 6. **EasySave.hpp** - 序列化系统
- **EasySave类**: 主要序列化接口
- **EasySaveSettings**: 配置结构
- **接口状态**: 基础JSON实现，完整功能需扩展

**设计功能**:
- 多格式序列化（JSON/Binary）
- 加密和压缩支持
- 缓存系统
- 备份/恢复机制
- 键值管理

**实现要求**: 需要加密库和压缩库

### 继承的模块

#### 7. **Config.hpp** - 基础配置（已存在）
- **完整实现**: 平台判断、字符串工具、内存管理
- **基础设施**: 为其他模块提供基础功能

#### 8. **Architecture.hpp** - 核心架构（已存在）
- **完整实现**: 依赖注入、事件系统、时间线管理
- **设计模式**: 单例、依赖注入、观察者模式

## 🔧 编译配置

### 依赖库
- **标准库**: C++17 std::filesystem
- **可选依赖**: nlohmann/json（配置管理）
- **待实现依赖**:
  - libcurl（Web模块）
  - OpenSSL/AES库（EasySave加密）
  - zlib（EasySave压缩）

### CMake配置
已在主CMakeLists.txt中添加依赖查找：
```cmake
find_package(nlohmann_json QUIET)
```

## 🚀 使用示例

### 文件操作
```cpp
#include "Convention/[Runtime]/File.hpp"
using namespace Convention;

ToolFile file("data/config.txt");
if (file.Exists()) {
    auto content = file.LoadAsText();
    // 处理内容
}

// 链式操作
ToolFile dataDir("assets");
auto configFile = dataDir | "config" | "settings.json";
```

### 配置管理
```cpp
#include "Convention/[Runtime]/GlobalConfig.hpp"
using namespace Convention;

GlobalConfig config("./data");
config["username"] = "player1";
config["level"] = 5;
config.SaveProperties();

// 日志记录
config.Log("Info", "Game started");
```

### 数学运算
```cpp
#include "Convention/[Runtime]/Math.hpp"
using namespace Convention;

auto result = Math::Sin(Math::DegToRad(90.0f));
auto random_value = Math::RandomRange(1, 100);
bool is_equal = Math::Equal(3.14f, Math::Constants<float>::PI, 0.01f);
```

## 📋 实现状态总结

| 模块 | 状态 | 完成度 | 备注 |
|------|------|--------|------|
| Config | ✅ 完整 | 100% | 已存在 |
| Architecture | ✅ 完整 | 100% | 已存在 |
| File | ✅ 完整 | 95% | 平台对话框需实现 |
| GlobalConfig | ✅ 完整 | 100% | 功能完整 |
| Math | ✅ 完整 | 100% | 功能完整 |
| Web | 🔧 接口 | 30% | 需HTTP库实现 |
| Plugins | 🔧 接口 | 40% | 需动态加载实现 |
| EasySave | 🔧 接口 | 20% | 需加密压缩实现 |

## 🎯 下一步实现建议

### 高优先级
1. **完善Web模块**: 集成libcurl实现HTTP功能
2. **完善EasySave**: 实现完整的序列化系统
3. **文件对话框**: 实现平台特定的文件选择功能

### 中优先级
1. **插件系统**: 实现动态库加载机制
2. **加密支持**: 为EasySave添加AES加密
3. **压缩支持**: 为EasySave添加Gzip压缩

### 低优先级
1. **性能优化**: 针对关键路径进行优化
2. **单元测试**: 为各模块添加测试用例
3. **文档完善**: 添加详细的API文档

## 📚 设计原则

本实现遵循以下设计原则：

1. **模块化**: 每个模块功能独立，依赖关系清晰
2. **类型安全**: 大量使用模板和类型检查
3. **RAII**: 资源管理遵循RAII原则
4. **异常安全**: 提供清晰的错误处理机制
5. **跨平台**: 基于标准库，支持主要平台
6. **向前兼容**: 接口设计考虑未来扩展性

这个实现为Convention-CPP提供了完整的Runtime基础设施，为后续开发提供了坚实的基础。