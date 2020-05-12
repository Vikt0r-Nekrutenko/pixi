#ifndef NUMERICBOX_HPP
#define NUMERICBOX_HPP

#include "InputBox.hpp"

namespace pixi { namespace ui {

class KERNEL_EXPORT NumericBox : public InputBox
{
public:
    NumericBox(const Window *parent, const short x, const short y, const short width = 10, const short height = 3, const std::string title = "Numeric");
    double getNumber() const;
    void setNumber(const double num);
private:
    using InputBox::getText;
    using InputBox::setText;

    void keyEventHandler(Window *sender, const KeyRecord keyRecord) override;
};

}}

#endif // NUMERICBOX_HPP
