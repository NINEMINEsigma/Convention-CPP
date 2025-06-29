#pragma once
#ifndef Convention_Runtime_Plugins_hpp
#define Convention_Runtime_Plugins_hpp

#include "Config.hpp"
#include "Architecture.hpp"

namespace Convention
{
    // Priority Queue implementation for plugin priorities
    template<typename T>
    class PriorityQueue
    {
    public:
        enum class Comparator
        {
            Less = -1,
            Equal = 0,
            Greater = 1
        };

    private:
        std::vector<T> elements;
        std::function<int(const T&, const T&)> compare_func;
        Comparator comparator;

    public:
        PriorityQueue(Comparator comp = Comparator::Less, size_t capacity = 1)
            : comparator(comp)
        {
            elements.reserve(std::max(capacity, size_t{1}));

            compare_func = [comp](const T& a, const T& b) -> int {
                if constexpr (std::is_arithmetic_v<T>) {
                    int result = (a > b) - (a < b);
                    return result * static_cast<int>(comp);
                } else {
                    if (a < b) return -1 * static_cast<int>(comp);
                    if (b < a) return 1 * static_cast<int>(comp);
                    return 0;
                }
            };
        }

        template<typename Compare>
        PriorityQueue(Compare comp, size_t capacity = 1)
            : compare_func(comp)
        {
            elements.reserve(std::max(capacity, size_t{1}));
        }

        // Basic properties
        size_t Size() const { return elements.size(); }
        size_t Count() const { return Size(); }
        size_t Capacity() const { return elements.capacity(); }
        bool IsEmpty() const { return elements.empty(); }

        const T& Top() const
        {
            if (IsEmpty()) throw std::runtime_error("Queue is empty");
            return elements[0];
        }

        // Core operations
        void Enqueue(const T& value)
        {
            elements.push_back(value);
            ShiftUp(elements.size() - 1);
        }

        void Enqueue(T&& value)
        {
            elements.push_back(std::move(value));
            ShiftUp(elements.size() - 1);
        }

        T Dequeue()
        {
            if (IsEmpty()) throw std::runtime_error("Queue is empty");

            T result = std::move(elements[0]);
            elements[0] = std::move(elements.back());
            elements.pop_back();

            if (!IsEmpty()) {
                ShiftDown(0);
            }

            return result;
        }

        bool TryDequeue(T& result)
        {
            if (IsEmpty()) {
                return false;
            }
            result = Dequeue();
            return true;
        }

        const T& Peek() const
        {
            return Top();
        }

        // Utility functions
        bool Contains(const T& item) const
        {
            return std::find(elements.begin(), elements.end(), item) != elements.end();
        }

        void Clear()
        {
            elements.clear();
        }

        std::vector<T> ToArray() const
        {
            return elements;
        }

        void TrimExcess()
        {
            if (elements.size() < elements.capacity() * 0.9) {
                elements.shrink_to_fit();
            }
        }

        void EnsureCapacity(size_t minCapacity)
        {
            if (elements.capacity() < minCapacity) {
                elements.reserve(minCapacity);
            }
        }

    private:
        void ShiftUp(size_t index)
        {
            while (index > 0) {
                size_t parent = (index - 1) / 2;
                if (compare_func(elements[index], elements[parent]) >= 0) break;

                std::swap(elements[index], elements[parent]);
                index = parent;
            }
        }

        void ShiftDown(size_t index)
        {
            size_t size = elements.size();
            while (true) {
                size_t left = 2 * index + 1;
                size_t right = 2 * index + 2;
                size_t smallest = index;

                if (left < size && compare_func(elements[left], elements[smallest]) < 0) {
                    smallest = left;
                }
                if (right < size && compare_func(elements[right], elements[smallest]) < 0) {
                    smallest = right;
                }

                if (smallest == index) break;

                std::swap(elements[index], elements[smallest]);
                index = smallest;
            }
        }
    };

    // Plugin base interface
    class IPlugin
    {
    public:
        virtual ~IPlugin() = default;
        virtual std::string GetName() const = 0;
        virtual std::string GetVersion() const = 0;
        virtual bool Initialize() = 0;
        virtual void Shutdown() = 0;
        virtual int GetPriority() const { return 0; }
    };

    // Plugin information
    struct PluginInfo
    {
        std::string name;
        std::string version;
        std::string description;
        std::vector<std::string> dependencies;
        int priority = 0;
        bool enabled = true;
    };

    // Plugin manager interface
    class PluginManager
    {
    public:
        static PluginManager& GetInstance()
        {
            static PluginManager instance;
            return instance;
        }

        // Plugin registration (interface only)
        template<typename T>
        bool RegisterPlugin()
        {
            static_assert(std::is_base_of_v<IPlugin, T>, "T must inherit from IPlugin");

            // TODO: Implement plugin registration
            // This would typically involve:
            // 1. Creating instance of T
            // 2. Calling Initialize()
            // 3. Adding to plugin registry
            // 4. Managing dependencies

            throw std::runtime_error("Plugin system implementation required");
        }

        bool LoadPlugin(const std::string& pluginPath)
        {
            // TODO: Implement dynamic plugin loading
            // This would typically involve:
            // 1. Loading shared library (.dll/.so/.dylib)
            // 2. Finding plugin entry point
            // 3. Creating plugin instance
            // 4. Registering plugin

            throw std::runtime_error("Dynamic plugin loading implementation required");
        }

        bool UnloadPlugin(const std::string& pluginName)
        {
            // TODO: Implement plugin unloading
            throw std::runtime_error("Plugin unloading implementation required");
        }

        std::vector<PluginInfo> GetLoadedPlugins() const
        {
            // TODO: Return list of loaded plugins
            return {};
        }

        template<typename T>
        T* GetPlugin(const std::string& name) const
        {
            // TODO: Find plugin by name and cast to type T
            throw std::runtime_error("Plugin retrieval implementation required");
        }

        bool IsPluginLoaded(const std::string& name) const
        {
            // TODO: Check if plugin is loaded
            return false;
        }

        void EnablePlugin(const std::string& name)
        {
            // TODO: Enable plugin
        }

        void DisablePlugin(const std::string& name)
        {
            // TODO: Disable plugin
        }

        // Plugin discovery
        std::vector<std::string> DiscoverPlugins(const std::string& directory) const
        {
            // TODO: Scan directory for plugin files
            throw std::runtime_error("Plugin discovery implementation required");
        }

        // Event system integration
        template<typename EventType>
        void BroadcastEvent(const EventType& event)
        {
            // TODO: Send event to all registered plugins
            // Integration with Architecture's event system
        }

    private:
        std::map<std::string, std::shared_ptr<IPlugin>> loaded_plugins;
        std::map<std::string, PluginInfo> plugin_info;
        PriorityQueue<std::shared_ptr<IPlugin>> priority_queue;

        PluginManager() = default;
    };

    // Platform-specific plugin utilities
    namespace Platform
    {
        // Windows-specific functionality (interface only)
        class WindowsPlugin : public IPlugin
        {
        public:
            std::string GetName() const override { return "WindowsPlugin"; }
            std::string GetVersion() const override { return "1.0.0"; }

            bool Initialize() override
            {
                // TODO: Initialize Windows-specific features
                return true;
            }

            void Shutdown() override
            {
                // TODO: Cleanup Windows-specific resources
            }

            // Windows-specific methods (interface only)
            bool ShowMessageBox(const std::string& title, const std::string& message)
            {
                // TODO: Implement using Windows API
                throw std::runtime_error("Windows API implementation required");
            }

            std::string GetSystemInfo()
            {
                // TODO: Get Windows system information
                throw std::runtime_error("Windows API implementation required");
            }
        };

        // Linux-specific functionality (interface only)
        class LinuxPlugin : public IPlugin
        {
        public:
            std::string GetName() const override { return "LinuxPlugin"; }
            std::string GetVersion() const override { return "1.0.0"; }

            bool Initialize() override
            {
                // TODO: Initialize Linux-specific features
                return true;
            }

            void Shutdown() override
            {
                // TODO: Cleanup Linux-specific resources
            }

            // Linux-specific methods (interface only)
            std::string GetSystemInfo()
            {
                // TODO: Get Linux system information
                throw std::runtime_error("Linux API implementation required");
            }
        };

        // Factory for platform-specific plugins
        std::shared_ptr<IPlugin> CreatePlatformPlugin()
        {
            if (PlatformIndicator::IsPlatformWindows) {
                return std::make_shared<WindowsPlugin>();
            } else if (PlatformIndicator::IsPlatformLinux) {
                return std::make_shared<LinuxPlugin>();
            }

            return nullptr;
        }
    }

    // Plugin helper macros
    #define DECLARE_PLUGIN(ClassName) \
        extern "C" Convention::IPlugin* CreatePlugin() { \
            return new ClassName(); \
        } \
        extern "C" void DestroyPlugin(Convention::IPlugin* plugin) { \
            delete plugin; \
        }

    #define REGISTER_PLUGIN(ClassName) \
        bool Register##ClassName() { \
            return Convention::PluginManager::GetInstance().RegisterPlugin<ClassName>(); \
        }
}

#endif // Convention_Runtime_Plugins_hpp