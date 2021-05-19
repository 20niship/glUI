#include "../glUI/glUI.hpp"
#include "../glUI/widget.hpp"
#include "customWidget.cpp"

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


class sprcApp{
private:
	struct RobotStatus{
		Vector2f pos;
		Vector2f vel;
		uint8_t battery;
		bool connected;
		bool isRready;
		std::string time;
		std::string error;

		double start_time;

		RobotStatus(){
			pos = Vector2f(0, 0);
			vel = Vector2f(0, 0);
			battery = 0;
			start_time = 0;
			isRready = false;
			connected = false;
			error = "NO";
		}

		std::string getAllMsg(){
			std::string msg = 
				"Time(s)    : " + std::to_string(glfwGetTime() - start_time) + "\n" +
				"Connection : " + (connected ? "OK" : "NO") +  "\n" +
				"Robot      : " + (isRready ? "READY" : "LOADING....") +  "\n" +
				"Position   : " + "(" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")\n" + 
				"Velocity   : " + "(" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")\n" + 
				"Error      : " + error;
			return msg;
		}

		void start(){ start_time = glfwGetTime(); }
	}status;

	// --------------------  data -----------------------
	uint16_t kyaku_pos = 0;
	std::vector<Vector2f> route;
	bool startProgram = false;
	bool TryConnect = false;
	std::string logMsg = "Log message...\nLog message...\nLog message...\nLog message...\nLog message...\nLog message...\nLog message...\n";
	std::string sendMsg = "please input command....";
	std::string StatusMsg = "Current Time = 00:00\nConnection = OK\nRobot = Ready\nBattery = High\nPosition = (0, 0); Velocity = (0, 0)\n Error = No";
	uint16_t runningMode = 0;
	int isField_x;

	uint16_t kyaku_pos_last;
	int isField_last;


	glUI::uihWnd myWnd;

public:
	void AddLogMessage(std::string new_msg){
		logMsg += new_msg + "\n";
	}



	sprcApp(){
		kyaku_pos = 0;
		startProgram = false;
		TryConnect = false;
		logMsg = "Log message...\nLog message...\nLog message...\nLog message...\nLog message...\nLog message...\nLog message...\n";
		sendMsg = "please input command....";
		StatusMsg = "Current Time = 00:00\nConnection = OK\nRobot = Ready\nBattery = High\nPosition = (0, 0); Velocity = (0, 0)\n Error = No";
		runningMode = 0;
		kyaku_pos_last = 0;
		isField_last = isField_x;

		std::cout << "main program start " << std::endl;
		glUI::startApp();

		std::cout << "Window create " << std::endl;
		myWnd = glUI::uiCreateWindow("test", Vector2d(640,650));
		std::cout << "window create finished " << std::endl;
		
		// ---------------------  initialize  -------------------
		auto mainUI = glUI::AdduiMain(myWnd, &kyaku_pos, &route, Vector2d(10,10), Vector2d(600,600));
		auto frame1 = AddFrame(myWnd, "frame 1", Vector2d(10,10), Vector2d(600,600));
		auto frame2 = AddFrame(myWnd, "frame 1", Vector2d(10,10), Vector2d(600,600));

		myWnd->setAlignType(glUI::uiWidgetAlignTypes::Horizontel_noSpace);
		frame1->setAlignType(glUI::uiWidgetAlignTypes::VertialListl);
		frame2->setAlignType(glUI::uiWidgetAlignTypes::VertialListl);
		frame1->setEnableUserResize(false);
		frame2->setEnableUserResize(false);

		AddTextBox(frame1, &logMsg, Vector2d(10,60), Vector2d(600,200));
		AddLabel(frame1, "Logが増えると自動的にスクロール！", Vector2d(0, 0), Vector2d(0, 0));
		
		AddTextBox(frame2, &sendMsg, Vector2d(10,60), Vector2d(600,200));
		AddButtonEx(frame2,  &startProgram, "START", Vector2d(30,40), Vector2d(100, 40));
		const std::string test_archive = "Field_Test_1\rFirld_Test_2\rTest_run1\rTest_run2\rRun_Left\rRun_Right\rManual";
		AddListBox(frame2, test_archive, &runningMode, Vector2d(10,60), Vector2d(600,200));
		AddTextBox(frame2, &StatusMsg, Vector2d(10,60), Vector2d(600,200));
		AddButton(frame2,  &TryConnect, "Connect", Vector2d(30,40), Vector2d(100, 40));
		AddRadioH(frame2, "Left\rRight", &isField_x,  Vector2d(30,40), Vector2d(100, 40));
		AddLabel(frame2,"Not Connected", Vector2d(10,30), Vector2d(100,50));

		status.start();
		StatusMsg = status.getAllMsg();
	}

	bool update(){
		static int count = 0;
		static double last_dt = glfwGetTime();

		myWnd->needRendering(true);
		// myWnd->needApplyAlignment();
		glUI::RenderAll();
		// std::this_thread::sleep_for(std::chrono::milliseconds(5));
		showFPS();
		count++;

		//100ms秒ごとに実行される
		if(glfwGetTime() - last_dt){
			StatusMsg = status.getAllMsg();
		}

		if(kyaku_pos_last != kyaku_pos){
			std::stringstream ss;
			ss << static_cast<std::bitset<12> >(kyaku_pos);
			AddLogMessage("New 花見客! << " + ss.str());
			kyaku_pos_last = kyaku_pos;
		}
		
		if(isField_last != isField_x){
			std::stringstream ss;
			ss << static_cast<std::bitset<16> >(kyaku_pos);
			AddLogMessage("Field Change << " + std::string(isField_x ? "Left" : "Right"));
			kyaku_pos_last = kyaku_pos;
			isField_last = isField_x;
		}

		return !glUI::uiShouldWindowClose();
	}
	
};



int main(){
	sprcApp app;
	// app.setup();

	while(app.update()){}
	LOGE << "Application Ended!";
}

