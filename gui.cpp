#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cmath>

#include "utils.h"
#include "tinyraycaster.h"
#include "window.h"
#include "event.h"

int main() {

    FrameBuffer fb{1024, 512, std::vector<uint32_t>(1024*512, pack_color(255, 255, 255))};

    GameState gs{ Map(),                                     // game map
                  {1.456, 1.345, M_PI/2, M_PI/3., 0, 0, 0, 0.5, 1},   // player
                  { {3.523, 9.812, 2, 0, 0.01, M_PI/2., 0, 100} }, //monsters lists
                  Texture("../walltext.bmp", SDL_PIXELFORMAT_ABGR8888),  // textures for the walls
                  Texture("../monsters.bmp", SDL_PIXELFORMAT_ABGR8888)}; // textures for the monsters

                  // GameState gs{ Map(),                                     // game map
                  //               {1.456, 1.345, 0.523, M_PI/3., 0, 0, 0, 0.5, 1},   // player
                  //               { {3.523, 3.812, 2, 0, 0.01, M_PI/2., 0, 100},
                  //             {1.834, 8.765, 0, 0, 0.01, M_PI/2., 0, 100},
                  //           {5.323, 5.365, 1, 0, 0.01, M_PI/2., 0, 100},
                  //         {14.32, 13.36, 3, 0, 0.01, M_PI/2., 0, 100},
                  //       {4.123, 10.76, 1, 0, 0.01, M_PI/2., 0, 100} }, //monsters lists
                                // Texture("../walltext.bmp", SDL_PIXELFORMAT_ABGR8888),  // textures for the walls
                                // Texture("../monsters.bmp", SDL_PIXELFORMAT_ABGR8888)}; // textures for the monsters
    if (!gs.tex_walls.count || !gs.tex_monst.count) {
        std::cerr << "Failed to load textures" << std::endl;
        return -1;
    }

    Window guiWindow(fb);
    Event eventHandler(gs.player);

    auto t1 = std::chrono::high_resolution_clock::now();
    auto t1_reload = std::chrono::high_resolution_clock::now();
    int fps_rate = 100; //Hz
    while (true) {
        // check elapsed time
        auto t2 = std::chrono::high_resolution_clock::now();
        auto t2_reload = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> fp_ms = t2 - t1;
        std::chrono::duration<double,  std::ratio<1>> elapsed_secs = t2 - t1;

        if (eventHandler.processEvent(t1_reload, t2_reload)) break; //check keyboard events at while loop frequency
        gs.update(elapsed_secs.count(), fb);  //update positions at the same rate as eventHandler which moves player

        if (fp_ms.count()<1/fps_rate*1000) { // sleep if less than faster than fps_rate
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            continue;
        }
        t1 = t2;

        render(fb, gs); // render the scene to the frambuffer
        guiWindow.display(); //display the scene using SDL
    }

    return 0;
}
