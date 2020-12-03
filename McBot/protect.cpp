#include "pch.h"
#include <iostream>
#include <fstream>
int seed = 114514;
bool penderman = true;
bool pexplode = true;
bool pfarm = true;
void entry() {
}
THook(void, "?transformOnFall@FarmBlock@@UEBAXAEAVBlockSource@@AEBVBlockPos@@PEAVActor@@M@Z"
	, void* __this,  void* a2, void* a3, void* a4, float a5) {
	if (pfarm) {
		return;
	}
	return original(__this, a2, a3, a4, a5);
}
THook(bool, "?canUse@EndermanTakeBlockGoal@@UEAA_NXZ",
	void* __this) {
	if (penderman) {
		return false;
	}
	return original(__this);
}
THook(void, "?explode@Level@@QEAAXAEAVBlockSource@@PEAVActor@@AEBVVec3@@M_N3M3@Z",
	void* __this, void* a2, void* a3, void* a4, float a5, bool a6, bool a7, float a8, bool a9) {
	if (pexplode) {
		return original(__this, a2, a3, a4, a5, a6, 0, a8, a9);
	}
	return original(__this, a2, a3, a4, a5, a6, a7, a8, a9);
}
THook(void*, "?write@StartGamePacket@@UEBAXAEAVBinaryStream@@@Z", void* this_, void* a) {
	*((unsigned int*)(((uintptr_t)this_) + 40)) = seed;
	return original(this_, a);
}
