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
struct BlockLegacy {
    string getBlockName() {
        return access(this, string, 120);
    }
    short getBlockItemID() {	// IDA VanillaItems::initCreativeItemsCallback Item::beginCreativeGroup "itemGroup.name.planks"2533 line
        short v3 = access(this, short, 312); ;
        if (v3 < 0x100) {
            return v3;
        }
        return (short)(255 - v3);
    }
};
struct Block {
    BlockLegacy* getBlockLegacy() {
        return SymCall( "?getLegacyBlock@Block@@QEBAAEBVBlockLegacy@@XZ",BlockLegacy*, Block*)(this);
    }
};
struct BlockActor {
    Block* getBlock() {
        return access(this, Block*, 16);
    }
    // 取方块位置
    BlockPos getPosition() {				// IDA BlockActor::BlockActor
        return access(this, BlockPos, 44);
    }
};
struct BlockSource {
    Block* getBlock(BlockPos* bp) {
        return SymCall("?getBlock@BlockSource@@QEBAAEBVBlock@@AEBVBlockPos@@@Z",Block*,
            BlockSource*,BlockPos*)(this, bp);
    }
};
struct ContainerManagerModel {
    // 取开容者
    Player* getPlayer() { return *reinterpret_cast<Player**>(reinterpret_cast<unsigned long long>(this) + 8); }
};
struct LevelContainerManagerModel : ContainerManagerModel {};
struct ItemStackBase {
    // 取物品名称
    inline class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char>> getName() const {
        class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char>>(ItemStackBase:: * rv)() const;
        *((void**)&rv) = dlsym("?getName@ItemStackBase@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ");
        return (this->*rv)();
    }

};
struct ItemStack : ItemStackBase {
    // 取物品ID
    inline int getId() const {
        int (ItemStack:: * rv)() const;
        *((void**)&rv) = dlsym("?getId@ItemStackBase@@QEBAFXZ");
        return (this->*rv)();
    }


    // 取容器内数量

    inline int getStackSize() const {
        int (ItemStack:: * rv)() const;
        *((void**)&rv) = dlsym("?getCount@ContainerItemStack@@QEBAHXZ");
        return (this->*rv)();
    }
};

struct ContainerItemStack : ItemStack {};