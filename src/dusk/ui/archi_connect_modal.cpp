#include <dusk/ui/archi_connect_modal.hpp>
#include <dusk/ui/string_button.hpp>

#include <chrono>
#include <thread>
#include "dusk/archipelago/archipelago_context.hpp"
#include "dusk/logging.h"
#include "m_Do/m_Do_audio.h"

namespace dusk::ui {


static std::atomic connectStatus = ArchiConnectModal::ConnectionStatus::None;

void CreateSetupConnectionInfoModal();

ArchiConnectModal::ArchiConnectModal() :
    Modal({
       .title = "Archipelago",
       .bodyRml = "Connecting to Server...",
       .onDismiss = [this](Modal& modal) {
           mDoAud_seStartMenu(kSoundWindowClose);
           modal.pop(false);
       },
       .icon = "verifying",
    }) {
    mRoot->SetProperty("white-space", "pre-line");
}

void ArchiConnectModal::update() {
    Modal::update();

    auto currentStatus = connectStatus.load();

    if (currentStatus != mDisplayedStatus) {
        mDisplayedStatus = currentStatus;

        if (currentStatus == ConnectionStatus::Success ||
        currentStatus == ConnectionStatus::Error)
        {
            if (currentStatus == ConnectionStatus::Success) {
                mDoAud_seStartMenu(kSoundSeedGenerateSuccess);
                set_icon("celebration");
                set_body("Sucessfully Connected to server!");
            } else {
                mDoAud_seStartMenu(kSoundSeedGenerateError);
                set_icon("error");
                set_body("Failed to Connect to server.");
            }
            add_action({
                .label = "OK",
                .onPressed = [currentStatus](Modal& modal) {
                    mDoAud_seStartMenu(kSoundWindowClose);
                    modal.pop(false);

                    // show connection setup modal on failure
                    if (currentStatus == ConnectionStatus::Error) {
                        CreateSetupConnectionInfoModal();
                    }
                }
            });

            // Refocus so that we focus the new button
            focus();

            mDisplayedStatus = ConnectionStatus::Ready;
        }else if (currentStatus == ConnectionStatus::Generating) {
            set_body("Loading seed data into game...");
        }else if (currentStatus == ConnectionStatus::Disconnecting) {
            set_body("Disconnecting from previous server...");
        }

        connectStatus.store(mDisplayedStatus);
    }
}

void HandleArchipelagoConnect() {
    // if a connection was already established, disconnect before attempting a new connection.
    if (archi::ArchipelagoContext::IsConnected()) {
        connectStatus.store(ArchiConnectModal::ConnectionStatus::Disconnecting);
        archi::ArchipelagoContext::DisconnectFromServer();
    }

    if (!archi::ArchipelagoContext::ConnectToServer(dComIfGs_getDataNum(), true)) {
        archi::ArchipelagoContext::DisconnectFromServer();
        connectStatus.store(ArchiConnectModal::ConnectionStatus::Error);
        return;
    }

    connectStatus.store(ArchiConnectModal::ConnectionStatus::Generating);

    // Location scouts are requested once by the AP message thread right after connecting. That
    // request/response can occasionally get delayed or lost (e.g. while the connection is also
    // being flooded with a large backlog of item messages on reconnect), and there was previously
    // no timeout here at all - a lost response meant this spun forever, burning a CPU core and
    // leaving the player stuck on "Loading seed data into game..." with no way out but a force-close.
    constexpr auto kScoutTimeout = std::chrono::seconds(15);
    constexpr int kMaxScoutAttempts = 3;
    bool receivedScouts = false;

    for (int attempt = 1; attempt <= kMaxScoutAttempts && !receivedScouts; attempt++) {
        if (attempt > 1) {
            DuskLog.warn("Location scouts not received after {}s, retrying (attempt {}/{})",
                kScoutTimeout.count(), attempt, kMaxScoutAttempts);
            archi::ArchipelagoContext::RequestAllLocationScout();
        }

        const auto waitStart = std::chrono::steady_clock::now();
        while (!archi::ArchipelagoContext::IsReceivedLocationScouts()) {
            if (!archi::ArchipelagoContext::IsConnected()) {
                DuskLog.error("Lost connection while waiting for location scouts.");
                connectStatus.store(ArchiConnectModal::ConnectionStatus::Error);
                return;
            }
            if (std::chrono::steady_clock::now() - waitStart > kScoutTimeout) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        receivedScouts = archi::ArchipelagoContext::IsReceivedLocationScouts();
    }

    if (!receivedScouts) {
        DuskLog.error("Failed to receive location scouts after {} attempts, giving up.", kMaxScoutAttempts);
        archi::ArchipelagoContext::DisconnectFromServer();
        connectStatus.store(ArchiConnectModal::ConnectionStatus::Error);
        return;
    }

    archi::ArchipelagoContext::GenerateLocalWorldData();

    connectStatus.store(ArchiConnectModal::ConnectionStatus::Success);
}

void ConnectAndLoadArchipelago() {
    connectStatus.store(ArchiConnectModal::ConnectionStatus::Connecting);
    std::thread archiConnectThread(HandleArchipelagoConnect);
    archiConnectThread.detach();

    push_document(std::make_unique<ArchiConnectModal>());

    if (auto* doc = top_document()) {
        doc->focus();
    }
}

void CreateSetupConnectionInfoModal() {
    auto& doc = push_document(std::make_unique<MultiTextInputModal>(Modal::Props{
        .title = "Connection Info",
        .bodyRml = "",
        .actions = {
            ModalAction{
                .label = "Connect",
                .onPressed = [](Modal& modal) {
                    auto textModal = dynamic_cast<MultiTextInputModal*>(&modal);

                    int dataNum = dComIfGs_getDataNum();

                    archi::ArchipelagoContext::SetServerIp(textModal->get_input_text(0), dataNum);
                    archi::ArchipelagoContext::SetPassword(textModal->get_input_text(1), dataNum);
                    archi::ArchipelagoContext::SetSlotName(textModal->get_input_text(2), dataNum);

                    modal.pop(false);

                    ConnectAndLoadArchipelago();
                },
            },
            ModalAction{
                .label = "Cancel",
                .onPressed = [](Modal& modal) {
                    // TODO: cancelling this modal will still progress file select to starting the save,
                    // which could potentially allow a user to break their save file.

                    modal.pop(false);
                },
            },
        },
        .icon = "information"
    }));
    auto inputModal = dynamic_cast<MultiTextInputModal*>(&doc);

    int dataNum = dComIfGs_getDataNum();

    inputModal->add_input_text("Server IP", archi::ArchipelagoContext::GetServerIp(dataNum));
    inputModal->add_input_text("Password", archi::ArchipelagoContext::GetPassword(dataNum));
    inputModal->add_input_text("Slot Name", archi::ArchipelagoContext::GetSlotName(dataNum));
}

void BeginArchipelagoConnectionUI(bool forceChangeConnection) {
    if (forceChangeConnection) {
        CreateSetupConnectionInfoModal();
        return;
    }

    int dataNum = dComIfGs_getDataNum();
    bool hasConnectInfo = (!archi::ArchipelagoContext::GetServerIp(dataNum).empty() && !archi::ArchipelagoContext::GetSlotName(dataNum).empty());

    if (hasConnectInfo) {
        ConnectAndLoadArchipelago();
    }else {
        CreateSetupConnectionInfoModal();
    }
}
}