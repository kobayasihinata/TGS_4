#include "DebugInformation.h"

std::unordered_map<const char*, float>DebugInfomation::draw_list;
bool DebugInfomation::draw_flg;
class InputKey* DebugInfomation::input = InputKey::Get();

void DebugInfomation::Update()
{
	draw_list.clear();
	if (input->InputKey::GetKeyState(KEY_INPUT_X) == eInputState::Pressed)
	{
		draw_flg = !draw_flg;
	}
}

void DebugInfomation::Draw()
{
#ifdef _DEBUG
	int old = GetFontSize();
	SetFontSize(12);
	if (draw_flg)
	{
		int box_size = 0;
		int max_size = 0;
		for (const auto& draw_list : draw_list)
		{
			GetDrawFormatStringSize(&box_size, 0, 0, "%s:%f", draw_list.first, draw_list.second);
			max_size = (box_size > max_size) ? box_size : max_size;
		}
		DrawBox(DEBUG_X, DEBUG_Y, DEBUG_X + max_size + 10, DEBUG_Y + draw_list.size() * 13, 0x000000, true);
		DrawBox(DEBUG_X, DEBUG_Y, DEBUG_X + max_size + 10, DEBUG_Y + draw_list.size() * 13, 0xffffff, false);

		int i = 0;
		for (const auto& draw_list : draw_list)
		{
			DrawFormatString(DEBUG_X + 2, DEBUG_Y + i * 11, 0x00ff00, "%s:%f", draw_list.first, draw_list.second);
			i++;
		}
	}
	SetFontSize(old);
#endif // _DEBUG
}

void DebugInfomation::Add(const char* _c, float _num)
{
	draw_list.insert(std::make_pair(_c, _num));
}

void DebugInfomation::Add(const char* _c, int _num)
{
	draw_list.insert(std::make_pair(_c, (float)_num));
}

void DebugInfomation::Add(const char* _c, bool _num)
{
	draw_list.insert(std::make_pair(_c, (float)_num));
}