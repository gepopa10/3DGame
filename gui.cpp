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

    FrameBuffer fb{768, 608, std::vector<uint32_t>(768*608, pack_color(255, 255, 255))};

    Texture death_knight = Texture("../death_knight.png", SDL_PIXELFORMAT_ABGR8888, 4, 3);        // textures for the monsters
    Texture hans = Texture("../hans.png", SDL_PIXELFORMAT_ABGR8888, 4, 3);
    Texture grosse = Texture("../grosse.png", SDL_PIXELFORMAT_ABGR8888, 4, 3);
    Texture static_monsters = Texture("../monsters.bmp", SDL_PIXELFORMAT_ABGR8888, 4, 1);
    Texture ss_guard = Texture("../ss.png", SDL_PIXELFORMAT_ABGR8888, 8, 7);
    Texture guard = Texture("../guard.png", SDL_PIXELFORMAT_ABGR8888, 8, 7);
    Texture weapons = Texture("../weapons.png", SDL_PIXELFORMAT_ABGR8888, 5, 4);
    Texture objects = Texture("../objects_crop.png", SDL_PIXELFORMAT_ABGR8888, 5, 10);
    Texture hitler = Texture("../hitler.png", SDL_PIXELFORMAT_ABGR8888, 7, 6);

    // GameState gs{ Map(),                                                                                             // game map
    //               {1.456, 1.345, weapons, M_PI/2, M_PI/3., 0, 0, 0, 0.5, true ,100},                                 // player
    //               {std::make_shared<Monster>(1.523, 8.812, static_monsters, 3, 0, 0.6, 0, 0, 100),
    //                std::make_shared<Hitler>(1.523, 8.812, hitler),
    //                std::make_shared<Guard>(13.523, 2.812, ss_guard)},
    //                Texture("../hitler_walls.png", SDL_PIXELFORMAT_ABGR8888, 10, 1)};                                // textures for the walls

    float offset = 0.5; //to put elements in the middle of a cell so we can specify only the left upper coordinate of cell
    GameState gs{ Map(),
                  {1.456, 1.345, weapons, M_PI/2, M_PI/3., 0, 0, 0, 0.5, true ,100},


                  {
                    //Elements

                    //Zone 1
                    //Sprites
                    std::make_shared<Sprite>(1+offset, 4+offset, objects, 11),
                    std::make_shared<Sprite>(4+offset, 3+offset, objects, 21),
                    std::make_shared<Sprite>(1+offset, 7+offset, objects, 38),
                    std::make_shared<Sprite>(5+offset, 6+offset, objects, 21),
                    std::make_shared<Sprite>(6+offset, 7+offset, objects, 37),
                    std::make_shared<Sprite>(7+offset, 4+offset, objects, 37),
                    std::make_shared<Sprite>(9+offset, 4+offset, objects, 37),
                    std::make_shared<Sprite>(8+offset, 6+offset, objects, 11),
                    std::make_shared<Sprite>(9+offset, 7+offset, objects, 38),
                    //Monsters
                    std::make_shared<Guard>(4+offset, 5+offset, guard),
                    std::make_shared<Guard>(8+offset, 5+offset, guard),
                    std::make_shared<Guard>(8+offset, 2+offset, guard),
                    std::make_shared<Guard>(2+offset, 7+offset, guard),

                    //Zone 2
                    //Sprites
                    std::make_shared<Sprite>(15+offset, 8+offset, objects, 9),
                    std::make_shared<Sprite>(15+offset, 12+offset, objects, 9),
                    std::make_shared<Sprite>(15+offset, 16+offset, objects, 9),
                    std::make_shared<Sprite>(13+offset, 9+offset, objects, 2),
                    std::make_shared<Sprite>(13+offset, 13+offset, objects, 3),
                    std::make_shared<Sprite>(13+offset, 9+offset, objects, 2),
                    std::make_shared<Sprite>(13+offset, 16+offset, objects, 3),
                    std::make_shared<Sprite>(19+offset, 13+offset, objects, 9),
                    std::make_shared<Sprite>(19+offset, 16+offset, objects, 3),
                    std::make_shared<Sprite>(22+offset, 16+offset, objects, 3),
                    std::make_shared<Sprite>(21+offset, 10+offset, objects, 2),
                    std::make_shared<Sprite>(21+offset, 5+offset, objects, 2),
                    std::make_shared<Sprite>(20+offset, 2+offset, objects, 2),
                    std::make_shared<Sprite>(23+offset, 2+offset, objects, 2),
                    //Monsters
                    std::make_shared<Guard>(13+offset, 7+offset, guard),
                    std::make_shared<Guard>(13+offset, 12+offset, guard),
                    std::make_shared<Guard>(13+offset, 16+offset, guard),
                    std::make_shared<Guard>(21+offset, 11+offset, guard),
                    std::make_shared<Guard>(21+offset, 14+offset, guard),
                    std::make_shared<Guard>(20+offset, 3+offset, guard),
                    std::make_shared<Guard>(20+offset, 6+offset, guard),
                    std::make_shared<Guard>(22+offset, 6+offset, guard),
                    std::make_shared<Guard>(22+offset, 3+offset, guard),
                    std::make_shared<Boss>(21+offset, 1+offset, death_knight,3, 20, 0.6, M_PI/2, 0, 500),

                    //Zone 3
                    //Sprites
                    std::make_shared<Sprite>(26+offset, 2+offset, objects, 40),
                    std::make_shared<Sprite>(28+offset, 2+offset, objects, 40),
                    std::make_shared<Sprite>(25+offset, 8+offset, objects, 48),
                    std::make_shared<Sprite>(25+offset, 10+offset, objects, 48),
                    std::make_shared<Sprite>(25+offset, 16+offset, objects, 48),
                    std::make_shared<Sprite>(25+offset, 18+offset, objects, 48),
                    std::make_shared<Sprite>(30+offset, 6+offset, objects, 48),
                    std::make_shared<Sprite>(30+offset, 12+offset, objects, 48),
                    std::make_shared<Sprite>(30+offset, 14+offset, objects, 48),
                    std::make_shared<Sprite>(30+offset, 20+offset, objects, 48),
                    std::make_shared<Sprite>(30+offset, 23+offset, objects, 41),
                    std::make_shared<Sprite>(25+offset, 23+offset, objects, 41),
                    std::make_shared<Sprite>(30+offset, 28+offset, objects, 16),
                    std::make_shared<Sprite>(28+offset, 28+offset, objects, 16),
                    std::make_shared<Sprite>(26+offset, 28+offset, objects, 16),
                    std::make_shared<Sprite>(26+offset, 30+offset, objects, 16),
                    std::make_shared<Sprite>(28+offset, 30+offset, objects, 16),
                    std::make_shared<Sprite>(30+offset, 30+offset, objects, 16),
                    //Monsters
                    std::make_shared<Guard>(29+offset, 2+offset, ss_guard),
                    std::make_shared<Guard>(26+offset, 7+offset, ss_guard),
                    std::make_shared<Guard>(30+offset, 7+offset, ss_guard),
                    std::make_shared<Guard>(30+offset, 11+offset, ss_guard),
                    std::make_shared<Guard>(30+offset, 15+offset, ss_guard),
                    std::make_shared<Guard>(30+offset, 19+offset, ss_guard),
                    std::make_shared<Guard>(30+offset, 22+offset, ss_guard),
                    std::make_shared<Guard>(25+offset, 22+offset, ss_guard),
                    std::make_shared<Guard>(25+offset, 19+offset, ss_guard),
                    std::make_shared<Guard>(25+offset, 15+offset, ss_guard),
                    std::make_shared<Guard>(25+offset, 11+offset, ss_guard),
                    std::make_shared<Boss>(25+offset, 25+offset, grosse,3, 20, 0.6, M_PI/2, 0, 1000, true),
                    std::make_shared<Boss>(29+offset, 25+offset, hans,3, 20, 0.6, M_PI/2, 0, 1000, true),

                    //Zone 4
                    //Sprites
                    std::make_shared<Sprite>(21+offset, 30+offset, objects, 16),
                    std::make_shared<Sprite>(21+offset, 28+offset, objects, 4),
                    std::make_shared<Sprite>(21+offset, 27+offset, objects, 16),
                    std::make_shared<Sprite>(22+offset, 21+offset, objects, 16),
                    std::make_shared<Sprite>(22+offset, 19+offset, objects, 4),
                    std::make_shared<Sprite>(18+offset, 20+offset, objects, 16),
                    std::make_shared<Sprite>(14+offset, 19+offset, objects, 4),
                    std::make_shared<Sprite>(14+offset, 20+offset, objects, 16),
                    std::make_shared<Sprite>(12+offset, 19+offset, objects, 17),
                    std::make_shared<Sprite>(12+offset, 20+offset, objects, 46),
                    std::make_shared<Sprite>(18+offset, 22+offset, objects, 4),
                    std::make_shared<Sprite>(18+offset, 23+offset, objects, 17),
                    std::make_shared<Sprite>(20+offset, 24+offset, objects, 46),
                    std::make_shared<Sprite>(16+offset, 24+offset, objects, 16),
                    std::make_shared<Sprite>(14+offset, 24+offset, objects, 16),
                    std::make_shared<Sprite>(12+offset, 24+offset, objects, 17),
                    std::make_shared<Sprite>(12+offset, 23+offset, objects, 4),
                    std::make_shared<Sprite>(12+offset, 25+offset, objects, 46),
                    std::make_shared<Sprite>(17+offset, 28+offset, objects, 16),
                    std::make_shared<Sprite>(14+offset, 28+offset, objects, 16),
                    std::make_shared<Sprite>(17+offset, 30+offset, objects, 16),
                    std::make_shared<Sprite>(14+offset, 30+offset, objects, 16),
                    std::make_shared<Sprite>(8+offset, 28+offset, objects, 18),
                    std::make_shared<Sprite>(6+offset, 28+offset, objects, 18),
                    std::make_shared<Sprite>(4+offset, 28+offset, objects, 18),
                    std::make_shared<Sprite>(2+offset, 28+offset, objects, 18),
                    std::make_shared<Sprite>(8+offset, 30+offset, objects, 18),
                    std::make_shared<Sprite>(6+offset, 30+offset, objects, 18),
                    std::make_shared<Sprite>(4+offset, 30+offset, objects, 18),
                    std::make_shared<Sprite>(2+offset, 30+offset, objects, 18),
                    //Monsters
                    std::make_shared<Guard>(21+offset, 29+offset, ss_guard),
                    std::make_shared<Guard>(22+offset, 27+offset, ss_guard),
                    std::make_shared<Guard>(20+offset, 20+offset, ss_guard),
                    std::make_shared<Guard>(17+offset, 20+offset, ss_guard),
                    std::make_shared<Guard>(13+offset, 20+offset, ss_guard),
                    std::make_shared<Guard>(17+offset, 25+offset, ss_guard),
                    std::make_shared<Guard>(14+offset, 25+offset, ss_guard),
                    std::make_shared<Guard>(18+offset, 29+offset, ss_guard),
                    std::make_shared<Guard>(15+offset, 29+offset, ss_guard),
                    std::make_shared<Guard>(12+offset, 29+offset, ss_guard),

                    //Zone 4
                    //Sprites
                    std::make_shared<Sprite>(9+offset, 20+offset, objects, 7),
                    std::make_shared<Sprite>(1+offset, 20+offset, objects, 7),
                    std::make_shared<Sprite>(4+offset, 11+offset, objects, 28),
                    std::make_shared<Sprite>(2+offset, 11+offset, objects, 7),
                    std::make_shared<Sprite>(2+offset, 13+offset, objects, 7),
                    std::make_shared<Sprite>(1+offset, 13+offset, objects, 21),
                    std::make_shared<Sprite>(3+offset, 15+offset, objects, 21),
                    std::make_shared<Sprite>(2+offset, 15+offset, objects, 24),
                    std::make_shared<Sprite>(1+offset, 15+offset, objects, 21),
                    //Monsters
                    std::make_shared<Guard>(2+offset, 25+offset, ss_guard),
                    std::make_shared<Guard>(2+offset, 23+offset, ss_guard),
                    std::make_shared<Guard>(2+offset, 21+offset, ss_guard),
                    std::make_shared<Guard>(8+offset, 25+offset, ss_guard),
                    std::make_shared<Guard>(8+offset, 23+offset, ss_guard),
                    std::make_shared<Guard>(8+offset, 21+offset, ss_guard),
                    std::make_shared<Guard>(5+offset, 19+offset, ss_guard),
                    std::make_shared<Guard>(5+offset, 17+offset, ss_guard),
                    std::make_shared<Guard>(7+offset, 13+offset, ss_guard),
                    std::make_shared<Guard>(9+offset, 13+offset, ss_guard),
                    std::make_shared<Guard>(9+offset, 10+offset, ss_guard),
                    std::make_shared<Guard>(7+offset, 10+offset, ss_guard),
                    std::make_shared<Guard>(2+offset, 10+offset, ss_guard),

                    std::make_shared<Hitler>(1+offset, 14+offset, hitler, 3, 20, 0.6, M_PI/2, 0, 2000)
                  },
                   //wall textures
                   Texture("../hitler_walls.png", SDL_PIXELFORMAT_ABGR8888, 10, 1)};

    if (!gs.tex_walls.count || !death_knight.count) {
        std::cerr << "Failed to load textures" << std::endl;
        return -1;
    }

    Window guiWindow(fb, gs);
    Event eventHandler(gs);

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
        gs.update(elapsed_secs.count());  //update positions at the same rate as eventHandler which moves player

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
