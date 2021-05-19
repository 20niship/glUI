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

	std::cout << "Window create " << std::endl;
	glUI::uihWnd myWnd = glUI::uiCreateWindow("test", Vector2d(640,650));
	std::cout << "window create finished " << std::endl;
	std::cout << "Add Widget!\n";
	
	std::string str1 = "Hello World!!, これは日本語ですよ\nThis is testはThis\n is testは\nここに入力して検索を行うファイル編集\n選択表示移動実";

	char buffer[200];
	bool test = true;

	// for(int i=0; i< 5; i++){
	// 	sprintf (buffer, "Line Number = %d, Hwllodfghjkuifsfis;heios;ksffvd\n", i);
	// 	str1 += buffer;
	// }

	for(int i=0; i< 3; i++){
		sprintf (buffer, "Line Number = %d", i);
	}

	uint16_t vv = 0;

	auto frame = AddFrame(myWnd, "frame 1", Vector2d(10,10), Vector2d(600,600));
	auto frame2 = AddFrame(frame, "frame 2", Vector2d(30,10), Vector2d(200,500));
	auto frame3 = AddFrame(frame, "frame 3", Vector2d(200,10), Vector2d(200,600));
	auto frame4 = AddFrame(frame, "frame 4", Vector2d(400,10), Vector2d(200,600));

	frame->setAlignType(glUI::uiWidgetAlignTypes::HorizontalList);
	myWnd->setAlignType(glUI::uiWidgetAlignTypes::Horizontel_noSpace);
	frame2->setEnableUserResize(false);
	frame3->setEnableUserResize(false);
	frame4->setEnableUserResize(false);

	MY_ASSERT(AddLabel(frame2,"lavel1", Vector2d(10,30), Vector2d(100,50)));

	MY_ASSERT(AddTextBox(frame2, &str1, Vector2d(10,60), Vector2d(600,200)));
	auto listBox = AddListBox(frame2, "List__ 1\rList__2\rList__3\rLisst__4\r", &vv, Vector2d(10,60), Vector2d(600,200));
	listBox->setParamString("title", "これはテスト");


	MY_ASSERT(AddCheckbox(frame2, &test, "test1", Vector2d(30,20), Vector2d(80, 120)));
	MY_ASSERT(AddButton(frame3, &test, "test2", Vector2d(30,40), Vector2d(100, 40)));
	MY_ASSERT(AddButton(frame3, &test, "test3", Vector2d(30,60), Vector2d(100, 40)));	

	for(int i=0; i<3; i++){
		sprintf (buffer, "test = %d", i);
		AddButton(frame2, &test, buffer, Vector2d(30,60), Vector2d(100, 40));
		AddButton(frame4, &test, buffer, Vector2d(30,60), Vector2d(100, 40));
	}

	// MY_ASSERT(AddTextBox(frame4, &str1, Vector2d(30,100), Vector2d(80, 200)));


	std::cout << "start Rendering!\n";
	int count = 0;
	while (!glUI::uiShouldWindowClose()) {
		// myWnd->needRendering(true);
		glUI::RenderAll();
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		showFPS();
		count++; 
	}

	glUI::terminateUI();
	std::cout << "Process Finished!!";
	return 0;
}
