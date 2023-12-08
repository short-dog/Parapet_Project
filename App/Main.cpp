//
// Created by BSHORT2 on 11/1/2023.
//
#include "src/Parapet.h"

int main(){
    windowSettings settings;
    settings.setsize(1024, 768);
    settings.windowMode = windowed;

    auto window = createWindow(settings);

    runApp(window, std::make_shared<Parapet>());
    runProgram();

    return 1;
}