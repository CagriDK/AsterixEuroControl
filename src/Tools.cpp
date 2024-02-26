#include "../include/Tools.h"

double Tools::getEpochTime()
{
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch());
    return static_cast<double>(ns.count()) * 0.000001; // convert to microseconds
}

bool Tools::isFileExist(const std::string &filePath)
{
    std::ifstream file(filePath);
    return file.good();
}

bool Tools::isDirectoryExist(const std::string &directoryPath)
{
    struct stat info;
    if (stat(directoryPath.c_str(), &info) != 0)
    {
        return false;
    }
    else if (info.st_mode & S_IFDIR)
    {
        return true;
    }
    return false;
}

int Tools::MakeDirectory(const std::string &dirPath)
{
    int retval = -1;
    if(!isDirectoryExist(dirPath))
    {
#ifdef WIN32
        retval = mkdir(dirPath.c_str());
#else
        retval = mkdir(dirPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif    
    }
    else{
        return retval;
    }
    return retval;
}

void Tools::waitReal_usec(size_t waitMicSec)
{
    struct timespec reqDelay = {0};
    reqDelay.tv_sec = 0;
    reqDelay.tv_nsec = waitMicSec * 1000;
    nanosleep(&reqDelay, (struct timespec *)NULL);
}

bool Tools::WriteToFile(const std::string &filePath, const std::string &data)
{
    bool retval = false;
    if(data != "" && filePath != "")
    {
        std::ofstream file(filePath);
        if (file.is_open())
        {
            file << data;
            file.flush();
            retval = true;
        }
        file.close();
    }
    return retval;
}