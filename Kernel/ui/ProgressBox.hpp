#ifndef PROGRESSBOX_HPP
#define PROGRESSBOX_HPP

#include "Box.hpp"

namespace pixi { namespace ui {

class KERNEL_EXPORT ProgressBox : public Box
{
public:
    ProgressBox(const Window *parent, const short x, const short y, const short width, const short height, const float value, const Color barColor = Color::FG_DARK_GREY, const std::string title = "Progress");
    void increase();

private:
    Color m_barColor;
    float m_progress = 0.f;
    float m_value    = 0.f;
    float m_real     = 0.f;
};

}}
#endif // PROGRESSBOX_HPP
