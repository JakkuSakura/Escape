#if !defined(READ_MAP_H)
#define READ_MAP_H
#include "components.h"
#include <string>
#include <vector>
namespace Escape
{
class ReadMap
{
public:
typedef std::vector<std::tuple<TerrainData, Position, Rotation>> EleListType;
    ReadMap(const std::string &filaname);
    const EleListType &readMapFile();
    ~ReadMap();
private:
    EleListType data;
};


} // namespace Escape


#endif // READ_MAP_H
