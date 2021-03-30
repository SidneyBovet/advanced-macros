#pragma once

#include "macro_player/actions.hpp"

#include <memory>
#include <string>

namespace macro_player::process_launch
{
    class ProcessLauncher
    {
    public:
        ProcessLauncher();
        ~ProcessLauncher();

        ProcessLauncher(const ProcessLauncher &) = delete;
        ProcessLauncher &operator=(const ProcessLauncher &) = delete;
        ProcessLauncher(ProcessLauncher &&) = delete;
        ProcessLauncher &operator=(ProcessLauncher &&) = delete;

        // Runs the given command (possibly UTF-8 encoded)
        void start_process_detached(const actions::ProcessLaunch &command);

    private:
        class impl;
        std::unique_ptr<impl> m_pimpl;
    };
}
