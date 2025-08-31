#ifndef _SDLWRAPPER_HPP_
#define _SDLWRAPPER_HPP_

#include <string>
#include <vector>
#include <cstdint>
#include <map>

namespace okazawa
{

class Window {
public:
    virtual ~Window();
    virtual bool is_inited() const = 0;
    virtual int add_texture(const std::string& path)  = 0;
    virtual uint32_t get_id() const = 0;
    virtual void set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) = 0;
    virtual void scrclear() = 0;
    virtual void update() = 0;
};

class SDL {
    struct callbacks {
        void (*draw)(SDL*, uint32_t winid,void*) = nullptr;//描画
        void (*event)(SDL*, void*) = nullptr;//イベント発生時
        void (*exit)(SDL*,void*) = nullptr;//終了前
        void (*terminate)(SDL*,void*) = nullptr;//異常終了時

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

    Window* get_win(uint32_t winid) const;

    Window* mkwindow(const std::string& title, int sizex, int sizey);
    void rmwindow(uint32_t winid);
    void rmwindow(Window* win);

    void set_user_data_ptr(void *ptr);//コールバック関数への任意のデータへのポインタをセット
    int mainloop();
};

}

#endif