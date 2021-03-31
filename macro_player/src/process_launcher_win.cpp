#include "macro_player/process_launcher.hpp"
#include "macro_player/logging.hpp"

#include <Windows.h>

#include <vector>

namespace macro_player::process_launch
{

    class ProcessLauncher::impl
    {
    public:
        void start_process_detached(const actions::ProcessLaunch &command)
        {
            STARTUPINFO info = { sizeof(info) };
            PROCESS_INFORMATION processInfo;

            ZeroMemory(&info, sizeof(info));
            info.cb = sizeof(info);
            ZeroMemory(&processInfo, sizeof(processInfo));

            // Microsoft wants a modifiable char*, let's copy it
            std::vector<char> cmd(command.commandLine.begin(), command.commandLine.end());
            cmd.push_back('\0');
            spdlog::info("Starting process {}", cmd.data());

            LPCSTR current_dir = "C:\\";
            DWORD creationFlags = CREATE_DEFAULT_ERROR_MODE | DETACHED_PROCESS;
            if (!CreateProcess(NULL,
                               cmd.data(),
                               NULL,
                               NULL,
                               FALSE,
                               creationFlags,
                               NULL,
                               current_dir,
                               &info,
                               &processInfo))
            {
                spdlog::error("CreateProcess failed with error {}", GetLastError());
                return;
            }

            CloseHandle(processInfo.hProcess);
            CloseHandle(processInfo.hThread);
        }
    };

    ProcessLauncher::ProcessLauncher() : m_pimpl(std::make_unique<impl>())
    {
    }

    ProcessLauncher::~ProcessLauncher() = default;

    void ProcessLauncher::start_process_detached(const actions::ProcessLaunch &command)
    {
        m_pimpl->start_process_detached(command);
    }
}