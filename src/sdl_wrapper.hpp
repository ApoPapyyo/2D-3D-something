#ifndef _SDLWRAPPER_HPP_
#define _SDLWRAPPER_HPP_

#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include "vec2d.hpp"

namespace okazawa
{

class Window {
public:
    virtual ~Window();
    virtual bool is_inited() const = 0;
    virtual uint32_t mktexture(const std::string& path)  = 0;
    virtual uint32_t mktexture(int sizex, int sizey) = 0;
    virtual void rmtexture(uint32_t id) = 0;
    virtual uint32_t get_id() const = 0;
    virtual void set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) = 0;
    virtual void scrclear() = 0;
    virtual void update() = 0;
    virtual void draw_point(vec2d pos) = 0;
    virtual void draw_points(const std::vector<vec2d>& pos) = 0;
    virtual void draw_line(line2d pos) = 0;
    virtual void draw_lines(const std::vector<vec2d>& pos) = 0;
    virtual void draw_rect(rect2d pos) = 0;
    virtual void draw_rects(const std::vector<rect2d>& pos) = 0;
    virtual void fill_rect(rect2d pos) = 0;
    virtual void fill_rects(const std::vector<rect2d>& pos) = 0;
    virtual void draw_texture(uint32_t id, rect2d pos) = 0;
};

class SDL {
    struct callbacks {
        void (*draw)(SDL*, uint32_t winid,void*) = nullptr;//描画
        void (*event)(SDL*, void*) = nullptr;//イベント発生時
        void (*exit)(SDL*,void*) = nullptr;//終了前
        void (*terminate)(SDL*,void*) = nullptr;//異常終了時
        void (*init)(SDL*, void*) = nullptr;//開始前
    };
    std::map<uint32_t, Window*> m_windows;
    bool m_inited;
    void* m_user_data;
    void* m_sdl_event;
    bool m_exit;
    bool m_disabled;
    inline static bool created = false;
public:
    callbacks callback_funcs;
    SDL();
    ~SDL();
    int init();

    std::vector<uint32_t> get_winid() const;
    Window* get_win(uint32_t winid) const;

    uint32_t mkwindow(const std::string& title, int sizex, int sizey);
    void rmwindow(uint32_t winid);
    void rmwindow(Window* win);

    void set_user_data_ptr(void *ptr);//コールバック関数への任意のデータへのポインタをセット
    int mainloop();
};

}

#endif