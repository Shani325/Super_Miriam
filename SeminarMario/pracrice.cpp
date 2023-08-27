//#include "Animation.h"
//#include <opencv2/opencv.hpp>
//#include <Windows.h>
//
//// before you start: open SeminarMario project properties, go to 
//// Debugging -> Environment
//// set the following value:
//// PATH=$(PATH);../OpenCV_451/bin
//// hit Ctrl + F5 and see a walking lady. that's our hero!
//// press Esc to exit.
//// read carefully the comments below - it contains explanations and instructions.
//// and do the exercises.
//void goRight();
//// 1.
//// draws an animation of our hero that ducks in place, and stays down till you 
//// press the 'w' key. (if (key == 'w'))
//void duck();
//void standAfterDuck();
//// 2.
//// draws an animation of our hero that walks to the right, and ACTUALLLY MOVES RIGHT
//// with a constant speed, of (1/10)*(image width) per 100 milliseconds (=0.1 second).
//
//
//// 3.
//// draw our hero jumps up and right, and stays standing after jump finishes.
//void jumpRight();
//void stand();
//
//// 4.
//// write a main() that:
//// when user presses 'd' - hero walks right
//// when user presses 's' - hero ducks
//// when user presses 'w' - hero jumps
//// when user presses 'a' - hero stops (if it was walking right before)
//
//// 5.
//// in walkRight - when hero reaches the canvas boundery - don't let it go outside of image.
//// force the hero x coordinate to be such that we'll see her.
//// hint: you can get hero image cv::Mat with frame.image
//
//// 6.
//// do the same with jumpRight
//int screen_width = 1200;
//int screen_height = 700;
//cv::Mat background = cv::imread(R"(../Animations/background.png)", cv::IMREAD_UNCHANGED);
//cv::Mat canvas = background.clone();
//cv::Point topLeft(canvas.size().width / 45, canvas.size().height * 2 / 4.8);
//int key = cv::waitKey(100);
//
//cv::Point moveRight(cv::Point topLeft)//Change the location of the hero on the screen to right side.
//{
//	if (topLeft.x + 10 >= screen_width)
//		topLeft.x = 0;
//	topLeft.x += 10;// 0.1 * canvas.size().width;
//	return topLeft;
//}
//
//int mainFunc()
//{
//	cv::resize(background, background, cv::Size(screen_width, screen_height));
//	bool isToExit = false;
//	key = cv::waitKey(100);
//	stand();//Deafult status
//	while (!isToExit)//move the hero by the input from the player.
//	{
//		key = cv::waitKey(100);
//		switch (key)
//		{
//		case 'd':
//			goRight();
//			break;
//		case 's':
//			duck();
//			break;
//		case 'w':
//			jumpRight();
//			break;
//		case 27:
//			isToExit = true;
//			break;
//		default:
//			break;
//		}
//
//	}
//
//	return 0;
//}
//
//void goRight()//cause the hero goes to the right side.
//{
//	Animation animation(R"(../Animations/Hero/runRight)");
//
//	while (key != 'a')
//	{
//		for (int i = 0; i < animation.numFrames() && key != 'a'; i++)
//		{
//			Frame const& frame = animation.getFrame(i);			
//			canvas = background.clone();
//			topLeft = moveRight(topLeft);
//			drawFrame(frame, canvas, topLeft);
//			cv::imshow("test", canvas);
//			key = cv::waitKey(100);
//		}
//	}
//	stand();
//}
//
//void duck()//Cuase the hero ducking.
//{
//	Animation animation(R"(../Animations/Hero/duckDown)");
//	for (int i = 0; i < animation.numFrames(); i++)
//	{
//		Frame const& frame = animation.getFrame(i);
//		canvas = background.clone();
//		drawFrame(frame, canvas, topLeft);
//		cv::imshow("test", canvas);
//		key = cv::waitKey(100);
//	}
//
//	while (key != 'w')
//		key = cv::waitKey(100);
//
//	standAfterDuck();
//}
//
//void standAfterDuck()//Cause the hero to stand after ducking.
//{
//	Animation animation(R"(../Animations/Hero/standAfterDuck)");
//
//	for (int i = 0; i < animation.numFrames(); i++)
//	{
//		Frame const& frame = animation.getFrame(i);
//		cv::Mat canvas = background.clone();
//		drawFrame(frame, canvas, topLeft);
//		cv::imshow("test", canvas);
//		key = cv::waitKey(100);
//	}
//}
//
//void jumpRight()//Cause the hero jump to the right side.
//{
//
//	Animation animation(R"(../Animations/Hero/jump)");
//
//	for (int i = 0; i < animation.numFrames(); i++)
//	{
//		Frame const& frame = animation.getFrame(i);
//		cv::Mat canvas = background.clone();
//		canvas = background.clone();
//		topLeft = moveRight(topLeft);
//		drawFrame(frame, canvas, topLeft);
//		cv::imshow("test", canvas);
//		key = cv::waitKey(100);
//	}
//
//	stand();
//}
//
//void stand()//Cause the hero stand on place.
//{
//	Animation animation(R"(../Animations/Hero/idle)");
//
//	for (int i = 0; i < animation.numFrames(); i++)
//	{
//		Frame const& frame = animation.getFrame(i);
//		canvas = background.clone();
//		drawFrame(frame, canvas, topLeft);
//		cv::imshow("test", canvas);
//		key = cv::waitKey(100);
//	}
//
//}

