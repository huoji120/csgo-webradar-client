#include "fradar.h"
#include "../sdk/sdk.h"
#include "../json/json.h"
#include "../json/json-forwards.h"
#include "../offsets.h"


#include <iostream>
#include <unistd.h>
#include <sstream>

void FRadar::Radar(uintptr_t localPlayer)
{
    Vector vecOrigin;
    Vector viewAngle;
    int health;
    int localIndex;
    std::vector<CBaseEntity> entvec;

    if (!m_mem.Read(localPlayer + Netvar::CBaseEntity::m_vecOrigin, &vecOrigin)) {
        LogWait("[FAim/Aim] Failed to read CBaseEntity::m_vecOrigin");
        return;
    }

    if (!m_mem.Read(Offset::Engine::ClientState + Offset::Static::ViewAngles, &viewAngle)) {
        LogWait("[FAim/Aim] Failed to read ClientState::ViewAngles");
        return;
    }

    if (!m_mem.Read(localPlayer + Netvar::CBaseEntity::index, &localIndex)) {
        LogWait("[FAim/Aim] Failed to read CBaseEntity::index");
        return;
    }

    CEntInfo entInfo;
    if (!m_mem.Read(Offset::Client::EntityList, &entInfo)) {
        LogWait("[FAim/Aim] Failed to read EntityList");
        return;
    }

    bool firstRead = true;

    while (entInfo.m_pNext != NULL) {
        if (!firstRead) {
            if (!m_mem.Read(entInfo.m_pNext, &entInfo)) {
                LogWait("[FAim/Aim] Failed to read EntityList");
            }
        }

        firstRead = false;
        if (entInfo.m_pEntity == 0) {
            continue;
        }

        CBaseEntity ent;

        if (!m_mem.Read(entInfo.m_pEntity, &ent)) {
            continue;
        }

        if (ent.index == localIndex) {
            health = ent.m_iHealth;
            continue;
        }

        if (ent.m_iTeamNum != TEAM_T && ent.m_iTeamNum != TEAM_CT) {
            continue;
        }

        if (ent.m_iHealth < 1) {
            continue;
        }

        entvec.push_back(ent);
    }

    Json::Value data;

    for (auto ent : entvec)
    {
        data[ent.index]["position"]["x"] = ent.m_vecOrigin.x;
        data[ent.index]["position"]["y"] = ent.m_vecOrigin.y;
        data[ent.index]["position"]["z"] = ent.m_vecOrigin.z;
        data[ent.index]["health"] = ent.m_iHealth;
    }

    data["localPlayer"]["position"]["x"] = vecOrigin.x;
    data["localPlayer"]["position"]["y"] = vecOrigin.y;
    data["localPlayer"]["position"]["z"] = vecOrigin.z;
    data["localPlayer"]["health"] = health;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_URL, "localhost");
    curl_easy_setopt(curl, CURLOPT_PORT, "13337");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.asCString());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
}


void FRadar::Run()
{
    uintptr_t localPlayer;
    Log("[Radar] Started");
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    while (!ShouldStop()) {
        if (!m_mem.Read(Offset::Client::LocalPlayer, &localPlayer)) {
            LogWait("[Radar] Failed to read local player address.");
            continue;
        }

        Radar(localPlayer);
        WaitMs(5);
    }

    curl_easy_cleanup(curl);
    Log("[Radar] Stopped");
}
