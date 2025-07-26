#pragma once
#ifndef Convention_Runtime_File_hpp
#define Convention_Runtime_File_hpp

#include "Config.hpp"

namespace Convention
{
    class ToolFile
    {
    private:
        std::filesystem::path FullPath;
		//mutable std::fstream OriginControlStream;
        //mutable bool StreamOpen = false;

    public:
        ToolFile(const std::filesystem::path& path) : FullPath(path) {}
        ToolFile(const std::string& path) : FullPath(path) {}
        ToolFile(const char* path) : FullPath(path) {}
		ToolFile(const ToolFile& other) : FullPath(other.FullPath) {}
		ToolFile(ToolFile&& other) : FullPath(std::move(other.FullPath)) {}
		ToolFile& operator=(const ToolFile& other)
		{
			this->FullPath = other.FullPath;
			return *this;
		}
		ToolFile& operator=(ToolFile&& other)
		{
			this->FullPath = std::move(other.FullPath);
			return *this;
		}

        // Convert to string
        operator std::string() const { return FullPath.string(); }
        std::string ToString() const { return FullPath.string(); }

        // Path operations
        std::string GetFullPath() const { return FullPath.string(); }
        std::string GetName(bool ignore_extension = false) const
        {
            if (ignore_extension) {
                return FullPath.stem().string();
            }
            return FullPath.filename().string();
        }
        std::string GetExtension() const
        {
            if (IsDir()) return "";
            return FullPath.extension().string();
        }

        // Path combination operator
        ToolFile operator|(const std::string& rightPath) const
        {
            return ToolFile(FullPath / rightPath);
        }

        // Existence check
        bool Exists() const { return std::filesystem::exists(FullPath); }
        operator bool() const { return Exists(); }

        // Type checking
        bool IsDir() const
        {
            if (Exists()) {
                return std::filesystem::is_directory(FullPath);
            }
            auto pathStr = FullPath.string();
            return pathStr.back() == '/' || pathStr.back() == '\\';
        }
        bool IsFile() const { return !IsDir(); }
        bool IsFileEmpty() const
        {
            if (!IsFile()) throw std::runtime_error("Target is not a file");
            return std::filesystem::file_size(FullPath) == 0;
        }

        // File operations
        ToolFile& Refresh() { return *this; }

        ToolFile& Create()
        {
            if (IsDir()) {
                std::filesystem::create_directories(FullPath);
            } else {
                MustExistsPath();
                std::ofstream file(FullPath);
                file.close();
            }
            return *this;
        }

        ToolFile& Open(const std::string& path)
        {
            Close();
            FullPath = path;
            return *this;
        }

        /*ToolFile& Open(std::ios::openmode mode = std::ios::in | std::ios::out)
        {
            Close();
            OriginControlStream.open(FullPath, mode);
            StreamOpen = OriginControlStream.is_open();
            return *this;
        }*/

        ToolFile& Close()
        {
            /*if (StreamOpen)
			{
                OriginControlStream.close();
                StreamOpen = false;
            }*/
            return *this;
        }

        // File content operations
        std::string LoadAsText() const
        {
            if (!IsFile()) throw std::runtime_error("Target is not a file");
            std::ifstream file(FullPath);
            if (!file.is_open()) throw std::runtime_error("Cannot open file");

            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }

        std::vector<uint8_t> LoadAsBinary() const
        {
            if (!IsFile()) throw std::runtime_error("Target is not a file");
            std::ifstream file(FullPath, std::ios::binary);
            if (!file.is_open()) throw std::runtime_error("Cannot open file");

            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);

            std::vector<uint8_t> result(size);
            file.read(reinterpret_cast<char*>(result.data()), size);
            return result;
        }

        void SaveAsText(const std::string& data)
        {
            MustExistsPath();
            std::ofstream file(FullPath);
            if (!file.is_open()) throw std::runtime_error("Cannot create file");
            file << data;
        }

        void SaveAsBinary(const std::vector<uint8_t>& data)
        {
            MustExistsPath();
            std::ofstream file(FullPath, std::ios::binary);
            if (!file.is_open()) throw std::runtime_error("Cannot create file");
            file.write(reinterpret_cast<const char*>(data.data()), data.size());
        }

        static void SaveDataAsBinary(const std::string& path, const std::vector<uint8_t>& data)
        {
            ToolFile(path).SaveAsBinary(data);
        }

        // Path management
        ToolFile& MustExistsPath()
        {
            auto parent = FullPath.parent_path();
            if (!parent.empty() && !std::filesystem::exists(parent))
			{
                std::filesystem::create_directories(parent);
            }
            return *this;
        }

        ToolFile& TryCreateParentPath()
        {
            try {
                MustExistsPath();
            } catch (...) {
                // Ignore errors
            }
            return *this;
        }

        // Navigation
        ToolFile BackToParentDir() const
        {
            return ToolFile(FullPath.parent_path());
        }

        ToolFile GetParentDir() const
        {
            return ToolFile(FullPath.parent_path());
        }

        // Directory operations
        std::vector<std::string> DirIter() const
        {
            if (!IsDir()) throw std::runtime_error("Target is not a directory");
            std::vector<std::string> result;
            for (const auto& entry : std::filesystem::directory_iterator(FullPath)) {
                result.push_back(entry.path().string());
            }
            return result;
        }

        std::vector<ToolFile> DirToolFileIter() const
        {
            if (!IsDir()) throw std::runtime_error("Target is not a directory");
            std::vector<ToolFile> result;
            for (const auto& entry : std::filesystem::directory_iterator(FullPath)) {
                result.emplace_back(entry.path());
            }
            return result;
        }

        size_t DirCount() const
        {
            if (!IsDir()) throw std::runtime_error("Target is not a directory");
            return std::distance(
                std::filesystem::directory_iterator(FullPath),
                std::filesystem::directory_iterator{}
            );
        }

        ToolFile& DirClear()
        {
            if (!IsDir()) throw std::runtime_error("Target is not a directory");
            for (const auto& entry : std::filesystem::directory_iterator(FullPath)) {
                std::filesystem::remove_all(entry);
            }
            return *this;
        }

        // File operations
        ToolFile& Rename(const std::string& newPath)
        {
            std::filesystem::rename(FullPath, newPath);
            FullPath = newPath;
            return *this;
        }

        ToolFile& Move(const std::string& path)
        {
            return Rename(path);
        }

        ToolFile& Copy(const std::string& path, ToolFile& copyTo)
        {
            if (IsDir()) {
                std::filesystem::copy(FullPath, path, std::filesystem::copy_options::recursive);
            } else {
                std::filesystem::copy_file(FullPath, path);
            }
            copyTo = ToolFile(path);
            return *this;
        }

        ToolFile& Delete()
        {
            Close();
            if (IsDir()) {
                std::filesystem::remove_all(FullPath);
            } else {
                std::filesystem::remove(FullPath);
            }
            return *this;
        }

        ToolFile& Remove() { return Delete(); }

        // File system info
        std::filesystem::file_time_type GetTimestamp() const
        {
            return std::filesystem::last_write_time(FullPath);
        }

        // Platform specific file dialogs (interface only)
        static std::vector<std::string> SelectMultipleFiles(const std::string& filter = "All Files|*.*", const std::string& title = "Select Files")
        {
            // Platform specific implementation needed
            throw std::runtime_error("Platform specific implementation required");
        }

        static std::string SelectFile(const std::string& filter = "All Files|*.*", const std::string& title = "Select File")
        {
            // Platform specific implementation needed
            throw std::runtime_error("Platform specific implementation required");
        }

        static std::string SaveFile(const std::string& filter = "All Files|*.*", const std::string& title = "Save File")
        {
            // Platform specific implementation needed
            throw std::runtime_error("Platform specific implementation required");
        }

        static std::string SelectFolder(const std::string& description = "Select Folder")
        {
            // Platform specific implementation needed
            throw std::runtime_error("Platform specific implementation required");
        }

        static std::string BrowseFile(const std::vector<std::string>& extensions = {})
        {
            // Platform specific implementation needed
            throw std::runtime_error("Platform specific implementation required");
        }

        static ToolFile BrowseToolFile(const std::vector<std::string>& extensions = {})
        {
            return ToolFile(BrowseFile(extensions));
        }

        // Helper for creating files inside directory
        ToolFile& MakeFileInside(const std::string& source, bool isDeleteSource = false)
        {
            if (!IsDir()) throw std::runtime_error("Target is not a directory");
            auto destPath = FullPath / std::filesystem::path(source).filename();

            if (isDeleteSource) {
                std::filesystem::rename(source, destPath);
            } else {
                std::filesystem::copy_file(source, destPath);
            }
            return *this;
        }

        ~ToolFile()
        {
            Close();
        }
    };
}

#endif // Convention_Runtime_File_hpp
