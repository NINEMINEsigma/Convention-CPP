#pragma once
#ifndef Convention_Runtime_Web_hpp
#define Convention_Runtime_Web_hpp

#include "Config.hpp"
#include "File.hpp"
#include <functional>

namespace Convention
{
    // Forward declaration for HTTP response
    struct HttpResponseMessage
    {
        int status_code = 0;
        std::string reason_phrase;
        std::map<std::string, std::string> headers;
        std::vector<uint8_t> content;
        bool is_success = false;

        bool IsSuccessStatusCode() const { return is_success; }

        std::string GetContentAsString() const
        {
            return std::string(content.begin(), content.end());
        }
    };

    class ToolURL
    {
    private:
        std::string url;
        std::vector<uint8_t> cached_data;

    public:
        explicit ToolURL(const std::string& url) : url(url) {}

        // Convert to string
        operator std::string() const { return url; }
        std::string ToString() const { return url; }

        // URL properties
        std::string FullURL() const { return url; }
        std::string GetFullURL() const { return url; }

        std::string GetFilename() const
        {
            if (url.empty()) return "";

            size_t pos = url.find_last_of('/');
            if (pos == std::string::npos) return "";

            std::string path = url.substr(pos + 1);
            size_t query_pos = path.find('?');
            if (query_pos != std::string::npos) {
                path = path.substr(0, query_pos);
            }

            return path;
        }

        std::string GetExtension() const
        {
            std::string filename = GetFilename();
            if (filename.empty()) return "";

            size_t pos = filename.find_last_of('.');
            if (pos == std::string::npos) return "";

            return filename.substr(pos);
        }

        bool ExtensionIs(const std::vector<std::string>& extensions) const
        {
            std::string ext = GetExtension();
            if (ext.empty()) return false;

            // Convert to lowercase for case-insensitive comparison
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

            // Remove leading dot if present
            if (ext[0] == '.') ext = ext.substr(1);

            for (const auto& extension : extensions) {
                std::string comp_ext = extension;
                std::transform(comp_ext.begin(), comp_ext.end(), comp_ext.begin(), ::tolower);
                if (comp_ext[0] == '.') comp_ext = comp_ext.substr(1);

                if (ext == comp_ext) return true;
            }

            return false;
        }

        // Validation
        bool IsValid() const { return ValidateURL(); }
        operator bool() const { return IsValid(); }

        bool ValidateURL() const
        {
            if (url.empty()) return false;

            // Simple URL validation - check for http/https protocol
            return url.find("http://") == 0 || url.find("https://") == 0;
        }

        // File type checking
        bool IsText() const
        {
            return ExtensionIs({"txt", "html", "htm", "css", "js", "xml", "csv"});
        }

        bool IsJson() const
        {
            return ExtensionIs({"json"});
        }

        bool IsImage() const
        {
            return ExtensionIs({"jpg", "jpeg", "png", "gif", "bmp", "svg"});
        }

        bool IsDocument() const
        {
            return ExtensionIs({"pdf", "doc", "docx", "xls", "xlsx", "ppt", "pptx"});
        }

        // HTTP Methods (Interface only - requires implementation with HTTP library like libcurl)
        bool GetAsync(std::function<void(HttpResponseMessage*)> callback) const
        {
            // TODO: Implement with HTTP library
            HttpResponseMessage response;
            response.status_code = 501;
            response.reason_phrase = "Not Implemented";
            response.is_success = false;
            callback(&response);
            return false;
        }

        bool Get(std::function<void(HttpResponseMessage*)> callback) const
        {
            return GetAsync(callback);
        }

        bool PostAsync(std::function<void(HttpResponseMessage*)> callback,
                      const std::map<std::string, std::string>& formData = {}) const
        {
            // TODO: Implement with HTTP library
            HttpResponseMessage response;
            response.status_code = 501;
            response.reason_phrase = "Not Implemented";
            response.is_success = false;
            callback(&response);
            return false;
        }

        bool Post(std::function<void(HttpResponseMessage*)> callback,
                 const std::map<std::string, std::string>& formData = {}) const
        {
            return PostAsync(callback, formData);
        }

        // Content loading (Interface only)
        std::string LoadAsText() const
        {
            // TODO: Implement HTTP request
            throw std::runtime_error("HTTP implementation required");
        }

        std::vector<uint8_t> LoadAsBinary() const
        {
            // TODO: Implement HTTP request
            throw std::runtime_error("HTTP implementation required");
        }

        template<typename T>
        T LoadAsJson() const
        {
            // TODO: Implement HTTP request + JSON parsing
            throw std::runtime_error("HTTP implementation required");
        }

        // Async versions (Interface only)
        std::future<std::string> LoadAsTextAsync() const
        {
            return std::async(std::launch::async, [this]() {
                return LoadAsText();
            });
        }

        std::future<std::vector<uint8_t>> LoadAsBinaryAsync() const
        {
            return std::async(std::launch::async, [this]() {
                return LoadAsBinary();
            });
        }

        template<typename T>
        std::future<T> LoadAsJsonAsync() const
        {
            return std::async(std::launch::async, [this]() {
                return LoadAsJson<T>();
            });
        }

        // File operations
        ToolURL Open(const std::string& new_url)
        {
            return ToolURL(new_url);
        }

        void Save(const std::string& localPath = "") const
        {
            std::string path = localPath.empty() ? GetFilename() : localPath;
            if (path.empty()) path = "downloaded_file";

            auto data = LoadAsBinary();
            ToolFile(path).SaveAsBinary(data);
        }

        void SaveAsText(const std::string& localPath = "") const
        {
            std::string path = localPath.empty() ? GetFilename() : localPath;
            if (path.empty()) path = "downloaded_file.txt";

            auto text = LoadAsText();
            ToolFile(path).SaveAsText(text);
        }

        void SaveAsJson(const std::string& localPath = "") const
        {
            std::string path = localPath.empty() ? GetFilename() : localPath;
            if (path.empty()) path = "downloaded_file.json";

            auto text = LoadAsText();
            ToolFile(path).SaveAsText(text);
        }

        void SaveAsBinary(const std::string& localPath = "") const
        {
            std::string path = localPath.empty() ? GetFilename() : localPath;
            if (path.empty()) path = "downloaded_file.bin";

            auto data = LoadAsBinary();
            ToolFile(path).SaveAsBinary(data);
        }

        // Download operations
        ToolURL Download(const std::string& localPath = "") const
        {
            Save(localPath);
            std::string path = localPath.empty() ? GetFilename() : localPath;
            return ToolURL("file://" + std::filesystem::absolute(path).string());
        }

        std::future<ToolURL> DownloadAsync(const std::string& localPath = "") const
        {
            return std::async(std::launch::async, [this, localPath]() {
                return Download(localPath);
            });
        }
    };

    // Simple HTTP Client interface (requires implementation)
    class HttpClient
    {
    public:
        static HttpClient& GetInstance()
        {
            static HttpClient instance;
            return instance;
        }

        // Basic HTTP operations (interface only)
        HttpResponseMessage Get(const std::string& url) const
        {
            // TODO: Implement with HTTP library
            HttpResponseMessage response;
            response.status_code = 501;
            response.reason_phrase = "Not Implemented";
            response.is_success = false;
            return response;
        }

        HttpResponseMessage Post(const std::string& url,
                               const std::string& content = "",
                               const std::string& contentType = "application/json") const
        {
            // TODO: Implement with HTTP library
            HttpResponseMessage response;
            response.status_code = 501;
            response.reason_phrase = "Not Implemented";
            response.is_success = false;
            return response;
        }

        HttpResponseMessage Put(const std::string& url,
                              const std::string& content = "",
                              const std::string& contentType = "application/json") const
        {
            // TODO: Implement with HTTP library
            HttpResponseMessage response;
            response.status_code = 501;
            response.reason_phrase = "Not Implemented";
            response.is_success = false;
            return response;
        }

        HttpResponseMessage Delete(const std::string& url) const
        {
            // TODO: Implement with HTTP library
            HttpResponseMessage response;
            response.status_code = 501;
            response.reason_phrase = "Not Implemented";
            response.is_success = false;
            return response;
        }

        // Set headers for subsequent requests
        void SetHeader(const std::string& name, const std::string& value)
        {
            default_headers[name] = value;
        }

        void SetTimeout(int timeoutSeconds)
        {
            timeout_seconds = timeoutSeconds;
        }

    private:
        std::map<std::string, std::string> default_headers;
        int timeout_seconds = 30;
    };

    // Convenience functions
    inline HttpResponseMessage HttpGet(const std::string& url)
    {
        return HttpClient::GetInstance().Get(url);
    }

    inline HttpResponseMessage HttpPost(const std::string& url, const std::string& content = "")
    {
        return HttpClient::GetInstance().Post(url, content);
    }

    // URL utilities
    inline std::string UrlEncode(const std::string& value)
    {
        // TODO: Implement URL encoding
        // For now, return as-is (basic implementation needed)
        return value;
    }

    inline std::string UrlDecode(const std::string& value)
    {
        // TODO: Implement URL decoding
        // For now, return as-is (basic implementation needed)
        return value;
    }
}

#endif // Convention_Runtime_Web_hpp