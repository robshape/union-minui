#ifndef _FILEUTILS_H_
#define _FILEUTILS_H_

#include <string>
#include <vector>

namespace File_utils
{
    // File operations

    void copyFile(const std::vector<std::string> &p_src, const std::string &p_dest);

    void moveFile(const std::vector<std::string> &p_src, const std::string &p_dest);

    void removeFile(const std::vector<std::string> &p_files);

    void executeFile(const std::string &p_file);

    void makeDirectory(const std::string &p_file);

    void renameFile(const std::string &p_file1, const std::string &p_file2);

    // File utilities

    const bool fileExists(const std::string &p_path);

    const unsigned long int getFileSize(const std::string &p_file);

    void formatSize(std::string &p_size);

    const std::string getFileName(const std::string &p_path);

    const std::string getPath(const std::string &p_path);

    const std::string getSelfExecutionPath(void);

    const std::string getSelfExecutionName(void);

    void stringReplace(std::string &p_string, const std::string &p_search, const std::string &p_replace);

    const std::string specialChars(const std::string &p_string);

    // Dialogs

    void diskInfo(void);

    void diskUsed(const std::vector<std::string> &p_files);
}

#endif
