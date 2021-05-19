/**
 * @file widget.cpp
 * @brief 各Widgetの継承クラスの実装と、Add○○(Widget名)の実装
 * @author 20Niship Nishimiya Tadashi
 * @date 2021-01-17
 * @details 詳細な説明
 */


#pragma once
#include "glUI.hpp"

namespace glUI {

#if 0
class uiScrollbarV : public uiWidget{
private:
	uint16_t last_value,last_inner_size;
	uint16_t *inner_size, *value;
	bool isSelecting;
public:
    uiScrollbarV(uihWidget parent, uint16_t *value, Vector2d pos_, Vector2d size_, uint16_t *inner_size);
	void render();
	bool CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings) override;
};


class uiScrollbarH : public uiWidget{
private:
	uint16_t last_value, last_inner_size;
	uint16_t *inner_size, *value;
	bool isSelecting;
public:
    uiScrollbarH(uihWidget parent,uint16_t *value, Vector2d pos_, Vector2d size_, uint16_t *inner_size);
	void render();
	bool CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings) override;
};
#endif


class uiLabel : public uiWidget{
public:
    uiLabel(uihWidget parent, std::string text_, Vector2d pos_, Vector2d size_);

	void render();
	bool CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings) override;
};


class uiButton : public uiWidget{
private:
	bool *value;
	bool last_value;
public:
    uiButton(uihWidget parent, std::string text_, bool *value_, Vector2d pos_, Vector2d size_);
	void render();
	bool CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings) override;
};


class uiButtonEx : public uiWidget{
private:
	bool *value;
	bool last_value;
public:
    uiButtonEx(uihWidget parent, std::string text_, bool *value_, Vector2d pos_, Vector2d size_);
	void render();
	bool CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings) override;
};



class uiCheckBox : public uiWidget{
private:
	bool *value;
	bool last_value;
public:
    uiCheckBox(uihWidget parent, std::string text_, bool *value_, Vector2d pos_, Vector2d size_);
	void render();
	bool CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings) override;
};



class uiSliderH : public uiWidget{
private:
	float *value;
	float last_value;
	// float last_value, min_value, max_value, delta_value;
public:
    uiSliderH(uihWidget parent, std::string text_, float *value_, Vector2d pos_, Vector2d size_);
	void render();
	bool CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings) override;
};



class uiListBox : public uiWidget{
private:
	uint16_t last_value;
	uint16_t *value;
	std::string last_text;
public:
    uiListBox(uihWidget parent, std::string texts_, uint16_t *value_, Vector2d pos_, Vector2d size_);
	void render();
	bool CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings) override;
};


class uiRadioH : public uiWidget{
private:
	int last_value;
	int *value;
	std::string last_text;
public:
    uiRadioH(uihWidget parent, std::string texts_, int *value_, Vector2d pos_, Vector2d size_);
	void render();
	bool CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings) override;
};


class uiRadioV : public uiWidget{
private:
	int last_value;
	int *value;
	std::string last_text;
public:
    uiRadioV(uihWidget parent, std::string texts_, int *value_, Vector2d pos_, Vector2d size_);
	void render();
	bool CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings) override;
};


class uiTextBox : public uiWidget{
private:
	std::string last_value;
	uiVector<uint16_t>line_heights;
	uint16_t TextRectX, TextRectY;
	std::string *value;
public:
    uiTextBox(uihWidget parent, std::string *value_, Vector2d pos_, Vector2d size_);
	void render();
	bool CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings) override;
};



class uiFrame : public uiWidget{
public:
    uiFrame(uihWidget parent, std::string title_, Vector2d pos_, Vector2d size_);
	void render();
	bool CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings) override;
};


// -----------------------------------------------------
//    [SECTION] Add Widget
// -----------------------------------------------------
uihWidget AddLabel(uihWidget to, std::string text, Vector2d pos = Vector2d(), Vector2d size = Vector2d());
uihWidget AddButton(uihWidget to, bool *value, std::string text, Vector2d pos = Vector2d(), Vector2d size = Vector2d());
uihWidget AddButtonEx(uihWidget to, bool *value, std::string text, Vector2d pos = Vector2d(), Vector2d size = Vector2d());
uihWidget AddCheckbox(uihWidget to, bool *value, std::string text, Vector2d pos = Vector2d(), Vector2d size = Vector2d());
uihWidget AddSliderH(uihWidget to, float *value, std::string text, Vector2d pos = Vector2d(), Vector2d size = Vector2d());
uihWidget AddTextBox(uihWidget to, std::string *value, Vector2d pos, Vector2d size);
uihWidget AddListBox(uihWidget to, std::string texts, uint16_t *value, Vector2d pos_, Vector2d size_);
uihWidget AddRadioH(uihWidget to, std::string texts, int *value, Vector2d pos_, Vector2d size_);
uihWidget AddRadioV(uihWidget to, std::string texts, int *value, Vector2d pos_, Vector2d size_);
uihWidget AddFrame(uihWidget to, std::string title, Vector2d pos_, Vector2d size_);






// これから追加予定......
#if 0
bool AddSeparator();        // separator, generally horizontal. inside a menu bar or in horizontal layout mode, this becomes a vertical separator.
bool AddDummy(const Vector2f& size);       // add a dummy item of given size. unlike InvisibleButton(), Dummy() won't take the mouse click or be navigable into.
bool AddLabelColored(uihWidget to, const Vector3f& col, std::string *value, Vector2d pos = Vector2d(), Vector2d size = Vector2d()); // shortcut for PushStyleColor(ImGuiCol_Text, col); Text(fmt, ...); PopStyleColor();
bool AddTextColoredV(uihWidget to, const Vector3f& col, std::string *value, Vector2d pos = Vector2d(), Vector2d size = Vector2d());

bool AddSmallButton(uihWidget to, bool *value, std::string text, Vector2d pos = Vector2d(), Vector2d size = Vector2d());  // button with FramePadding=(0,0) to easily embed within text
bool AddArrowButton(uihWidget to, bool *value, std::string text, float theta, Vector2d pos = Vector2d(), Vector2d size = Vector2d());   // square button with an arrow shape
void AddImage(uihWidget to, std::string *fname, std::string title = "", Vector2d pos = Vector2d(), Vector2d size = Vector2d());
void AddImage(uihWidget to, unsigned char *immg_buffer, uint16_t w, uint16_t h, std::string title = "", Vector2d pos = Vector2d(), Vector2d size = Vector2d());
void AddImage(uihWidget to, cv::Mat *img, std::string title = "", Vector2d pos = Vector2d(), Vector2d size = Vector2d());
bool AddImageButton(uihWidget to, bool *value_, std::string text, Vector2d pos = Vector2d(), Vector2d size = Vector2d());
bool AddRadioButton(uihWidget to, bool *value_, std::string text, Vector2d pos = Vector2d(), Vector2d size = Vector2d());           // shortcut to handle the above pattern when value is an integer
void AddProgressBar(uihWidget to, float *value, std::string text, Vector2d pos = Vector2d(), Vector2d size = Vector2d());
bool AddCombo(uihWidget to, std::vector<std::string> texts, uint16_t *value, Vector2d pos_, Vector2d size_);
bool AddDragNumber(uihWidget to, float *value, std::string text, Vector2d pos = Vector2d(), Vector2d size = Vector2d());

// Widgets: Input with Keyboard
bool AddInputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
bool AddInputTextMultiline(const char* label, char* buf, size_t buf_size, const ImVec2& size = ImVec2(0,0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
bool AddInputTextWithHint(const char* label, const char* hint, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
bool AddInputNumber(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", ImGuiInputTextFlags flags = 0);
bool AddColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
bool AddColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);
bool AddColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags = 0);
bool AddColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = NULL);
bool AddColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, ImVec2 size = ImVec2(0,0));  // display a colored square/button, hover for details, return true when pressed.

// Widgets: Trees
// - TreeNode functions return true when the node is open, in which case you need to also call TreePop() when you are finished displaying the tree node contents.
bool AddTreeNode(const char* label);
bool AddTreeNodeV(const char* str_id, const char* fmt, va_list args) IM_FMTLIST(2);
bool AddTreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0);
bool AddTreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);

// Widgets: Selectables
bool AddSelectable(const char* label, bool Addselected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0,0));  // "bool Addselected" carry the selection state (read-only). Selectable() is clicked is returns true so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height
bool AddSelectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0,0));       // "bool* p_selected" point to the selection state (read-write), as a convenient helper.

// Widgets: List Boxes
// - FIXME: To be consistent with all the newer API, ListBoxHeader/ListBoxFooter should in reality be called BeginListBox/EndListBox. Will rename them.
bool AddListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1);

// Widgets: Data Plotting
void AddPlotLines(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
void AddPlotLines(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));
void AddPlotHistogram(const char* label, const float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float));
void AddPlotHistogram(const char* label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0));

// Widgets: Value() Helpers.
void AddValue(const char* prefix, bool Addb);

// Widgets: Menus
bool AddMenu(const char* label, const char* shortcut = NULL, bool Addselected = false, bool Addenabled = true);  // return true when activated. shortcuts are displayed for convenience but not processed by ImGui at the moment
bool AddOverlay(const char* label, const char* shortcut = NULL, bool Addselected = false, bool Addenabled = true);  // return true when activated. shortcuts are displayed for convenience but not processed by ImGui at the moment
bool AddTopMenu(const char* label, const char* shortcut = NULL, bool selected = false, bool enabled = true);  // return true when activated. shortcuts are displayed for convenience but not processed by ImGui at the moment
#endif
}
