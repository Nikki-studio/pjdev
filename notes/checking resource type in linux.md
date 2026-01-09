# This is just a sample code showing us how to distiguish the resource type in Linux
```cpp
#include <filesystem>
#include <iostream>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
namespace fs = std::filesystem;

void checkFileType(const std::string& path) {
    if (fs::exists(path)) {
        std::cout << "Path: " << path << std::endl;
        
        // Check file type using filesystem
        auto status = fs::status(path);
        auto type = status.type();
        
        if (type == fs::file_type::regular)
            std::cout << "  Type: Regular file" << std::endl;
        else if (type == fs::file_type::directory)
            std::cout << "  Type: Directory" << std::endl;
        else if (type == fs::file_type::character)
            std::cout << "  Type: Character device" << std::endl;
        else if (type == fs::file_type::block)
            std::cout << "  Type: Block device" << std::endl;
        else if (type == fs::file_type::fifo)
            std::cout << "  Type: FIFO/pipe" << std::endl;
        else if (type == fs::file_type::socket)
            std::cout << "  Type: Socket" << std::endl;
        else
            std::cout << "  Type: Other" << std::endl;
        
        // Get additional info using stat
        struct stat sb;
        if (stat(path.c_str(), &sb) == 0) {
            if (S_ISREG(sb.st_mode))  std::cout << "  stat: Regular file" << std::endl;
            if (S_ISDIR(sb.st_mode))  std::cout << "  stat: Directory" << std::endl;
            if (S_ISCHR(sb.st_mode))  std::cout << "  stat: Character device" << std::endl;
            if (S_ISBLK(sb.st_mode))  std::cout << "  stat: Block device" << std::endl;
            if (S_ISFIFO(sb.st_mode)) std::cout << "  stat: FIFO" << std::endl;
            if (S_ISSOCK(sb.st_mode)) std::cout << "  stat: Socket" << std::endl;
        }
    } else {
        std::cout << "Path doesn't exist: " << path << std::endl;
    }
    std::cout << "----------------" << std::endl;
}

int main() {
    // Check different types
    checkFileType("/dev/null");        // Character device
    checkFileType("/dev/sda");         // Block device
    checkFileType("/tmp/mysocket");    // Socket (if created)
    checkFileType("/etc/passwd");      // Regular file
    checkFileType("/tmp");             // Directory
    checkFileType("/proc/self/fd/0");  // Symbolic link (to stdin)
    
    return 0;
}
```
