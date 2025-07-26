#pragma once
#ifndef Convention_Runtime_GlobalConfig_hpp
#define Convention_Runtime_GlobalConfig_hpp

#include "Config.hpp"
#include "File.hpp"
#include <nlohmann/json.hpp>

namespace Convention
{
    class GlobalConfig
    {
    public:
        static std::string ConstConfigFile;

        static void InitExtensionEnv()
        {
            ConstConfigFile = "config.json";
        }

        static void GenerateEmptyConfigJson(ToolFile& file)
        {
            nlohmann::json config;
            config["properties"] = nlohmann::json::object();
            file.SaveAsText(config.dump(4));
            file.Close();
        }

    private:
        size_t configLogging_tspace = 19; // "Property not found".length()
        ToolFile DataDir;
        std::map<std::string, nlohmann::json> data_pair;

    public:
        GlobalConfig(const std::string& dataDir, bool isTryCreateDataDir = false, bool isLoad = true)
            : GlobalConfig(ToolFile(dataDir), isTryCreateDataDir, isLoad) {}

        GlobalConfig(ToolFile dataDir, bool isTryCreateDataDir = false, bool isLoad = true)
            : DataDir(std::move(dataDir))
        {
            // Build up data folder
            if (!DataDir.IsDir()) {
                DataDir = DataDir.BackToParentDir();
            }
            if (!DataDir.Exists()) {
                if (isTryCreateDataDir) {
                    DataDir.MustExistsPath();
                } else {
                    throw std::runtime_error("Data dir not found: " + DataDir.GetFullPath());
                }
            }

            // Build up init data file
            auto configFile = GetConfigFile();
            if (!configFile.Exists()) {
                GenerateEmptyConfigJson(configFile);
            } else if (isLoad) {
                LoadProperties();
            }
        }

        ~GlobalConfig() = default;

        ToolFile GetConfigFile() const { return DataDir | ConstConfigFile; }
        ToolFile ConfigFile() const { return GetConfigFile(); }

        ToolFile GetFile(const std::string& path, bool isMustExist = false)
        {
            auto file = DataDir | path;
            if (isMustExist)
			{
                file.MustExistsPath();
            }
            return file;
        }

        bool EraseFile(const std::string& path)
        {
            auto file = DataDir | path;
			if (file.Exists())
			{
				file.MustExistsPath();
				if (file.IsFile())
				{
					file.Remove();
					file.Create();
				}
				return true;
			}
            return false;
        }

        bool RemoveFile(const std::string& path)
        {
            auto file = DataDir | path;
            if (file.Exists()) {
                try {
                    file.Delete();
                    return true;
                } catch (...) {}
            }
            return false;
        }

        bool CreateFile(const std::string& path)
        {
            auto file = DataDir | path;
            if (file.Exists()) return false;
            if (!file.GetParentDir().Exists()) return false;
            file.Create();
            return true;
        }

        // Data access with operator[]
        nlohmann::json& operator[](const std::string& key)
        {
            return data_pair[key];
        }

        const nlohmann::json& operator[](const std::string& key) const
        {
            auto it = data_pair.find(key);
            if (it == data_pair.end()) {
                throw std::out_of_range("Key not found: " + key);
            }
            return it->second;
        }

        bool Contains(const std::string& key) const
        {
            return data_pair.find(key) != data_pair.end();
        }

        bool Remove(const std::string& key)
        {
            auto it = data_pair.find(key);
            if (it != data_pair.end()) {
                data_pair.erase(it);
                return true;
            }
            return false;
        }

        // Iterator support for foreach/for loops
        auto begin() { return data_pair.begin(); }
        auto end() { return data_pair.end(); }
        auto begin() const { return data_pair.cbegin(); }
        auto end() const { return data_pair.cend(); }

        size_t DataSize() const { return data_pair.size(); }

        GlobalConfig& SaveProperties()
        {
            auto configFile = GetConfigFile();
            nlohmann::json config;
            config["properties"] = nlohmann::json::object();

            for (const auto& [key, value] : data_pair) {
                config["properties"][key] = value;
            }

            configFile.SaveAsText(config.dump(4));
            return *this;
        }

        GlobalConfig& LoadProperties()
        {
            auto configFile = GetConfigFile();
            if (!configFile.Exists()) {
                data_pair.clear();
            } else {
                try {
                    auto content = configFile.LoadAsText();
                    auto config = nlohmann::json::parse(content);

                    if (config.contains("properties") && config["properties"].is_object()) {
                        data_pair.clear();
                        for (auto& [key, value] : config["properties"].items()) {
                            data_pair[key] = value;
                        }
                    } else {
                        throw std::runtime_error("Can't find properties in config file");
                    }
                } catch (const nlohmann::json::exception& e) {
                    throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
                }
            }
            return *this;
        }

        ToolFile GetLogFile()
        {
            auto configFile = GetConfigFile();
            auto logName = configFile.GetName(true) + "_log.txt";
            return GetFile(logName, true);
        }

        ToolFile LogFile() { return GetLogFile(); }

    private:
        std::function<void(const std::string&)> MyDefaultLogger;

    public:
        std::function<void(const std::string&)> DefaultLogger() const
        {
            return MyDefaultLogger ? MyDefaultLogger : [](const std::string& msg) { std::cout << msg << std::endl; };
        }

        void SetDefaultLogger(std::function<void(const std::string&)> logger)
        {
            MyDefaultLogger = std::move(logger);
        }

        virtual void Log(const std::string& messageType, const std::string& message, std::function<void(const std::string&)> logger)
        {
            configLogging_tspace = std::max(configLogging_tspace, messageType.length());

            auto currentTime = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(currentTime);
            auto tm = *std::localtime(&time_t);

            std::ostringstream timeStr;
            timeStr << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");

            std::string padding(configLogging_tspace / 2, ' ');
            std::string paddingEnd(configLogging_tspace - configLogging_tspace / 2, ' ');

            std::ostringstream logMessage;
            logMessage << "[" << timeStr.str() << "]" << padding << messageType << paddingEnd << ": " << message;

            if (logger) {
                logger(logMessage.str());
            } else {
                DefaultLogger()(logMessage.str());
            }
        }

        void Log(const std::string& messageType, const std::string& message)
        {
            Log(messageType, message, nullptr);
        }

        void LogPropertyNotFound(const std::string& message, std::function<void(const std::string&)> logger = nullptr, const std::string& defaultValue = "")
        {
            std::string fullMessage = message;
            if (!defaultValue.empty()) {
                fullMessage += " (default: " + defaultValue + ")";
            }
            Log("Property not found", fullMessage, logger);
        }

        void LogPropertyNotFound(const std::string& message, const std::string& defaultValue = "")
        {
            LogPropertyNotFound(message, nullptr, defaultValue);
        }

        void LogMessageOfPleaseCompleteConfiguration()
        {
            Log("Configuration", "Please complete the configuration");
        }

        template<typename T>
        T FindItem(const std::string& key, const T& defaultValue = T{}) const
        {
            auto it = data_pair.find(key);
            if (it != data_pair.end()) {
                try {
                    return it->second.get<T>();
                } catch (const nlohmann::json::exception&) {
                    LogPropertyNotFound("Cannot convert value for key: " + key);
                }
            } else {
                LogPropertyNotFound("Key not found: " + key);
            }
            return defaultValue;
        }
    };

    // Static member definition
    std::string GlobalConfig::ConstConfigFile = "config.json";

    class ProjectConfig : public GlobalConfig
    {
    private:
        static std::string ProjectConfigFileFocus;

    public:
        static void InitExtensionEnv()
        {
            ProjectConfigFileFocus = "Assets/";
        }

        ProjectConfig(bool isLoad = true) : GlobalConfig(ProjectConfigFileFocus, true, isLoad) {}

        static void SetProjectConfigFileFocus(const std::string& path)
        {
            ProjectConfigFileFocus = path;
        }

        static std::string GetProjectConfigFileFocus()
        {
            return ProjectConfigFileFocus;
        }
    };

    // Static member definition
    std::string ProjectConfig::ProjectConfigFileFocus = "Assets/";
}

#endif // Convention_Runtime_GlobalConfig_hpp
