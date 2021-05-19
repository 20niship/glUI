/**
 * @file glUI.cpp
 * @brief glUIについての基本的な関数群。
 * @author 20Niship Nishimiya Tadashi
 * @date 2021-01-17
 * @details 詳細な説明
 */

#include "glUI.hpp"
#include "widget.hpp"
#include "logger.hpp"

namespace glUI{

std::ostream& operator<<(std::ostream& os, const Vector2f& v){
    os << "(" << v.x << ", " << v.y <<")";
    return os;
}



void setDefaultStyle(uiStyle *style){
    style->WindowPadding_x = 5.0f;
    style->WindowPadding_y=5.0f;
    style->WindowBorderSize=5.0f;
    style->WindowMinSize_x=50.0f;
    style->WindowMinSize_y=50.0f;
    style->WindowTitleAlign_x=5.0f;
    style->WindowTitleAlign_y=5.0f;
    style->WindowMenuButtonPosition=5.0f;
    style->ScrollbarWidth=20.0f;
    style->ScrollbarRounding=5.0f;

    style->WidgetBorderSize=5.0f;
    style->WidgetOuterSpace_x=5.0f;
    style->WidgetOuterSpace_y=5.0f;
    style->WidgetInnerSpace_x=5.0f;
    style->WidgetInnerSpace_y=5.0f;
    
	style->WidgetMinSize_x = 30;
	style->WidgetMinSize_y = 30;
    style->IndentSpacing=5.0f;
    style->ColumnsMinSpacing=5.0f;
    style->MouseCursorScale=5.0f;

    style->FontSize=16.0f;
    style->FontName="Meiryo.ttf";
	style->textlign = TextAlignPos::center;
	style->TextOverflowHidden = true;
	style->TextAutoSpacing = true;
    style->TextSpacing = 2.0f;

    style->TitlebarHeight = style->FontSize + style->WidgetInnerSpace_y*2 + 5;

    style->col_Text            = Vector3f(1.0f, 1.0f, 1.0f);
    style->col_TextDisabled    = Vector3f(0.55, 0.55, 0.55);
    style->col_TextHovered     = Vector3f(1.0f, 1.0f, 1.0f);
    style->col_TextNoEmphasize = Vector3f(1.0f, 1.0f, 1.0f);
    style->col_WndBg           = Vector3f(0.17, 0.17, 0.17);
    style->col_wndLine         = Vector3f(0.40, 0.40, 0.40);
    style->col_wndScroll       = Vector3f(0.42, 0.42, 0.42);
    style->col_wndScrollBg     = Vector3f(0.80, 0.60, 0.15);
    style->col_PopupBg         = Vector3f(0.0f, 0.0f, 0.0f);
    style->col_PopupLine       = Vector3f(1.0f, 1.0f, 1.0f);

    style->col_WidgetBg           = Vector3f(0.09, 0.09, 0.09);
    style->col_WidgetBgHover      = Vector3f(0.4f, 0.5f, 0.8f);
    style->col_WidgetBgSelected   = Vector3f(0.16, 0.16, 0.30);
    style->col_WidgetBgDisabled   = Vector3f(0.3f, 0.2f, 0.1f);
    style->col_WidgetBgMain       = Vector3f(0.09, 0.09, 0.09);
    style->col_WidgetLine         = Vector3f(0.40, 0.40, 0.40);
    style->col_WidgetLineHover    = Vector3f(1.0f, 1.0f, 1.0f);
    style->col_WidgetLineSelected = Vector3f(0.16, 0.67, 0.95);
    style->col_WidgetLineDisabled = Vector3f(0.7f, 0.7f, 0.7f);
}
void setDrakMode(uiStyle *style){}


uiContext _ctx;
uiContext* getContext(){ return &_ctx;}
uiStyle   *getStyle  (){ return &(_ctx.style);}
uiDrawList *getDrawlistStack(){ MY_ASSERT(_ctx.RenderingWindow); if(!_ctx.RenderingWindow){return nullptr;} return _ctx.RenderingWindow->getDLstack(); }
void setRenderingWindow(uiWindow *wnd){ _ctx.RenderingWindow = wnd; }
uihWnd getRenderingWindow(){ return _ctx.RenderingWindow; }
uiFont *getTextRenderer(){ return &(_ctx.text_renderer); }
uiShader *getShaderObject(){ return &(_ctx.shader); }
uiTexture *getTextureRenderer(){ return &(_ctx.texture_renderer);}


// -----------------------------------------------------
//    [SECTION] Widgets
// -----------------------------------------------------

/**
 * Rules
 *   - setAlignhogehoge関数内で、自身のWidgetのサイズを変更してはいけない
 * 　- 1. アラインメントを計算する（初期サイズの決定と、カーソルでサイズ変更するのがOKかを確認する
 *   - 2. 1.で計算したアライメントをVectorで保存
 *   - 3. その値をもとに子WidgetのsetSize()とsetWidth()を行う
 *       - setSizeが呼ばれたからとってリサイズできたわけではない。リサイズできたかどうかはbool値で返される。
 *       - 子Widgetがある場合、sethogehogeのisTryをTrueにして実行
 * 
 * 方法1. Widgetの端っこにリサイズイベントを設定する
 * 　　問題点：1.Widgetが一つの時に内側しか反応しない。2.サイズ変更の処理が面倒（parent widgetにアクセスする必要がある）
 * 方法2. Widgetの間にリサイズイベントを設定する←〇
 * 　　Callback関数で、子WidgetのHoverを調べる前に、エッジ上にマウスがいるかどうかを判定
 * 
 * Width：
 * 　1. max(childWidgets.innerSize.x) <= size.x : 
 *     1-1. setWidth()を行う。実際に拡大できたかどうかは確かめる必要が無い（setSizeの中で実装される）
 * 　2. childWidgets.innerSize.x > size.xのChildWidgetについて
 *     2-1. child.flag.expandXが可能ならば、setSize()を出してできるだけ小さくしてみる。
 *     2-2. setSizeで予定通りのサイズに変更できた場合は、それでOk
 *     2-3. setSizeが失敗し、まだchildの方が大きい場合、 this->scrollXが許可されていればスクロールバーを表示
 *     2-4. 許可されていなければoverflow:hiddenで。DrawListのsetClipRect()を実行する
 * Height:
 *   1. 普通にすべて足す, この時に、Y軸についてResize可能なWidgetの個数を確認する
 *   2. 合計が元のサイズ以上（desiredWholeSize.y > InnerSize.y）の時、リサイズ
 * 
 */

[[deprecated]] void uiWidget::calcAlignVertial(bool spacing){
    if(widgets.size() == 0) return; 
    auto style = getStyle();
    
	uint16_t titlebar_y = 0;
	if(flags.EnableTitlebar) titlebar_y  = style->FontSize + style->WidgetInnerSpace_y*2 + 4;
    
    Vector2d outerSpace;
    outerSpace.x = spacing ? style->WidgetOuterSpace_x : 0; 
    outerSpace.y = spacing ? style->WidgetOuterSpace_y : 0; 

    uint16_t desiredHeight = 0;
    uint16_t desiredWidth = 0;
    uint16_t numExHeightWidgets = 0;
    uint16_t numShHeightWidgets = 0;
    Vector2d tmpMinS, tmpMaxS, tmpCurS;


    bool EnScrollX = false;
    bool EnScrollY = false; // thisウィジェットのスクロールバーを表示するか（右と下にスペースを作るか
    for(uint16_t i = 0; i<widgets.size(); i++){
        if(widgets[i]->getEnableAutoExpandY()) numExHeightWidgets ++;
        if(widgets[i]->getEnableAutoShrinkY()) numShHeightWidgets ++;

        EnScrollX |= !(widgets[i]->getEnableAutoExpandX());
        EnScrollY |= !(widgets[i]->getEnableAutoExpandY());
    }

    for(uint16_t i =0; i< widgets.size(); i++){
        tmpCurS = widgets[i]->getInnerSize(); //getSize();
        // std::cout << "render size = (" << tmpCurS.x << ", " << tmpCurS.y << ")\n";

        //WIDTH SETTINGS:
        if(size.x - outerSpace.x -  (EnScrollX ? (style->ScrollbarWidth) : 0) >= tmpCurS.x + outerSpace.x*2){
            if(widgets[i]->getEnableAutoExpandX()){ widgets[i]->setWidth(size.x - outerSpace.x - (EnScrollX ? (style->ScrollbarWidth) : 0)); }else{ widgets[i]->setWidth(tmpCurS.x); }
        }else{
            if(widgets[i]->getEnableAutoShrinkX()){ widgets[i]->setWidth(size.x - outerSpace.x - (EnScrollX ? (style->ScrollbarWidth) : 0)); }else{ widgets[i]->setWidth(tmpCurS.x); }
        }

        //HEIGHT SETTINGS:
        desiredHeight += tmpCurS.y + outerSpace.y*2;
    }

    // 高さの調整（各Widgetごとに拡大縮小
    if(desiredHeight < size.y -  (EnScrollY ? (style->ScrollbarWidth) : 0) - titlebar_y){
        if(numExHeightWidgets > 0){
            int expand_each = (size.y - desiredHeight - (EnScrollY ? (style->ScrollbarWidth) : 0) - titlebar_y) / numExHeightWidgets; //拡大できるサイズ
            for(int i=0; i<widgets.size(); i++){
                if(widgets[i]->getEnableAutoExpandY()){ widgets[i]->setHeight(std::min<int>(widgets[i]->getInnerSize().y + expand_each, size.y- (EnScrollY ? (style->ScrollbarWidth) : 0) - titlebar_y)); }
                else                                  { widgets[i]->setHeight(std::min<int>(widgets[i]->getInnerSize().y, size.y - (EnScrollY ? (style->ScrollbarWidth) : 0) - titlebar_y)); }
            }
        }else{
            for(int i=0; i<widgets.size(); i++) widgets[i]->setHeight(widgets[i]->getInnerSize().y);
        }
    }else{
        if(numShHeightWidgets > 0){
            int shrink_each =  (desiredHeight + (EnScrollY ? (style->ScrollbarWidth) : 0) + titlebar_y - size.y) / numShHeightWidgets;
            for(int i=0; i<widgets.size(); i++){
                if(widgets[i]->getEnableAutoShrinkY()){ widgets[i]->setHeight(std::max<int>(int(widgets[i]->getInnerSize().y) - int(shrink_each), style->WidgetMinSize_y)); }
                else                                  { widgets[i]->setHeight(widgets[i]->getInnerSize().y); }
            }
        }else{
            for(int i=0; i<widgets.size(); i++) widgets[i]->setHeight(widgets[i]->getInnerSize().y);
        }
    }

    // それぞれのサイズの確認と表示位置の設定
    desiredWidth  = 0; desiredHeight  = 0;
    for(int i = 0; i<widgets.size(); i++){
        tmpCurS = widgets[i]->getSize();
    
        widgets[i]->setPos(Vector2d(pos.x + outerSpace.x - scrollPos.x, pos.y + titlebar_y + desiredHeight - scrollPos.y));
        desiredWidth = std::max<int>(desiredWidth, tmpCurS.x);
        desiredHeight += tmpCurS.y + outerSpace.y*2;
    }

    // setSize(desiredWholeSize, -1)
    innerSize.x = desiredWidth;
    innerSize.y = desiredHeight + titlebar_y;
    needRendering(true);
    flags.needApplyAlignment = 0;
    flags.needCalcInnerSize = 0;
}



[[deprecated]] void uiWidget::calcAlignHorizontal(bool spacing){
    if(widgets.size() == 0) { flags.needApplyAlignment = false; flags.needCalcInnerSize = false; return; }
    auto style = getStyle();
    
	uint16_t titlebar_y = 0;
	if(flags.EnableTitlebar) titlebar_y  = style->FontSize + style->WidgetInnerSpace_y*2 + 4;
    
    Vector2d outerSpace;
    outerSpace.x = spacing ? (style->WidgetOuterSpace_x) : 0; 
    outerSpace.y = spacing ? (style->WidgetOuterSpace_y) : 0; 

    uint16_t desiredWidth = 0;
    uint16_t desiredHeight = 0;
    uint16_t numExWidthWidgets = 0;
    uint16_t numShWidthWidgets = 0;
    Vector2d tmpMinS, tmpMaxS, tmpCurS;

    bool EnScrollX = false;
    bool EnScrollY = false; // thisウィジェットのスクロールバーを表示するか（右と下にスペースを作るか
    for(uint16_t i = 0; i<widgets.size(); i++){
        if(widgets[i]->getEnableAutoExpandX()) numExWidthWidgets ++;
        if(widgets[i]->getEnableAutoShrinkX()) numShWidthWidgets ++;

        EnScrollX |= !(widgets[i]->getEnableAutoExpandX());
        EnScrollY |= !(widgets[i]->getEnableAutoExpandY());
    }
 
    for(uint16_t i =0; i< widgets.size(); i++){
        tmpCurS = widgets[i]->getInnerSize(); //getSize();
        std::cout << "render size = (" << tmpCurS.x << ", " << tmpCurS.y << ")\n";

        //HEIGHT SETTINGS:
        if(size.y - outerSpace.y -  (EnScrollY ? (style->ScrollbarWidth) : 0) >= tmpCurS.y + outerSpace.y*2){
            if(widgets[i]->getEnableAutoExpandY()){ widgets[i]->setHeight(size.y - outerSpace.y - (EnScrollY ? (style->ScrollbarWidth) : 0) - titlebar_y); }else{ widgets[i]->setWidth(tmpCurS.y); }
        }else{
            if(widgets[i]->getEnableAutoShrinkY()){ widgets[i]->setHeight(size.y - outerSpace.y - (EnScrollY ? (style->ScrollbarWidth) : 0) - titlebar_y); }else{ widgets[i]->setWidth(tmpCurS.y); }
        }

        //HEIGHT SETTINGS:
        desiredWidth += tmpCurS.x + outerSpace.y*2;
    }

    // 高さの調整（各Widgetごとに拡大縮小
    if(desiredWidth < size.x -  (EnScrollX ? (style->ScrollbarWidth) : 0)){
        if(numExWidthWidgets > 0){
            int expand_each = (size.x - desiredWidth - (EnScrollX ? (style->ScrollbarWidth) : 0)) / numExWidthWidgets; //拡大できるサイズ
            for(int i=0; i<widgets.size(); i++){
                if(widgets[i]->getEnableAutoExpandY()){ widgets[i]->setWidth(std::min<int>(widgets[i]->getInnerSize().x + expand_each, size.x - (EnScrollX ? (style->ScrollbarWidth) : 0) )); }
                else                                  { widgets[i]->setWidth(std::min<int>(widgets[i]->getInnerSize().x,               size.x - (EnScrollX ? (style->ScrollbarWidth) : 0) )); }
            }
        }else{
            for(int i=0; i<widgets.size(); i++) widgets[i]->setHeight(widgets[i]->getInnerSize().x);
        }
    }else{
        if(numShWidthWidgets > 0){
            int shrink_each =  (desiredWidth + (EnScrollX ? (style->ScrollbarWidth) : 0) - size.x) / numShWidthWidgets;
            for(int i=0; i<widgets.size(); i++){
                if(widgets[i]->getEnableAutoShrinkY()){ widgets[i]->setWidth(std::max<int>(int(widgets[i]->getInnerSize().x) - int(shrink_each), style->WidgetMinSize_y)); }
                else                                  { widgets[i]->setWidth(widgets[i]->getInnerSize().x); }
            }
        }else{
            for(int i=0; i<widgets.size(); i++) widgets[i]->setWidth(widgets[i]->getInnerSize().x);
        }
    }

    // それぞれのサイズの確認と表示位置の設定
    desiredWidth  = 0; desiredWidth  = 0;
    for(int i = 0; i<widgets.size(); i++){
        tmpCurS = widgets[i]->getSize();
        widgets[i]->setPos(Vector2d(pos.x + outerSpace.x - scrollPos.x + desiredWidth, pos.y + titlebar_y));
        std::cout << "position : " << widgets[i]->getPosX() << ", " << widgets[i]->getPosY() << std::endl;
        desiredWidth += tmpCurS.x + outerSpace.y*2;
        desiredHeight = std::max<int>(desiredHeight, tmpCurS.x);
    }

    innerSize.y = desiredHeight + titlebar_y;
    innerSize.x = desiredWidth;
    needRendering(true);
    flags.needApplyAlignment = 0;
    flags.needCalcInnerSize = 0;
}




/**
 * 現在リサイズしようとしているWidgetの前後のWidgetをWA、WBとする
 * リサイズ可能な条件
 *  - 両方のWidgetが伸縮
 */
bool uiWidget::tryResizeAlignment(uint8_t index, bool axis_x, int new_pos){
    if(align_type == uiWidgetAlignTypes::Horizontel_noSpace || 
       align_type == uiWidgetAlignTypes::Vertical_noSpace) return false;

    if(align_type == uiWidgetAlignTypes::HorizontalList && index >= childSizelist_x.size()) return false;
    if(align_type == uiWidgetAlignTypes::VertialListl   && index >= childSizelist_y.size()) return false;
    
    std::cout << "tryResizeAlignment start...\n";
    disp(axis_x);
    if(axis_x){
        std::cout << "childSizelist_x = ";
        for(int i=0; i<childSizelist_x.size(); i++)std::cout << childSizelist_x[i] << ", ";
        std::cout << "\n";


        bool bigger = new_pos > childSizelist_x[index];
        disp(bigger);
        if(bigger){
            bool OK_left = (
                (index == 0) ||
                (widgets[index-1]->getEnableAutoShrinkX() && (new_pos - childSizelist_x[index-1]) <= widgets[index-1]->getInnerWidth()) || 
                (widgets[index-1]->getEnableAutoExpandX() && (new_pos - childSizelist_x[index-1]) >= widgets[index-1]->getInnerWidth()));
                
            bool OK_right = (
                (index == childSizelist_x.size()-1) ||
                (widgets[index-1]->getEnableAutoShrinkX() && (childSizelist_x[index] - new_pos) <= widgets[index]->getInnerWidth()) || 
                (widgets[index-1]->getEnableAutoExpandX() && (childSizelist_x[index] - new_pos) >= widgets[index]->getInnerWidth()));
              
            if(OK_left && OK_right){
                childSizelist_x[index] = new_pos;
                return true;
            }
        }else{

        }
    }else{
        std::cout << "childSizelist_y = ";
        for(int i=0; i<childSizelist_y.size(); i++)std::cout << childSizelist_y[i] << ", ";
        std::cout << "\n";

        bool OK_upper = (
            (index == 0) ||
            (widgets[index-1]->getEnableAutoShrinkY() && (new_pos - childSizelist_y[index-1]) <= widgets[index-1]->getInnerHeight()) || 
            (widgets[index-1]->getEnableAutoExpandY() && (new_pos - childSizelist_y[index-1]) >= widgets[index-1]->getInnerHeight()));
            
        bool OK_lower = (
            (index == childSizelist_y.size()-1) ||
            (widgets[index-1]->getEnableAutoShrinkY() && (childSizelist_y[index] - new_pos) <= widgets[index]->getInnerHeight()) || 
            (widgets[index-1]->getEnableAutoExpandY() && (childSizelist_y[index] - new_pos) >= widgets[index]->getInnerHeight()));
            
        if(OK_upper && OK_lower){
            childSizelist_x[index] = new_pos;
            return true;
        }

    }

    std::cout << "tryResizeAlignment end!!\n";
    needApplyAlignment();
    needRendering(true);

    return true;
}


bool uiWidget::tryResizeAlignment_2(bool axis_x, int old_pos, int new_pos){
    if(axis_x){
        int old_pos_relative = old_pos - pos.x;
        int new_pos_relative = new_pos - pos.x;
        disp(childSizelist_x.size());
        disp(childSizelist_y.size());
        for(int i=0; i<childSizelist_x.size(); i++){
            if(std::abs<int>(childSizelist_x[i] - old_pos_relative) < 5){
                LOGD << "Index found --> " + std::to_string(i);
                tryResizeAlignment(i, true, new_pos_relative);
                return true;
            }
        }
    }else{
        int old_pos_relative = old_pos - pos.y;
        int new_pos_relative = new_pos - pos.y;
        for(int i=0; i<childSizelist_y.size(); i++){
            if(std::abs<int>(childSizelist_y[i] - old_pos_relative) < 5){
                LOGD << "Index found  Y --> " + std::to_string(i);
                tryResizeAlignment(i, false, new_pos_relative);
                return true;
            }
        }
    }
    return false;
}



void uiWidget::initAlignOneDimention(){
   //とりあえず最小値をもとに決定し、伸ばす方針で。
   //値が足りない場合はエラー出力
    if(widgets.size() == 0) { flags.needApplyAlignment = false; flags.needCalcInnerSize = false; return; }

    //TODO とりあえずこういう条件分岐にしてるけど、後でちゃんとしたほうが良さそう、、、、
    auto style = getStyle();
    
    bool isXalign = (align_type == uiWidgetAlignTypes::HorizontalList) || 
                    (align_type == uiWidgetAlignTypes::Horizontel_noSpace);

    bool spacing = (align_type == uiWidgetAlignTypes::HorizontalList) || 
                   (align_type == uiWidgetAlignTypes::VertialListl);
    

    // if(flags.setupFinished && !spacing) return;
    // if(spacing && !flags.needApplyAlignment) return;
    if(flags.setupFinished && !flags.needApplyAlignment)return;
    if(!flags.Active)return;
    if(flags.CollapsingTitlebar && flags.EnableTitlebar)return;


	int16_t titlebar_y = 0;
	if(flags.EnableTitlebar) titlebar_y  = style->FontSize + style->WidgetInnerSpace_y*2 + 4;
    
    uint16_t numExWidthWidgets = 0;
    uiVector<float>tmp_size_list;
    float desiredSize = 0;

    bool EnScrollX = false;
    bool EnScrollY = false; // thisウィジェットのスクロールバーを表示するか（右と下にスペースを作るか
    for(uint16_t i = 0; i<widgets.size(); i++){
        if(isXalign){
            desiredSize  += widgets[i]->getInnerWidth();
            if(widgets[i]->getEnableAutoExpandX()) numExWidthWidgets ++;
        }else{
            desiredSize  += widgets[i]->getInnerHeight();
            if(widgets[i]->getEnableAutoExpandY()) numExWidthWidgets ++;
        }

        EnScrollX |= !(widgets[i]->getEnableAutoExpandX());
        EnScrollY |= !(widgets[i]->getEnableAutoExpandY());
    }

    if(spacing){
        if(isXalign)desiredSize += widgets.size() * style->WidgetOuterSpace_x;
        else desiredSize += widgets.size() * style->WidgetOuterSpace_y;
    }

    float expand_each;
    if(numExWidthWidgets == 0) expand_each = 0; 
    else   expand_each = ((isXalign ? (size.x - EnScrollX * style->ScrollbarWidth) :
                         (size.y - EnScrollX * style->ScrollbarWidth - titlebar_y) ) -  desiredSize) / numExWidthWidgets;
    
    std::cout << "start init Alignment\n";
    if(isXalign) { childSizelist_x.resize(widgets.size()+1); childSizelist_x[0] = 0; }
    else         { childSizelist_y.resize(widgets.size()+1); childSizelist_y[0] = 0; }
    // disp(childSizelist_x.size());
    // disp(childSizelist_y.size());

    float cursor = 0;
    for(int i = 0; i<widgets.size(); i++){
        if((widgets[i]->getEnableAutoExpandY() && isXalign) || 
           (widgets[i]->getEnableAutoExpandY() && !isXalign)) cursor += expand_each;

        if(isXalign) cursor += widgets[i]->getInnerWidth();
        else         cursor += widgets[i]->getInnerHeight();

        if(spacing){
            if(isXalign)cursor += style->WidgetOuterSpace_x * 2;
            else        cursor += style->WidgetOuterSpace_y * 2;
        } 

        if(isXalign) childSizelist_x[i+1] = cursor;
        else         childSizelist_y[i+1] = cursor;
    }

    // std::cout << "####################   Init Allinmengt!\n";
    // disp(isXalign);
    // disp(spacing);
    // for(int i=0; i<childSizelist_x.size(); i++) disp(childSizelist_x[i]);
    // std::cout << "aaaaa!\n";

    // for(int i=0; i<childSizelist_y.size(); i++) disp(childSizelist_y[i]);

    needRendering(true);
    flags.needApplyAlignment = 1;
    flags.needCalcInnerSize = 0;

    flags.setupFinished = 1;

    std::cout << "Finished!\n";
}



void uiWidget::applyAlignOneDimention(){
    //値が足りない場合はエラー出力
    if(!flags.Active)return;
    if(flags.CollapsingTitlebar && flags.EnableTitlebar)return;

    if(widgets.size() == 0) { flags.needApplyAlignment = false; flags.needCalcInnerSize = false; return; }
    auto style = getStyle();
    
    bool isXalign = (align_type == uiWidgetAlignTypes::HorizontalList) || 
                    (align_type == uiWidgetAlignTypes::Horizontel_noSpace);

    bool spacing = (align_type == uiWidgetAlignTypes::HorizontalList) || 
                   (align_type == uiWidgetAlignTypes::VertialListl);
    
    bool EnScrollX = false;
    bool EnScrollY = false; // thisウィジェットのスクロールバーを表示するか（右と下にスペースを作るか
    if(isXalign){
        float inner_size_x_sum = 0;
        for(uint16_t i = 0; i<widgets.size(); i++){
            inner_size_x_sum += widgets[i]->getInnerWidth();
            EnScrollX |= !(widgets[i]->getEnableAutoExpandX());
            EnScrollY |= ((!widgets[i]->getEnableAutoExpandY()) && widgets[i]->getInnerHeight() > size.y);
        }
        if(inner_size_x_sum < size.x) EnScrollX = false;
    }else{
        float inner_size_y_sum = 0;
        for(uint16_t i = 0; i<widgets.size(); i++){
            EnScrollX |= ((!widgets[i]->getEnableAutoExpandX()) && widgets[i]->getInnerWidth() > size.x);
            EnScrollY |= !(widgets[i]->getEnableAutoExpandY());
            inner_size_y_sum += widgets[i]->getInnerHeight();
        }
        if(inner_size_y_sum < size.y) EnScrollY= false;
    }
    
	int16_t titlebar_y = 0;
	if(flags.EnableTitlebar) titlebar_y  = style->FontSize + style->WidgetInnerSpace_y*2 + 4;


    // width
    if(isXalign){
        for(int i=0; i<widgets.size(); i++){
            // std::cout << "size.x, pos.x   = ";
            float SX = childSizelist_x[i+1] - childSizelist_x[i];
            widgets[i]->setWidth(SX);
            widgets[i]->setPosX(pos.x + childSizelist_x[i] - scrollPos.x);
            // std::cout << widgets[i]->getWidth() << ", " << widgets[i]->getPosX() << std::endl;
        }
    }else{
        for(int i=0; i<widgets.size(); i++){
            // std::cout << "size.x, pos.x AA = ";
            bool isInsideBigger = widgets[i]->getInnerWidth() > size.x - EnScrollX*style->ScrollbarWidth;

            if((widgets[i]->getEnableAutoExpandX() &&  isInsideBigger) || 
               (widgets[i]->getEnableAutoShrinkX() && !isInsideBigger)) {
                widgets[i]->setWidth(size.x - EnScrollX*style->ScrollbarWidth);
            }else{
                widgets[i]->setWidth(widgets[i]->getInnerWidth());
            }
            widgets[i]->setPosX(pos.x - scrollPos.x);
            // std::cout << widgets[i]->getWidth() << ", " << widgets[i]->getPosX() << std::endl;
        }
    }

    // height
    if(isXalign){
        for(int i=0; i<widgets.size(); i++){
            bool isInsideBigger = widgets[i]->getInnerHeight() > size.y - EnScrollX*style->ScrollbarWidth;

            if((widgets[i]->getEnableAutoExpandY() &&  isInsideBigger) || 
               (widgets[i]->getEnableAutoShrinkY() && !isInsideBigger)) {
                widgets[i]->setHeight(size.y - EnScrollX*style->ScrollbarWidth);
            }else{
                widgets[i]->setHeight(widgets[i]->getInnerHeight());
            }
            widgets[i]->setPosY(pos.y + titlebar_y - scrollPos.y);
            // std::cout << "size.y, pos.y   = " << widgets[i]->getHeight() <<", " << widgets[i]->getPosY() << std::endl;
        }
    }else{
        for(int i=0; i<widgets.size(); i++){
            float SY = childSizelist_y[i+1] - childSizelist_y[i];
            widgets[i]->setHeight(SY);
            widgets[i]->setPosY(pos.y + childSizelist_y[i] + titlebar_y- scrollPos.y);
            // std::cout << "size.y, pos.y AA = " << widgets[i]->getHeight() <<", " << widgets[i]->getPosY() << std::endl;
        }
    }


    float innerSize_max_x = 0;
    float innerSize_max_y = 0;
    for(int i=0; i<widgets.size(); i++){
        innerSize_max_x = std::max<float>(innerSize_max_x, widgets[i]->getInnerWidth());
        innerSize_max_y = std::max<float>(innerSize_max_y, widgets[i]->getInnerHeight());
    }

    if(isXalign){
        innerSize.x = childSizelist_x.back() + titlebar_y;
        innerSize.y = innerSize_max_y;
    }else{
        innerSize.x = innerSize_max_x;
        innerSize.y = childSizelist_y.back() + titlebar_y;
    }

    needRendering(true);
    flags.needApplyAlignment = 0;
    flags.needCalcInnerSize = 0;
}



void uiWidget::renderScrollbars(){
    auto style = getStyle();
    uiDrawList *DrawList = getDrawlistStack();

    if((lastInnerSize != innerSize) || (scrollPos != lastScrollPos)){ needRendering(true); if(widgets.size() > 0) needApplyAlignment();}

    Vector2f pos_scrollX = Vector2f(pos.x, pos.y + size.y - style->ScrollbarWidth*0.9);
    Vector2f pos_scrollY = Vector2f(pos.x + size.x - style->ScrollbarWidth*0.9, pos.y);

    if(innerSize.y > size.y && flags.needRendering > 0){
        DrawList[0].AddRectangle(pos_scrollY, Vector2f(style->ScrollbarWidth, size.y - style->ScrollbarWidth), -1, style->col_wndScrollBg);
        DrawList[0].AddRectangle(Vector2f(pos_scrollY.x+1, pos.y + scrollPos.y*size.y/innerSize.y), 
                                 Vector2f(style->ScrollbarWidth-1, (size.y - style->ScrollbarWidth)*size.y/innerSize.y), -1, style->col_wndScroll);
    }

    if(innerSize.x > size.x && flags.needRendering > 0){
        DrawList[0].AddRectangle(pos_scrollX, Vector2f(size.x - style->ScrollbarWidth,  style->ScrollbarWidth), -1, style->col_wndScrollBg);
        DrawList[0].AddRectangle(Vector2f(pos.x + scrollPos.x*size.x/innerSize.x, pos_scrollX.y+1), 
                                 Vector2f(size.x*(size.x - style->ScrollbarWidth)/innerSize.x, style->ScrollbarWidth-1), -1, style->col_wndScroll);
    }

    lastInnerSize = innerSize;
    lastScrollPos = scrollPos;
}




void uiWidget::CallbackScrollbars(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){
    auto style = getStyle();
    bool isContains;

    // Vertical
    if(innerSize.y >= size.y){
        isContains = (pos.x + size.x - style->ScrollbarWidth*0.9 <= vec2_1.x) && (vec2_1.x <= pos.x + size.x) && (pos.y <= vec2_1.y) && (vec2_1.y < pos.y + size.y);
        if(((flag ==uiCallbackFlags::LMouseDown) && isContains) || isSelectingScrollY){ 
            int tmp_sy =  (float(vec2_1.y) - float(pos.y) - float((size.y - style->ScrollbarWidth)*size.y)/(innerSize.y*2.0))*innerSize.y/size.y;
            isSelectingScrollY = true;
            scrollPos.y = std::clamp<int>(tmp_sy, 0, int(innerSize.y - size.y));
            needRendering(true);
            // needApplyAlignment();
        }
        if(flag ==uiCallbackFlags::LMouseUP) isSelectingScrollY = false; 
        if(flag ==uiCallbackFlags::MouseScroll && num_2 != 0){
            scrollPos.y = std::clamp<int>(int(scrollPos.y) - num_2*15, 0, innerSize.y - size.y);
            // needApplyAlignment();
            needRendering(true);
        }
    }else{
        if(flag ==uiCallbackFlags::MouseScroll && num_2 != 0){
            if(parent != this) parent->CallbackScrollbars(flag, vec2_1, num_1, num_2, strings);
        }
    }


    if(innerSize.x >= size.x){
        //Horizontal
        isContains = (pos.x <= vec2_1.x) && (vec2_1.x <= pos.x + size.x) && (pos.y + size.y - style->ScrollbarWidth*0.9 <= vec2_1.y) && (vec2_1.y < pos.y + size.y);
        if(((flag ==uiCallbackFlags::LMouseDown) && isContains) || isSelectingScrollX){
            int tmp_sy =  (float(vec2_1.x) - float(pos.x) - float(size.x*(size.x - style->ScrollbarWidth))/(innerSize.x*2.0))* innerSize.x/size.x ;
            isSelectingScrollX = true;
            scrollPos.x  = std::clamp<int>(tmp_sy, 0, innerSize.x - size.x);
            needRendering(true);
        }
        if(flag ==uiCallbackFlags::LMouseUP) isSelectingScrollX = false;
    }else{
        if(flag ==uiCallbackFlags::MouseScroll && num_1 != 0){
            if(parent != this) parent->CallbackScrollbars(flag, vec2_1, num_1, num_2, strings);
        }
    }
}


void uiWidget::CallbackTitlebar(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){
	if(flag != uiCallbackFlags::LMouseDown) return;
    if(!containsTitlebar(vec2_1.x, vec2_1.y)) return;
    if(!flags.EnableTitlebar) return;

    CollapseTitlebar(!flags.CollapsingTitlebar);
    parent->needRendering(true);
    parent->needApplyAlignment();
    needCalcInnerSize(); //TODO:Frame in Frameの設計がなされている時はParent->parentにする必要があるような....
}


bool uiWidget::CallbackResizer(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){
    if(!(flag == uiCallbackFlags::LMouseDown || flag == uiCallbackFlags::LMouseUP
      || flag == uiCallbackFlags::MouseMove  || flag == uiCallbackFlags::OffHover)) return false;
    auto window = getRenderingWindow()->getGLFWwindow();

    constexpr int ui_inner_gap_ = 3;
    uiRect resizeRect_ld(pos.x + size.x - 30, pos.y + size.y - 30, 30, 30); 
    uiRect resizeRect_top  (pos.x                         , pos.y,                          size.x, ui_inner_gap_); 
    uiRect resizeRect_btm  (pos.x                         , pos.y + size.y - ui_inner_gap_, size.x, ui_inner_gap_); 
    uiRect resizeRect_left (pos.x                         , pos.y,                          ui_inner_gap_, size.y); 
    uiRect resizeRect_right(pos.x + size.x - ui_inner_gap_, pos.y,                          ui_inner_gap_, size.y); 


    if(!flags.EnableUserResize){
        bool isMouseOn = ((flag == uiCallbackFlags::MouseMove) && ((num_2 & 0x01) != 0)) || flag == uiCallbackFlags::LMouseDown;

        // リサイズ開始かどうかを判断
        if(flag == uiCallbackFlags::LMouseDown){
            if(resizeRect_ld.isContains(vec2_1.x, vec2_1.y)){
                LOGW << "Resize Frame start!!";
                isSelectingResizer = true;
                glfwSetCursor(window, getContext()->cursors.Ibeam);

            }else if(resizeRect_btm.isContains(vec2_1.x, vec2_1.y)){
                LOGW << "Resize Frame start!!";
                isSelectingEdge_bottom = true;
                glfwSetCursor(window, getContext()->cursors.VResize);

            }else if(resizeRect_top.isContains(vec2_1.x, vec2_1.y)){
                LOGW << "Resize Frame start!!";
                isSelectingEdge_top = true;
                glfwSetCursor(window, getContext()->cursors.VResize);

            }else if(resizeRect_left.isContains(vec2_1.x, vec2_1.y)){
                LOGW << "Resize Frame left start!!";
                isSelectingEdge_left = true;
                glfwSetCursor(window, getContext()->cursors.HResize);

            }else if(resizeRect_right.isContains(vec2_1.x, vec2_1.y)){
                LOGW << "Resize Frame right start!!";
                isSelectingEdge_right = true;
                glfwSetCursor(window, getContext()->cursors.HResize);
            }

        }

        if(isSelectingResizer){
            if(isMouseOn) {
                setWidth (vec2_1.x - pos.x);
                setHeight(vec2_1.y - pos.y);
                LOGW << "Resizing Frame !!";
                parent->needRendering(true);
                return true;
            } else if(flag == uiCallbackFlags::LMouseUP) {
                isSelectingResizer = false;
                LOGW << "Resize Frame end!!";
                setWidth (vec2_1.x - pos.x);
                setHeight(vec2_1.y - pos.y);
                parent->needRendering(true);
                glfwSetCursor(window, NULL);
                std::cout << "delete cursor\n";
                return true;
            }


        }else if(isSelectingEdge_left){
            if(isMouseOn) {
            //    (flag == uiCallbackFlags:::LMouseDown && ))
                setWidth(size.x - vec2_1.x + pos.x);
                setPosX(vec2_1.x);
                LOGW << "Resizing Frame left!!";
                parent->needRendering(true);
                return true;

            } else if(flag == uiCallbackFlags::LMouseUP) {
                isSelectingEdge_left = false;
                LOGW << "Resize Frame left end!!";
                setWidth(size.x - vec2_1.x + pos.x);
                setPosX(vec2_1.x);
                parent->needRendering(true);
                glfwSetCursor(window, NULL);
                std::cout << "delete cursor\n";
                return true;

            }


        }else if(isSelectingEdge_right){
            if(isMouseOn) {
            //    (flag == uiCallbackFlags:::LMouseDown && ))
                setWidth(vec2_1.x - pos.x);
                LOGW << "Resizing rightFrame !!";
                parent->needRendering(true);
                return true;
            } else if(flag == uiCallbackFlags::LMouseUP) {
                isSelectingEdge_right = false;
                LOGW << "Resize right Frame end!!";
                setWidth(vec2_1.x - pos.x);
                parent->needRendering(true);
                glfwSetCursor(window, NULL);
                std::cout << "delete cursor\n";
                return true;
            }


        }else if(isSelectingEdge_bottom){
            if(isMouseOn) {
                setHeight(vec2_1.y - pos.y);
                LOGW << "Resizing bottom Frame !!";
                parent->needRendering(true);
                return true;
            } else if(flag == uiCallbackFlags::LMouseUP) {
                isSelectingEdge_bottom = false;
                LOGW << "Resize bottom Frame end!!";
                setHeight(vec2_1.y - pos.y);
                parent->needRendering(true);
                needApplyAlignment();
                needCalcInnerSize();
                glfwSetCursor(window, NULL);
                std::cout << "delete cursor\n";
                return true;
            }


        }else if(isSelectingEdge_top){
            if(isMouseOn) {
                setPosY(vec2_1.y);
                setHeight(vec2_1.y - pos.y);
                LOGW << "Resizing Frame !!";
                parent->needRendering(true);
                return true;
            } else if(flag == uiCallbackFlags::LMouseUP) {
                isSelectingEdge_top = false;
                LOGW << "Resize Frame end!!";
                setHeight(vec2_1.y - pos.y);
                setPosY(vec2_1.y);
                parent->needRendering(true);
                glfwSetCursor(window, NULL);
                return true;
            }
        }
    }

    for(int i=0; i<widgets.size(); i++){
        if(widgets[i]->CallbackResizer(flag, vec2_1, num_1, num_2, strings)) return true;
    }

    return false;
}


// -----------------------------------------------------
//    [SECTION] Windows
// -----------------------------------------------------


/**
 * Callbackの仕様について
 * @param uiWindow::HoveringWidget : 現在Hoverしている
 * @param uiWindow::FocusedWidget  :マウス移動以外のアクション （クリックする or キーが押される or コマンドが送られる or Drag&Dropがされる)ことで選択されるWidget。HoveringWidgetに設定される
 *   他のWidgetに対してアクションが行われる（マウスクリック、キーを押す、スクロールバーetc)ことでFocusが離れ、新しいFocusedWidgetが設定される
 * - Hoveringはnullptrになることはあるが、FocuedWidgetはnullにはならない
 * - Window Creation時点において、Hovering = Focused = thisポインタになっている？nullptr?
 * - 毎回FocusedWidgetとHoveringWidgetに対してCallbackが行われる
 */ 


void uiWindow::calcHoveringWidget(int x, int y){
    if(!HoveringWidget && HoveringWidget->contains(x,y) && HoveringWidget->isActive()) return;
    bool tmp_search = false;
    auto lastHovering = HoveringWidget;
    HoveringWidget = this;
    // this->setHoveringFlag(true);
    while(true){
        tmp_search = false;
        for(int i=0; i < HoveringWidget->widgets.size(); i++){
            if(HoveringWidget->widgets[i]->contains(x, y) && HoveringWidget->widgets[i]->isActive()){ 
                HoveringWidget = HoveringWidget->widgets[i]; 
                tmp_search = true;
                if(HoveringWidget->widgets.size() == 0){ 
                    if(lastHovering != HoveringWidget){
                        lastHovering->setHoveringFlag(false);
                        HoveringWidget->setHoveringFlag(true);
                    }
                    return; 
                }
            }
        }
        if(!tmp_search){ 
            if(lastHovering != HoveringWidget){
                lastHovering->setHoveringFlag(false);
                HoveringWidget->setHoveringFlag(true);
            }
            return; 
        }
    }
}

bool uiWindow::CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings){ 
    double xpos, ypos;
    glfwGetCursorPos(glfw_window, &xpos, &ypos);

    // 一次的に機能停止
    // if(CallbackResizer(flag, Vector2d(xpos, ypos), num_1, num_2, strings)) return true;


    switch (flag){
    case uiCallbackFlags::OnHover:break;
    case uiCallbackFlags::OffHover:break;
    case uiCallbackFlags::ValueChange:break;
    case uiCallbackFlags::CharInput:break;
    case uiCallbackFlags::ShouldClose:break;

    case uiCallbackFlags::ResizeTo:
        size.x = vec2_1.x;
        size.y = vec2_1.y;
        needRendering(true);
        needApplyAlignment();
        needCalcInnerSize();
        break;

    case uiCallbackFlags::MouseMove:
        calcHoveringWidget(xpos, ypos);
        if(HoveringWidget != this)
            HoveringWidget->CallbackFunc(uiCallbackFlags::MouseMove, Vector2d(xpos, ypos), HoveringWidget == FocusedWidget, num_2, nullptr);
        if(FocusedWidget  != this && HoveringWidget != FocusedWidget) 
            FocusedWidget->CallbackFunc(uiCallbackFlags::MouseMove, Vector2d(xpos, ypos), HoveringWidget == FocusedWidget, num_2, nullptr);
        break;

    case uiCallbackFlags::LMouseDown:
    case uiCallbackFlags::RMouseDown:
    case uiCallbackFlags::CMouseDown:
        calcHoveringWidget(xpos, ypos);
        if(FocusedWidget != HoveringWidget){
            FocusedWidget->setFocusedFlag(false);
            FocusedWidget = HoveringWidget;
            FocusedWidget->setFocusedFlag(true);
        }
        if(FocusedWidget  != this) FocusedWidget->CallbackFunc(flag, Vector2d(xpos, ypos), num_1, num_2, strings);
        break;

    case uiCallbackFlags::LMouseUP:
    case uiCallbackFlags::RMouseUP:
    case uiCallbackFlags::CMouseUP:
        calcHoveringWidget(xpos, ypos);
        if(FocusedWidget  != this) FocusedWidget->CallbackFunc(flag, Vector2d(xpos, ypos), num_1, num_2, strings);
        break;

    case uiCallbackFlags::MouseScroll:
        calcHoveringWidget(xpos, ypos);
        if(FocusedWidget != HoveringWidget){
            FocusedWidget->setFocusedFlag(false);
            FocusedWidget = HoveringWidget;
            FocusedWidget->setFocusedFlag(true);
        }
        if(FocusedWidget  != this) FocusedWidget->CallbackFunc(uiCallbackFlags::MouseScroll, Vector2d(xpos, ypos), num_1, num_2, strings);
        break;

    case uiCallbackFlags::DragDrop:
        calcHoveringWidget(xpos, ypos);
        FocusedWidget->setFocusedFlag(false);
        FocusedWidget = HoveringWidget;
        FocusedWidget->setFocusedFlag(true);
        if(FocusedWidget != this) FocusedWidget->CallbackFunc(uiCallbackFlags::DragDrop, Vector2d(xpos, ypos),num_1, num_2, strings);
        printf("drop %d\n", num_1);
        for (int i = 0; i < num_1; i++) printf("%s\n", strings[i]);
        break;



    case uiCallbackFlags::Keyboard:
        calcHoveringWidget(xpos, ypos);
        FocusedWidget->setFocusedFlag(false);
        FocusedWidget = HoveringWidget;
        FocusedWidget->setFocusedFlag(true);
        if(FocusedWidget != this) FocusedWidget->CallbackFunc(uiCallbackFlags::Keyboard, Vector2d(xpos, ypos), num_1, num_2, strings);

        switch(num_1){
            case GLFW_KEY_F11:
                if(num_2 == GLFW_PRESS) setFullScreen(!wndStyle.isFullScreen);
                break;
            case GLFW_KEY_ESCAPE:
                if(wndStyle.isFullScreen && num_2 == GLFW_PRESS) setFullScreen(false);
                break;
        }

        break;

default:
    LOGE << "Not set flag!!\n";
    disp((uint16_t)flag);

    break;
}

return true;
}




uiWindow::uiWindow(const char *name_, const Vector2d size_){
    std::cout << "window initialize.....\n";

    glfw_window = glfwCreateWindow(size_.x, size_.y, name_, NULL, NULL);
    if (!glfw_window) {
        std::cerr << "ERROR: could not open window with GLFW3\n";
        glfwTerminate();
        abort();
        return;
    }

    flags.EnableChildWidget = 1;
    int x,y,w,h;
    glfwGetWindowPos(glfw_window, &x, &y );
    glfwGetWindowSize(glfw_window, &w, &h);
    wndPos = Vector2d(x, y);
    pos = Vector2d(0, 0);
    size  = Vector2d(w, h);
    glfwSetWindowTitle(glfw_window, name_);
    wndStyle.isFullScreen = false;
    FocusedWidget = this;
    HoveringWidget = this;
    parent = this;
    this->setFocusedFlag(true);
    // _needRenderAll = 3;

    glfwSetWindowUserPointer(glfw_window, this);
    glfwSetWindowSizeCallback(glfw_window, windowSizeCB);
    glfwSetFramebufferSizeCallback(glfw_window, windowSizeCB);

    glfwSetMouseButtonCallback(glfw_window, mouseButtonCB);
    glfwSetCursorPosCallback(glfw_window, mousePosCB);
    glfwSetScrollCallback(glfw_window, mouseScrollCB);
    glfwSetKeyCallback(glfw_window, keyFunCB);
    glfwSetCharCallback(glfw_window, charFunCB);
    glfwSetDropCallback(glfw_window, dropCB);

    DrawLists[0].clearVBOs();
    DrawLists[1].clearVBOs();
    needRendering(true);

    DrawLists[0].setWindow(glfw_window);
    DrawLists[1].setWindow(glfw_window);
}

uiWindow::~uiWindow(){
    std::cout << "uiWindow terminate called!!\n";
    glfwSetWindowShouldClose(glfw_window, GLFW_TRUE);
    glfwDestroyWindow(glfw_window);
    std::cout << "WW\n";

    DrawLists[0].~uiDrawList();
    std::cout << "N\n";

    DrawLists[1].~uiDrawList();
    std::cout << "K\n";

    // ~uiWidgetの方でdeleteしているのでカット    
    // for(int i=0; i< widgets.size(); i++){
    //     delete widgets[i];
    // }
    std::cout << "Q\n";
}

void uiWindow::setFullScreen(bool fullscreen){
    static Vector2d BeforeFullscreenSize = Vector2d(640, 480);

    //https://stackoverflow.com/questions/47402766/switching-between-windowed-and-full-screen-in-opengl-glfw-3-2/47462358
    if ( fullscreen ){
        LOGI << "set to fullscreen";
        // backup window position and window size
        int x,y,w,h;
        glfwGetWindowPos(glfw_window, &x, &y );
        glfwGetWindowSize(glfw_window, &w, &h);
        wndPos = Vector2d(x, y);
        BeforeFullscreenSize  = Vector2d(w, h);
        // get resolution of monitor
        const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        wndStyle.isFullScreen = true;
        // switch to full screen
        auto monitor =  glfwGetPrimaryMonitor();
        glfwSetWindowMonitor( glfw_window, monitor, 0, 0, mode->width, mode->height, 0 );
    }else{
        LOGI << "Diable fullscreen";
        // restore last window size and position
        glfwSetWindowMonitor( glfw_window, nullptr,  wndPos.x, wndPos.y, BeforeFullscreenSize.x, BeforeFullscreenSize.y, 0 );
        wndStyle.isFullScreen = false;
    }
}


void uiWindow::render(){
    setRenderingWindow(this);

    calcInnerSize_recursize();
    // calcAlignment_recursize();

    // TODO ここにinitを持ってくるべきではないのでは？
    initAlignment_recursive();
    applyAlignment_recursive();

    glfwMakeContextCurrent(glfw_window);
    _ctx.RenderingWindow = this;
    if(flags.needRendering > 0){
        for(int i=0; i<widgets.size(); i++){
            widgets[i]->uiWidget::needRendering(true);
        }
    }
    DrawLists[0].setClipRect(Vector2f(0, 0), Vector2f(size.x, size.y));
    DrawLists[1].setClipRect(Vector2f(0, 0), Vector2f(size.x, size.y));

    for(int i=0; i<widgets.size(); i++){
        widgets[i]->render();
    }
    if(DrawLists[0].getCmdNum() > 0 || DrawLists[1].getCmdNum() > 0){
        // std::cout << "render!!\n";

        if(flags.needRendering > 0){
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(1.0, 0.0, 0.0, 1.0);
            needRendering(false);
        }
        DrawLists[0].Render();
        DrawLists[1].Render();
        glfwSwapBuffers(glfw_window);

        DrawLists[0].clearVBOs();
        DrawLists[1].clearVBOs();
    }

    glfwPollEvents(); // update other events like input handling
}



uihWnd uiCreateWindow(const char *name, const Vector2d size){
    auto ctx = getContext();
    ctx->windows.push_back( new uiWindow(name, size));
    return (ctx->windows).back();
}


bool uiShouldWindowClose(){
    for(int i=0; i<_ctx.windows.size(); i++){
        if(glfwWindowShouldClose(_ctx.windows[i]->getGLFWwindow())){
            return true;
        }
    }    
    return false;
}

void startApp(){
    auto ctx = getContext();
    setDefaultStyle(getStyle());

    // text_renderer = new uiFont;
    ctx->windows.clear();
    
    ctx->CurrentWindow   = nullptr;
    ctx->RenderingWindow  = nullptr;
    ctx->CurrentWidget   = nullptr;
    ctx->LastHoverWidget = nullptr;

    ctx->isSetupFinished = false;


    if (!glfwInit()) {
        std::cerr << "ERROR: could not start GLFW3\n";
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    ctx->text_renderer.init();
    ctx->text_renderer.setLanguage(FontLanguage::Japansese);
    ctx->text_renderer.build();

    ctx->setupCursors();

    ctx->isSetupFinished = false;
}

void terminateUI(){
	std::cout << "terminate glUI......AAAAAA";
    for(int i=0; i<_ctx.windows.size(); i++){
        std::cout << "LLA\n";
        _ctx.windows[i]->~uiWindow();
        std::cout << "LLB\n";
    }
    std::cout << "AAAA\n";

    _ctx.windows.clear();
    std::cout << "BBB\n";

    auto shader = getShaderObject();
    std::cout << "CCC\n";
    
    shader->~uiShader();
    std::cout << "DDD\n";

    auto texture = getTextureRenderer();
    std::cout << "EEE\n";
    
    texture->deleteAllTextures();
    std::cout << "FFF\n";

    glfwTerminate();
    std::cout << "GGGG\n";

    auto textRenderer = getTextRenderer();
    std::cout << "FFF\n";

    textRenderer->~uiFont();
    std::cout << "done!!\n";
}


}
