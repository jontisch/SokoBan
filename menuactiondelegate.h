#ifndef MENUACTIONDELEGATE_H
#define MENUACTIONDELEGATE_H


class MenuActionDelegate
{
public:
    MenuActionDelegate();
    virtual void executeMenuAction(int action) = 0;
private:

};

#endif // MENUACTIONDELEGATE_H
