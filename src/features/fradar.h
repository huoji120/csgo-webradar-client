#pragma once

#include "../process/process.h"
#include "fbase.h"
#include <curl/curl.h>

class FRadar final : public FBase {
    Process &m_mem;
    public:
        FRadar(Process &mem) : m_mem(mem) {};
    protected:
        void Run();
    private:
        void Radar(uintptr_t localPlayer);
        CURL *curl;
};
