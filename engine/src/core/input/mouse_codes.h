#pragma once

namespace PhysiXal {

	// Mouse codes
	typedef enum class MouseCode : U16
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<I32>(mouseCode);
		return os;
	}
}

#define PX_MOUSE_BUTTON_0      ::PhysiXal::Mouse::Button0
#define PX_MOUSE_BUTTON_1      ::PhysiXal::Mouse::Button1
#define PX_MOUSE_BUTTON_2      ::PhysiXal::Mouse::Button2
#define PX_MOUSE_BUTTON_3      ::PhysiXal::Mouse::Button3
#define PX_MOUSE_BUTTON_4      ::PhysiXal::Mouse::Button4
#define PX_MOUSE_BUTTON_5      ::PhysiXal::Mouse::Button5
#define PX_MOUSE_BUTTON_6      ::PhysiXal::Mouse::Button6
#define PX_MOUSE_BUTTON_7      ::PhysiXal::Mouse::Button7
#define PX_MOUSE_BUTTON_LAST   ::PhysiXal::Mouse::ButtonLast
#define PX_MOUSE_BUTTON_LEFT   ::PhysiXal::Mouse::ButtonLeft
#define PX_MOUSE_BUTTON_RIGHT  ::PhysiXal::Mouse::ButtonRight
#define PX_MOUSE_BUTTON_MIDDLE ::PhysiXal::Mouse::ButtonMiddle