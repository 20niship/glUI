/**
 * @file glUI.cpp
 * @brief glUIについての基本的な関数群。
 * @author 20Niship Nishimiya Tadashi
 * @date 2021-01-17
 * @details 詳細な説明
 */

#pragma once

#include <iostream>
#include <codecvt>
#include <string>
#include <cmath>
#include <unordered_map>
#include <variant>

#include <opencv2/opencv.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h> 


#include "common.hpp"
#include "logger.hpp"

#include "renderer.hpp"
#include "MessageBox.hpp"

namespace glUI{

struct uiContext;
struct uiStyle;
class uiWindow;
class uiShader;
class uiTexture;
class uiFont;
class uiWidget;
class uiDrawList;
// struct uiDrawListStack;


using uihWnd = uiWindow*;
using uihWidget = uiWidget*;
// using uiWindowFlags = uint16_t;
// using uiFrameFlags = uint16_t;
// using uiCallbackFlags = uint16_t;
// using uiWidgetAlignTypes = uint8_t;


// -----------------------------------------------------
//    [SECTION] main process
// -----------------------------------------------------
uihWnd uiCreateWindow(const char *name, const Vector2d size=Vector2d(640, 480));
void setStyle(float *ui_style);
void setColor(const char *color_style, char *colors);


// INTERNAL FUNCTIONS!!
uiDrawList *getDrawlistStack(); //現在レンダリングに使われている
uiFont *getTextRenderer();
uiShader *getShaderObject();
uiTexture *getTextureRenderer();
uihWnd getWindow(uihWidget);
uiStyle *getStyle();
uiContext *getContext();

void setRenderingWindow(uiWindow *wnd);
uihWnd getRenderingWindow();
bool uiShouldWindowClose();
void terminateUI();
void startApp();

// rendering 
void RenderAll();
short int searchFocusedPanel(short, short);
short int sesarchFocusedWidget(uihWnd, unsigned short, unsigned short);



enum class uiCallbackFlags : uint16_t{
	OnHover        = 0,  // Widget上にマウスがHoverされたとき
	OffHover       = 1,  // Widget上のRectからマウスが離れた時（
	ValueChange    = 2,  // 値が変更されたとき（Userによって？）
	ResizeTo       = 3,  // 
 
	MouseMove      = 4,  // マウスが移動した時（HoverしているWidgetとFocusされたWidgetに送信される）
	LMouseDown     = 5,  // Left mouse down
	RMouseDown     = 6,  // Right mouse down
	CMouseDown     = 7,  // Center mouse down
	LMouseUP       = 8,  // Left mouse up
	RMouseUP       = 9,  // Right mouse up
	CMouseUP       = 10, // Center mouse up
	MouseScroll    = 11, // マウスがスクロールされた（スクロール量はnumに入っている）
	DragDrop       = 12, // ファイルがD＆Dされた（numにファイル数、const char **stringsにファイル名のリスト）
	ShouldClose    = 13, // ウィンドウが閉じられる直前 glfwShouldWindowCloseがtrueになった時
	Keyboard       = 14, // Keyboard input
	CharInput      = 15  // char input (keyboard?)
};


struct uiStyle{
	float WindowPadding_x;
	float WindowPadding_y;
	float WindowBorderSize;
	float WindowMinSize_x;
	float WindowMinSize_y;
	float WindowTitleAlign_x;
	float WindowTitleAlign_y;
	float WindowMenuButtonPosition;
	float ScrollbarWidth;
	float ScrollbarRounding;

	float WidgetBorderSize;
	float WidgetOuterSpace_x;
	float WidgetOuterSpace_y;
	float WidgetInnerSpace_x;
	float WidgetInnerSpace_y;
	float WidgetMinSize_x;
	float WidgetMinSize_y;
	float IndentSpacing;
	float ColumnsMinSpacing;
	float TitlebarHeight;

	float MouseCursorScale;
	float FontSize;
	std::string FontName;
	TextAlignPos textlign;
	bool TextOverflowHidden;
	bool TextAutoSpacing;
	float TextSpacing;
	
	using colorType = Vector3f;
	colorType col_Text;
	colorType col_TextDisabled;
	colorType col_TextHovered;
	colorType col_TextNoEmphasize;
	colorType col_WndBg;
	colorType col_wndLine;
	colorType col_wndScroll;
	colorType col_wndScrollBg;
	colorType col_PopupBg;
	colorType col_PopupLine;

	colorType col_WidgetBg;
	colorType col_WidgetBgHover;
	colorType col_WidgetBgSelected;
	colorType col_WidgetBgDisabled;
	colorType col_WidgetBgMain;
	colorType col_WidgetLine;
	colorType col_WidgetLineHover;
	colorType col_WidgetLineSelected;
	colorType col_WidgetLineDisabled;
};

void setDefaultStyle(uiStyle *style);
void setDrakMode(uiStyle *style);


// -----------------------------------------------------
//    [SECTION] Widget
// -----------------------------------------------------

// enum class uiFrameFlags_ : uint16_t {
// 	Visible                  = 1 << 1,
// 	Active                   = 1 << 2,
// 	EnableScroll_X           = 1 << 4,   // Always show scrollbar (even if ContentSize.y < Size.y)
// 	EnableScroll_Y           = 1 << 12,   // Always show scrollbar (even if ContentSize.y < Size.y)
//     EnableMouseInputs        = 1 << 5,   // Enable mouse callback(mouse Click or mouse Move)
// 	EnableKeyboardInputs     = 1 << 6,   // Enable keyboard callback
// 	EnableChildWidget        = 1 << 7,   // Enable child window and show child windows
// 	EnableDragDropInputs     = 1 << 14,  // Enable child window and show child windows

// 	// [internal] Please access via getter/setter functions
// 	EnableAutoExpandX        = 1 << 9,   // Widgetの大きさが小さく親Widgetに余白が余っている場合、ギリギリまで拡大する
// 	EnableAutoExpandY        = 1 << 10,  // Widgetの大きさが小さく親Widgetに余白が余っている場合、ギリギリまで拡大する
// 	EnableUserResize         = 1 << 11,  // Widgetの大きさが小さく親Widgetに余白が余っている場合、ギリギリまで拡大する

// 	needRenderingAll         = 1 << 13   // 強制的にレンダリングを行う（VertexArrayとかに描画データを格納）
// 	// bool Show Srcoll = ((RectがFrame全体の大きさより大きい) && !Noscroll) || EnableScrollbarAlways
// };


enum class uiWidgetAlignTypes : uint8_t{
	Absolute,          // 絶対座標によってWidgetの一を指定します。デフォルトです。
	HorizontalList,    // Listで左から右にWidgetを格納します。同列に複数のWidgetを追加するにはFrame Widgetを追加してください
	VertialListl,      // Listで上から下にWidgetを格納します。同行に複数のWidgetを追加するにはFrame Widgetを追加してください
	Horizontel_noSpace, // HorizontalListのuiStyle->WidgetOuterSpaceを無くして、Widgetどうしをくっつけたもの
	Vertical_noSpace,   // VertialListlのuiStyle->WidgetOuterSpaceを無くして、Widgetどうしをくっつけたもの
	Grid,              // GridによってWidgetを配置します。
};


class uiWidget {
public:
	uiVector<uiWidget *> widgets;

	struct uiWidgetFlags_{
		// unsigned short Visible               : 1 = 1; 
		unsigned int Active                : 1 = 1;
		unsigned int EnableScroll_X        : 1 = 1;   // Always show scrollbar (even if ContentSize.y < Size.y)
		unsigned int EnableScroll_Y        : 1 = 1;   // Always show scrollbar (even if ContentSize.y < Size.y)
		unsigned int EnableMouseInputs     : 1 = 1;   // Enable mouse callback(mouse Click or mouse Move)
		unsigned int EnableKeyboardInputs  : 1 = 1;   // Enable keyboard callback
		unsigned int EnableChildWidget     : 1 = 1;   // Enable child window and show child windows
		unsigned int EnableDragDropInputs  : 1 = 0;   // Enable child window and show child windows
		unsigned int EnableUserResize      : 1 = 0;

		// [internal] Please access via getter/setter functions
		unsigned int EnableAutoExpandX     : 1 = 0;   // Widgetの大きさが小さく親Widgetに余白が余っている場合、ギリギリまで拡大する
		unsigned int EnableAutoExpandY     : 1 = 0;   // Widgetの大きさが小さく親Widgetに余白が余っている場合、ギリギリまで拡大する
		unsigned int EnableAutoShrinkX     : 1 = 0;   // setSize()で style->minSizeまで縮小できる
		unsigned int EnableAutoShrinkY     : 1 = 0;   // setSize()で style->minSizeまで縮小できる
		unsigned int EnableTitlebar        : 1 = 0;   // Titlebar = style->FontSize + style->WidgetInnerSize.x
		unsigned int CollapsingTitlebar    : 1 = 0;
		unsigned int isHovering            : 1 = 0;
		unsigned int isFocused             : 1 = 0;
		// unsigned int EnableUserResize       : 1 = 0;   // UserがsetSizeでリサイズすることを許可する
		unsigned int needRendering         : 2 = 3;   // 強制的にレンダリングを行う（VertexArrayとかに描画データを格納）
		unsigned int needApplyAlignment    : 1 = 1;   // 自身と子のWidgetのサイズを計算し直す必要がある。
		unsigned int needCalcInnerSize     : 1 = 1;   // Alignmentを計算する時にInnerSizeをレンダリング関数で計算し直す。
		unsigned int setupFinished         : 1 = 0;   // initAlignment()関数が呼ばれたかどうか
	}flags;

// protected:
	uiWidget *parent;
	Vector2d size;
	Vector2d pos;
	uiWidgetAlignTypes align_type;

	// スクロール関連
	Vector2d scrollPos, lastScrollPos;
	Vector2d innerSize, lastInnerSize;
	bool isSelectingScrollX, isSelectingScrollY;
	bool isSelectingResizer, isSelectingEdge_left, isSelectingEdge_top, isSelectingEdge_bottom, isSelectingEdge_right;
	
	uiVector<float> childSizelist_x; //左右方向に複数のWidgetを並べるときの、ふちのX座標のリスト
	uiVector<float> childSizelist_y; //上下方向...

	using paramType = std::variant<float, bool, std::string, uint64>;
    std::unordered_map<std::string, paramType> params{
        {"name", std::string("test")},
        {"title", std::string("__title__")},
		// {"indentX", 0.0f}, // getMinSize, getMaxSizeでプラスされる
		// {"indentY", 0.0f}, // デフォルトでは0
    };
protected:
	void renderScrollbars();
	void CallbackScrollbars(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings);
	void CallbackTitlebar  (uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings);
	bool CallbackResizer   (uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings);
	void CallbackAlignment (uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings);

	[[deprecated]] void calcAlignGrid(){}
	[[deprecated]] void calcAlignHorizontal(bool spacing);
	[[deprecated]] void calcAlignVertial(bool spacing);

	void initAlignOneDimention();
	void initAlignGrid(){}
	void applyAlignOneDimention();
	void applyAlignGrid(){}
	bool tryResizeAlignment(uint8_t index, bool axis_x, int new_pos);
	bool tryResizeAlignment_2(bool axis_x, int old_pos, int new_pos);

// deprecated!!
#if 0
	[[deprecated]] inline void calcAlignment_recursize(){
		if(!flags.needApplyAlignment) return;
		
		switch(align_type){
			case uiWidgetAlignTypes::Absolute: break; //do nothing !! 
			case uiWidgetAlignTypes::Grid:           calcAlignGrid(); break;
			case uiWidgetAlignTypes::HorizontalList: calcAlignHorizontal(true);break;
			case uiWidgetAlignTypes::VertialListl:   calcAlignVertial(true);break;
			case uiWidgetAlignTypes::Horizontel_noSpace:   calcAlignHorizontal(false);break;
			case uiWidgetAlignTypes::Vertical_noSpace:   calcAlignVertial(false);break;
		}
		
		for(int i=0; i<widgets.size(); i++){
			widgets[i]->calcAlignment_recursize();
		}
		flags.needApplyAlignment = false;
		flags.needCalcInnerSize = false; 
	}

#endif

	inline void applyAlignment_recursive(){
		if(!flags.needApplyAlignment) return;

		switch(align_type){
			case uiWidgetAlignTypes::Absolute: break; //do nothing !! 
			case uiWidgetAlignTypes::Grid: applyAlignGrid(); break;

			case uiWidgetAlignTypes::HorizontalList:
			case uiWidgetAlignTypes::VertialListl:
			case uiWidgetAlignTypes::Horizontel_noSpace:
			case uiWidgetAlignTypes::Vertical_noSpace:
				applyAlignOneDimention();
				break;
			default:
				LOGE << "not found function to apply child widgets!";
				break;;
		}
		
		for(int i=0; i<widgets.size(); i++){
			widgets[i]->applyAlignment_recursive();
		}
		flags.needApplyAlignment = false;
	}


	inline void initAlignment_recursive(){
		if(!flags.needApplyAlignment)return;

		switch(align_type){
			case uiWidgetAlignTypes::Absolute: break; //do nothing !! 
			case uiWidgetAlignTypes::Grid: initAlignGrid(); break;

			case uiWidgetAlignTypes::HorizontalList:
			case uiWidgetAlignTypes::VertialListl:
			case uiWidgetAlignTypes::Horizontel_noSpace:
			case uiWidgetAlignTypes::Vertical_noSpace:
				initAlignOneDimention();
				break;
			default:
				LOGE << "not found function to init alignment child widgets!";
				break;;
		}
		
		for(int i=0; i<widgets.size(); i++){
			widgets[i]->initAlignment_recursive();
		}
	}

	inline void calcInnerSize_recursize(){
		if(!flags.needCalcInnerSize)return;
		needRendering(true); 
        for(int i=0; i<widgets.size(); i++) {widgets[i]->calcInnerSize_recursize(); widgets[i]->render();} 
        auto DrawLists = getDrawlistStack();
        DrawLists[0].clearVBOs();
        DrawLists[1].clearVBOs();
		flags.needCalcInnerSize = false;
	}

public:
	uiWidget(const uiWidget&) = delete;
	uiWidget& operator=(const uiWidget&) = delete;

	uiWidget():parent(this){
		needApplyAlignment();
		needCalcInnerSize();
		needRendering(true);
	    isSelectingScrollX = false;
		isSelectingScrollY = false;
		isSelectingResizer = false;
	    isSelectingEdge_left = false;
		isSelectingEdge_top = false;
		isSelectingEdge_bottom = false;
		isSelectingEdge_right = false;
		align_type = uiWidgetAlignTypes::Absolute;
		childSizelist_x.resize(10);
		childSizelist_x.resize(10);
	}
	
	// uiWidget(Vector2d pos, Vector2d size, void *ptr_data){
	// 	setPos(pos);
	// 	setSize(size);
	// }

	virtual ~uiWidget(){
		for(int i=0; i< widgets.size(); i++){
			delete widgets[i];
		}
	}

	virtual void render(){LOGE << "Please overwrite uiWidget::render() function";}
	virtual bool CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){ LOGE << "Please overwrite uiWidget::CallbackFunc()"; return true; }

	bool setSize(Vector2d size_){
		if(align_type == uiWidgetAlignTypes::Absolute){size = size_; return true;}
		auto last_size = size;
		setWidth(size_.x); setHeight(size_.y);
		if(size != last_size) return true;
		return false;  //TODO: 同じ階層の他のWidgetにResize Callbackを出して、OKかどうかを確認する
	}

	inline void setWidth(int x) {
		// auto style = getStyle();

		// if(size.x < x){
		// 	if(flags.EnableAutoExpandX) size.x = x;
		// 	else size.x = std::max<int>(size.x, style->WidgetMinSize_x);
		// }else{
		// 	if(flags.EnableAutoShrinkX) size.x = x;
		// 	else size.x = std::max<int>(size.x, style->WidgetMinSize_x);
		// }

		// auto parent_horizontal =
		// 	( parent->getAlignType() == uiWidgetAlignTypes::HorizontalList) || 
		//      ( parent->getAlignType() == uiWidgetAlignTypes::Horizontel_noSpace);

		// if(parent_horizontal && parent != this && flags.setupFinished){
		// 	LOGW << "tryResizeAlignment_2() start!!";
		// 	parent->tryResizeAlignment_2(true, size.x + pos.x, pos.x + x);
		// }else{
			// auto oldPos = size.x + pos.x;
			size.x = x;
			needRendering(true);
		// }
	}


	inline void setHeight(int y) {
		// auto style = getStyle();
		// auto oldPos = size.y + pos.y;
		size.y = y;
		// LOGE << "setHeight " << y;
		// if(size.y < y){
		// 	if(flags.EnableAutoExpandY) size.y = y;
		// 	else size.y = std::max<int>(size.y, style->WidgetMinSize_y);
		// }else{
		// 	if(flags.EnableAutoShrinkY) size.y = y;
		// 	else size.y = std::max<int>(size.y, style->WidgetMinSize_y);
		// }
		needRendering(true);

		// if((align_type == uiWidgetAlignTypes::VertialListl) || 
		// 	(align_type == uiWidgetAlignTypes::Vertical_noSpace)){
		// 	tryResizeAlignment(childSizelist_x.size()-1, false, size.y);
		// }
		// auto parent_horizontal = 
		// 	( parent->getAlignType() == uiWidgetAlignTypes::VertialListl) || 
		//      ( parent->getAlignType() == uiWidgetAlignTypes::Vertical_noSpace);
		// if(parent_horizontal && parent != this){
		// 	parent->tryResizeAlignment_2(true, oldPos, pos.y + size.y);
		// }
	}


	inline uint16_t  getWidth()                   { return size.x;}
	inline uint16_t  getHeight()                  { return size.y;}
	inline uint16_t  getPosX()                    { return pos.x;}
	inline uint16_t  getPosY()                    { return pos.y;}
	inline Vector2d  getPos()                     { return pos;}
	inline Vector2d  getSize()                    { return size;}
	inline Vector2d  getInnerSize()               { return innerSize; }
	inline int       getInnerWidth()              { return innerSize.x; }
	inline int       getInnerHeight()             { return innerSize.y; }
	inline void      setPos(Vector2d pos_)        { pos.x = pos_.x; pos.y = pos_.y; needRendering(true);}
	inline void      setPosX(int x_)               { pos.x = x_; needRendering(true);}
	inline void      setPosY(int y_)               { pos.y = y_; needRendering(true);}
	inline bool      contains(int x, int y)       { return ((x >= pos.x) && (x <= pos.x + size.x) && (y >= pos.y) && (y <= pos.y + size.y)); }
	inline bool     containsTitlebar(int x, int y){ auto style = getStyle(); return ((x >= pos.x) && (x <= pos.x + size.x) && (y >= pos.y) && (y <= pos.y + style->TitlebarHeight)); }
	inline void      needRendering(bool necessary){ if(necessary){ flags.needRendering = 3; for(int i=0; i< widgets.size(); i++){ widgets[i]->needRendering(necessary);} /*if(parent != this && parent != nullptr){parent->needRendering(true); }*/ }else{if(flags.needRendering > 0) flags.needRendering --;}}
	inline void      needApplyAlignment ()         { needCalcAlignment__recursize_child(); impl_needCalcAlinment_parent();} //
	inline void      needCalcInnerSize()          { impl_needCalcInnerSize_parent();} //
	inline uiWidget *getParentWidget()            { return parent; }
	inline void      setParentWidget(uiWidget *w) { parent = w;    }

	// [internal] Do Not Use!!!
	inline void      impl_needCalcInnerSize_parent(){flags.needCalcInnerSize = true; if(parent == this)return; parent->impl_needCalcInnerSize_parent(); }
	inline void      impl_needCalcAlinment_parent() {flags.needApplyAlignment = true; if(parent == this)return; parent->impl_needCalcAlinment_parent(); }

	inline void needCalcAlignment__recursize_child(){
		flags.needApplyAlignment = true;
        for(int i=0; i<widgets.size(); i++) {widgets[i]->needCalcAlignment__recursize_child();}
	}

	// flags settings
	// inline void     setVisivle         (bool  v)     { flags.Visible   = v;         needRendering(); }
	inline void     setActive          (bool  v)      { flags.Active = v;         needRendering(true); for(int i=0; i<widgets.size();i++){ widgets[i]->setActive(v); }      }
	inline void     setEnableScroll_X  (bool  v)      { flags.EnableScroll_X = v; needRendering(true); }
	inline void     setEnableScroll_Y  (bool  v)      { flags.EnableScroll_Y = v; needRendering(true); }
	inline void     setEnableUserInput (bool  v)      { if(!v){return;}  flags.EnableKeyboardInputs = 1; flags.EnableMouseInputs = 1; }
	inline void     setEnableUserResize(bool  v)      { flags.EnableUserResize = v; }
	inline void     setEnablAutoResize (bool  v)      { flags.EnableAutoExpandX = v; flags.EnableAutoExpandY = v; }
	inline void     CollapseTitlebar   (bool  c)      { flags.CollapsingTitlebar = c; for(int i=0; i<widgets.size();i++){ widgets[i]->setActive(!c); }}
	inline void     setHoveringFlag    (bool  v)      { flags.isHovering  = v; needRendering(true); CallbackFunc((v ? uiCallbackFlags::OnHover : uiCallbackFlags::OffHover), Vector2d(0, 0), 0, 0, nullptr);}
	inline void     setFocusedFlag     (bool  v)      { flags.isFocused   = v; needRendering(true); }
	inline void     setAlignType(uiWidgetAlignTypes v){ align_type = v; needRendering(true); needApplyAlignment();}
	inline uiWidgetAlignTypes     getAlignType()      { return align_type;}
	inline bool     getEnableAutoExpandX()            { return flags.EnableAutoExpandX > 0; }
	inline bool     getEnableAutoExpandY()            { return flags.EnableAutoExpandY > 0; }
	inline bool     getEnableAutoShrinkX()            { return flags.EnableAutoShrinkX > 0; }
	inline bool     getEnableAutoShrinkY()            { return flags.EnableAutoShrinkY > 0; }
	inline bool     isActive            ()            { return flags.Active; }
	
	// 子Widget全てをUserリサイズ不可にする
	inline void     setChildEnablAutoResize (bool  v) { 
		if(!v) return;
		for(int i=0; i<widgets.size(); i++){
			widgets[i]->setChildEnablAutoResize(v);
		}
	}

	// inline Vector2d getMinSize() { 
	// 	Vector2d tmp__ = innerSize; 
	// 	// if(widgets.size() == 0){
	// 		auto style = getStyle(); 
	// 		if(flags.EnableAutoShrinkX){tmp__.x = std::min<float>(tmp__.x, style->WidgetMinSize_x);} 
	// 		if(flags.EnableAutoShrinkY){tmp__.y = std::min<float>(tmp__.x, style->WidgetMinSize_y);}
	// 	// }else{
	// 	//   setAlignment(align_type);
	// 	// }
	// 	return tmp__;
	// }
	// inline Vector2d getMaxSize() { 
	// 	Vector2d tmp__ = innerSize; 
	// 	if(flags.EnableAutoExpandX){tmp__.x = (uint16_t)-1;}
	// 	if(flags.EnableAutoExpandY){tmp__.y = (uint16_t)-1;}
	// 	return tmp__;
	// }


	inline bool AddWidget(uihWidget widget_){
		 if (widget_ == nullptr) return false;
		 if(!flags.EnableChildWidget){ std::cerr << "!flags.EnableChildWidget @ AddWidget Failed!!!;" << std::endl; return false;}
		 widgets.push_back(widget_);
		 return true;
	}


	inline bool setParamFloat (std::string name, float       val){ if((params.count(name) == 0) || std::holds_alternative<float      >(params[name])){ params[name] = val; needRendering(true); needApplyAlignment();needCalcInnerSize(); return true;}  AbortMessage("uiWidget::setParam(std::string " + name + ", float ) failed!!"); return false;  }
	inline bool setParamBool  (std::string name, bool        val){ if((params.count(name) == 0) || std::holds_alternative<bool       >(params[name])){ params[name] = val; needRendering(true); needApplyAlignment();needCalcInnerSize(); return true;}  AbortMessage("uiWidget::setParam(std::string " + name + ", bool ) failed!!"); return false;  }
	inline bool setParamInt   (std::string name, uint64      val){ if((params.count(name) == 0) || std::holds_alternative<uint64     >(params[name])){ params[name] = val; needRendering(true); needApplyAlignment();needCalcInnerSize(); return true;}  AbortMessage("uiWidget::setParam(std::string " + name + ", uint64 ) failed!!"); return false;  }
	inline bool setParamString(std::string name, std::string val){ if((params.count(name) == 0) || std::holds_alternative<std::string>(params[name])){ params[name] = val; needRendering(true); needApplyAlignment();needCalcInnerSize(); return true;}  AbortMessage("uiWidget::setParam(std::string " + name + ", std::string ) failed!!"); return false;  }
		
	inline bool getFloatParam  (std::string name, float       *val){ if((params.count(name) > 0) && std::holds_alternative<float      >(params[name])){ *val = std::get<float      >(params[name]); return true;} AbortMessage("uiWidget::setParam() failed!!");return false; }
	inline bool getBoolParams  (std::string name, bool        *val){ if((params.count(name) > 0) && std::holds_alternative<bool       >(params[name])){ *val = std::get<bool       >(params[name]); return true;} AbortMessage("uiWidget::setParam() failed!!");return false; }
	inline bool getIntParams   (std::string name, uint64      *val){ if((params.count(name) > 0) && std::holds_alternative<bool       >(params[name])){ *val = std::get<uint64     >(params[name]); return true;} AbortMessage("uiWidget::setParam() failed!!");return false; }
	inline bool getStringParams(std::string name, std::string *val){ if((params.count(name) > 0) && std::holds_alternative<std::string>(params[name])){ *val = std::get<std::string>(params[name]); return true;} AbortMessage("uiWidget::setParam() failed!!");return false; }
	
	inline float       getFloatParam(  std::string name){ if((params.count(name) > 0) && (std::holds_alternative<float      >(params[name]))){ return std::get<float      >(params[name]); } AbortMessage("float uiWidget::setParam(" + name + ") failed!!"); return -1; }
	inline uint64      getIntParam(    std::string name){ if((params.count(name) > 0) && (std::holds_alternative<uint64     >(params[name]))){ return std::get<uint64     >(params[name]); } AbortMessage("float uiWidget::setParam(" + name + ") failed!!"); return 0; }
	inline bool        getBoolParams(  std::string name){ if((params.count(name) > 0) && (std::holds_alternative<bool       >(params[name]))){ return std::get<bool       >(params[name]); } AbortMessage("bool uiWidget::setParam(" + name + ") failed!!"); return false; }
	inline std::string getStringParams(std::string name){ if((params.count(name) > 0) && (std::holds_alternative<std::string>(params[name]))){ return std::get<std::string>(params[name]); } AbortMessage("std::string uiWidget::setParam(" + name + ") failed!!"); return ""; }

	inline void __showAllParams(){
		for (auto itr = params.begin(); itr != params.end(); ++itr){
			std::cout << "key = " << itr->first << ", val = "; // 値を表示
			auto val = itr->second;
			if (std::holds_alternative<std::string>(val)) std::cout << std::get<std::string>(val) << std::endl;
			if (std::holds_alternative<float      >(val)) std::cout << std::get<float      >(val) << std::endl;
			if (std::holds_alternative<uint64     >(val)) std::cout << std::get<uint64     >(val) << std::endl;
			if (std::holds_alternative<bool       >(val)) std::cout << std::get<bool       >(val) << std::endl;
		}
	}
	// set widget flags
};




// -----------------------------------------------------
//    [SECTION] Window
// -----------------------------------------------------
// Flags for ImGui::Begin()
// enum class uiWindowFlags_: uint16_t{
//     NoScroll                 = 1 << 0,   // Enable scrollbars (overflow:hidden;)
// 	FullWindow               = 1 << 1,   // フルサイズで表示
//     UserResize               = 1 << 2,   // Enable user resizing
//     AutoResize               = 1 << 3,   // Widgetの大きさに応じて自動的にリサイズする
//     EnableTitleBar           = 1 << 4,   // Enable title bar
// 	EnableBackground         = 1 << 5,
// 	EnableScrollbarAlways    = 1 << 6,   // Always show scrollbar (even if ContentSize.y < Size.y)
//     EnableMouseInputs        = 1 << 7,   // Enable mouse callback(mouse Click or mouse Move)
// 	EnableKeyboardInputs     = 1 << 8,   // Enable keyboard callback
// 	EnableDragDropInputs     = 1 << 9,   // Enable DragDrop
// 	EnableChildWindow        = 1 << 10,  // Enable child window and show child windows
// 	EnablePopups             = 1 << 11,   // Enable popup Window
//     // [Internal] Do not use!!!!
// 	// needRendering            = 1 << 12
// };


class uiWindow final: public uiWidget{
private:
	struct _wndStyle{
        unsigned char EnableTitleBar   : 1; //Enable title bar
	    unsigned char EnableChildWindow: 1; // Enable child window and show child windows
	    unsigned char EnablePopups     : 1;  // Enable popup Window
		unsigned char isFullScreen     : 1;
	}wndStyle;

	GLFWwindow *glfw_window;
	Vector2d wndPos; // uiWidget::posは、calcAlignmentで加算されて使われているので、Windowのposは常に(0, 0)になっている。
	//その代わりに、モニター内でのWindowの位置を示す変数としてwndPosがある

	// std::string name;
	// char _needRenderAll;
	// uint16_t flags;

	uiWidget *HoveringWidget;
	uiWidget *FocusedWidget;

	uiDrawList DrawLists[2]; //0 = main, 1=forwardのDrawList、CurrentWindowに設定されていればそこからアクセスできる

	void calcHoveringWidget(int x, int y);

    static uiWindow* getThisPtr   (GLFWwindow *window)                                              { return static_cast<uiWindow*>(glfwGetWindowUserPointer(window));}
    static void      mouseButtonCB(GLFWwindow *window, int button, int action, int mods)            { 
		if (action == GLFW_RELEASE){
			switch(button){
				case GLFW_MOUSE_BUTTON_RIGHT:  getThisPtr(window)->CallbackFunc(uiCallbackFlags::RMouseUP, Vector2d(0, 0), 0,0, nullptr);break;
				case GLFW_MOUSE_BUTTON_LEFT :  getThisPtr(window)->CallbackFunc(uiCallbackFlags::LMouseUP, Vector2d(0, 0), 0,0, nullptr);break;
				case GLFW_MOUSE_BUTTON_MIDDLE: getThisPtr(window)->CallbackFunc(uiCallbackFlags::CMouseUP, Vector2d(0, 0), 0,0, nullptr);break;
				default : MY_ASSERT("undefinded mouse button!\n");
			}
		}else{
			switch(button){
				case GLFW_MOUSE_BUTTON_RIGHT:  getThisPtr(window)->CallbackFunc(uiCallbackFlags::RMouseDown, Vector2d(0, 0), 0,0, nullptr);break;
				case GLFW_MOUSE_BUTTON_LEFT :  getThisPtr(window)->CallbackFunc(uiCallbackFlags::LMouseDown, Vector2d(0, 0), 0,0, nullptr);break;
				case GLFW_MOUSE_BUTTON_MIDDLE: getThisPtr(window)->CallbackFunc(uiCallbackFlags::CMouseDown, Vector2d(0, 0), 0,0, nullptr);break;
				default : MY_ASSERT("undefinded mouse button!\n");
				}
		}	
	}
    static void      windowSizeCB (GLFWwindow *window, int width, int height)                       { getThisPtr(window)->CallbackFunc(uiCallbackFlags::ResizeTo, Vector2d(width, height), 0,0,nullptr);  }
    static void      mousePosCB   (GLFWwindow *window, double x, double y)                          { 
		int state = 0;
		state |=  glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)  == GLFW_PRESS;
        state |= (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) << 1;
        state |= (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) << 2;
		getThisPtr(window)->CallbackFunc(uiCallbackFlags::MouseMove,   Vector2d(x, y), 0, state, nullptr); 
	}
    static void      mouseScrollCB(GLFWwindow *window, double x, double y)                          { getThisPtr(window)->CallbackFunc(uiCallbackFlags::MouseScroll, Vector2d(0, 0), x, y, nullptr); }
    static void      keyFunCB     (GLFWwindow* window, int key, int scancode, int action, int mods) { getThisPtr(window)->CallbackFunc(uiCallbackFlags::Keyboard,    Vector2d(0, 0), key, action, nullptr); }
    static void      charFunCB    (GLFWwindow* window, unsigned int charInfo)                       { getThisPtr(window)->CallbackFunc(uiCallbackFlags::CharInput,   Vector2d(0, 0), charInfo, 0, nullptr);  }
    static void      dropCB       (GLFWwindow *window, int num, const char **paths)                 { getThisPtr(window)->CallbackFunc(uiCallbackFlags::DragDrop,    Vector2d(0, 0), num, 0, paths); } 


	inline void setFocusedWidget(uihWidget w){

	}

	//関数隠ぺい
	using uiWidget::setSize;
	using uiWidget::setPos;

public:
	uiWindow(const char *name_, const Vector2d size_);
	~uiWindow();

	void setFullScreen(bool fullscreen);
	void render() override;
	bool CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings) override;
	inline uiDrawList   *getDLstack(){  return DrawLists;    }
	inline GLFWwindow   *getGLFWwindow(){  return glfw_window; }

	inline void setWindowSize(int x, int y){
		if(!glfw_window) std::cerr << "No window error!" << std::endl;
		glfwSetWindowSize(glfw_window, x, y);
		int _x,_y;
		glfwGetWindowSize(glfw_window, &_x, &_y );
		uiWidget::setSize(Vector2d(_x, _y));
	}

	inline void setWindowPos(int x, int y){
		if(glfw_window) std::cerr << "No window error!" << std::endl;
		glfwSetWindowPos(glfw_window, x, y);
		
		int _x,_y;
		glfwGetWindowPos(glfw_window, &_x, &_y );
		uiWidget::setPos(Vector2d(x, y));
	}
};


// -----------------------------------------------------
//    [SECTION] Main (Context)
// -----------------------------------------------------
struct uiContext{
	uiStyle   style;    // window style and color settings

	uiVector<uiWindow*> windows;

	uiFont    text_renderer;
	uiTexture texture_renderer;
	uiShader  shader;
	bool      isSetupFinished;

	uiWindow *RenderingWindow;
	uiWindow *CurrentWindow;
	uiWidget *CurrentWidget;
	uiWidget *LastHoverWidget;

	struct cursors_{
		GLFWcursor* Arrow;
		GLFWcursor* Ibeam; //テキスト入力中のカーソル
		GLFWcursor* CrossHair; // 十字の線。
		GLFWcursor* Hand;
		GLFWcursor* HResize;
		GLFWcursor* VResize;
		GLFWcursor* VHResize; // 左上-右下のリサイズカーソル
		// GLFWcursor* VHResize_2;
	}cursors;

	double  time;
	int     fps;

	void setupCursors(){
	    cursors.Arrow = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    	cursors.CrossHair = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    	cursors.Hand = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    	cursors.HResize = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    	cursors.VResize = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    	// cursors.VHResize = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);

		if(cursors.Arrow == NULL) LOGE << "failed to create arrow cursor";
		if(cursors.CrossHair == NULL) LOGE << "failed to create CrossHair cursor";
		if(cursors.Hand == NULL) LOGE << "failed to create Hand cursor";
		if(cursors.HResize == NULL) LOGE << "failed to create HResize cursor";
		if(cursors.VResize == NULL) LOGE << "failed to create VResize cursor";
		std::cout << "cursor create finished\n";
	}
};


} //namespace glUI



