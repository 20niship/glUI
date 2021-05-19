#include "../glUI/glUI.hpp"
#include "../glUI/widget.hpp"


#include <iostream>
#include <chrono>
#include <thread>

void showFPS(){
	static uint64_t fps = 0;
	static double lastTime = 0;
	double currentTime = glfwGetTime();
	fps++;
	if ( currentTime - lastTime >= 1.0 ){ // If last cout was more than 1 sec ago
		disp(fps);
		lastTime = currentTime;
		fps = 0;
	}
}


int main() {
	std::cout << "main program start " << std::endl;
	glUI::startApp();
	glUI::uihWnd myWnd = glUI::uiCreateWindow("test", Vector2d(640,650));
	
	while (!glUI::uiShouldWindowClose()) {
        auto DrawlistStack = myWnd->getDLstack();    
        DrawlistStack[0].AddRectangle(Vector2f(10.0, 10.0), Vector2f(100.0, 30.0), -1, Vector3f(0.0, 0.2, 0.4));
        DrawlistStack[0].AddRectangle(Vector2f(10.0, 10.0), Vector2f(100.0, 30.0), 3, Vector3f(1.0, 0.9, 0.4));
        
        DrawlistStack[0].AddString(Vector2f(10.0, 10.0), Vector2f(100.0, 30.0), "Hello, World", Vector3f(1.0, 0.0, 0.0), glUI::TextAlignPos::left);
        
        DrawlistStack[0].AddArrowDown(Vector2f(10,50), Vector2f(40, 40), Vector3f(1.0,1.0,1.0));
        DrawlistStack[0].AddArrowUp(Vector2f(70,50), Vector2f(40, 40), Vector3f(1.0,1.0,1.0));
        DrawlistStack[0].AddArrowLeft(Vector2f(130,50), Vector2f(40, 40), Vector3f(1.0,1.0,1.0));
        DrawlistStack[0].AddArrowRight(Vector2f(190,50), Vector2f(40, 40), Vector3f(1.0,1.0,1.0));

        DrawlistStack[0].AddCircle(Vector2f(200,60), Vector2f(80, 80), Vector3f(1.0,0.8,1.0));
        DrawlistStack[0].AddLine(Vector2f(130,20), Vector2f(130, 100), Vector3f(0.5,0.5,1.0), 2);

        auto pos1 = Vector2f(10, 100);
        auto pos2 = Vector2f(100, 200);
        DrawlistStack[0].AddRectangle(pos1, pos2-pos1, -1, Vector3f(0.0, 0.2, 0.4));
        DrawlistStack[0].AddBezier(pos1, Vector2f(0.7, 0.3), pos2, Vector2f(0.3, 0.5), Vector3f(1.0, 1.0, 1.0), 4);
        DrawlistStack[0].AddBezier(pos2, Vector2f(0.7, 0.3), pos1, Vector2f(0.3, 0.5), Vector3f(1.0, 0.0, 0.3), 4);
		
        myWnd->needRendering(true);
		glUI::RenderAll();
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		showFPS();
	}

	glUI::terminateUI();
	std::cout << "Process Finished!!";
	return 0;
}
