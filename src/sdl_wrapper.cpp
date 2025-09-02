#include "sdl_wrapper.hpp"
#include <iostream>
#include <cstdlib>

#define Error(mes) cerr << __func__<< "(): " << mes << endl

extern "C" {
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
}

namespace okazawa
{

using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::map;

Window::~Window()
{
}

class _Window : public Window{
    SDL_Window *m_win;
    SDL_Renderer *m_rend;
    map<uint32_t, SDL_Texture*> m_textures;
    bool inited;
public:
    _Window(const string& title, int width, int height)
        : m_win(nullptr)
        , m_rend(nullptr)
        , inited(false)
    {
        m_win = SDL_CreateWindow(
            title.c_str(),                  // ウィンドウタイトル
            SDL_WINDOWPOS_CENTERED,         // X位置
            SDL_WINDOWPOS_CENTERED,         // Y位置
            width,                            // 幅
            height,                            // 高さ
            SDL_WINDOW_SHOWN                // 表示フラグ
        );
        if(!m_win) {
            Error(SDL_GetError());
            return;
        }

        m_rend = SDL_CreateRenderer(m_win, -1, SDL_RENDERER_ACCELERATED);
        if(!m_rend) {
            Error(SDL_GetError());
            SDL_DestroyWindow(m_win);
            m_win = nullptr;
            return;
        }

        inited = true;
    }

    ~_Window()
    {
        for(auto& [id, t]: m_textures) {
            SDL_DestroyTexture(t);
            t = nullptr;
        }
        m_textures.clear();
        if(m_rend) {
            SDL_DestroyRenderer(m_rend);
            m_rend = nullptr;
        }
        if(m_win) {
            SDL_DestroyWindow(m_win);
            m_win = nullptr;
        }
    }

    bool is_inited() const override
    {
        return inited;
    }

    uint32_t mktexture(const string& image_path) override
    {
        if(!inited) {
            Error("Instance is not initialized.");
            return 0;
        }
        SDL_Surface* s = IMG_Load(image_path.c_str());
        if(!s) {
            Error(IMG_GetError());
            return 0;
        }

        SDL_Texture* t = SDL_CreateTextureFromSurface(m_rend, s);
        SDL_FreeSurface(s);

        if(!t) {
            Error(SDL_GetError());
            return 0;
        }
        uint32_t id = 1;
        for(const auto&[i, t] : m_textures) {
            if(id == i) id++;
        }
        m_textures[id] = t;
        return id;
    }

    uint32_t mktexture(int sizex, int sizey) override
    {
        if(!inited) {
            Error("Instance is not initialized.");
            return 0;
        }
        SDL_Texture* t = SDL_CreateTexture(m_rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, sizex, sizey);
        if(!t) {
            Error(SDL_GetError());
            return 0;
        }

        uint32_t id = 1;
        for(const auto&[i, t] : m_textures) {
            if(id == i) id++;
        }
        m_textures[id] = t;
        return id;
    }

    void rmtexture(uint32_t id) override
    {
        if(!inited) {
            Error("Instance is not initialized.");
            return;
        }
        if(!m_textures.count(id)) {
           Error("no such a texture.");
            return;
        }

        SDL_Texture* t = m_textures[id];
        SDL_DestroyTexture(t);
        m_textures.erase(id);
    }

    uint32_t get_id() const override
    {
        if(!inited) {
            Error("Instance is not initialized.");
            return 0;
        }
        return SDL_GetWindowID(m_win);
    }

    void set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) override
    {
        if(!inited) {
            Error("Instance is not initialized.");
            return;
        }
        SDL_SetRenderDrawColor(m_rend, r, g, b, a);
    }

    void scrclear() override
    {
        if(!inited) {
            Error("Instance is not initialized.");
            return;
        }
        SDL_RenderClear(m_rend);
    }

    void update() override
    {
        if(!inited) {
            Error("Instance is not initialized.");
            return;
        }
        SDL_RenderPresent(m_rend);
    }

    void draw_point(vec2d pos) override
    {
        if(!inited) {
            Error("Instance is not initialized.");
            return;
        }
        SDL_RenderDrawPoint(m_rend, pos.get_x(), pos.get_y());
    }

    void draw_points(const vector<vec2d>& pos) override
    {
        if(!inited) {
            Error("Instance is not initialized.");
            return;
        }
        vector<SDL_Point> tmp(pos.size());
        int i = 0;
        for(const auto& p: pos) {
            tmp[i++] = (SDL_Point){p.get_x(), p.get_y()};
        }
        SDL_RenderDrawPoints(m_rend, tmp.data(), tmp.size());
    }

    void draw_line(line2d pos) override
    {
        if(!inited) {
            Error("Instance is not initialized.");
            return;
        }
        SDL_RenderDrawLine(m_rend, pos.begin.get_x(), pos.begin.get_y(), pos.end.get_x(), pos.end.get_y());
    }

    void draw_lines(const vector<vec2d>& pos) override
    {
        if(!inited) {
            Error("Instance is not initialized.");
            return;
        }
        vector<SDL_Point> tmp(pos.size());
        int i = 0;
        for(const auto& p: pos) {
            tmp[i++] = (SDL_Point){p.get_x(), p.get_y()};
        }
        SDL_RenderDrawLines(m_rend, tmp.data(), tmp.size());
    }

    void draw_rect(rect2d pos) override
    {
        if(!inited) {
            Error("Instance is not initialized.");
            return;
        }
        SDL_Rect tmp = {pos.ref.get_x(), pos.ref.get_y(), pos.size.get_x(), pos.size.get_y()};
        SDL_RenderDrawRect(m_rend, &tmp);
    }

    void draw_rects(const vector<rect2d>& pos) override
    {
        if(!inited) {
            Error("Instance is not initialized.");
            return;
        }
        vector<SDL_Rect> tmp(pos.size());
        int i = 0;
         for(const auto& p: pos) {
            tmp[i++] = (SDL_Rect){p.ref.get_x(), p.ref.get_y(), p.size.get_x(), p.size.get_y()};
        }
        SDL_RenderDrawRects(m_rend, tmp.data(), tmp.size());
    }

    void fill_rect(rect2d pos) override
    {
        if(!inited) {
            Error("Instance is not initialized.");
            return;
        }
        SDL_Rect tmp = {pos.ref.get_x(), pos.ref.get_y(), pos.size.get_x(), pos.size.get_y()};
        SDL_RenderFillRect(m_rend, &tmp);
    }

    void fill_rects(const vector<rect2d>& pos) override
    {
        if(!inited) {
            Error("Instance is not initialized.");
            return;
        }
        vector<SDL_Rect> tmp(pos.size());
        int i = 0;
         for(const auto& p: pos) {
            tmp[i++] = (SDL_Rect){p.ref.get_x(), p.ref.get_y(), p.size.get_x(), p.size.get_y()};
        }
        SDL_RenderFillRects(m_rend, tmp.data(), tmp.size());
    }

    void draw_texture(uint32_t id, rect2d pos) override
    {
        if(!inited) {
            Error("Instance is not initialized.");
            return;
        }
        if(!m_textures.count(id)) {
           Error("no such a texture.");
            return;
        }
        SDL_Rect tmp = {pos.ref.get_x(), pos.ref.get_y(), pos.size.get_x(), pos.size.get_y()};
        SDL_RenderCopy(m_rend, m_textures[id], NULL, &tmp);
    }
};

SDL::SDL()
    : m_windows()
    , m_inited(false)
    , m_user_data(nullptr)
    , m_sdl_event(new SDL_Event)
    , m_exit(false)
    , m_disabled(false)
{
    if(SDL::created) {
        m_disabled = true;
        return;
    }
    SDL::created = true;
}

SDL::~SDL()
{
    if(!m_disabled) for(auto& [id, w]: m_windows) {
        delete w;
    }
    m_windows.clear();
    if(m_sdl_event) delete reinterpret_cast<SDL_Event*>(m_sdl_event);
    if(!m_disabled) SDL_Quit();
}

int SDL::init()
{
    if(m_disabled) return -1;
    // SDLの初期化
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cerr << "okazawa::SDL::init(): " << SDL_GetError() << endl;
        return 1;
    }

    // SDL_imageの初期化 (PNG対応)
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cerr << "okazawa::SDL::init(): " << SDL_GetError() << endl;
        return 1;
    }
    m_inited = true;
    return 0;
}

uint32_t SDL::mkwindow(const string& title, int width, int height)
{
    if(!m_inited) {
        Error("Instance is not initialized.");
        return 0;
    }

    Window* ret = new _Window(title, width, height);
    if(!ret->is_inited()) {
        delete ret;
        Error("window creating failed.");
        return 0;
    }

    m_windows[ret->get_id()] = ret;
    return ret->get_id();
}

void SDL::rmwindow(uint32_t winid)
{
    if(!m_inited) {
        Error("Instance is not initialized.");
        return;
    }
    if(!m_windows.count(winid)) {
        Error("no such a window (winid==" << winid << ").");
        return;
    }
    auto w = m_windows[winid];
    m_windows.erase(winid);
    delete w;

    if(m_windows.size() == 0) m_exit = true;
}

void SDL::rmwindow(Window* win)
{
    if(!m_inited) {
        Error("Instance is not initialized.");
        return;
    }
    for(auto iter = m_windows.begin(); iter != m_windows.end(); ) {
        if(iter->second == win) {
            iter = m_windows.erase(iter);
            delete win;
        } else iter++;
    }

    if(m_windows.size() == 0) m_exit = true;
}

void SDL::set_user_data_ptr(void *ptr)
{
    m_user_data = ptr;
}

vector<uint32_t> SDL::get_winid() const
{
    vector<uint32_t> ret;
    for(const auto& [id, w]: m_windows) {
        ret.push_back(id);
    }
    return ret;
}

Window* SDL::get_win(uint32_t winid) const
{
    if(!m_inited) {
        Error("Instance is not initialized.");
        return nullptr;
    }
    if(m_windows.count(winid)) {
        return m_windows.at(winid);
    }
    Error("winid is invalid.");
    return nullptr;
}

int SDL::mainloop()
{
    if(!m_inited) {
        Error("Instance is not initialized.");
        return 1;
    }
    if(callback_funcs.draw == nullptr) {
        Error("draw() is not set.");
        return 1;
    }

    SDL_Event& e = *reinterpret_cast<SDL_Event *>(m_sdl_event);

    if(callback_funcs.init) callback_funcs.init(this, m_user_data);

    while (!m_exit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT: m_exit = true; break;

                case SDL_APP_TERMINATING:
                    if(callback_funcs.terminate) callback_funcs.terminate(this, m_user_data);
                    std::exit(-1);
                    break;
                case SDL_APP_LOWMEMORY: break;
                case SDL_APP_WILLENTERBACKGROUND: break;
                case SDL_APP_DIDENTERBACKGROUND: break;
                case SDL_APP_WILLENTERFOREGROUND: break;
                case SDL_APP_DIDENTERFOREGROUND: break;

                case SDL_WINDOWEVENT:
                    if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
                        // 閉じるボタンが押された
                        rmwindow(e.window.windowID);
                    }
                    break;
                /*
                case SDL_SYSWMEVENT: break;

                case SDL_KEYDOWN: break;
                case SDL_KEYUP: break;
                case SDL_TEXTEDITING: break;
                case SDL_TEXTINPUT: break;
                case SDL_KEYMAPCHANGED: break;

                case SDL_MOUSEMOTION: break;
                case SDL_MOUSEBUTTONDOWN: break;
                case SDL_MOUSEBUTTONUP: break;
                case SDL_MOUSEWHEEL: break;

                case SDL_JOYAXISMOTION: break;
                case SDL_JOYBALLMOTION: break;
                case SDL_JOYHATMOTION: break;
                case SDL_JOYBUTTONDOWN: break;
                case SDL_JOYBUTTONUP: break;
                case SDL_JOYDEVICEADDED: break;
                case SDL_JOYDEVICEREMOVED: break;

                case SDL_CONTROLLERAXISMOTION: break;
                case SDL_CONTROLLERBUTTONDOWN: break;
                case SDL_CONTROLLERBUTTONUP: break;
                case SDL_CONTROLLERDEVICEADDED: break;
                case SDL_CONTROLLERDEVICEREMOVED: break;
                case SDL_CONTROLLERDEVICEREMAPPED: break;

                case SDL_FINGERDOWN: break;
                case SDL_FINGERUP: break;
                case SDL_FINGERMOTION: break;

                case SDL_DOLLARGESTURE: break;
                case SDL_DOLLARRECORD: break;
                case SDL_MULTIGESTURE: break;

                case SDL_CLIPBOARDUPDATE: break;

                case SDL_DROPFILE: break;
                case SDL_DROPTEXT: break;
                case SDL_DROPBEGIN: break;
                case SDL_DROPCOMPLETE: break;

                case SDL_AUDIODEVICEADDED: break;
                case SDL_AUDIODEVICEREMOVED: break;

                case SDL_RENDER_TARGETS_RESET: break;
                case SDL_RENDER_DEVICE_RESET: break;
                */

                default: break;  // 未知のイベント
            }
        }
        for(auto&[winid, win]: m_windows) {
            callback_funcs.draw(this, winid, m_user_data);
            m_windows[winid]->update();
        }
        
        SDL_Delay(1);
    }

    if(callback_funcs.exit) callback_funcs.exit(this, m_user_data);
    return 0;
}



}