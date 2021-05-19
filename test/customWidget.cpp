#include "../glUI/glUI.hpp"
#include <vector>
#include <bitset>

namespace glUI{


class uiMain : public uiWidget{
private:
    uint16_t *kyaku_pos;
    uint16_t last_kyaku_pos;

     std::vector<Vector2f> *route;
     std::vector<Vector2f> last_route;

    float scale = 0.0;
    float title_bar;

    Vector2f getKyakuSize(){
        return Vector2f(450.0f * scale, 317.0f * scale);
    }

    Vector2f getKyakuPos(int i){
        auto kyaku_size = getKyakuSize();
        if(i < 3){
            return Vector2f(pos.x + (572.0f + i*900.0f) * scale, 1915 * scale + pos.y + title_bar);
        }else if(i < 7){
            return Vector2f(pos.x + (-2475.0f + i*900.0f) * scale, 3365 * scale + pos.y + title_bar);
        }else{
            return Vector2f(pos.x + (-5728.0f + i*900.0f) * scale, 4818 * scale + pos.y + title_bar);
        }
    }

    int getKyakuIndex(int x, int y){
        auto _s = getKyakuSize();
         for(int i=0; i<10; i++){
             auto _p = getKyakuPos(i);
             uiRect R(_p.x, _p.y, _s.x, _s.y);
             if(R.isContains(x, y)) return i;
         }
         return -1;
     }

public:
    uiMain(uihWidget parent_, uint16_t *kyaku_pos_, std::vector<Vector2f> *route_, Vector2d pos_, Vector2d size_){
        parent = parent_;
        setSize(size_);
        setPos(pos_);
        setParamString("title", "Trajectory Viewer");
        needApplyAlignment();
        needCalcInnerSize();
        needRendering(true);
        flags.EnableAutoExpandX = 0;
        flags.EnableAutoExpandY = 0;
        flags.EnableAutoShrinkX = 0;
        flags.EnableAutoShrinkY = 0;
        flags.EnableUserResize  = 1;
        
        kyaku_pos = kyaku_pos_;
        last_kyaku_pos = 0;

        route = route_;
        last_route.resize(0);

        setAlignType(uiWidgetAlignTypes::VertialListl);
    }


	void render() {
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

        if((lastScrollPos != scrollPos) || (last_kyaku_pos != *kyaku_pos)){ needRendering(true);}
        if(!flags.needRendering) return;

        
        uint16_t title_bar = flags.EnableTitlebar ? (style->FontSize + style->WidgetInnerSpace_y*2 + 5) : 0;

        //タイトルバー描画
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

        DrawList[0].setClipRect(pos_ + Vector2f(0, title_bar), size_ - Vector2f(0, title_bar));
        DrawList[1].setClipRect(pos_ + Vector2f(0, title_bar), size_ - Vector2f(0, title_bar));
        // DrawList[0].setClipRect(pos_ + Vector2f(0, title_bar), Vector2f(100, 100));


        if(flags.CollapsingTitlebar && flags.EnableTitlebar){ needRendering(false); return; } 
        
        scale = float(size.y - title_bar) / 7000.0f;
        auto kyaku_size = getKyakuSize();

        DrawList[0].AddLineFast(Vector2f(pos.x, pos.x + scale * 1000.0f), Vector2f(pos.x + size.x, pos.x + scale * 1000.0f), Vector3f(0.2, 0.2, 0.2), 3);
        DrawList[0].AddLineFast(Vector2f(pos.x, pos.x + scale * 6000.0f), Vector2f(pos.x + size.x, pos.x + scale * 6000.0f), Vector3f(0.2, 0.2, 0.2), 3);

        bool isField_x = true;
        if(isField_x) DrawList[0].AddRectangle(Vector2f(pos.x + 2500.0f * scale, pos.y + 6000.0f*scale), Vector2f(1000.0f*scale, 1000.0f*scale), 3, Vector3f(0.9, 0.5, 0.3));
        else          DrawList[0].AddRectangle(Vector2f(pos.x                  , pos.y + 6000.0f*scale), Vector2f(1000.0f*scale, 1000.0f*scale), 3, Vector3f(0.9, 0.5, 0.3));

        // 弁当箱
        for(int i=0; i<10; i++){
            bool _iON = (*kyaku_pos & (1 << i)) > 0;
            auto kyaku_pos_px = getKyakuPos(i);

            if(_iON){
                DrawList[0].AddRectangle(kyaku_pos_px, kyaku_size, -1, Vector3f(0.3, 0.2, 0.3));  
            }else{
                DrawList[0].AddRectangle(kyaku_pos_px, kyaku_size, -1, Vector3f(0.8, 0.5, 0.3));  
            }
            DrawList[0].AddRectangle(kyaku_pos_px, kyaku_size, 3, Vector3f(0.9, 0.9, 0.9));  
            DrawList[0].AddString(kyaku_pos_px, kyaku_size, std::to_string(i).c_str(), Vector3f(1.0, 1.0, 1.0), TextAlignPos::center);
        }

        last_kyaku_pos = *kyaku_pos;

        // if((*route) != last_route){
        //     for(int i=0; i<route->size(); i++){
        //         DrawList[0].AddCircle(Vector)
        //     }
        // }
        DrawList[0].AddBezier(pos_ + size_, Vector2f(1.0, 0.0), pos_ + Vector2f(30, size_.y/2.0), Vector2f(0.1, 0.9), Vector3f(0.3, 0.8, 0.3), 2);
        DrawList[0].AddBezier(pos_ + Vector2f(30, size_.y/2.0), Vector2f(0.7, 0.3), pos_ + Vector2f(size_.x, 30), Vector2f(0.5, 0.5), Vector3f(0.3, 0.8, 0.3), 2);

        innerSize.x = parent->getHeight() / 2;
        innerSize.y = parent->getHeight();
        needRendering(false);
    }



	bool CallbackFunc(uiCallbackFlags flag, Vector2d vec2_1, int num_1, int num_2, const char **strings) override{
        auto style = getStyle();
        CallbackTitlebar(flag, vec2_1, num_1, num_2, strings);

        if(flag == uiCallbackFlags::LMouseDown){
            auto i = getKyakuIndex(vec2_1.x, vec2_1.y);
            if(i < 0)return false;

            *kyaku_pos ^= (1 << i); //iビット目を反転
            std::cout << "new kyaku pos = (" << *kyaku_pos << ") " << std::showbase << std::bitset<13>(*kyaku_pos) << std::noshowbase << std::endl;
        }

        return true;
    }
};



inline uihWidget AdduiMain(uihWidget to, uint16_t *kyaku_pos_, std::vector<Vector2f> *route_,Vector2d pos, Vector2d size){
	uihWidget w = new uiMain(to, kyaku_pos_, route_, pos, size);
	w->setParentWidget(to);
	if(to->AddWidget(w)){ return w;}
	else{ delete w; return nullptr;}
}


}