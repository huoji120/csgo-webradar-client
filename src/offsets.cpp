#include "offsets.h"
#include "global.h"
#include "process/process.h"
#include "sdk/cbaseentity.h"
#include "sdk/centitylist.h"

#include <sstream>
#include <iomanip>

#define OFFSET(section, name) uintptr_t Offset::section::name = 0;

#define SIGNATURE(name, sig, offset) \
    constexpr char name[] = sig; \
    constexpr char name ## Offset = offset;

#define FindInClient(mm, sig) \
    mm.FindInModule(CLIENT_SO, Sig::sig, Sig::sig ## Offset);

#define FindInEngine(mm, sig) \
    mm.FindInModule(ENGINE_SO, Sig::sig, Sig::sig ## Offset);

#define PrintOffset(section, name, start) \
    printf(#section "::" #name " = %#lx\n", Offset::section::name - start);

OFFSET(Client, LocalPlayer);
OFFSET(Client, EntityList);
OFFSET(Engine, ClientState);

namespace Sig {
    SIGNATURE(LocalPlayer, "48 89 E5 74 0E 48 8D 05", 0x7);
    SIGNATURE(EntityList, "55 48 89 E5 48 83 EC 10 8B 47 34 48 8D 75 F0 89 45 F0 48 8B 05 ?? ?? ?? ?? 48 8B 38", 0x12);
    SIGNATURE(ClientState, "48 8B 05 ?? ?? ?? ?? 55 48 8D 3D ?? ?? ?? ?? 48 89 E5 FF 50 28", 0x0);
};

// TODO: Error handling
bool Signatures::Find(Process &mem) {
    uintptr_t localPlayerLea = FindInClient(mem, LocalPlayer);
    uintptr_t localPlayer = mem.GetCallAddress(localPlayerLea);
    Offset::Client::LocalPlayer = localPlayer;

    uintptr_t entityListMov = FindInClient(mem, EntityList);
    uintptr_t entityList = mem.GetAbsoluteAddress(entityListMov, 3, 7);
    mem.Read(entityList, &entityList);
    mem.Read(entityList, &entityList);
    Offset::Client::EntityList = entityList + 0x8;

    uintptr_t splitScreenMgrLea = FindInEngine(mem, ClientState);
    uintptr_t clientState = mem.GetAbsoluteAddress(splitScreenMgrLea, 3, 7);
    mem.Read(clientState + 0x8, &clientState);
    Offset::Engine::ClientState = clientState + 0x8;

    return true;
}

void Signatures::Print(Process &mem) {
    const uintptr_t sClient = mem.GetModuleStart(CLIENT_SO);
    const uintptr_t sEngine = mem.GetModuleStart(ENGINE_SO);
    printf("\n######## Offsets ########\n\n");
    PrintOffset(Client, EntityList, sClient);
    PrintOffset(Client, LocalPlayer, sClient);
    PrintOffset(Engine, ClientState, sEngine);
    printf("\n#########################\n\n");
}
