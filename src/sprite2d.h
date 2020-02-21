#if !defined(SPRITE2D_H)
#define SPRITE2D_H

namespace Escape
{
class Sprite2D
{
public:
    virtual ~Sprite2D() {}
};
class Rectangle : public Sprite2D {

};
} // namespace Escape


#endif // SPRITE2D_H
