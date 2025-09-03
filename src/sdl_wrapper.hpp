#ifndef _SDLWRAPPER_HPP_
#define _SDLWRAPPER_HPP_

#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include "vec2d.hpp"
#include "colour.hpp"

namespace okazawa
{
using winid_t = uint32_t;
using textureid_t = uint32_t;
class Window {
public:
    virtual ~Window();
    virtual bool is_inited() const = 0;
    virtual textureid_t mktexture(const std::string& path)  = 0;
    virtual textureid_t mktexture(int sizex, int sizey) = 0;
    virtual void rmtexture(textureid_t id) = 0;
    virtual winid_t get_id() const = 0;
    virtual void set_colour(const colour& c) = 0;
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
    virtual void draw_texture(textureid_t id, rect2d pos) = 0;
};
struct Event;
class SDL {
    static bool _init;
    bool m_init_fail;
protected:
    std::map<winid_t, Window*> m_windows;
    bool m_exit;//このフラグがtrueになるとループを抜けて終了
    uint32_t m_delay_ms;
    
    //このクラスを継承すれば以下とmainloop()自体をオーバーライド可能
    virtual void init();
    virtual void handle_event();//イベントを処理するにはSDL.hをインクルードする必要あり
    virtual void draw();
    virtual void exit();
    virtual void terminate();
public:
    //このクラスを継承せず使う場合のコールバック系
    struct callback_func_t {
        void (*init)(SDL*) = nullptr;
        void (*event)(SDL*) = nullptr;
        void (*draw)(SDL*, Window*) = nullptr;
        void (*exit)(SDL*) = nullptr;
        void (*terminate)(SDL*) = nullptr;
    };
    callback_func_t callback_funcs;
    void* userdata;

    SDL();
    virtual ~SDL();
    bool is_failed() const;//初期化に失敗したかどうか

    std::vector<winid_t> get_winids() const;
    Window* get_win(winid_t winid) const;
    winid_t get_winid(Window* w) const;

    virtual winid_t mkwindow(const std::string& title, int sizex, int sizey);
    void rmwindow(winid_t winid);
    void rmwindow(Window* win);

    virtual int mainloop();
};

}

#endif