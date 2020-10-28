#ifndef ISTATES_H
#define ISTATES_H

namespace pla::client::states
{

class IStates{
public:
    virtual void clear() = 0;
    virtual void handleEvent() = 0;
    virtual void handleInput() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
};

} // namespace

#endif