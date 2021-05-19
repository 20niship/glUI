/**
 * @file renderer.hpp
 * @brief レンダリングを行うための関数とクラス。
 * @author 20Niship Nishimiya Tadashi
 * @date 2021-01-17
 * @details GLFWとGLEW、Freetypeを使ったレンダリングエンジン。各Windowにつき2つのuiDrawListのインスタンスを持っており、それらを操作することでWidgetをレンダリングする
 */

#include "glUI.hpp"


namespace glUI{

// -----------------------------------------------------
//    [SECTION] uiDrawList
// -----------------------------------------------------


GLuint vao;
GLuint vertex_vbo, color_vbo, uv_vbo;

uiDrawList::~uiDrawList(){
    glDeleteBuffers(1, &vertex_vbo);
    glDeleteBuffers(1, &color_vbo);
    glDeleteBuffers(1, &uv_vbo);
    glDeleteVertexArrays(1, &vao);
}

uiDrawList::uiDrawList():CurveFineness(3.0){
    clearVBOs();
    setClipRect(Vector2f(0.0f, 0.0f), Vector2f(FLT_MAX, FLT_MAX));
}

void uiDrawList::clearVBOs() {
    vertex_array.resize(0); 
    col_array.resize(0);
    cord_array.resize(0);
    cmds.clear();
}

void uiDrawList::setWindow(GLFWwindow *hwnd){ _hWnd = hwnd; }


void uiDrawList::AddRectangle(Vector2f pos, Vector2f size, float line_width, Vector3f col){
    if(isOutsideCliprect(pos, size))return;

    if(line_width < 0){
        vertex_array.push_back(uiMax(pos.x, cliprect_left));				vertex_array.push_back(uiMax(pos.y, cliprect_top));//左上
        vertex_array.push_back(uiMax(pos.x, cliprect_left));			    vertex_array.push_back(uiMin(pos.y + size.y, cliprect_bottom));//左下
        vertex_array.push_back(uiMin(pos.x + size.x, cliprect_right));  	vertex_array.push_back(uiMin(pos.y + size.y, cliprect_bottom));//右下
        vertex_array.push_back(uiMin(pos.x + size.x, cliprect_right));      vertex_array.push_back(uiMax(pos.y, cliprect_top)); //右上


        col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
        col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
        col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
        col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);

        cord_array.push_back({0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});  
        // for(int i=0; i<8; i++){cord_array.push_back(0.0f);}

    }else{
        AddRectangle(pos, Vector2f(size.x,     line_width), -1, col);
        AddRectangle(pos, Vector2f(line_width, size.y),     -1, col);
        AddRectangle(pos + Vector2f(0,  size.y -line_width), Vector2f(size.x,     line_width), -1, col);
        AddRectangle(pos + Vector2f(size.x - line_width, 0), Vector2f(line_width, size.y),     -1, col);
    }
}


void uiDrawList::AddImage(Vector2f pos, Vector2f size, Vector2f coord1, Vector2f coord2, GLint textureID, Vector3f col) {
    if(isOutsideCliprect(pos, size))return;
    vertex_array.push_back(uiMax(pos.x, cliprect_left));				vertex_array.push_back(uiMax(pos.y, cliprect_top));//左上
    vertex_array.push_back(uiMax(pos.x, cliprect_left));			    vertex_array.push_back(uiMin(pos.y + size.y, cliprect_bottom));//左下
    vertex_array.push_back(uiMin(pos.x + size.x, cliprect_right));  	vertex_array.push_back(uiMin(pos.y + size.y, cliprect_bottom));//右下
    vertex_array.push_back(uiMin(pos.x + size.x, cliprect_right));      vertex_array.push_back(uiMax(pos.y, cliprect_top)); //右上


    cord_array.push_back(coord2.x);  			cord_array.push_back(coord2.y);
    cord_array.push_back(coord1.x);  			cord_array.push_back(coord1.y);
    cord_array.push_back(coord2.x);  			cord_array.push_back(coord1.y);
    cord_array.push_back(coord1.x);  			cord_array.push_back(coord2.y);

    col_array.push_back(1.0f);		col_array.push_back(1.0f);		col_array.push_back(1.0f);
    col_array.push_back(1.0f);		col_array.push_back(1.0f);		col_array.push_back(1.0f);
    col_array.push_back(1.0f);		col_array.push_back(1.0f);		col_array.push_back(1.0f);
    col_array.push_back(1.0f);		col_array.push_back(1.0f);		col_array.push_back(1.0f);
    // cord_array.push_back({0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});  
}

void uiDrawList::AddImage(Vector2f pos, Vector2f size, Vector2f coord1, Vector2f coord2, GLint textureID) {
    AddImage(pos, size, coord1, coord2, textureID, Vector3f(1.0f,1.0f,1.0f));
}

void uiDrawList::AddChar(const Vector2f pos, uiWchar str, Vector3f col){
    Vector2f size_tmp;
    Vector2f uv_1, uv_2;

    uiVector<float> dheight_list;
    uiVector<Vector2f> charSize_list;

    uiGlyph *g;
    auto textrenderer = getTextRenderer();

    // textrenderer.RenderChar(static_cast<FT_ULong>(u32str[n]), &size_tmp, &dheight, &uv_1, &uv_2);
    g = textrenderer->FindGlyph(str);
    size_tmp.x  = g->U1 - g->U0;
    size_tmp.y = g->V1 - g->V0;

    if(isOutsideCliprect(pos, size_tmp)) return;
    vertex_array.push_back(uiMax(pos.x, cliprect_left));			   vertex_array.push_back(uiMax(pos.y, cliprect_top));//左上
    vertex_array.push_back(uiMax(pos.x, cliprect_left));			   vertex_array.push_back(uiMin(pos.y + size_tmp.y, cliprect_bottom));//左下
    vertex_array.push_back(uiMin(pos.x + size_tmp.x, cliprect_right)); vertex_array.push_back(uiMin(pos.y + size_tmp.y, cliprect_bottom));//右下
    vertex_array.push_back(uiMin(pos.x + size_tmp.x, cliprect_right)); vertex_array.push_back(uiMax(pos.y, cliprect_top)); //右上


    //(U0, V0), (U0, V1),(U1, V1), (U1, V0)
    cord_array.push_back(g->U0 + uiMax<float>(0.0f, cliprect_left  - pos.x));  cord_array.push_back(g->V0 + uiMax<float>(0.0f, cliprect_top - pos.y));
    cord_array.push_back(g->U0 + uiMax<float>(0.0f, cliprect_left  - pos.x));  cord_array.push_back(uiMin<float>(g->V1, g->V0 + cliprect_bottom - pos.y));
    cord_array.push_back(uiMin<float>(g->U1, g->U0 + cliprect_right - pos.x)); cord_array.push_back(uiMin<float>(g->V1, g->V0 + cliprect_bottom - pos.y));
    cord_array.push_back(uiMin<float>(g->U1, g->U0 + cliprect_right - pos.x)); cord_array.push_back(g->V0 + uiMax<float>(0.0f, cliprect_top - pos.y));

    col_array.push_back(col.x); col_array.push_back(col.y);  col_array.push_back(col.z);
    col_array.push_back(col.x); col_array.push_back(col.y);  col_array.push_back(col.z);
    col_array.push_back(col.x); col_array.push_back(col.y);  col_array.push_back(col.z);
    col_array.push_back(col.x); col_array.push_back(col.y);  col_array.push_back(col.z);
}



Vector2d uiDrawList::AddString(const Vector2f pos, Vector2f boundingBox, const char text[], Vector3f col, TextAlignPos text_align){
    std::u32string  u32str  = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().from_bytes(text);
    Vector2f size_tmp;
    float dheight;
    Vector2f uv_1, uv_2;
    auto style = getStyle();
    
    uiVector<float> dheight_list;
    uiVector<Vector2f> charSize_list;

    Vector2f wholeTextSize(0.0f, 0.0f);
    float wholeWidth = 0.0f;
    uiGlyph *g;
    auto textrenderer = getTextRenderer();
    for (uint32_t n = 0; n < u32str.size(); n++){
        // textrenderer.RenderChar(static_cast<FT_ULong>(u32str[n]), &size_tmp, &dheight, &uv_1, &uv_2);
        g = textrenderer->FindGlyph(u32str[n]);
        size_tmp.x  = g->U1 - g->U0;
        size_tmp.y = g->V1 - g->V0;
        dheight = g->dHeight;
        uv_1 = Vector2f(g->U0, g->V0);
        uv_2 = Vector2f(g->U1, g->V1);

        wholeWidth += size_tmp.x +  style->TextSpacing; // char_width
        dheight_list.push_back(dheight);
        charSize_list.push_back(size_tmp);

        cord_array.push_back(uv_1.x);  			cord_array.push_back(uv_1.y);
        cord_array.push_back(uv_1.x);  			cord_array.push_back(uv_2.y);
        cord_array.push_back(uv_2.x);  			cord_array.push_back(uv_2.y);
        cord_array.push_back(uv_2.x);  			cord_array.push_back(uv_1.y);

        col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
        col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
        col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
        col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
    }

    float dLinePos = 0.0f;
    float dLow = 0.0f;
    for (uint32_t n = 0; n < u32str.size(); n++){
        dLinePos = std::max(dLinePos, dheight_list[n]);
        dLow     =  std::max(dLow,    charSize_list[n].y - dheight_list[n]);
    }

    Vector2f margin = (boundingBox - Vector2f(wholeWidth, dLinePos + dLow))/2.0;

    float xx;
    float yy;
    switch(static_cast<int>(text_align)){
        case static_cast<int>(TextAlignPos::center): xx = margin.x + pos.x;      yy=pos.y + dLinePos+margin.y; break;
        case static_cast<int>(TextAlignPos::left  ): xx = pos.x;  			     yy=pos.y + dLinePos+margin.y;;break;
        case static_cast<int>(TextAlignPos::right ): xx = margin.x * 2 + pos.x;  yy=pos.y + dLinePos+margin.y*2;break;
        default: std::cerr << "Not set TextAlignPos style!!"; abort();
    }

    for(uint32_t i=0; i<u32str.size(); i++){
        vertex_array.push_back(xx);							vertex_array.push_back(yy - dheight_list[i]);//左下
        vertex_array.push_back(xx);							vertex_array.push_back(yy - dheight_list[i]+charSize_list[i].y);//左上
        vertex_array.push_back(xx + charSize_list[i].x);  	vertex_array.push_back(yy - dheight_list[i]+charSize_list[i].y);//右下
        vertex_array.push_back(xx + charSize_list[i].x);    vertex_array.push_back(yy - dheight_list[i]); //右上

        xx += charSize_list[i].x +  style->TextSpacing; // char_width
    }

    return Vector2d(wholeWidth, dLinePos + dLow);
}





Vector2d uiDrawList::AddString(const Vector2f pos, Vector2f boundingBox, const char text[], Vector3f col, TextAlignPos text_align, float char_scale){
    std::u32string  u32str  = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().from_bytes(text);
    Vector2f size_tmp;
    float dheight;
    Vector2f uv_1, uv_2;
    auto style = getStyle();
    
    uiVector<float> dheight_list;
    uiVector<Vector2f> charSize_list;

    Vector2f wholeTextSize(0.0f, 0.0f);
    float wholeWidth = 0.0f;
    uiGlyph *g;
    auto textrenderer = getTextRenderer();
    for (uint32_t n = 0; n < u32str.size(); n++){
        // textrenderer.RenderChar(static_cast<FT_ULong>(u32str[n]), &size_tmp, &dheight, &uv_1, &uv_2);
        g = textrenderer->FindGlyph(u32str[n]);
        size_tmp.x  = g->U1 - g->U0;
        size_tmp.y = g->V1 - g->V0;
        dheight = g->dHeight;
        uv_1 = Vector2f(g->U0, g->V0);
        uv_2 = Vector2f(g->U1, g->V1);

        wholeWidth += char_scale * size_tmp.x +  style->TextSpacing; // char_width
        dheight_list.push_back(dheight * char_scale);
        charSize_list.push_back(size_tmp * char_scale);

        cord_array.push_back(uv_1.x);  			cord_array.push_back(uv_1.y);
        cord_array.push_back(uv_1.x);  			cord_array.push_back(uv_2.y);
        cord_array.push_back(uv_2.x);  			cord_array.push_back(uv_2.y);
        cord_array.push_back(uv_2.x);  			cord_array.push_back(uv_1.y);

        col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
        col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
        col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
        col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
    }

    float dLinePos = 0.0f;
    float dLow = 0.0f;
    for (uint32_t n = 0; n < u32str.size(); n++){
        dLinePos = std::max(dLinePos, dheight_list[n]);
        dLow     =  std::max(dLow,    charSize_list[n].y - dheight_list[n]);
    }

    Vector2f margin = (boundingBox - Vector2f(wholeWidth, dLinePos + dLow))/2.0f;

    float xx;
    float yy;
    switch(static_cast<int>(text_align)){
        case static_cast<int>(TextAlignPos::center): xx = margin.x + pos.x;      yy=pos.y + dLinePos+margin.y; break;
        case static_cast<int>(TextAlignPos::left  ): xx = pos.x;  			     yy=pos.y + dLinePos+margin.y;;break;
        case static_cast<int>(TextAlignPos::right ): xx = margin.x * 2 + pos.x;  yy=pos.y + dLinePos+margin.y*2;break;
        default: std::cerr << "Not set TextAlignPos style!!"; abort();
    }

    for(uint32_t i=0; i<u32str.size(); i++){
        vertex_array.push_back(xx);							vertex_array.push_back(yy - dheight_list[i]);//左下
        vertex_array.push_back(xx);							vertex_array.push_back(yy - dheight_list[i]+charSize_list[i].y);//左上
        vertex_array.push_back(xx + charSize_list[i].x);  	vertex_array.push_back(yy - dheight_list[i]+charSize_list[i].y);//右下
        vertex_array.push_back(xx + charSize_list[i].x);    vertex_array.push_back(yy - dheight_list[i]); //右上

        xx += charSize_list[i].x +  style->TextSpacing; // char_width
    }

    return Vector2d(wholeWidth, (dLinePos + dLow));
}





void uiDrawList::AddCircle(Vector2f pos, Vector2f size, Vector3f col, float line_width){
    float a = std::pow(size.x/2.0, 2); 
    float b = size.y/2.0;         
    MY_ASSERT(a >= 0 || b >= 0);
    
    constexpr float dX = 3.0f;

    auto func = [&](int x) { return b * std::sqrt(1.0f - x*x/a); };
    if(line_width < 0){
        for(int x_ = -size.x/2; x_ < size.x/2; x_+=dX){
            AddPoly4(Vector2f(x_ + pos.x, pos.y + func(x_)),
                     Vector2f(x_ + pos.x, pos.y - func(x_)),
                     Vector2f(x_ + pos.x + dX, pos.y - func(x_ + dX)),
                     Vector2f(x_ + pos.x + dX, pos.y + func(x_ + dX)), col, -1);
        }
    }else{
        for(int x_=0; x_<size.x; x_+=dX){
            AddLine(Vector2f(x_, pos.y + func(x_)), Vector2f(x_ + dX, pos.y + func(x_ + dX)), col, line_width);
            AddLine(Vector2f(x_, pos.y - func(x_)), Vector2f(x_ + dX, pos.y - func(x_ + dX)), col, line_width);
        }   
    }

}

void uiDrawList::AddLine(Vector2f pos1, Vector2f pos2, Vector3f col, float line_width){
    MY_ASSERT(line_width > 0);
    float theta;
    if(pos2.x == pos1.x){theta = 2.0f / 3.141592;}
    else{ theta = std::atan((pos2.y- pos1.y)/(pos2.x- pos1.x));}

    vertex_array.push_back(pos1.x);	                              vertex_array.push_back(pos1.y);//左上
    vertex_array.push_back(pos1.x - line_width*std::sin(theta));  vertex_array.push_back(pos1.y + line_width*std::cos(theta));//左上
    vertex_array.push_back(pos2.x - line_width*std::sin(theta));  vertex_array.push_back(pos2.y + line_width*std::cos(theta));//左上
    vertex_array.push_back(pos2.x);		  		  vertex_array.push_back(pos2.y);//左上

    col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
    col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
    col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
    col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);

    cord_array.push_back({0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});  
}


void uiDrawList::AddLineFast(Vector2f pos1, Vector2f pos2, Vector3f col, float line_width){
    MY_ASSERT(line_width > 0);

    vertex_array.push_back(pos1.x);	              vertex_array.push_back(pos1.y);//左上
    vertex_array.push_back(pos1.x + line_width);  vertex_array.push_back(pos1.y + line_width);//左上
    vertex_array.push_back(pos2.x + line_width);  vertex_array.push_back(pos2.y + line_width);//左上
    vertex_array.push_back(pos2.x);		  		  vertex_array.push_back(pos2.y);//左上

    col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
    col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
    col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
    col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);

    cord_array.push_back({0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});  
}

namespace _Bezier{
    inline float Bezier_fA(float aA1, float aA2) { return 1.0f - 3.0f * aA2 + 3.0f * aA1; }
    inline float Bezier_fB(float aA1, float aA2) { return 3.0f * aA2 - 6.0 * aA1; }
    inline float Bezier_fC(float aA1) { return 3.0 * aA1; }

    inline float CalcBezier(float aT, float aA1, float aA2) { return  ((Bezier_fA(aA1, aA2)*aT + Bezier_fB(aA1, aA2))*aT + Bezier_fC(aA1))*aT; }
    inline float GetSlope(float aT, float aA1, float aA2) { return 3.0 * Bezier_fA(aA1, aA2)*aT*aT + 2.0 * Bezier_fB(aA1, aA2) * aT + Bezier_fC(aA1); }

    inline float getForX(float aX, const float mx1, const float mx2) {
    float currentSlope;
    float aGuessT = aX;
    for (uint8_t i = 0; i < 4; i++) {
        currentSlope = GetSlope(aGuessT, mx1, mx2);
        if (currentSlope == 0.0) return aGuessT;
        float currentX = CalcBezier(aGuessT, mx1, mx2) - aX;
        aGuessT -= currentX / currentSlope;
    }
    return aGuessT;
    }

    inline float getBezier(const Vector2f dir1, const Vector2f dir2, float aX) {
    if (dir1.x == dir1.y && dir2.x == dir2.y) return aX; // linear
    return CalcBezier(getForX(aX, dir1.x, dir2.x), dir1.y, dir1.y);
    }
}

void uiDrawList::AddBezier(Vector2f pos1, Vector2f dir1, Vector2f pos2, Vector2f dir2, Vector3f col, float line_width){
    const uint8_t BezierFiness = (std::abs(pos1.x - pos2.x) + std::abs(pos1.y - pos2.y)) / 5;
    float dx = pos2.x - pos1.x;
    float dy = pos2.y - pos1.y;
    float last_y = 0.0f;
    for(float xx = 1.0f/BezierFiness; xx<1.0; xx += 1.0f/BezierFiness){
        float yy = _Bezier::getBezier(dir1, dir2, xx);
        AddLine(Vector2f(pos1.x + dx*(xx - 1.0f/BezierFiness), pos1.y + dy*last_y),
                Vector2f(pos1.x + dx*xx, pos1.y + dy*yy), col, line_width);
        last_y = yy;
    }
}

void uiDrawList::AddCheck(Vector2f pos, Vector2f size, Vector3f col, float line_width){
    AddLine(pos + Vector2f(size.x*0.1, size.y*0.5), pos + Vector2f(size.x*0.5, size.y*0.9), col, line_width);
    AddLine(pos + Vector2f(size.x*0.5, size.y*0.9), pos + Vector2f(size.x*0.9, size.y*0.1), col, line_width);
}


void uiDrawList::AddArrowDown(Vector2f pos, Vector2f size, Vector3f col){
    constexpr int space = 3;
    AddPoly4(pos + Vector2f(space,  space), 
             pos - Vector2f(0,      space) + Vector2f(size.x/2, size.y),
             pos - Vector2f(0,      space) + Vector2f(size.x/2, size.y),
             pos + Vector2f(-space, space) + Vector2f(size.x  ,      0), col, -1 );
}

void uiDrawList::AddArrowUp(Vector2f pos, Vector2f size, Vector3f col){
    constexpr int space = 3;
    AddPoly4(pos + Vector2f(space, -space) + Vector2f(0       , size.y), 
             pos - Vector2f(space,  space) + Vector2f(size.x  , size.y),
             pos + Vector2f(0,      space) + Vector2f(size.x/2,      0),
             pos + Vector2f(0,      space) + Vector2f(size.x/2,      0), col, -1 );
}


void uiDrawList::AddArrowLeft(Vector2f pos, Vector2f size, Vector3f col){
    constexpr int space = 3;
    AddPoly4(pos + Vector2f(space,      0) + Vector2f(0     , size.y/2), 
             pos + Vector2f(space,      0) + Vector2f(0     , size.y/2),
             pos - Vector2f(space,  space) + Vector2f(size.x, size.y  ),
             pos + Vector2f(-space, space) + Vector2f(size.x,        0), col, -1 );
}


void uiDrawList::AddArrowRight(Vector2f pos, Vector2f size, Vector3f col){
    constexpr int space = 3;
    AddPoly4(pos + Vector2f(space,  space), 
             pos + Vector2f(space, -space) + Vector2f(0       , size.y),
             pos - Vector2f(space,      0) + Vector2f(size.x, size.y/2),
             pos - Vector2f(space,      0) + Vector2f(size.x, size.y/2), col, -1 );
}

void uiDrawList::AddPoly4(Vector2f pos1, Vector2f pos2, Vector2f pos3, Vector2f pos4, Vector3f col, float line_width){
    if(line_width < 0){
        vertex_array.push_back(pos1.x);		  		vertex_array.push_back(pos1.y);//左上
        vertex_array.push_back(pos2.x);		  		vertex_array.push_back(pos2.y);//左上
        vertex_array.push_back(pos3.x);		  		vertex_array.push_back(pos3.y);//左上
        vertex_array.push_back(pos4.x);		  		vertex_array.push_back(pos4.y);//左上

        col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
        col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
        col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);
        col_array.push_back(col.x);		col_array.push_back(col.y);  col_array.push_back(col.z);

        cord_array.push_back({0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});  
    }else{
        AddLineFast(pos1, pos2, col, line_width);
        AddLineFast(pos2, pos3, col, line_width);
        AddLineFast(pos3, pos4, col, line_width);
        AddLineFast(pos4, pos1, col, line_width);
    }
}


void uiDrawList::Render() {
    static bool isInitFinished_r = false;
    if(!isInitFinished_r){
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vertex_vbo); 
        glGenBuffers(1, &color_vbo);
        glGenBuffers(1, &uv_vbo);
        isInitFinished_r = true;
    }
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_array.size_in_bytes(), (const GLvoid*)vertex_array.Data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, color_vbo); 
    glBufferData(GL_ARRAY_BUFFER, col_array.size_in_bytes(), (const GLvoid*)col_array.Data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
    glBufferData(GL_ARRAY_BUFFER, cord_array.size_in_bytes(), (const GLvoid*)cord_array.Data, GL_STATIC_DRAW); 

    int window_width, window_height;
    glfwGetWindowSize(_hWnd, &window_width, &window_height);

    const float projectionMatrix[4][4] = {
        { 2.0f / float(window_width), 0.0f, 0.0f, 0.0f },
        { 0.0f, -2.0f / float(window_height), 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { -1.0f, 1.0f, 0.0f, 1.0f }
    };

    auto shader = getShaderObject();
    auto projMatLoc = glGetUniformLocation(shader->getShaderID(), "projectionMatrix");
    auto textureLoc = glGetUniformLocation(shader->getShaderID(), "texture");
    auto uvSizeLoc  = glGetUniformLocation(shader->getShaderID(), "uvSize");

    shader->EnableShader();

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    auto textrenderer = getTextRenderer();
    glUniform2f(uvSizeLoc, 1.0f / float(textrenderer->getTexWidth()), 1.0f/float(textrenderer->getTexHeight()));
    
    // disp(1.0f / float(textrenderer.getTexWidth()));
    // disp(1.0f/float(textrenderer.getTexWidth()));
    // disp(vertex_array.size());
    
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, &projectionMatrix[0][0]);
    glUniform1i(textureLoc, 0);

    glLineWidth(1.0f);
    glViewport(0, 0, window_width, window_height);


    //glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
    auto texture_obj = getTextureRenderer();
    glBindTexture(GL_TEXTURE_2D, texture_obj->getTexID_chars());
    glDrawArrays(GL_QUADS, 0, vertex_array.size()/2);

    // for (int order_index = 0; order_index < draw_orders.size()-1; order_index += 3) {
    // 	if (draw_orders[order_index] == CMD_LIST_DRAW_TEXTURE2D) {
    // 		glBindTexture(GL_TEXTURE_2D, draw_orders[order_index + 2]);
    // 		glDrawArrays(GL_TRIANGLE_STRIP, draw_orders[order_index + 1], 4);
    // 	}
    // 	else {
    // 		glBindTexture(GL_TEXTURE_2D, textureObj.whiteTexture);
    // 		glDrawArrays(draw_orders[order_index], draw_orders[order_index + 1], draw_orders[order_index + 2]);
    // 	}
    // }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}



// テクスチャ作成する
// format = GL_RGB : カラー画像、GL_COLOR_INDEX：単一の値で構成されるカラー指標
//filter_type = GL_NEAREST, GL_LINEARがある
GLuint uiTexture::loadTexture(GLubyte *tex_data, int w, int h, GLenum format, GLint filter){
    GLuint idTemp;
    std::cout << "loading texture... (" << w << ", " << h << ")";
    glGenTextures(1, &idTemp);

    float index[] = { 0.0, 1.0 };

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glPixelMapfv(GL_PIXEL_MAP_I_TO_R, 2, index);
    glPixelMapfv(GL_PIXEL_MAP_I_TO_G, 2, index);
    glPixelMapfv(GL_PIXEL_MAP_I_TO_B, 2, index);
    glPixelMapfv(GL_PIXEL_MAP_I_TO_A, 2, index);

    // テクスチャをGPUに転送
    glBindTexture(GL_TEXTURE_2D, idTemp);
    // void glTexImage2D(GLenum target,
    // 　				GLint level,
    // 　				GLint internalFormat,
    // 　				GLsizei width,
    // 　				GLsizei height,
    // 　				GLint border,
    // 　				GLenum format,
    // 　				GLenum type,
    // 　				const void * data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, format, GL_BITMAP, tex_data);

    // テクスチャを拡大縮小する時のフィルタリング方法を指定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    //ラッピング方法を指定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // テクスチャのアンバインド
    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout << "Done!\n";

    return idTemp;
}


GLuint uiTexture::loadTextureFromFile(std::string filename) {
    // Raw画像の場合
    // // ファイルの読み込み
    // std::ifstream fstr(filename, std::ios::binary);
    // const size_t fileSize = static_cast<size_t>(fstr.seekg(0, fstr.end).tellg());
    // fstr.seekg(0, fstr.beg);
    // char* textureBuffer = new char[fileSize];
    // fstr.read(textureBuffer, fileSize);

    cv::Mat img = cv::imread(filename);
    if(img.empty()) return 0;
    GLuint idtemp =  loadTexture(img.data, img.rows, img.cols, GL_RGB, GL_LINEAR);
    texIDs.push_back(idtemp);
    return idtemp;
}

GLuint uiTexture::loadIconTexture(){
    static GLubyte iconTex[] = {
        #include "Glyphs/IconTexture.csv"
    };

    constexpr int iconTex_width = 1104;
    constexpr int iconTex_height = 384;

    TexID_icons = loadTexture(iconTex, iconTex_width, iconTex_height, GL_COLOR_INDEX, GL_NEAREST);
    return TexID_icons;
}



void uiTexture::deleteAllTextures(){
    for(int i=0; i<texIDs.size(); i++){
        glDeleteTextures(1, &texIDs[i]);
    } 
    texIDs.clear();
}


uiShader::uiShader(){}
void uiShader::EnableShader() { glUseProgram(shader_id); }
void uiShader::setup(){
    std::cout << "shader setup...\n";
    vertex_shader =	
        "#version 400\n"
        "layout(location = 0) in vec2 position;\n"
        "layout(location = 1) in vec2 vuv;\n"
        "layout(location = 2) in vec3 color;\n"
        "uniform mat4 projectionMatrix;"
        "uniform vec2 uvSize;"
        "out vec2 Flag_uv;"
        "out vec3 outColor;"
        "void main(void) {"
        "outColor = color;"
        "Flag_uv = vec2(vuv.x*uvSize.x, vuv.y*uvSize.y);"
            //"Flag_uv = vuv;n"
        "gl_Position = projectionMatrix *  vec4(position, 0.0f, 1.0f);"
        "}\n";

    fragment_shader =
        "#version 400\n"
        "in vec2 Flag_uv;"
        "in vec3 outColor;"
        "uniform sampler2D texture;"
        "out vec4 outFragmentColor; \n"
        "void main(void) {"
        "outFragmentColor = vec4(outColor, 1.0) * texture2D(texture, Flag_uv);"
        //"outFragmentColor = vec4(outColor, 1.0) * texture2D(texture, Flag_uv); n"
        "}\n";

    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);

    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    shader_id = glCreateProgram();


    GLint success = 0;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE){
        std::cerr << "[ERROR] Vertex shader Compile Failed\n";
    }else{
        std::cout << "vertex Shader compile success!!\n";
    }

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE){
        std::cerr << "フラグメントシェーダー作成に失敗！！";
    }else{
        std::cout << "fragment Shader compile success!!\n";
    }
    glAttachShader(shader_id, vs);
    glAttachShader(shader_id, fs);
    glLinkProgram(shader_id);
}

uiShader::~uiShader(){
    glDeleteShader(shader_id);
}


// -----------------------------------------------------
//    [SECTION] uiFont
// -----------------------------------------------------
void uiFont::AddGlyph(uiWchar c, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x){}
void uiFont::AddRemapChar(uiWchar dst, uiWchar src, bool overwrite_dst){}
void uiFont::SetGlyphVisible(uiWchar c, bool visible){}
bool uiFont::IsGlyphRangeUnused(unsigned int c_begin, unsigned int c_last){return true;}

constexpr uiWchar _FallbackGlyphNumber = 0x0030;

uiWchar* uiFont::GetGlyphRangesKorean(){return nullptr;}
uiWchar* uiFont::GetGlyphRangesChinese(){return nullptr;}
uiWchar* uiFont::GetGlyphRangesCyrillic(){return nullptr;}
uiWchar* uiFont::GetGlyphRangesThai(){return nullptr;}
uiWchar* uiFont::GetGlyphRangesVietnamese(){return nullptr;}

uiWchar* uiFont::GetGlyphRangesJapanese(){
    static uiWchar ranges[] = {
        #include "Glyphs/GlyphRange_JPN.csv"
         _FallbackGlyphNumber,
    };
    nGlyph = sizeof(ranges) / sizeof(uiWchar);
    return ranges;
}

uiWchar* uiFont::GetGlyphRangesEnglish(){
    static uiWchar ranges[] = {
        #include "Glyphs/GlyphRange_ENG.csv"
        _FallbackGlyphNumber,
    };
    nGlyph = sizeof(ranges) / sizeof(uiWchar);
    return ranges;
}

uiFont::uiFont():TexWidth(0),
                TexHeight(0),
                isBuildFinished(false),
                _Data(nullptr){ 
    init();
}


uiFont::~uiFont(){
    if(_Data != nullptr) free(_Data);
    FT_Done_Face    ( face );
    FT_Done_FreeType( library );
    std::cout << "1";

    TexWidth = 0; TexHeight = 0;
    // if (FallbackGlyph != nullptr) free(FallbackGlyph);
    isBuildFinished = false;
    std::cout << "3";

    IndexLookup.clear();
    std::cout << "4";

    Glyphs.clear();
    std::cout << "5";

}


void uiFont::init(){
    std::cout << "init uiFont" << std::endl;
    TexWidth = 0;
    TexHeight = 0;
    isBuildFinished = false;
    
    if(_Data != nullptr) free(_Data);
    _Data = nullptr;

    IndexLookup.clear();
    Glyphs.clear();

    auto style = getStyle();
    if(style == nullptr) {std::cout << "[error] cannot handle style \n";}
    setStyle(style);
}

bool uiFont::setLanguage(FontLanguage l){
    // static uiWchar *glyphrange_tmp = nullptr;
    switch(l){
        case FontLanguage::Japansese: GlyphRanges = GetGlyphRangesJapanese();break;
        case FontLanguage::Chinese:   GlyphRanges = GetGlyphRangesChinese(); break;
        case FontLanguage::English:   GlyphRanges = GetGlyphRangesEnglish(); break;
        case FontLanguage::Korean :   GlyphRanges = GetGlyphRangesKorean(); break;
        default: return false;
    }
    return true;
}


//フォンﾄトをレンダリングする
bool uiFont::build(){
    init();
    std::cout << "building font objects!!" << std::endl;

    MY_ASSERT(FontSize > 0);
    MY_ASSERT(GlyphRanges != nullptr);
    MY_ASSERT(isBuildFinished == false);
    
    
    { // 色々初期化
        TexWidth = 0;
        TexHeight = 0;
        isBuildFinished = false;
        if(_Data != nullptr) free(_Data);
        _Data = nullptr;

        IndexLookup.clear();
        Glyphs.clear();
    }

    // ImFontAtlasBuildInit(atlas);
    // // Clear atlas
    // atlas->TexID = NULL;
    // atlas->TexWidth = atlas->TexHeight = 0;
    // atlas->TexUvScale = ImVec2(0.0f, 0.0f);
    // atlas->TexUvWhitePixel = ImVec2(0.0f, 0.0f);
    // atlas->ClearTexData();

    // // Temporary storage for building
    // ImVector<ImFontBuildSrcData> src_tmp_array;
    // ImVector<ImFontBuildDstData> dst_tmp_array;
    // src_tmp_array.resize(atlas->ConfigData.Size);
    // dst_tmp_array.resize(atlas->Fonts.Size);
    // memset(src_tmp_array.Data, 0, (size_t)src_tmp_array.size_in_bytes());
    // memset(dst_tmp_array.Data, 0, (size_t)dst_tmp_array.size_in_bytes());


    Glyphs.resize(nGlyph);

    FT_Error error, error2;
    unsigned int TexCapacityHeight = 128;

    { //フォントレンダリングエンジンの初期化
        desiredTextSize = (2048 > GL_MAX_TEXTURE_SIZE) ? int( std::pow(2, int(log2(GL_MAX_TEXTURE_SIZE)))) : 2048;
        std::cout << "desired texture width = " << desiredTextSize << std::endl;

        _Data = (unsigned char *)malloc(desiredTextSize*TexCapacityHeight/8);
        memset(_Data, 0x00, desiredTextSize*TexCapacityHeight/8);
        
        error = FT_Init_FreeType( &library ) ;
        if (error) { std::cerr << "[ERROR] Failed to init FreeType library!" << std::endl; }


		error2 = FT_New_Face( library, FontName.c_str(), 0, &face);
		if ( error2 == FT_Err_Unknown_File_Format ){
			std::cerr << "[ERROR] Font file format is not supported!! " << std::endl; return false;
		}else if ( error2 ){
			std::cerr << "[ERROR] Font file not found or it is broken! " << std::endl; return false;
		}
		
		// 3. 文字サイズを設定
		// error = FT_Set_Char_Size(face, 0,
		//                 size, // 幅と高さ
		//                 100, 100);  // 水平、垂直解像度
		error2 = FT_Set_Pixel_Sizes(face, FontSize, 0);
		slot = face->glyph;  // グリフへのショートカット

        if(error || error2) {
            std::cout << "an error happened during initializing freetype library\n";
            return false;
        }
    }

    unsigned int CursorY = 0;
    unsigned int CursorX = 0;
    unsigned int TextMaxHeight = 0;

    FT_UInt glyph_index;
    int  i, j, p, q, index, w, h, x_max, y_max;
    uiGlyph g_tmp;

    std::cout << "start rendering main glyphs...... n=" << nGlyph << std::endl;

    int __nGlyph_enabled = 0;
    for(unsigned int n = 0; n<nGlyph; n++){
        //reder one char
        glyph_index = FT_Get_Char_Index( face, GlyphRanges[n]);
		error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
		// FT_GlyphSlot_Oblique(face->glyph );
		// FT_GlyphSlot_Embolden(face->glyph );

		error2 = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
        if(error || error2){
            std::cout << "No matching glyph for char-> " << GlyphRanges[n] << std::endl;
        }else{
            w = (slot->bitmap).width;
            h = (slot->bitmap).rows;

            if(w + CursorX > desiredTextSize){
                CursorX = 0;
                CursorY = TextMaxHeight;
            }
            
            while(TexCapacityHeight <= CursorY + h + 100){
                constexpr int glow_dh = 256;
                unsigned char *new_data = (unsigned char *)malloc(desiredTextSize * (TexCapacityHeight + glow_dh) / 8);

                if(_Data){
                    memcpy(new_data, _Data, desiredTextSize*TexCapacityHeight/ 8);
                    free(_Data);
                }
                _Data = new_data;
                memset(_Data + desiredTextSize * TexCapacityHeight/8, 0x00, desiredTextSize * glow_dh/8);
                TexCapacityHeight += glow_dh;
            }

            x_max = w+CursorX;
            y_max = h+CursorY;

            for ( i = CursorX, p = 0; i < x_max; i++, p++ ){
                for ( j = CursorY, q = 0; j < y_max; j++, q++ ){
                    if ( p < 0      || q < 0       || p >= w || q >= h) continue;
                    index = j*desiredTextSize + i;
                    _Data[index/8] |= static_cast<unsigned char>(((slot->bitmap).buffer[q * w + p] > 70 ? 1 : 0)
                                                                     * std::pow(2.0, 7-index%8));
                }
            }

            // IndexLookup.push_back(__nGlyph_enabled);
            // g_tmp.Codepoint = GlyphRanges[n];
            g_tmp.dHeight   = slot->bitmap_top;
            g_tmp.U0        = CursorX;
            g_tmp.V0        = CursorY;
            g_tmp.U1        = CursorX + w;
            g_tmp.V1        = CursorY + h;
            Glyphs[n] = g_tmp;

            // std::cout << "[" << __nGlyph_enabled << "] '" << GlyphRanges[n] << " ("<< char(GlyphRanges[n]) << ")' --> at (" << g_tmp.U0 << ", " << g_tmp.V0 << ", " << g_tmp.U1 << ", " << g_tmp.V1 << ")\n";
            
            __nGlyph_enabled++;
            CursorX += w;
            TextMaxHeight = std::max(TextMaxHeight, CursorY + h);
        }
    }

    std::cout << "All glyph was rendered!" << std::endl;
    { //Lookuptableの作成
        IndexLookup.resize(65536);
        for(int i=0; i<IndexLookup.size(); i++){
            IndexLookup[i] = 0xFFFF;
        }

        for(uiWchar i=0; i<__nGlyph_enabled; i++){
            //auto z = Glyphs[i].Codepoint;
            unsigned short z = GlyphRanges[i];
            IndexLookup[z] = i;
        }

    }

    FallbackGlyph = &(Glyphs[Glyphs.size()-1]);
    std::cout << "Index lookuptable was created! " << std::endl;

    bool loop = false; i=0;
    while(loop){
        if (int(int(_Data[i/8])/std::pow(2.0, 7-i%8))%2 == 1){
            TexUvWhitePixel = Vector2f(i%desiredTextSize, i/desiredTextSize);
            std::cout << "White texture found!" << std::endl;
            loop = false;
        }
        i++;
    }

    nGlyph = __nGlyph_enabled;
    TexWidth = desiredTextSize;
    TexHeight = TextMaxHeight + CursorY;
    TexHeight_capacity = TexCapacityHeight;
    isBuildFinished = true;

//   std::cout << std::endl;    std::cout << std::endl;
//   std::cout << "----------------------------------------------------------" <<std::endl;
//   for(int j=0; j<TexHeight; j++){
//     for(int i=0; i< TexWidth; i++){
//         int index = j*TexWidth + i;
//         if (int(int(_Data[index/8])/std::pow(2.0, 7-index%8))%2 == 1){
//           std::cout <<"#";
//         }else{
//           std::cout << ".";
//         }
//     }
//     std::cout << std::endl;
//   }
//   std::cout << "----------------------------------------------------------" <<std::endl;
//   std::cout << std::endl;    std::cout << std::endl;


  std::cout << "uiFont init finished!" << std::endl;
  return true;
}

void uiFont::setStyle(uiStyle *style){
    FontSize = style->FontSize;
    Spacing  = style->TextSpacing;
    FontName = style->FontName;
}


uiGlyph* uiFont::FindGlyph(uiWchar c){
    if (c >= (size_t)IndexLookup.Size)
        return FallbackGlyph;
    const uiWchar i = IndexLookup.Data[c];
    if (i == 0xFFFF){
        return FallbackGlyph;
    }
    return &Glyphs[i];
}

bool uiFont::getSize(uiWchar c, Vector2d *size, unsigned int *dH){
    auto g = FindGlyph(c);
    size->x = g->U1 - g->U0;
    size->y = g->V1 - g->V0;
    *dH = g->dHeight;
    return !(g == FallbackGlyph);
}


const char* CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width){
    return nullptr;
}



void RenderAll(){
    auto _ctx = getContext();
    auto shader = getShaderObject();
    auto texture = getTextureRenderer();

    if(!_ctx->isSetupFinished){
        for(int i=0; i<_ctx->windows.size(); i++){
            glfwMakeContextCurrent( _ctx->windows[i]->getGLFWwindow());

            // start GLEW extension handler
            glewExperimental = GL_TRUE;
            glewInit();

            const GLubyte *renderer;
            const GLubyte *version;
            renderer = glGetString(GL_RENDERER);
            version = glGetString(GL_VERSION);
            printf("Renderer: %s\n", renderer);
            printf("OpenGL version supported %s\n", version);

            glDisable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            auto fontobj = getTextRenderer();
            auto idtemp  = texture->loadTexture(fontobj->getData(), 
                                               fontobj->getTexWidth(), 
                                               fontobj->getTexHeight(),
                                               GL_COLOR_INDEX, GL_NEAREST);
            texture->setTexID_chars(idtemp);
            texture->loadIconTexture();

            //shaderのセットアップ
            shader->setup();

            _ctx->isSetupFinished = true;
        }

        //テキストレンダラーのセットアップ
        // switched to glUI::startApp() function in glUI.cpp
        // auto font = getTextRenderer();
        // font->setLanguage(FontLanguage::Japansese);
        // font->build();


        //テクスチャのセットアップ
        // switched to glUI::startApp() function in glUI.cpp
        //https://stackoverflow.com/questions/327642/opengl-and-monochrome-texture
        // auto idtemp = texture->loadTexture(font->getData(), font->getTexWidth(), font->getTexHeight(), GL_COLOR_INDEX, GL_NEAREST);
        // texture->setTexID_chars(idtemp);
    }

    for(int i=0; i<_ctx->windows.size(); i++){
        _ctx->windows[i]->render();
    }
}



} //namespace glUI