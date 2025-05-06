#pragma once

namespace Zenith {

	class FatalSignal {
	private:
		using Proc = void();
		using ProcFn = std::function<Proc>;

		static FatalSignal s_State;

		std::vector<ProcFn> m_Callbacks;
		long m_Timeout;
		bool m_Active = false;

		void Handler(const char* what);

		static void Timeout();
		static void Terminate();

	public:
		// Kill the program immediately. Do not run any handlers or fallbacks.
		static void Die();

		static void Install(long timeout = 2000);

		static void Add(ProcFn& fn);
	};
}