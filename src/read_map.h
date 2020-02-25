#if !defined(READ_MAP_H)
#define READ_MAP_H
#include "components.h"
#include <string>
#include <vector>
#include "MyECS.h"
namespace Escape
{
class ReadMap
{
private:
    std::string filename;
public:
    ReadMap(const std::string &filaname);
    World *readMapFile(World *world);
    ~ReadMap();
};


} // namespace Escape


#endif // READ_MAP_H
