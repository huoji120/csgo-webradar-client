#pragma once

#include "process/process.h"
#include <cstdint>

class Signatures {
public:
    static bool Find(Process &mem);
    static void Print(Process &mem);
};

namespace Offset {
    namespace Client {
        extern uintptr_t EntityList;
        extern uintptr_t LocalPlayer;
    };
    namespace Engine {
        extern uintptr_t ClientState;
    };

    namespace Static {
        constexpr size_t SignOnState = 0x1a0;
        constexpr size_t ViewAngles = 0x8E20;
    };
};

namespace Netvar {
    namespace CBaseEntity {
        constexpr size_t index = 0x94;
        constexpr size_t m_vecOrigin = 0x16c;
        constexpr size_t m_bDormant = 0x121;
        constexpr size_t m_iTeamNum = 0x128;
        constexpr size_t m_lifeState = 0x293;
    };
    namespace CBasePlayer {
        constexpr size_t m_flFlashDuration = 0xabf8;
    };
};