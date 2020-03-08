

#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <imgui.h>
#include <vector>
#include <string>
#include <map>
#include <initializer_list>
#include <iostream>
#include <queue>
#include <sstream>
#include <fstream>
#include <tuple>
#include "types.h"
#include "Logger.h"

template <typename TT> using uptr = std::unique_ptr<TT>;
