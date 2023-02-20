#pragma once

namespace PhysiXal {

	class Timestep
	{
	public:
		// Timestep (Delta time)
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		operator float() const { return m_Time; }

		float GetFramesPerSecond() const { return 1000.0f / m_Time / 1000.0f; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};
}