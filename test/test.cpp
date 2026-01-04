#include "iostream"
#include "vulkan/vulkan.h"

class Vulkan{
    VkApplicationInfo appinfo;
    VkInstance instance;


    public:
    Vulkan(){};

    void init(std::string appname, std::string enginename){
        appinfo.apiVersion=VK_API_VERSION_1_2;
        appinfo.applicationVersion=VK_MAKE_VERSION(1,0,0);
        appinfo.engineVersion=VK_MAKE_VERSION(1,0,0);
        appinfo.pApplicationName=appname.c_str();
        appinfo.pEngineName=enginename.c_str();
        appinfo.pNext=NULL;
        appinfo.sType=VK_STRUCTURE_TYPE_APPLICATION_INFO;

        VkInstanceCreateInfo createInfo={};
        createInfo.
    }
};