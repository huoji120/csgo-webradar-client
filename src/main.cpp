#include "features/fradar.h"
#include "process/process.h"
#include "engine.h"
#include "global.h"
#include "offsets.h"

#include <chrono>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <thread>

#include <signal.h>
#include <unistd.h>

#define LOG(X) std::cout << X << std::flush
bool shouldQuit = false;

void exitHandle(int)
{
    shouldQuit = true;
}

void connectSignals(struct sigaction &handle)
{
    handle.sa_handler = exitHandle;
    sigemptyset(&handle.sa_mask);
    handle.sa_flags = 0;
    sigaction(SIGINT, &handle, NULL);
    sigaction(SIGQUIT, &handle, NULL);
}

int main() {
    if (getuid() != 0) {
        LOG("This app should be ran as root");
        return 1;
    }

    struct sigaction ccHandle;
    connectSignals(ccHandle);

    Process proc(PROCESS_NAME);

    LOG("Waiting for process...");

    while (!proc.Attach() && !shouldQuit) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    LOG("Done.\nWaiting for client and engine library...");

    while (!shouldQuit) {
        proc.ParseModules();
        if (proc.HasModule(CLIENT_SO) && proc.HasModule(ENGINE_SO)) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (shouldQuit) {
        return 0;
    }

    Signatures::Find(proc);
    Signatures::Print(proc);

    auto& eng = Engine::GetInstance();
    eng.SetProcessManager(&proc);
    eng.Update(true);

    FRadar fRadar(proc);

    while (!shouldQuit) {
        if (!proc.IsValid()) {
            shouldQuit = true;
            LOG("Lost connection to process... Exiting.\n");
            break;
        }

        // ### BEGIN MENU HACKS ###

        // ### END MENU HACKS ###

        // ### BEGIN IN-GAME HACKS ###
        if (eng.IsConnected()) {
            fRadar.Start();

            while (eng.IsConnected() && !shouldQuit) {
                eng.Update();
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            fRadar.Stop();
        }
        // ### END IN-GAME HACKS ###
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}