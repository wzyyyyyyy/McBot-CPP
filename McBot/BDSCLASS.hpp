#include "pch.h"
#ifndef access
#  define access(ptr, type, off) (*((type *) (((uintptr_t) ptr) + off)))
#endif
using namespace std;

struct BlockPos{
public:
	int x, y,z;
};
template <typename A, typename T>
class AutomaticID {
    T id;
public:
    AutomaticID() {
        id = 0;
    }
    AutomaticID(T x) {
        id = x;
    }
    operator T() {
        return id;
    }
};
class Vec3 {
public:
    float x, y, z;
};
struct Actor {
    inline AutomaticID<class Dimension, int> getDimension() const {
        class AutomaticID<class Dimension, int>(Actor:: * rv)() const;
        *((void**)&rv) = dlsym("?getDimensionId@Actor@@UEBA?AV?$AutomaticID@VDimension@@H@@XZ");
        return (this->*rv)();
    }
    inline std::basic_string<char, struct std::char_traits<char>, class std::allocator<char>> const& getNameTag() const {
        class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char>> const& (Actor:: * rv)() const;
        *((void**)&rv) = dlsym("?getNameTag@Actor@@UEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ");
        return (this->*rv)();
    }
    const unsigned char isStand() {
        return *reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned long long>(this) + 376);
    }
    inline Vec3 const& getPos() const {
        Vec3 const& (Actor:: * rv)() const;
        *((void**)&rv) = dlsym("?getPos@Actor@@UEBAAEBVVec3@@XZ");
        return (this->*rv)();
    }
};
struct Mob : Actor {};
struct Player : Mob {};