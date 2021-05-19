/**
 * @file widget.cpp
 * @brief 各Widgetの継承クラスの実装と、Add○○(Widget名)の実装
 * @author 20Niship Nishimiya Tadashi
 * @date 2021-01-17
 * @details 詳細な説明
 */

#include "glUI.hpp"
#include "widget.hpp"

#include <vector>

namespace glUI {

#if 0
// -----------------------------------------------------
//   [SECTION]   Scroll bar
// -----------------------------------------------------
uiScrollbarV::uiScrollbarV(uihWidget parent, uint16_t *value_, Vector2d pos_, Vector2d size_, uint16_t *inner_size_){
	std::cout << "uiButton setup!!\n";
	value = value_;
	inner_size = inner_size_;
	last_inner_size = 0;
	setSize(size_);
	setPos(pos_);
}

void uiScrollbarV::render() {
	auto pos_  = Vector2f(pos.x,  pos.y);
	auto size_ = Vector2f(size.x, size.y);
	auto style = getStyle();
	uiDrawList *DrawList = getDrawlistStack();

	if((last_inner_size != *inner_size) || (*(uint16_t *)value != last_value)) needRendering(true);

	if(*inner_size > size.y && flags.needRendering > 0){
		DrawList[0].AddRectangle(pos_, size_, -1, style->col_wndScrollBg);
		DrawList[0].AddRectangle(Vector2f(pos.x+1, pos.y + *(uint16_t *)value*size.y/ *inner_size), 
								 Vector2f(size.x-1, size.y*size.y/ *inner_size), -1, style->col_wndScroll);
		needRendering(false);
		parent->needApplyAlignment();
		needCalcInnerSize();
		last_inner_size = *inner_size;
		last_value = *(uint16_t *)value;
	}
}

bool uiScrollbarV::CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){
	uiRect scrollYRect;
	scrollYRect.posX  = pos.x;  scrollYRect.posY   = pos.y;
	scrollYRect.width = size.x; scrollYRect.height = size.y;
	bool isContains = (pos.x <= vec2_1.x) && (vec2_1.x <= pos.x + size.x) && (pos.y <= vec2_1.y) && (vec2_1.y < pos.y + size.y);
	
	if(((flag ==uiCallbackFlags::MouseMove) && (num_2 & 0b01) && (isSelecting)) || 
	   ((flag ==uiCallbackFlags::LMouseDown) && isContains) || 
	   (isSelecting)){  //left button pushing
		int tmp_sy =  (float(vec2_1.y - pos.y) - float(size.y*size.y)/(*inner_size*2.0))* (*inner_size)/size.y ;
		isSelecting = true;
		*value  = std::min(std::max(tmp_sy, 0), int(*inner_size - size.y));
		needRendering(true);
		parent->needApplyAlignment();
		needCalcInnerSize();
	}
	
	if(flag ==uiCallbackFlags::LMouseUP){ isSelecting = false; }
	if(flag ==uiCallbackFlags::MouseScroll){ 
		if(num_2 != 0){ *value = std::clamp<uint16_t>((int)*value - num_2*15, 0, *inner_size - size.y); }
	}
	return true;
}



uiScrollbarH::uiScrollbarH(uihWidget parent, uint16_t *value_, Vector2d pos_, Vector2d size_, uint16_t *inner_size_){
	std::cout << "uiButton setup!!\n";
	value = value_;
	
	inner_size = inner_size_;
	last_inner_size = 0;
	setSize(size_);
	setPos(pos_);
}

void uiScrollbarH::render() {
	auto pos_  = Vector2f(pos.x,  pos.y);
	auto size_ = Vector2f(size.x, size.y);
	auto style = getStyle();
	uiDrawList *DrawList = getDrawlistStack();


	if((last_inner_size != *inner_size) || (*(uint16_t *)value != last_value)) needRendering(true);
	if(*inner_size > size.x && flags.needRendering > 0){
		DrawList[0].AddRectangle(pos_, size_, -1, style->col_wndScrollBg);
		DrawList[0].AddRectangle(Vector2f(pos.x + *(uint16_t *)value*size.x/ *inner_size, pos.y+1), 
								 Vector2f(size.x*size.x/ *inner_size, size.y-1), -1, style->col_wndScroll);
		parent->needApplyAlignment();
		needCalcInnerSize();

		last_inner_size = *inner_size;
		last_value = *(uint16_t *)value;
		needRendering(false);
	}
}

bool uiScrollbarH::CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){
	uiRect scrollYRect;
	scrollYRect.posX  = pos.x;  scrollYRect.posY   = pos.y;
	scrollYRect.width = size.x; scrollYRect.height = size.y;
	bool isContains = (pos.x <= vec2_1.x) && (vec2_1.x <= pos.x + size.x) && (pos.y <= vec2_1.y) && (vec2_1.y < pos.y + size.y);
	
	if(((flag ==uiCallbackFlags::MouseMove) && (num_2 & 0b01) && (isSelecting)) || 
	   ((flag ==uiCallbackFlags::LMouseDown) && isContains) || 
	   (isSelecting)){  //left button pushing

		int tmp_sy =  (float(vec2_1.x - pos.x) - float(size.x*size.x)/(*inner_size*2.0))* (*inner_size)/size.x ;
		// for(int i=0; i<line_heights.size()-1; i++){
		// 	if(scrollpos_y < line_heights[i+1]){
		// 		scrollpos_y = line_heights[i];
		// 	}
		// }
		isSelecting = true;
		*value  = std::min(std::max(tmp_sy, 0), int(*inner_size - size.x));
		needRendering(true);
		parent->needApplyAlignment();
		needCalcInnerSize();
	}
	
	if(flag ==uiCallbackFlags::LMouseUP){
		isSelecting = false;
	}
	return true;
}


#endif



// -----------------------------------------------------
//   [SECTION]   Widget Text
// -----------------------------------------------------

uiLabel::uiLabel(uihWidget parent_, std::string text_, Vector2d pos_, Vector2d size_){
	parent = parent_;
	setParamString("name", text_);
	setSize(size_);
	setPos(pos_);
}

void uiLabel::render() {
	auto pos_  = Vector2f(pos.x,  pos.y);
	auto size_ = Vector2f(size.x, size.y);
	uiDrawList *DrawList = getDrawlistStack();
	
	if(!flags.Active){
		needRendering(false);
		std::cout << "Not visivle or not active widget\n";
		return;
	}
	Vector2d tmp_size;
	if(flags.needRendering){
		DrawList[0].AddString(pos_, size_, getStringParams("name").c_str(), Vector3f(1.0,1.0,1.0), TextAlignPos::left);
		needRendering(false);
	}
	innerSize = tmp_size;
}

bool uiLabel::CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){
	return true;
}


uiTextBox::uiTextBox(uihWidget parent_, std::string *text_, Vector2d pos_, Vector2d size_){
	last_value = "";
	parent = parent_;
	value = text_;
	line_heights.reserve(10);
	flags.EnableAutoExpandX = 1;
	flags.EnableAutoExpandY = 1;
	flags.EnableAutoShrinkX = 1;
	flags.EnableAutoShrinkY = 1;
	setSize(size_);
	setPos(pos_);
}

void uiTextBox::render() {
	uiDrawList *DrawList = getDrawlistStack();
	auto style = getStyle();
    auto textrenderer = getTextRenderer();
	auto pos_  = Vector2f(pos.x,  pos.y);
	auto size_ = Vector2f(size.x, size.y);

	if(!flags.Active){
		needRendering(false);
		std::cout << "Not visivle or not active widget\n";
		return;
	}
		
	if((*value != last_value) ||(innerSize != lastInnerSize) || (scrollPos != lastScrollPos)) {
		needRendering(true); 
	}
	if(flags.needRendering == 0) return;
	last_value = *value;
	
	//draw background
	DrawList[0].AddRectangle(pos_, size_, -1, style->col_WidgetBg);
	DrawList[0].AddRectangle(pos_, size_, 3, style->col_WidgetLine);

    std::u32string  u32str = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().from_bytes(last_value);
    Vector2d size_tmp;
    unsigned int dheight;
	line_heights.resize(0);

    uint16_t CursorX = style->WidgetInnerSpace_x; 
	uint16_t CursorY = style->WidgetInnerSpace_y;
	uint16_t TextMaxHeight = 0;
	uint16_t TextMaxWidth = 0;

    for (uint32_t n = 0; n < u32str.size(); n++){
		textrenderer->getSize(u32str[n], &size_tmp, &dheight);
		// "A\nA\tA\rA\0A";
		// \n = 10, \t = 9, \r = 13, \1 = 1 <-- \1 = \0x01と書くこともできる
		// \1 ~ ; (59)まではコマンドとする（アイコンを追加できる）
		
		if(((CursorX + size_tmp.x + style->TextSpacing > size.x - style->ScrollbarWidth) && !(flags.EnableScroll_X))|| (u32str[n] == 10)){
			CursorY = TextMaxHeight + style->TextSpacing;
			line_heights.push_back(TextMaxHeight);
			TextMaxWidth = std::max(TextMaxWidth, CursorX);
			CursorX = style->WidgetInnerSpace_x;
		}
		if(u32str[n] == 10) continue;

		if((scrollPos.y <= CursorY) && (CursorY + style->FontSize - dheight + size_tmp.y < scrollPos.y + size.y) && 
		   (scrollPos.x <= CursorX) && (CursorX + size_tmp.x < scrollPos.x + size.x)){ //描画領域に入っていたら
			DrawList[0].AddChar(Vector2f(CursorX + pos.x - scrollPos.x, CursorY + style->FontSize - dheight + pos.y - scrollPos.y), u32str[n], style->col_Text);
		}

		CursorX += size_tmp.x + style->TextSpacing;
		TextMaxHeight = std::max(TextMaxHeight, (uint16_t)(CursorY + size_tmp.y));
    }

	innerSize.x = TextMaxWidth + style->ScrollbarWidth;
	innerSize.y = TextMaxHeight + style->ScrollbarWidth;
	renderScrollbars();

	needRendering(false);
}

bool uiTextBox::CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){
	
	if(flag  == uiCallbackFlags::MouseScroll){
		std::cout << "Mouse scroll on TextBox\n";
	}

	CallbackScrollbars(flag, vec2_1, num_1, num_2, strings);
	return true;
}


// -----------------------------------------------------
//   [SECTION]   Widget Button
// -----------------------------------------------------

uiButton::uiButton(uihWidget parent_, std::string text_, bool *value_, Vector2d pos_, Vector2d size_){
	parent = parent_;
	value = value_;
	last_value = !(*value);
	setSize(size_);
	setPos(pos_);
}

void uiButton::render() {
	if(!flags.Active){
		needRendering(false);
		std::cout << "Not visivle or not active widget\n";
		return;
	}

	Vector2d tmp_size;
	if(*value != last_value){needRendering(true);}
	if(flags.needRendering){
		uiDrawList *DrawList = getDrawlistStack();
		auto style = getStyle();
		auto pos_  = Vector2f(pos.x,  pos.y);
		auto size_ = Vector2f(size.x, size.y);
		if(*value){
			DrawList[0].AddRectangle(pos_, size_, -1, style->col_WidgetBg);
		}else{
			if(flags.isHovering){		
				DrawList[0].AddRectangle(pos_, size_, -1, style->col_WidgetBgSelected);
			}else{
				DrawList[0].AddRectangle(pos_, size_, -1, style->col_WidgetBgHover);
			}
		}
		DrawList[0].AddRectangle(pos_, size_, 3, style->col_WidgetLine);
		tmp_size = DrawList[0].AddString(pos_, size_, getStringParams("name").c_str(), style->col_Text, TextAlignPos::center);

		last_value = *value;
		needRendering(false);
	}
	auto style = getStyle();
	innerSize = Vector2d(tmp_size.x + style->WidgetInnerSpace_x*2, tmp_size.y + style->WidgetInnerSpace_y*2);
}

bool uiButton::CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){
	if ( flag == uiCallbackFlags::LMouseDown && contains(vec2_1.x, vec2_1.y)){
	// if ( flag == uiCallbackFlags::LMouseDown){
		*value = !(*value);
		needRendering(true);
	}
	return true;
}

uiButtonEx::uiButtonEx(uihWidget parent_, std::string text_, bool *value_, Vector2d pos_, Vector2d size_){
	parent = parent_;
	setParamString("name", text_);
	setParamString("name", text_);
	setParamInt("bgCol_on", 0x1122AA);
	setParamInt("bgCol_off", 0xAA2200);
	setParamInt("ColText_on", 0xFFFFFF);
	setParamInt("ColText_off", 0xFFFFFF);
	setParamFloat("scale",  1.5f);

	value = value_;
	last_value = !(*value);
	setSize(size_);
	setPos(pos_);
}

void uiButtonEx::render() {
	// color_on <- 24bitのint型 (uint64から)
	// color_off <- 24bitのint型 (uint64から)
	// bgcolor_on 同様
	// bgcolor_off 同様
	// scale <- float 1.0で標準

	if(!flags.Active){
		needRendering(false);
		std::cout << "Not visivle or not active widget\n";
		return;
	}

	uint64 tmp_value;
	tmp_value = getIntParam("bgCol_on");      const Vector3f bgCol_on  (float(tmp_value >> 16) / 255.0f, float((tmp_value >> 8) & 0xFF) / 255.0f, float(tmp_value | 0xFF) / 255.0f);
	tmp_value = getIntParam("bgCol_off");     const Vector3f bgCol_off (float(tmp_value >> 16) / 255.0f, float((tmp_value >> 8) & 0xFF) / 255.0f, float(tmp_value | 0xFF) / 255.0f);
	tmp_value = getIntParam("ColText_on");    const Vector3f ColTxt_on (float(tmp_value >> 16) / 255.0f, float((tmp_value >> 8) & 0xFF) / 255.0f, float(tmp_value | 0xFF) / 255.0f);
	tmp_value = getIntParam("ColText_off");   const Vector3f ColTxt_off(float(tmp_value >> 16) / 255.0f, float((tmp_value >> 8) & 0xFF) / 255.0f, float(tmp_value | 0xFF) / 255.0f);

	float scale = getFloatParam("scale");

	Vector2d tmp_size;
	if(*value != last_value){needRendering(true);}
	if(flags.needRendering){
		uiDrawList *DrawList = getDrawlistStack();
		auto style = getStyle();
		auto pos_  = Vector2f(pos.x,  pos.y);
		auto size_ = Vector2f(size.x, size.y);
		if(*value){
			DrawList[0].AddRectangle(pos_, size_, -1, bgCol_on);
		}else{
			// if(flags.isHovering){		
			// 	DrawList[0].AddRectangle(pos_, size_, -1, bgCol_off);
			// }else{
				DrawList[0].AddRectangle(pos_, size_, -1, bgCol_off);
			// }
		}
		DrawList[0].AddRectangle(pos_, size_, 3, style->col_WidgetLine);
		tmp_size = DrawList[0].AddString(pos_, size_, getStringParams("name").c_str(), ColTxt_on, TextAlignPos::center, scale);

		last_value = *value;
		needRendering(false);
	}
	auto style = getStyle();
	innerSize = Vector2d(tmp_size.x + style->WidgetInnerSpace_x*2, tmp_size.y + style->WidgetInnerSpace_y*2);
}


bool uiButtonEx::CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){
	if ( flag == uiCallbackFlags::LMouseDown && contains(vec2_1.x, vec2_1.y)){
	// if ( flag == uiCallbackFlags::LMouseDown){
		*value = !(*value);
		needRendering(true);
	}
	return true;
}







uiCheckBox::uiCheckBox(uihWidget parent_, std::string text_, bool *value_, Vector2d pos_, Vector2d size_){
	parent = parent_;
	setParamString("name", text_);	
	value = value_;
	last_value = !(*value);
	flags.EnableAutoExpandX = 0;
	flags.EnableAutoExpandY = 0;
	flags.EnableAutoShrinkX = 0;
	flags.EnableAutoShrinkY = 0;
	setSize(size_);
	setPos(pos_);
}


void uiCheckBox::render() {
	Vector2d tmp_size;
	if(!flags.Active){
		needRendering(false);
		std::cout << "Not visivle or not active widget\n";
		return;
	}

	if(*value != last_value){ needRendering(true); }
	if(flags.needRendering){
		uiDrawList *DrawList = getDrawlistStack();
		auto style = getStyle();
		auto pos_  = Vector2f(pos.x,  pos.y);
		auto size_ = Vector2f(size.x, size.y);
		DrawList[0].AddRectangle(pos_ , size_, -1, style->col_WidgetBg);
		DrawList[0].AddRectangle(pos_ + Vector2f(2, 2), Vector2f(size_.y-4, size_.y-4), -1, style->col_PopupBg);

		if(*value){
			DrawList[0].AddCheck(pos_+ Vector2f(2, 2), Vector2f(size_.y-4, size_.y-4), style->col_WidgetLineHover, 3);
		}
		DrawList[0].AddRectangle(pos_, size_, 3, style->col_WidgetLine);
		tmp_size = DrawList[0].AddString(pos_ + Vector2f(size.y, 0), size_ - Vector2f(size.y, 0), getStringParams("name").c_str(), style->col_Text, TextAlignPos::center);

		last_value = *value;
		needRendering(false);
	}
	auto style = getStyle();
	innerSize = Vector2d(tmp_size.x + tmp_size.y + style->WidgetInnerSpace_x*4 , tmp_size.y + style->WidgetInnerSpace_y*2);
}


bool uiCheckBox::CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){
	// if ( flag == uiCallbackFlags::LMouseDown && contains(vec2_1.x, vec2_1.y)){
	if ( flag == uiCallbackFlags::LMouseDown){
		*value = !(*value);
		std::cout << "value changed!\n";
	}
	return true;
}


// -----------------------------------------------------
//   [SECTION]   Widget Sliders
// -----------------------------------------------------

uiSliderH::uiSliderH(uihWidget parent_, std::string text_, float *value_, Vector2d pos_, Vector2d size_){
	parent = parent_;
	setParamString("name", text_);
	value = value_;
	last_value = *value_;
	setParamFloat("min_value", 0.0f);
	setParamFloat("max_value", 100.0f);
	setParamFloat("delta_value", 1.0f);
	flags.EnableAutoExpandX = true;
	flags.EnableAutoShrinkX = true;
	setSize(size_);
	setPos(pos_);
}


void uiSliderH::render() {
	if(!flags.Active){
		needRendering(false);
		std::cout << "Not visivle or not active widget\n";
		return;
	}

	Vector2d tmp_size;
	if((*value != last_value) || ((flags.needRendering))){
		uiDrawList *DrawList = getDrawlistStack();
		// bool v = <>getValue();
		auto pos_  = Vector2f(pos.x,  pos.y);
		auto size_ = Vector2f(size.x, size.y);
		float pos_x = pos_.x + size_.x * (*value - getFloatParam("min_value"))/(getFloatParam("max_value")-getFloatParam("min_value"));

		DrawList[0].AddRectangle(pos_, size_, -1, Vector3f(0.3, 0.3, 0.6));
		DrawList[0].AddRectangle(pos_, size_,  2, Vector3f(1.0, 0.7, 0.7));

		DrawList[0].AddRectangle(pos_, Vector2f(size_.x, 5), -1, Vector3f(0.3, 0.3, 0.6));
		DrawList[0].AddRectangle(Vector2f(pos_x, pos_.y), Vector2f(10,30), -1, Vector3f(1.0,1.0,1.0));
		char tt[20];
		sprintf (tt, "v=%f", *value);
		tmp_size = DrawList[0].AddString(pos_, size_, tt, Vector3f(1.0,1.0,1.0), TextAlignPos::center);
		last_value = *value;
		needRendering(false);
	}
	innerSize = Vector2d(tmp_size.x+20, tmp_size.y);
}


bool uiSliderH::CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){
	if ( flag == uiCallbackFlags::LMouseDown){
		*value = float((vec2_1.x - pos.x)*(getFloatParam("max_value") - getFloatParam("min_value")))/float(size.x) - getFloatParam("min_value");
		std::cout << "value changed!\n";
	}else if(flag ==uiCallbackFlags::MouseScroll){
		std::cout << "MOUSESCROLL" << vec2_1.x << vec2_1.y << std::endl;
		*value += getFloatParam("delta_value");
	}else if(flag ==uiCallbackFlags::MouseMove){
		if (num_2 & 0b01){
			*value = float((vec2_1.x - pos.x)*(getFloatParam("max_value") - getFloatParam("min_value")))/float(size.x) - getFloatParam("min_value");
		}
	}
	*value = std::min(std::max(*value, getFloatParam("min_value")), getFloatParam("max_value"));
	return true;
}





// -----------------------------------------------------
//   [SECTION]   Widget ListBox
// -----------------------------------------------------
uiListBox::uiListBox(uihWidget parent_, std::string texts_, uint16_t *value_, Vector2d pos_, Vector2d size_){
	parent = parent_;
	auto style = getStyle();
	value = value_;
	flags.EnableAutoExpandX = true;
	flags.EnableAutoExpandY = 0;
	flags.EnableAutoShrinkY = 0;
	flags.EnableAutoShrinkX = true;
	last_value = 0;
	setSize(size_);
	setPos(pos_);
	setParamFloat("row_height", float(style->FontSize + style->WidgetInnerSpace_y*2 + style->TextSpacing*2));
	setParamString("text", texts_);
}

void uiListBox::render() {
	uiDrawList *DrawList = getDrawlistStack();
	auto style = getStyle();
	auto pos_  = Vector2f(pos.x,  pos.y);
	auto size_ = Vector2f(size.x, size.y);
	Vector2d tmp_size;
	float row_height = getFloatParam("row_height");
	innerSize = Vector2d(0,0);
	
	if(!flags.Active){
		needRendering(false);
		std::cout << "Not visivle or not active widget\n";
		return;
	}

	auto text = getStringParams("text");
	
	if((*value != last_value) || (lastScrollPos != scrollPos) || (last_text != text)){ needRendering(true);}
	if(!flags.needRendering)return;

	if(!(flags.EnableScroll_Y)) scrollPos.y = 0;
	last_value = *(uint16_t *)value;

	if(flags.CollapsingTitlebar && flags.EnableTitlebar) size_.y = style->FontSize + style->WidgetInnerSpace_y*2 + 5; 
	else CollapseTitlebar(false);

	//draw background
	DrawList[0].AddRectangle(pos_, size_, -1, style->col_WidgetBg);
	DrawList[0].AddRectangle(pos_, size_,  3, style->col_WidgetLine);

	if(getStringParams("title") != "" ) flags.EnableTitlebar = 1;
	uint16_t title_bar = 0;
	if(flags.EnableTitlebar){
		title_bar  = style->FontSize + style->WidgetInnerSpace_y*2 + 5;
		DrawList[0].AddRectangle(pos_ , Vector2f(size.x, title_bar), -1, style->col_WidgetBgHover);
		DrawList[0].AddRectangle(pos_, Vector2f(size.x, title_bar),  3, style->col_PopupBg);
		DrawList[0].AddArrowDown(pos_ + Vector2f(2, 2), Vector2f(title_bar-5, title_bar-5), style->col_Text);
		tmp_size = DrawList[0].AddString(pos_ + Vector2f(title_bar, 0), Vector2f(size.x, title_bar), getStringParams("title").c_str(), style->col_Text, TextAlignPos::left);
	}
	innerSize.x = tmp_size.x + 20;

	std::vector<std::string> text_list;
    std::string str, s;                      
    std::stringstream ss{text};
    while ( getline(ss, s, '\r') ){ text_list.push_back(s); }

	if(! flags.CollapsingTitlebar){
		for(uint16_t i=0; i<text_list.size(); i++){
			if(( scrollPos.y <= i*row_height) && ((i+1)*row_height <  scrollPos.y + size.y)){ //描画領域に入っていたら
				if(i == *value){
					DrawList[0].AddRectangle(pos_ + Vector2f(0,i*row_height - scrollPos.y+ title_bar ), Vector2f(size.x, row_height), 6, style->col_WidgetLineSelected);
				}else{
					DrawList[0].AddRectangle(pos_ + Vector2f(0,i*row_height - scrollPos.y+ title_bar ), Vector2f(size.x, row_height), 3, style->col_WidgetLineDisabled);
				}
				tmp_size = DrawList[0].AddString(pos_ + Vector2f(style->WidgetInnerSpace_x, i*row_height - scrollPos.y+ title_bar ),  Vector2f(size.x, row_height), text_list[i].c_str(), style->col_Text, TextAlignPos::left);
			}
			innerSize.x = std::max<uint16_t>(tmp_size.x + style->WidgetInnerSpace_x*2 + style->TextSpacing*2, innerSize.x);
		}
		last_value = *value;
		innerSize.y = text_list.size()*row_height + 20 + title_bar;

		renderScrollbars();
	}else{
		// innerSize.y = size_.y;
		innerSize.y = title_bar;
	}

	last_text = text;
	needRendering(false);
}

bool uiListBox::CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){
	auto style = getStyle();
	CallbackTitlebar(flag, vec2_1, num_1, num_2, strings);

	uint16_t title_bar = 0;
	if(flags.EnableTitlebar) title_bar  = style->FontSize + style->WidgetInnerSpace_y*2 + 10;
	auto text = getStringParams("text");

	if(flag == uiCallbackFlags::LMouseDown){
		if(pos.x <= vec2_1.x && vec2_1.x <= pos.x + size.x - style->ScrollbarWidth && pos.y + title_bar <= vec2_1.y && vec2_1.y <= pos.y + size.y){
			uint16_t v = (vec2_1.y - pos.y + scrollPos.y - title_bar)/getFloatParam("row_height");
			auto text = getStringParams("text");
			*value = std::clamp<uint16_t>(v, 0, std::count(text.cbegin(), text.cend(), '\r')); //TODO 最後に\rが入っている場合は std::count(text.cbegin(), text.cend(), '\r')-1なんだよな...
			LOGE << "value = " << *value;
		}
	}

	// for(int i=0; i<widgets.size(); i++){
	// 	widgets[i]->CallbackFunc(flag, vec2_1, num_1, num_2, strings);
	// }

	CallbackScrollbars(flag, vec2_1, num_1, num_2, strings);

	return true;
}





// -----------------------------------------------------
//   [SECTION]   Radio Button
// -----------------------------------------------------

uiRadioH::uiRadioH(uihWidget parent_, std::string texts_, int *value_, Vector2d pos_, Vector2d size_){
	parent = parent_;
	auto style = getStyle();
	value = value_;
	flags.EnableAutoExpandX = true;
	flags.EnableAutoExpandY = 0;
	flags.EnableAutoShrinkY = 0;
	flags.EnableAutoShrinkX = true;
	last_value = 0;
	setSize(size_);
	setPos(pos_);
	setParamFloat("row_height", float(style->FontSize + style->WidgetInnerSpace_y*2 + style->TextSpacing*2));
	setParamString("text", texts_);
}


void uiRadioH::render() {
	uiDrawList *DrawList = getDrawlistStack();
	auto style = getStyle();
	auto pos_  = Vector2f(pos.x,  pos.y);
	auto size_ = Vector2f(size.x, size.y);
	Vector2d tmp_size;
	float row_height = getFloatParam("row_height");
	innerSize = Vector2d(0,0);
	
	if(!flags.Active){
		needRendering(false);
		std::cout << "Not visivle or not active widget\n";
		return;
	}

	auto text = getStringParams("text");
	
	if((*value != last_value) || (lastScrollPos != scrollPos) || (last_text != text)){ needRendering(true);}
	if(!flags.needRendering)return;

	if(!(flags.EnableScroll_Y)) scrollPos.y = 0;
	last_value = *value;

	if(flags.CollapsingTitlebar && flags.EnableTitlebar) size_.y = style->FontSize + style->WidgetInnerSpace_y*2 + 5; 
	else CollapseTitlebar(false);

	//draw background
	DrawList[0].AddRectangle(pos_, size_, -1, style->col_WidgetBg);
	DrawList[0].AddRectangle(pos_, size_,  3, style->col_WidgetLine);

	if(getStringParams("title") != "" ) flags.EnableTitlebar = 1;
	uint16_t title_bar = 0;
	if(flags.EnableTitlebar){
		title_bar  = style->FontSize + style->WidgetInnerSpace_y*2 + 5;
		DrawList[0].AddRectangle(pos_ , Vector2f(size.x, title_bar), -1, style->col_WidgetBgHover);
		DrawList[0].AddRectangle(pos_, Vector2f(size.x, title_bar),  3, style->col_PopupBg);
		DrawList[0].AddArrowDown(pos_ + Vector2f(2, 2), Vector2f(title_bar-5, title_bar-5), style->col_Text);
		tmp_size = DrawList[0].AddString(pos_ + Vector2f(title_bar, 0), Vector2f(size.x, title_bar), getStringParams("title").c_str(), style->col_Text, TextAlignPos::left);
	}
	innerSize.x = tmp_size.x + 20;

	std::vector<std::string> text_list;
    std::string str, s;                      
    std::stringstream ss{text};
    while ( getline(ss, s, '\r') ){ text_list.push_back(s); }

	float each_size = size.x / text_list.size();
	const float circle_size = 30;

	innerSize.x = 0;
	innerSize.y = row_height + style->WidgetInnerSpace_y*2 + title_bar;

	if(!flags.CollapsingTitlebar){
		for(uint16_t i=0; i<text_list.size(); i++){
			DrawList[0].AddCircle(pos_ + Vector2f(each_size*i + circle_size/2, title_bar+ circle_size/2), Vector2f(circle_size, circle_size),  Vector3f(0.3,0.3,0.3), -1 );
			if(i == *value)
				DrawList[0].AddCircle(pos_ + Vector2f(each_size*i + circle_size/2, title_bar+ circle_size/2), Vector2f(circle_size/2, circle_size/2),  Vector3f(0.8,0.3,0.3), -1 );
			tmp_size = DrawList[0].AddString(pos_ + Vector2f(each_size*i + circle_size, title_bar),  Vector2f(each_size - circle_size - style->WidgetInnerSpace_x, row_height), text_list[i].c_str(), style->col_Text, TextAlignPos::left);
			innerSize.x += tmp_size.x;
		}
		last_value = *value;
	}

	last_text = text;
	needRendering(false);
}

bool uiRadioH::CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){
	auto style = getStyle();
	CallbackTitlebar(flag, vec2_1, num_1, num_2, strings);


	auto text = getStringParams("text");
	int numList = std::count(text.cbegin(), text.cend(), '\r') + 1;

	uint16_t title_bar = 0;
	if(flags.EnableTitlebar) title_bar  = style->FontSize + style->WidgetInnerSpace_y*2 + 10;

	float each_size = size.x / numList;
	const float circle_size = 30;

	if(flag != uiCallbackFlags::LMouseDown)return false;
	
	if(pos.y + title_bar < vec2_1.y &&  vec2_1.y < circle_size + pos.y + title_bar && pos.x < vec2_1.x  && vec2_1.x < pos.x + size.x ){
		*value = std::clamp<uint16_t>((vec2_1.x - pos.x + 5)/each_size, 0, numList-1); //TODO 最後に\rが入っている場合は std::count(text.cbegin(), text.cend(), '\r')-1なんだよな...
		LOGE << "value = " << *value;
		return true;
	}
	return false;
}






uiRadioV::uiRadioV(uihWidget parent_, std::string texts_, int *value_, Vector2d pos_, Vector2d size_){
	parent = parent_;
	auto style = getStyle();
	value = value_;
	flags.EnableAutoExpandX = true;
	flags.EnableAutoExpandY = 0;
	flags.EnableAutoShrinkY = 0;
	flags.EnableAutoShrinkX = true;
	last_value = 0;
	setSize(size_);
	setPos(pos_);
	setParamFloat("row_height", float(style->FontSize + style->WidgetInnerSpace_y*2 + style->TextSpacing*2));
	setParamString("text", texts_);
}


void uiRadioV::render() {
	uiDrawList *DrawList = getDrawlistStack();
	auto style = getStyle();
	auto pos_  = Vector2f(pos.x,  pos.y);
	auto size_ = Vector2f(size.x, size.y);
	Vector2d tmp_size;
	float row_height = getFloatParam("row_height");
	innerSize = Vector2d(0,0);
	
	if(!flags.Active){
		needRendering(false);
		std::cout << "Not visivle or not active widget\n";
		return;
	}

	auto text = getStringParams("text");
	
	if((*value != last_value) || (lastScrollPos != scrollPos) || (last_text != text)){ needRendering(true);}
	if(!flags.needRendering)return;

	if(!(flags.EnableScroll_Y)) scrollPos.y = 0;
	last_value = *value;

	if(flags.CollapsingTitlebar && flags.EnableTitlebar) size_.y = style->FontSize + style->WidgetInnerSpace_y*2 + 5; 
	else CollapseTitlebar(false);

	//draw background
	DrawList[0].AddRectangle(pos_, size_, -1, style->col_WidgetBg);
	DrawList[0].AddRectangle(pos_, size_,  3, style->col_WidgetLine);

	if(getStringParams("title") != "") flags.EnableTitlebar = 1;
	uint16_t title_bar = 0;
	if(flags.EnableTitlebar){
		title_bar  = style->FontSize + style->WidgetInnerSpace_y*2 + 5;
		DrawList[0].AddRectangle(pos_ , Vector2f(size.x, title_bar), -1, style->col_WidgetBgHover);
		DrawList[0].AddRectangle(pos_, Vector2f(size.x, title_bar),  3, style->col_PopupBg);
		DrawList[0].AddArrowDown(pos_ + Vector2f(2, 2), Vector2f(title_bar-5, title_bar-5), style->col_Text);
		tmp_size = DrawList[0].AddString(pos_ + Vector2f(title_bar, 0), Vector2f(size.x, title_bar), getStringParams("title").c_str(), style->col_Text, TextAlignPos::left);
	}
	innerSize.x = tmp_size.x + 20;

	std::vector<std::string> text_list;
    std::string str, s;                      
    std::stringstream ss{text};
    while ( getline(ss, s, '\r') ){ text_list.push_back(s); }

	const float circle_size = 30;

	innerSize.x = 0;
	innerSize.y = row_height * text_list.size();

	if(!flags.CollapsingTitlebar){
		for(uint16_t i=0; i<text_list.size(); i++){
			DrawList[0].AddCircle(pos_ + Vector2f(circle_size/2, title_bar+ circle_size/2 + row_height*i), Vector2f(circle_size, circle_size),  Vector3f(0.3,0.3,0.3), -1 );
			if(i == *value){
				DrawList[0].AddCircle(pos_ + Vector2f(circle_size/2, title_bar+ circle_size/2 + row_height*i), Vector2f(circle_size/2, circle_size/2),  Vector3f(0.8,0.3,0.3), -1 );
			}else{
				DrawList[0].AddCircle(pos_ + Vector2f(circle_size/2, title_bar+ circle_size/2 + row_height*i), Vector2f(circle_size/2, circle_size/2),  Vector3f(0.3,0.8,0.8), -1 );
			}
			tmp_size = DrawList[0].AddString(pos_ + Vector2f(style->WidgetInnerSpace_x, row_height*i + title_bar),  Vector2f(size.x - style->WidgetInnerSpace_x*2, row_height), text_list[i].c_str(), style->col_Text, TextAlignPos::left);
			innerSize.x += std::max<int>(tmp_size.x, innerSize.x);
		}
	}

	last_text = text;
	needRendering(false);
}

bool uiRadioV::CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){
	auto style = getStyle();
	CallbackTitlebar(flag, vec2_1, num_1, num_2, strings);

	uint16_t title_bar = 0;
	if(flags.EnableTitlebar) title_bar  = style->FontSize + style->WidgetInnerSpace_y*2 + 10;

	auto text = getStringParams("text");
	std::vector<std::string> text_list;
    std::string str, s;                      
    std::stringstream ss{text};
    while ( getline(ss, s, '\r') ){ text_list.push_back(s); }


	const float circle_size = 30;
	float row_height = getFloatParam("row_height");

	if(flag != uiCallbackFlags::LMouseDown)return false;

	if(pos.x <= vec2_1.x && vec2_1.x <= pos.x + circle_size && pos.y + title_bar <= vec2_1.y && vec2_1.y <= pos.y + size.y){
		uint16_t v = (vec2_1.y - pos.y + scrollPos.y - title_bar)/row_height;
		auto text = getStringParams("text");
		*value = std::clamp<uint16_t>(v, 0, std::count(text.cbegin(), text.cend(), '\r')); //TODO 最後に\rが入っている場合は std::count(text.cbegin(), text.cend(), '\r')-1なんだよな...
		LOGE << "value = " << *value;
	}
	return false;
}





// -----------------------------------------------------
//    [SECTION] Frame
// -----------------------------------------------------

uiFrame::uiFrame(uihWidget parent_, std::string title_, Vector2d pos_, Vector2d size_){
	parent = parent_;
	setSize(size_);
	setPos(pos_);
	setParamString("title", title_);
	needApplyAlignment();
	needCalcInnerSize();
	needRendering(true);
	flags.EnableAutoExpandX = 1;
	flags.EnableAutoExpandY = 1;
	flags.EnableAutoShrinkX = 1;
	flags.EnableAutoShrinkY = 1;
	flags.EnableUserResize  = 1;

	setAlignType(uiWidgetAlignTypes::VertialListl);
}


void uiFrame::render() {
	if(!flags.Active){
		needRendering(false);
		std::cout << "Not visivle or not active widget\n";
		return;
	}

	uiDrawList *DrawList = getDrawlistStack();
	auto style = getStyle();
	auto pos_  = Vector2f(pos.x,  pos.y);
	auto size_ = Vector2f(size.x, size.y);
	
	DrawList[0].setClipRect(pos_, size_);
	DrawList[1].setClipRect(pos_, size_);

	if(lastScrollPos != scrollPos){ needRendering(true);}
	
	uint16_t title_bar = flags.EnableTitlebar ? (style->FontSize + style->WidgetInnerSpace_y*2 + 5) : 0;

	if(flags.needRendering){
		if(flags.CollapsingTitlebar && flags.EnableTitlebar) size_.y = style->FontSize + style->WidgetInnerSpace_y*2 + 5; 
		else CollapseTitlebar(false);

		DrawList[0].AddRectangle(pos_, size_, -1, style->col_WidgetBg);
		DrawList[0].AddRectangle(pos_, size_,  3, style->col_WidgetLine);

		if(getStringParams("title") != "" ) flags.EnableTitlebar = 1;
		if(flags.EnableTitlebar){
			DrawList[0].AddRectangle(pos_ , Vector2f(size.x, title_bar), -1, style->col_WidgetBgHover);
			DrawList[0].AddRectangle(pos_, Vector2f(size.x, title_bar),  3, style->col_PopupBg);
			if(flags.CollapsingTitlebar) DrawList[0].AddArrowRight(pos_ + Vector2f(2, 2), Vector2f(title_bar-5, title_bar-5), style->col_Text);
			else DrawList[0].AddArrowDown(pos_ + Vector2f(2, 2), Vector2f(title_bar-5, title_bar-5), style->col_Text);
			DrawList[0].AddString(pos_ + Vector2f(title_bar, 0), Vector2f(size.x, title_bar), getStringParams("title").c_str(), style->col_Text, TextAlignPos::left);
		}

	}

	DrawList[0].setClipRect(pos_ + Vector2f(0, title_bar), size_ - Vector2f(0, title_bar));
	DrawList[1].setClipRect(pos_ + Vector2f(0, title_bar), size_ - Vector2f(0, title_bar));
	// DrawList[0].setClipRect(pos_ + Vector2f(0, title_bar), Vector2f(100, 100));

	if(!flags.CollapsingTitlebar && flags.EnableTitlebar){
		// std::cout << "RENDER CHILD WIDGETS!!!!\n";
		for(uint16_t i=0; i<widgets.size(); i++) widgets[i]->render();
		renderScrollbars();
	}else{
		innerSize.y = size_.y;
	}

	needRendering(false);
}

bool uiFrame::CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){
	// auto style = getStyle();
	CallbackTitlebar(flag, vec2_1, num_1, num_2, strings);
	// CallbackResizer(flag, vec2_1, num_1, num_2, strings);

	// for(int i=0; i<widgets.size(); i++)
	//	 widgets[i]->CallbackFunc(flag, vec2_1, num_1, num_2, strings);
	// if(!flags.CollapsingTitlebar && flags.EnableTitlebar)
		CallbackScrollbars(flag, vec2_1, num_1, num_2, strings);
	return true;
}


// -----------------------------------------------------
//    [SECTION] Add Widget
// -----------------------------------------------------
uihWidget AddLabel(uihWidget to, std::string text, Vector2d pos, Vector2d size){
	uihWidget w = new uiLabel(to, text, pos, size);
	w->setParentWidget(to);
	if(to->AddWidget(w)){ return w;}
	else{ delete w; return nullptr;}
}

uihWidget AddButton(uihWidget to, bool *value, std::string text, Vector2d pos, Vector2d size){
	uihWidget w = new uiButton(to, text, value, pos, size);
	w->setParentWidget(to);
	if(to->AddWidget(w)){ return w;}
	else{ delete w; return nullptr;}
}

uihWidget AddButtonEx(uihWidget to, bool *value, std::string text, Vector2d pos, Vector2d size){
	uihWidget w = new uiButtonEx(to, text, value, pos, size);
	w->setParentWidget(to);
	if(to->AddWidget(w)){ return w;}
	else{ delete w; return nullptr;}
}

uihWidget AddCheckbox(uihWidget to, bool *value, std::string text, Vector2d pos, Vector2d size){
	uihWidget w = new uiCheckBox(to, text, value, pos, size);
	w->setParentWidget(to);
	if(to->AddWidget(w)){ return w;}
	else{ delete w; return nullptr;}
}

uihWidget AddSliderH(uihWidget to, float *value, std::string text, Vector2d pos, Vector2d size){
	uihWidget w = new uiSliderH(to, text, value, pos, size);
	w->setParentWidget(to);
	if(to->AddWidget(w)){ return w;}
	else{ delete w; return nullptr;}
}


uihWidget AddTextBox(uihWidget to, std::string *value, Vector2d pos, Vector2d size){
	uihWidget w = new uiTextBox(to, value, pos, size);
	w->setParentWidget(to);
	if(to->AddWidget(w)){ return w;}
	else{ delete w; return nullptr;}
}


uihWidget AddListBox(uihWidget to, std::string texts, uint16_t *value, Vector2d pos_, Vector2d size_){
	uihWidget w = new uiListBox(to, texts, value, pos_, size_);
	w->setParentWidget(to);
	if(to->AddWidget(w)){ return w;}
	else{ delete w; return nullptr;}
}


uihWidget AddRadioH(uihWidget to, std::string texts, int *value, Vector2d pos_, Vector2d size_){
	uihWidget w = new uiRadioH(to, texts, value, pos_, size_);
	w->setParentWidget(to);
	if(to->AddWidget(w)){ return w;}
	else{ delete w; return nullptr;}
}



uihWidget AddRadioV(uihWidget to, std::string texts, int *value, Vector2d pos_, Vector2d size_){
	uihWidget w = new uiRadioV(to, texts, value, pos_, size_);
	w->setParentWidget(to);
	if(to->AddWidget(w)){ return w;}
	else{ delete w; return nullptr;}
}


uihWidget AddFrame(uihWidget to, std::string title, Vector2d pos_, Vector2d size_){
	uihWidget w = new uiFrame(to, title, pos_, size_);
	w->setParentWidget(to);
	if(to->AddWidget(w)){ return w;}
	else{ delete w; return nullptr;}
}



} //namespace glUI


