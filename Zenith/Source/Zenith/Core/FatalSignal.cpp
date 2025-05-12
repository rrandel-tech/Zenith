#include "znpch.hpp"
#include "FatalSignal.hpp"

#ifndef ZN_PLATFORM_WINDOWS
#include <unistd.h>
#endif

#include <backward.hpp>

namespace Zenith {
	FatalSignal FatalSignal::s_State;

	void FatalSignal::Die() {
		auto st = backward::StackTrace();
		st.load_here(32);
		backward::Printer().print(st);

		_Exit(-1);
	}

	void FatalSignal::Timeout() {
		puts("FATAL SIGNAL TIMEOUT");
		Die();
	}

	void FatalSignal::Handler(const char* what) {
		if (m_Active) {
			puts("NESTED ERROR STATE");
			puts(what);
			Die();
		}

		puts("FATAL SIGNAL RECEIVED");
		puts(what);

		m_Active = true;

#ifndef ZN_PLATFORM_WINDOWS
		ualarm(m_Timeout * 1000, 0);
#else
		std::thread t([&] {
			auto dur = std::chrono::duration<long, std::milli>(m_Timeout);
			std::this_thread::sleep_for(dur);
			Timeout();
			});
#endif

		for (auto& fn : m_Callbacks) fn();

		Die();
	}

	void FatalSignal::Install(long timeout) {
		s_State.m_Timeout = timeout;

		std::set_terminate([] {
			auto eptr = std::current_exception();
			const char* what = "<none>";
			try {
				if (eptr) std::rethrow_exception(eptr);
			}
			catch (const std::exception& e) {
				what = e.what();
			}
			s_State.Handler(what);
			});

		void (*sig)(int) = [](int sig) {
			const char* name = "<none>";
			switch (sig) {
			case SIGABRT: name = "SIGABRT"; break;
			case SIGFPE: name = "SIGFPE"; break;
			case SIGILL: name = "SIGILL"; break;
			case SIGINT: name = "SIGINT"; break;
			case SIGSEGV: name = "SIGSEGV"; break;
			case SIGTERM: name = "SIGTERM"; break;
			}
			s_State.Handler(name);
			};

		signal(SIGABRT, sig);
		signal(SIGFPE, sig);
		signal(SIGILL, sig);
		signal(SIGINT, sig);
		signal(SIGSEGV, sig);
		signal(SIGTERM, sig);

#ifndef ZN_PLATFORM_WINDOWS
		signal(SIGALRM, [](int _) { Timeout(); });
#endif
	}

	void FatalSignal::Add(ProcFn& fn) {
		s_State.m_Callbacks.emplace_back(fn);
	}
}