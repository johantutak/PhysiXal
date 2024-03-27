#pragma once

#include <spdlog/sinks/base_sink.h>
#include <mutex>
#include <deque>
#include <chrono>

namespace PhysiXal {

    extern bool s_IsImGuiLoggingEnabled;

    // Custom ImGui log message structure
    struct ImGuiLogMessage {
        spdlog::level::level_enum Level;
        std::chrono::system_clock::time_point Time;
        std::string Message;

        ImGuiLogMessage(spdlog::level::level_enum level, const std::chrono::system_clock::time_point& time, const std::string& message)
            : Level(level), Time(time), Message(message) {}
    };

    // Custom ImGui log sink
    template<typename Mutex>

    class ImGuiLogSink : public spdlog::sinks::base_sink<Mutex> 
    {
    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override 
        {
            spdlog::memory_buf_t formatted;
            spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
            auto time = std::chrono::system_clock::now();
            
            // Only append new messages if ImGui logging is enabled
            if (s_IsImGuiLoggingEnabled)
            {
                m_messages.emplace_back(msg.level, time, fmt::to_string(formatted));
            }
        }

        void flush_() override {}

    public:
        const std::deque<ImGuiLogMessage>& GetMessages() const { return m_messages; }

        // In ImGuiLogSink class
        void Clear() 
        {
            std::lock_guard<Mutex> lock(this->mutex_); // Ensure thread safety
            m_messages.clear();
        }
    private:
        std::deque<ImGuiLogMessage> m_messages;
    };

    using ImGuiLogSink_mt = ImGuiLogSink<std::mutex>;

	class ImGuiWidgets
	{
	public:
	    // Dear ImGUI widgets
        void PerformanceStats();
		void VertexData();
        
        void LogWindow();
        static void ClearLogMessages();
	private:
        
	};
}