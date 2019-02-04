#include "application.h"

int main(int argc, char **argv)
{
    std::string iniFileLocation = "";
    if(argc > 1)
    {
        for(int i = 0; i < argc; i++){
        if ((std::string(argv[i]).find(std::string(".ini")))!= std::string::npos){
            iniFileLocation = argv[i];
            break;
            }
        }
    }
    Application *app = new Application(iniFileLocation);
    app->Run();
    delete app;
    return 0;
}
