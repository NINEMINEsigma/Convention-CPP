#pragma once
#ifndef Convention_Runtime_EasySave_hpp
#define Convention_Runtime_EasySave_hpp

#include "Config.hpp"
#include "File.hpp"
#include <nlohmann/json.hpp>

namespace Convention
{
    namespace EasySave
    {
        // Enums for configuration
        enum class Location { File, InternalMS, Cache };
        enum class Directory { PersistentDataPath, DataPath };
        enum class EncryptionType { None, AES };
        enum class CompressionType { None, Gzip };
        enum class Format { JSON };
        enum class ReferenceMode { ByRef, ByValue, ByRefAndValue };

        // Settings structure
        struct EasySaveSettings
        {
            Location location = Location::File;
            Directory directory = Directory::DataPath;
            EncryptionType encryptionType = EncryptionType::None;
            CompressionType compressionType = CompressionType::None;
            Format format = Format::JSON;
            ReferenceMode referenceMode = ReferenceMode::ByValue;

            std::string filePath = "SaveData.json";
            std::string encryptionPassword = "";

            EasySaveSettings() = default;
            EasySaveSettings(const std::string& path) : filePath(path) {}
        };

        // Main EasySave class interface
        class EasySave
        {
        public:
            // Basic save operations
            template<typename T>
            static void Save(const std::string& key, const T& value)
            {
                Save(key, value, EasySaveSettings());
            }

            template<typename T>
            static void Save(const std::string& key, const T& value, const std::string& filePath)
            {
                EasySaveSettings settings(filePath);
                Save(key, value, settings);
            }

            template<typename T>
            static void Save(const std::string& key, const T& value, const EasySaveSettings& settings)
            {
                // TODO: Implement serialization
                // This would typically involve:
                // 1. Converting T to JSON/Binary based on settings.format
                // 2. Applying compression if needed
                // 3. Applying encryption if needed
                // 4. Writing to file/cache based on settings.location

                if (settings.location == Location::File) {
                    // Simple JSON implementation for demonstration
                    nlohmann::json data;
                    try {
                        auto file = ToolFile(settings.filePath);
                        if (file.Exists()) {
                            auto content = file.LoadAsText();
                            if (!content.empty()) {
                                data = nlohmann::json::parse(content);
                            }
                        }

                        data[key] = value;  // This requires T to be JSON-serializable
                        file.SaveAsText(data.dump(4));
                    } catch (const std::exception& e) {
                        throw std::runtime_error("Save failed: " + std::string(e.what()));
                    }
                } else {
                    throw std::runtime_error("EasySave full implementation required");
                }
            }

            // Basic load operations
            template<typename T>
            static T Load(const std::string& key)
            {
                return Load<T>(key, EasySaveSettings());
            }

            template<typename T>
            static T Load(const std::string& key, const std::string& filePath)
            {
                EasySaveSettings settings(filePath);
                return Load<T>(key, settings);
            }

            template<typename T>
            static T Load(const std::string& key, const EasySaveSettings& settings)
            {
                // TODO: Implement deserialization
                if (settings.location == Location::File) {
                    // Simple JSON implementation for demonstration
                    try {
                        auto file = ToolFile(settings.filePath);
                        if (!file.Exists()) {
                            throw std::runtime_error("File not found: " + settings.filePath);
                        }

                        auto content = file.LoadAsText();
                        auto data = nlohmann::json::parse(content);

                        if (!data.contains(key)) {
                            throw std::runtime_error("Key not found: " + key);
                        }

                        return data[key].get<T>();
                    } catch (const std::exception& e) {
                        throw std::runtime_error("Load failed: " + std::string(e.what()));
                    }
                } else {
                    throw std::runtime_error("EasySave full implementation required");
                }
            }

            template<typename T>
            static T Load(const std::string& key, const T& defaultValue)
            {
                return Load(key, defaultValue, EasySaveSettings());
            }

            template<typename T>
            static T Load(const std::string& key, const T& defaultValue, const std::string& filePath)
            {
                EasySaveSettings settings(filePath);
                return Load(key, defaultValue, settings);
            }

            template<typename T>
            static T Load(const std::string& key, const T& defaultValue, const EasySaveSettings& settings)
            {
                try {
                    return Load<T>(key, settings);
                } catch (...) {
                    return defaultValue;
                }
            }

            // Raw data operations (interface only)
            static void SaveRaw(const std::vector<uint8_t>& bytes)
            {
                SaveRaw(bytes, EasySaveSettings());
            }

            static void SaveRaw(const std::vector<uint8_t>& bytes, const std::string& filePath)
            {
                EasySaveSettings settings(filePath);
                SaveRaw(bytes, settings);
            }

            static void SaveRaw(const std::vector<uint8_t>& bytes, const EasySaveSettings& settings)
            {
                // TODO: Implement raw byte saving with compression/encryption
                ToolFile(settings.filePath).SaveAsBinary(bytes);
            }

            static void SaveRaw(const std::string& str)
            {
                SaveRaw(str, EasySaveSettings());
            }

            static void SaveRaw(const std::string& str, const std::string& filePath)
            {
                EasySaveSettings settings(filePath);
                SaveRaw(str, settings);
            }

            static void SaveRaw(const std::string& str, const EasySaveSettings& settings)
            {
                std::vector<uint8_t> bytes(str.begin(), str.end());
                SaveRaw(bytes, settings);
            }

            // Load raw data
            static std::vector<uint8_t> LoadRawBytes()
            {
                return LoadRawBytes(EasySaveSettings());
            }

            static std::vector<uint8_t> LoadRawBytes(const std::string& filePath)
            {
                EasySaveSettings settings(filePath);
                return LoadRawBytes(settings);
            }

            static std::vector<uint8_t> LoadRawBytes(const EasySaveSettings& settings)
            {
                // TODO: Implement raw byte loading with decompression/decryption
                return ToolFile(settings.filePath).LoadAsBinary();
            }

            static std::string LoadRawString()
            {
                return LoadRawString(EasySaveSettings());
            }

            static std::string LoadRawString(const std::string& filePath)
            {
                EasySaveSettings settings(filePath);
                return LoadRawString(settings);
            }

            static std::string LoadRawString(const EasySaveSettings& settings)
            {
                auto bytes = LoadRawBytes(settings);
                return std::string(bytes.begin(), bytes.end());
            }

            // Serialization (interface only)
            template<typename T>
            static std::vector<uint8_t> Serialize(const T& value, const EasySaveSettings& settings = EasySaveSettings())
            {
                // TODO: Implement serialization to bytes
                throw std::runtime_error("Serialization implementation required");
            }

            template<typename T>
            static T Deserialize(const std::vector<uint8_t>& bytes, const EasySaveSettings& settings = EasySaveSettings())
            {
                // TODO: Implement deserialization from bytes
                throw std::runtime_error("Deserialization implementation required");
            }

            // Encryption/Decryption (interface only)
            static std::vector<uint8_t> EncryptBytes(const std::vector<uint8_t>& bytes, const std::string& password = "")
            {
                // TODO: Implement AES encryption
                throw std::runtime_error("Encryption implementation required");
            }

            static std::vector<uint8_t> DecryptBytes(const std::vector<uint8_t>& bytes, const std::string& password = "")
            {
                // TODO: Implement AES decryption
                throw std::runtime_error("Decryption implementation required");
            }

            static std::string EncryptString(const std::string& str, const std::string& password = "")
            {
                auto bytes = std::vector<uint8_t>(str.begin(), str.end());
                auto encrypted = EncryptBytes(bytes, password);
                return std::string(encrypted.begin(), encrypted.end());
            }

            static std::string DecryptString(const std::string& str, const std::string& password = "")
            {
                auto bytes = std::vector<uint8_t>(str.begin(), str.end());
                auto decrypted = DecryptBytes(bytes, password);
                return std::string(decrypted.begin(), decrypted.end());
            }

            // Compression/Decompression (interface only)
            static std::vector<uint8_t> CompressBytes(const std::vector<uint8_t>& bytes)
            {
                // TODO: Implement Gzip compression
                throw std::runtime_error("Compression implementation required");
            }

            static std::vector<uint8_t> DecompressBytes(const std::vector<uint8_t>& bytes)
            {
                // TODO: Implement Gzip decompression
                throw std::runtime_error("Decompression implementation required");
            }

            static std::string CompressString(const std::string& str)
            {
                auto bytes = std::vector<uint8_t>(str.begin(), str.end());
                auto compressed = CompressBytes(bytes);
                return std::string(compressed.begin(), compressed.end());
            }

            static std::string DecompressString(const std::string& str)
            {
                auto bytes = std::vector<uint8_t>(str.begin(), str.end());
                auto decompressed = DecompressBytes(bytes);
                return std::string(decompressed.begin(), decompressed.end());
            }

            // Key and file management
            static bool KeyExists(const std::string& key)
            {
                return KeyExists(key, EasySaveSettings());
            }

            static bool KeyExists(const std::string& key, const std::string& filePath)
            {
                EasySaveSettings settings(filePath);
                return KeyExists(key, settings);
            }

            static bool KeyExists(const std::string& key, const EasySaveSettings& settings)
            {
                try {
                    auto file = ToolFile(settings.filePath);
                    if (!file.Exists()) return false;

                    auto content = file.LoadAsText();
                    auto data = nlohmann::json::parse(content);
                    return data.contains(key);
                } catch (...) {
                    return false;
                }
            }

            static void DeleteKey(const std::string& key)
            {
                DeleteKey(key, EasySaveSettings());
            }

            static void DeleteKey(const std::string& key, const std::string& filePath)
            {
                EasySaveSettings settings(filePath);
                DeleteKey(key, settings);
            }

            static void DeleteKey(const std::string& key, const EasySaveSettings& settings)
            {
                try {
                    auto file = ToolFile(settings.filePath);
                    if (!file.Exists()) return;

                    auto content = file.LoadAsText();
                    auto data = nlohmann::json::parse(content);

                    if (data.contains(key)) {
                        data.erase(key);
                        file.SaveAsText(data.dump(4));
                    }
                } catch (const std::exception& e) {
                    throw std::runtime_error("DeleteKey failed: " + std::string(e.what()));
                }
            }

            static std::vector<std::string> GetKeys()
            {
                return GetKeys(EasySaveSettings());
            }

            static std::vector<std::string> GetKeys(const std::string& filePath)
            {
                EasySaveSettings settings(filePath);
                return GetKeys(settings);
            }

            static std::vector<std::string> GetKeys(const EasySaveSettings& settings)
            {
                try {
                    auto file = ToolFile(settings.filePath);
                    if (!file.Exists()) return {};

                    auto content = file.LoadAsText();
                    auto data = nlohmann::json::parse(content);

                    std::vector<std::string> keys;
                    for (auto& [key, value] : data.items()) {
                        keys.push_back(key);
                    }
                    return keys;
                } catch (...) {
                    return {};
                }
            }

            // File operations
            static bool FileExists()
            {
                return FileExists(EasySaveSettings());
            }

            static bool FileExists(const std::string& filePath)
            {
                EasySaveSettings settings(filePath);
                return FileExists(settings);
            }

            static bool FileExists(const EasySaveSettings& settings)
            {
                return ToolFile(settings.filePath).Exists();
            }

            static void DeleteFile()
            {
                DeleteFile(EasySaveSettings());
            }

            static void DeleteFile(const std::string& filePath)
            {
                EasySaveSettings settings(filePath);
                DeleteFile(settings);
            }

            static void DeleteFile(const EasySaveSettings& settings)
            {
                ToolFile(settings.filePath).Delete();
            }

            // Backup operations (interface only)
            static void CreateBackup()
            {
                CreateBackup(EasySaveSettings());
            }

            static void CreateBackup(const std::string& filePath)
            {
                EasySaveSettings settings(filePath);
                CreateBackup(settings);
            }

            static void CreateBackup(const EasySaveSettings& settings)
            {
                // TODO: Implement backup creation
                auto file = ToolFile(settings.filePath);
                if (file.Exists()) {
                    auto backupPath = settings.filePath + ".backup";
                    ToolFile copyTo(backupPath);
                    file.Copy(backupPath, copyTo);
                }
            }

            static bool RestoreBackup(const std::string& filePath)
            {
                EasySaveSettings settings(filePath);
                return RestoreBackup(settings);
            }

            static bool RestoreBackup(const EasySaveSettings& settings)
            {
                // TODO: Implement backup restoration
                auto backupPath = settings.filePath + ".backup";
                auto backupFile = ToolFile(backupPath);
                if (backupFile.Exists()) {
                    ToolFile originalFile(settings.filePath);
                    backupFile.Copy(settings.filePath, originalFile);
                    return true;
                }
                return false;
            }

            // Caching operations (interface only)
            static void StoreCachedFile()
            {
                StoreCachedFile(EasySaveSettings());
            }

            static void StoreCachedFile(const std::string& filePath)
            {
                EasySaveSettings settings(filePath);
                StoreCachedFile(settings);
            }

            static void StoreCachedFile(const EasySaveSettings& settings)
            {
                // TODO: Implement cache to file storage
                throw std::runtime_error("Cache system implementation required");
            }

            static void CacheFile()
            {
                CacheFile(EasySaveSettings());
            }

            static void CacheFile(const std::string& filePath)
            {
                EasySaveSettings settings(filePath);
                CacheFile(settings);
            }

            static void CacheFile(const EasySaveSettings& settings)
            {
                // TODO: Implement file to cache loading
                throw std::runtime_error("Cache system implementation required");
            }

            // Initialization
            static void Init()
            {
                // TODO: Initialize EasySave system
                // This would set up encryption keys, compression libraries, etc.
            }
        };
    }
}

#endif // Convention_Runtime_EasySave_hpp