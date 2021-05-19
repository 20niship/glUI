/**
 * @file widget.hpp
 * @brief 各Widgetの継承クラスの宣言と、Add○○(Widget名)の宣言
 * @author 20Niship Nishimiya Tadashi
 * @date 2021-01-17
 * @details 詳細な説明
 */

#pragma once

#include <iostream>
#include <codecvt>
#include <string>
#include <locale>
#include <algorithm>
#include <memory>
#include <cmath>

#include <opencv2/opencv.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h> 

#include <ft2build.h>
#include <freetype/ftsynth.h>
#include FT_FREETYPE_H


#include "common.hpp"
#include "logger.hpp"

#include "common.hpp"
#include "logger.hpp"


namespace glUI{
struct uiStyle;


enum class TextAlignPos{
	center, right, left
	// center_fixedSpacing = 0b0000,
	// left_fixedSpacing = 0b0001,
	// right_fixedSpacing = 0b0010,
	// center_autoSpacing = 0b0100,
	// left_autoSpacing = 0b0101,
	// right_autpSpacing = 0b0110,
};


// -----------------------------------------------------
//    [SECTION] Rendering
// -----------------------------------------------------

// constexpr uint16_t _IconTexNumX = 33;
enum class uiIcons{
 FolderClose   =  0,
 FolderOpen    =  1,
 Movie_1       =  2,
 Images_1      =  3,
 CleanPen      =  4,
 Movie_Cut     =  5,
 Polygons      =  6,
 Dropper       =  7,
 MinchoText_T  =  8,
 Search        =  9,
 Locked        =  10,
 EyeOpen       =  11,
 NoEye         =  12,
 Glasses       =  13,
 Key           =  14,
 MinchoText_Fx =  15,
 CurlyBracjet_L = 16,
 CurlyBracjet_R = 17,
 MinchoText_M   = 18,
 MinchoText_S   = 19,
 Erasor         = 20,
 Quatation      = 21,
 PaintRoller    = 22,

 CameraLenz        = 23,
 VideoRecoder_1    = 24,
 Camera_1          = 25,
 Combination       = 26,
 WiredCube         = 27,
 Sphear            = 28,
 xyzAxis           = 29,
 Hourglass_1       = 30,
 Package_1         = 31,
 Tag               = 32,
 Movie_2           = 33,
 Books_1           = 34,
 BatteryChaging    = 35,
 CrossMarkSquare   = 36,
 CheckMarkSquare   = 37,
 CrossMarkTriangle = 38,
 Check             = 39,
 Cross             = 40,
 CrossCircle       = 41,
 QuestionCircle    = 42,
 Thunderbolt       = 43,
 Gamepad           = 44,
 Git               = 45,

 CursorArrow       = 46,
 CursorHand        = 47,
 CursorNoHand      = 48,
 Circle1           = 49,
 WiredCircle       = 50,
 FountainPen       = 51,
 Brush             = 52,
 Pin               = 53,
 RunningPerson     = 54,
 Person            = 55,
 TimerClock        = 56,
 TimerClock_2      = 57,
 Hourglass_2       = 58,
 StopWatch         = 59,
 Clock             = 60,
 House             = 61,
 Security          = 62,
 TwinkleThree      = 63,
 TwinkleSingle     = 64,
 Mirror            = 65,
 Neuron            = 66,
 TwoDice           = 67,
 __Black           = 68,

TwistedSquare_1    = 69,
TwistedSquare_2    = 70,
Circle2            = 71, //かぶってる
TwoTriangle        = 72,
SquaredCircle      = 73,
Pentagon           = 74,
BoldArrow_R        = 75,
BoldArrow_L        = 76,
FourSquare         = 77,  // 歪んでる
GeometricShapes    = 78,
Volume             = 79,
Console_1          = 80,
Telescope          = 81,
WaterDrop          = 82,
HalfSquare         = 83,
HalfTransparent    = 84,
PhoneLocked        = 85,
Mountains          = 86,
Flower             = 87,
Share              = 88,
Ghost              = 89,
JigsawPuzzlePieces = 90,
JigsawPuzzle       = 91,
List               = 92,
Graph              = 93,
GeometricShape2    = 94,
NodeGraph          = 95,
BarGraph           = 96,
PieChart           = 97,
Connections        = 98,
Information        = 99,
Signals            = 100,
Filter             = 101,
TriangleRuler      = 102,
Sissor             = 103,
Compass            = 104,
Heart              = 105,
Ski                = 106,
SunLight           = 107,
Light              = 108,
Mouse              = 109,
Infinity           = 110,
Display            = 111,
Switches           = 112,
Sissor2            = 113, // 重複
Clipboard          = 114,

Open               = 115,
Save               = 116,
Download_1         = 117,
DownloadFinished   = 118,
Download_2         = 119,
CloseDoor          = 120,
Bluetooth          = 121,
Airplane           = 122,
TwoGears           = 123,
SingleGear         = 124,
Settins            = 125,
PowerOn            = 126,
PowerOff           = 127,
Mail               = 128,
Message            = 129,
AddNessage         = 130,
DisabledMessage    = 131,
SquareCrossLines   = 132,
SquareDashedLine   = 133,
LightSetting       = 134,
WiFi               = 135,
Telephone          = 136,

ArrowLeft          = 115,
ArrowUp            = 115,
ArrowRight         = 115,
ArrowDown          = 115,

};




// 1つの文字（グリフ）に対するデータ
struct uiGlyph{
    // unsigned int    Codepoint : 31;     // 0x0000..0xFFFF
    // unsigned int    Visible : 1;        // Flag to allow early out when rendering
	unsigned int    U0, V0, U1, V1;     // UVテクスチャの座標
	unsigned int    dHeight;            // グリフの最上点から基準ラインまでの高さ
};


enum class FontLanguage{
	Japansese,
	English,
	Chinese,
	Korean,
	Thai,
	Vietnamese,
	// JPN,ENG,CHI,
};

	
using uiWchar = unsigned short;

class uiFont {
// private:
public:
	FT_Library library;
	FT_Face face;
	FT_GlyphSlot slot;  // グリフへのショートカット

	// uiFontFlags    flags;
	unsigned short    desiredTextSize;

	float             Spacing;            // 字間、config()関数内でuiStyle->FontSpacingの値が設定される
	float             FontSize;           // フォントサイズ、config()関数内でuiStyle->FontSpacingの値が設定される　
	unsigned int      TexWidth;           // build()関数で作成される文字テクスチャのサイズ＜GL_MAX_TEXTURE_SIZE.
	unsigned int      TexHeight;          // build()関数で作成される文字テクスチャのサイズ＜GL_MAX_TEXTURE_SIZE.
	unsigned int      TexHeight_capacity; // 予約済み　のテクスちゃ老域の全体高さ
	bool              isBuildFinished;    // フォントの作成が終わったかどうか
	FontLanguage      language;
	uiWchar*          GlyphRanges;
	unsigned int      nGlyph;             //GlyphRangesの配列の数（つまりグリフの数）
	std::string       FontName;

	//bool              MouseCursorIcons;   // 
	Vector2f          TexUvWhitePixel;    // Texture coordinates to a white pixel
    uiVector<uiWchar> IndexLookup;        // 12-16 // out //            // Sparse. Index glyphs by Unicode code-point.
    uiVector<uiGlyph> Glyphs;             // すべての文字に対するグリフ
    uiGlyph*          FallbackGlyph;      //FinGlyphで上手くいかなかったときのGlyph（□の文字化けのやつ）
	
	unsigned char *_Data; // テクスチャデータ

    //float Scale;
	void              AddGlyph(uiWchar c, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x);
	void              AddRemapChar(uiWchar dst, uiWchar src, bool overwrite_dst = true); // Makes 'dst' character/glyph points to 'src' character/glyph. Currently needs to be called AFTER fonts have been built.
	void              SetGlyphVisible(uiWchar c, bool visible);
	bool              IsGlyphRangeUnused(unsigned int c_begin, unsigned int c_last);

    uiWchar*    GetGlyphRangesKorean();
    uiWchar*    GetGlyphRangesJapanese();
    uiWchar*    GetGlyphRangesEnglish();
    uiWchar*    GetGlyphRangesChinese();
    uiWchar*    GetGlyphRangesCyrillic(); 
    uiWchar*    GetGlyphRangesThai(); 
    uiWchar*    GetGlyphRangesVietnamese();

public:
	uiFont();
	~uiFont();
	void              init();
	bool              setLanguage(FontLanguage l);
	void              setStyle(uiStyle *style);
	bool              build();                           //フォンﾄトをレンダリングする
    uiGlyph*          FindGlyph(uiWchar c);              //
	bool              getSize(uiWchar c, Vector2d *size, unsigned int *dH);
    Vector2f          CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end = NULL, const char** remaining = NULL) const; // utf8
    char*             CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const;
	
	inline unsigned int getTexWidth() { return TexWidth;  }
	inline unsigned int getTexHeight(){ return TexHeight; }
	inline unsigned char *getData()   { return _Data;     }
	inline unsigned int getGlyphNum() { return Glyphs.size();    }

};


class uiShader{
private:
	const char* vertex_shader;
	const char* fragment_shader;
	GLuint vs, fs;
	GLuint shader_id;

public:
	uiShader();
	~uiShader();
	void EnableShader();
	void setup();
	inline GLuint getShaderID(){ return shader_id; }
};



class uiTexture{
private:
	uiVector<GLuint> texIDs;  //ID。
	GLuint TexID_chars; //文字表示用のテクスチャ
	GLuint TexID_icons; //アイコン表示用のテクスチャ

public:
	// テクスチャ作成する
	// format = GL_RGB : カラー画像、GL_COLOR_INDEX：単一の値で構成されるカラー指標
	//filter_type = GL_NEAREST, GL_LINEARがある
	GLuint loadTexture(GLubyte *tex_data, int w, int h, GLenum format=GL_RGB, GLint filter=GL_NEAREST);
	GLuint loadTextureFromFile(std::string filename);
	void   deleteTexture(GLuint texture){glDeleteTextures(1, &texture); texIDs.find_erase(texture); }
	void   deleteAllTextures();
	GLuint   loadIconTexture();
	inline GLuint getTexID_chars(){ return TexID_chars; }
	inline GLuint getTexID_icons(){ return TexID_icons; }
	inline void setTexID_chars(GLuint index){ TexID_chars = index; texIDs.push_back(index); }
};

class uiDrawList{
private:
	uiVector<float>     vertex_array;
	uiVector<float>     col_array;
	uiVector<float>     cord_array;
	
	uiVector<GLuint>    cmds;  //GLuint = unsigned intなので、正の数でなければならない
	GLFWwindow *_hWnd;
	// (draw_order,   index,      vertex_num ) 　<- Polygon描画の時
	// (DRAW_IMAGE,   index,      texture_num)   <- Texture描画の時
	//　(DRAW_TEXT,   pos_index, string_index)  <- 文字描画の時
	//  (SET_SCISSOR, scissor_index, ??) <- glScissorをセット
	//draw_order = (GL_LINE_LOOP, GL_LINES, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, 
	//              DRAW_IMAGE, DRAW_TEXT, DRAW_TEXT_DISABLED, DRAW_TEXT_, SET_SCISSOR)

	// GLuint vao;
	// GLuint vertex_vbo, color_vbo, uv_vbo;

	// uiDrawList(){ clear(); }

    const float CurveFineness;
	float cliprect_left, cliprect_top, cliprect_bottom, cliprect_right;

	inline bool isAllCotainsCliprect(Vector2f pos, Vector2f size){
		return cliprect_left <= pos.x && pos.x + size.x <= cliprect_right && 
			   cliprect_top  <= pos.y && pos.y + size.y <= cliprect_bottom;
	}

	/*! Outsideにある時（trueが返されたとき）はレンダリングをしない */
	inline bool isOutsideCliprect(Vector2f pos, Vector2f size){
		return pos.x + size.x <= cliprect_left || cliprect_right  <= pos.x || 
			   pos.y + size.y <= cliprect_top  || cliprect_bottom <= pos.y;
	}

public:
	uiDrawList();
	~uiDrawList();
	void clearVBOs();
	void setWindow(GLFWwindow *hwnd);
	void AddRectangle(Vector2f pos, Vector2f size, float line_width, Vector3f col);
	void AddImage(Vector2f pos, Vector2f size, Vector2f coord1, Vector2f coord2, GLint textureID);
	void AddImage(Vector2f pos, Vector2f size, Vector2f coord1, Vector2f coord2, GLint textureID, Vector3f col);
    void AddCircle(Vector2f pos, Vector2f size, Vector3f col, float line_width = -1.0);
    void AddLine(Vector2f pos, Vector2f pos2, Vector3f col, float line_width);
    void AddLineFast(Vector2f pos, Vector2f pos2, Vector3f col, float line_width);
    void AddBezier(Vector2f pos1, Vector2f dir1, Vector2f pos2, Vector2f dir2, Vector3f col, float line_width);
    void AddCheck(Vector2f pos, Vector2f size, Vector3f col, float line_width = 1.0);
	void AddArrowDown(Vector2f pos, Vector2f size, Vector3f col);
	void AddArrowUp(Vector2f pos, Vector2f size, Vector3f col);
	void AddArrowLeft(Vector2f pos, Vector2f size, Vector3f col);
	void AddArrowRight(Vector2f pos, Vector2f size, Vector3f col);
	void AddChar(const Vector2f pos, uiWchar str, Vector3f col);
	Vector2d AddString(const Vector2f pos, Vector2f boundingBox, const char text[], Vector3f col, TextAlignPos text_align);
	Vector2d AddString(const Vector2f pos, Vector2f boundingBox, const char text[], Vector3f col, TextAlignPos text_align, float char_scale);

    /*! posの指定は左回りで！ */
    void AddPoly4(Vector2f pos1, Vector2f pos2, Vector2f pos3, Vector2f pos4, Vector3f col, float line_width = -1.0);

	/*! ClipRectの内側でのみレンダリングを行い、外側にはクリップする */
	inline void setClipRect(Vector2f pos, Vector2f size){ 
		cliprect_left = pos.x; cliprect_right  = pos.x + size.x; 
		cliprect_top  = pos.y; cliprect_bottom = pos.y + size.y; 
	}

	void Render();

	inline int getCmdNum(){return vertex_array.size();}
};

}