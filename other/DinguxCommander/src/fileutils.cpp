#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sstream>
#include "fileutils.h"
#include "def.h"
#include "dialog.h"
#include "sdlutils.h"

#define SPECIAL_CHARS "\\`$();|{}&'\"*?<>[]!^~-#\n\r "

void File_utils::copyFile(const std::vector<std::string> &p_src, const std::string &p_dest)
{
    std::string l_command("");
    std::string l_destFile("");
    std::string l_fileName("");
    bool l_loop(true);
    bool l_confirm(true);
    bool l_execute(true);
    for (std::vector<std::string>::const_iterator l_it = p_src.begin(); l_loop && (l_it != p_src.end()); ++l_it)
    {
        l_execute = true;
        // Check if destination files already exists
        if (l_confirm)
        {
            l_fileName = getFileName(*l_it);
            l_destFile = p_dest + (p_dest.at(p_dest.size() - 1) == '/' ? "" : "/") + l_fileName;
            if (fileExists(l_destFile))
            {
                INHIBIT(std::cout << "File " << l_destFile << " already exists => ask for confirmation" << std::endl;)
                CDialog l_dialog("Question:", 0, 0);
                l_dialog.addLabel("Overwrite " + l_fileName + "?");
                l_dialog.addOption("Yes");
                l_dialog.addOption("Yes to all");
                l_dialog.addOption("No");
                l_dialog.addOption("Cancel");
                l_dialog.init();
                switch (l_dialog.execute())
                {
                    case 1:
                        // Yes
                        break;
                    case 2:
                        // Yes to all
                        l_confirm = false;
                        break;
                    case 3:
                        // No
                        l_execute = false;
                        break;
                    default:
                        // Cancel
                        l_execute = false;
                        l_loop = false;
                        break;
                }
            }
        }
        if (l_execute)
        {
            // Waiting message
            SDL_utils::pleaseWait();
            l_command = "\\cp -r " + specialChars(*l_it) + " " + specialChars(p_dest);
            INHIBIT(std::cout << "Command: " << l_command << std::endl;)
            system(l_command.c_str());
        }
    }
}

void File_utils::moveFile(const std::vector<std::string> &p_src, const std::string &p_dest)
{
    std::string l_command("");
    std::string l_destFile("");
    std::string l_fileName("");
    bool l_loop(true);
    bool l_confirm(true);
    bool l_execute(true);
    for (std::vector<std::string>::const_iterator l_it = p_src.begin(); l_loop && (l_it != p_src.end()); ++l_it)
    {
        l_execute = true;
        // Check if destination files already exists
        if (l_confirm)
        {
            l_fileName = getFileName(*l_it);
            l_destFile = p_dest + (p_dest.at(p_dest.size() - 1) == '/' ? "" : "/") + l_fileName;
            if (fileExists(l_destFile))
            {
                INHIBIT(std::cout << "File " << l_destFile << " already exists => ask for confirmation" << std::endl;)
                CDialog l_dialog("Question:", 0, 0);
                l_dialog.addLabel("Overwrite " + l_fileName + "?");
                l_dialog.addOption("Yes");
                l_dialog.addOption("Yes to all");
                l_dialog.addOption("No");
                l_dialog.addOption("Cancel");
                l_dialog.init();
                switch (l_dialog.execute())
                {
                    case 1:
                        // Yes
                        break;
                    case 2:
                        // Yes to all
                        l_confirm = false;
                        break;
                    case 3:
                        // No
                        l_execute = false;
                        break;
                    default:
                        // Cancel
                        l_execute = false;
                        l_loop = false;
                        break;
                }
            }
        }
        if (l_execute)
        {
            // Waiting message
            SDL_utils::pleaseWait();
            l_command = "\\mv " + specialChars(*l_it) + " " + specialChars(p_dest);
            INHIBIT(std::cout << "Command: " << l_command << std::endl;)
            system(l_command.c_str());
        }
    }
}

void File_utils::renameFile(const std::string &p_file1, const std::string &p_file2)
{
    bool l_execute(true);
    // Check if destination files already exists
    if (fileExists(p_file2))
    {
        INHIBIT(std::cout << "File " << p_file2 << " already exists => ask for confirmation" << std::endl;)
        CDialog l_dialog("Question:", 0, 0);
        l_dialog.addLabel("Overwrite " + getFileName(p_file2) + "?");
        l_dialog.addOption("Yes");
        l_dialog.addOption("No");
        l_dialog.init();
        if (l_dialog.execute() != 1)
            l_execute = false;
    }
    if (l_execute)
    {
        std::string l_command = "\\mv " + specialChars(p_file1) + " " + specialChars(p_file2);
        INHIBIT(std::cout << "Command: " << l_command << std::endl;)
        system(l_command.c_str());
    }
}

void File_utils::removeFile(const std::vector<std::string> &p_files)
{
    std::string l_command("");
    for (std::vector<std::string>::const_iterator l_it = p_files.begin(); l_it != p_files.end(); ++l_it)
    {
        l_command = "\\rm -rf " + specialChars(*l_it);
        INHIBIT(std::cout << "Command: " << l_command << std::endl;)
        system(l_command.c_str());
    }
}

void File_utils::makeDirectory(const std::string &p_file)
{
    std::string l_command = "\\mkdir -p " + specialChars(p_file);
    INHIBIT(std::cout << "Command: " << l_command << std::endl;)
    system(l_command.c_str());
}

const bool File_utils::fileExists(const std::string &p_path)
{
    struct stat l_stat;
    return stat(p_path.c_str(), &l_stat) == 0;
}

const std::string File_utils::getFileName(const std::string &p_path)
{
    size_t l_pos = p_path.rfind('/');
    return p_path.substr(l_pos + 1);
}

const std::string File_utils::getPath(const std::string &p_path)
{
    size_t l_pos = p_path.rfind('/');
    return p_path.substr(0, l_pos);
}

void File_utils::executeFile(const std::string &p_file)
{
    // Command
    std::string l_command = "./" + specialChars(getFileName(p_file));
    INHIBIT(std::cout << "File_utils::executeFile: " << l_command << " in " << getPath(p_file) << std::endl;)
    // CD to the file's location
    chdir(getPath(p_file).c_str());
    // Quit
    SDL_utils::hastalavista();
    // Execute file
    execlp("/bin/sh", "/bin/sh", "-c", l_command.c_str(), NULL);
    // If we're here, there was an error with the execution
    std::cerr << "Error executing file " << p_file << std::endl;
    // Relaunch DinguxCommander
    l_command = "./" + specialChars(getSelfExecutionName());
    INHIBIT(std::cout << "File_utils::executeFile: " << l_command << " in " << getSelfExecutionPath() << std::endl;)
    chdir(getSelfExecutionPath().c_str());
    execlp(l_command.c_str(), l_command.c_str(), NULL);
}

const std::string File_utils::getSelfExecutionPath(void)
{
    // Get execution path
    std::string l_exePath("");
    char l_buff[255];
    int l_i = readlink("/proc/self/exe", l_buff, 255);
    l_exePath = l_buff;
    l_exePath = l_exePath.substr(0, l_i);
    l_i = l_exePath.rfind("/");
    l_exePath = l_exePath.substr(0, l_i);
    return l_exePath;
}

const std::string File_utils::getSelfExecutionName(void)
{
    // Get execution path
    std::string l_exePath("");
    char l_buff[255];
    int l_i = readlink("/proc/self/exe", l_buff, 255);
    l_exePath = l_buff;
    l_exePath = l_exePath.substr(0, l_i);
    l_i = l_exePath.rfind("/");
    l_exePath = l_exePath.substr(l_i + 1);
    return l_exePath;
}

void File_utils::stringReplace(std::string &p_string, const std::string &p_search, const std::string &p_replace)
{
    // Replace all occurrences of p_search by p_replace in p_string
    size_t l_pos = p_string.find(p_search, 0);
    while (l_pos != std::string::npos)
    {
        p_string.replace(l_pos, p_search.length(), p_replace);
        l_pos = p_string.find(p_search, l_pos + p_replace.length());
    }
}

const std::string File_utils::specialChars(const std::string &p_string)
{
    // Insert a '\' before special characters
    std::string l_ret(p_string);
    const std::string l_specialChars(SPECIAL_CHARS);
    const size_t l_length = l_specialChars.size();
    std::string l_char("");
    for (unsigned int l_i = 0; l_i < l_length; ++l_i)
    {
        l_char = l_specialChars.substr(l_i, 1);
        stringReplace(l_ret, l_char, "\\" + l_char);
    }
    return l_ret;
}

const unsigned long int File_utils::getFileSize(const std::string &p_file)
{
    unsigned long int l_ret(0);
    struct stat l_stat;
    if (stat(p_file.c_str(), &l_stat) == -1)
        std::cerr << "File_utils::getFileSize: Error stat " << p_file << std::endl;
    else
        l_ret = l_stat.st_size;
    return l_ret;
}

void File_utils::diskInfo(void)
{
    std::string l_line("");
    // Execute command df -h
    {
        char l_buffer[256];
        FILE *l_pipe = popen("df -h", "r");
        if (l_pipe == NULL)
        {
            std::cerr << "File_utils::diskInfo: Error popen" << std::endl;
            return;
        }
        while (l_line.empty() && fgets(l_buffer, sizeof(l_buffer), l_pipe) != NULL)
            if (strstr(l_buffer, FILE_SYSTEM) != NULL)
                l_line = l_buffer;
        pclose(l_pipe);
    }
    if (!l_line.empty())
    {
        // Separate line by spaces
        std::istringstream l_iss(l_line);
        std::vector<std::string> l_tokens;
        copy(std::istream_iterator<std::string>(l_iss), std::istream_iterator<std::string>(), std::back_inserter<std::vector<std::string> >(l_tokens));
        // Display dialog
        CDialog l_dialog("Disk information:", 0, 0);
        l_dialog.addLabel("Size: " + l_tokens[1]);
        l_dialog.addLabel("Used: " + l_tokens[2] + " (" + l_tokens[4] + ")");
        l_dialog.addLabel("Available: " + l_tokens[3]);
        l_dialog.addOption("OK");
        l_dialog.init();
        l_dialog.execute();
    }
    else
        std::cerr << "File_utils::diskInfo: Unable to find " << FILE_SYSTEM << std::endl;
}

void File_utils::diskUsed(const std::vector<std::string> &p_files)
{
    std::string l_line("");
    // Waiting message
    SDL_utils::pleaseWait();
    // Build and execute command
    {
        std::string l_command("du -csh");
        for (std::vector<std::string>::const_iterator l_it = p_files.begin(); l_it != p_files.end(); ++l_it)
            l_command = l_command + " \"" + *l_it + "\"";
        char l_buffer[256];
        FILE *l_pipe = popen(l_command.c_str(), "r");
        if (l_pipe == NULL)
        {
            std::cerr << "File_utils::diskUsed: Error popen" << std::endl;
            return;
        }
        while (fgets(l_buffer, sizeof(l_buffer), l_pipe) != NULL);
        l_line = l_buffer;
        pclose(l_pipe);
    }
    // Separate line by spaces
    {
        std::istringstream l_iss(l_line);
        std::vector<std::string> l_tokens;
        copy(std::istream_iterator<std::string>(l_iss), std::istream_iterator<std::string>(), std::back_inserter<std::vector<std::string> >(l_tokens));
        l_line = l_tokens[0];
    }
    // Dialog
    std::ostringstream l_stream;
    CDialog l_dialog("Disk used:", 0, 0);
    l_stream << p_files.size() << " items selected";
    l_dialog.addLabel(l_stream.str());
    l_dialog.addLabel("Disk used: " + l_line);
    l_dialog.addOption("OK");
    l_dialog.init();
    l_dialog.execute();
}

void File_utils::formatSize(std::string &p_size)
{
    // Format 123456789 to 123,456,789
    int l_i = p_size.size() - 3;
    while (l_i > 0)
    {
        p_size.insert(l_i, ",");
        l_i -= 3;
    }
}
