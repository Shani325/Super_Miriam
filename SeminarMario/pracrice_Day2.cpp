//#include "Animation.h"
//#include <opencv2/opencv.hpp>
//
//// @2: include the slime enemy:
//#include "SlimeEntity.h"
//// @2: no need to write cv:: every time, we can just use namespace:
//using namespace cv;
//// @2: NOTE! you should "use namespace" only in cpp file, NOT in H files! (because it "passes on" with the #include statement.)
//
//
//// before you start: open SeminarMario project properties, go to 
//// Debugging -> Environment
//// set the following value:
//// PATH=$(PATH);../OpenCV_451/bin
//// hit Ctrl + F5 and see a walking lady. that's our hero!
//// press Esc to exit.
//// read carefully the comments below - it contains explanations and instructions.
////and do the exercises.
//int screen_width = 1200;
//int screen_height = 700;
//cv::Mat background = cv::imread(R"(../Animations/background.png)", cv::IMREAD_UNCHANGED);
//cv::Mat canvas = background.clone();
//cv::Point topLeft(canvas.size().width / 45, canvas.size().height * 2 / 4.8);
//int main()
//{
//	cv::resize(background, background, cv::Size(screen_width, screen_height));
//	// Animation is a class we wrote for saving you the handling with the 
//	// details of image processing and focusing on code design. iyH we'll get 
//	// to it in the image processing course.
//	Animation animation(R"(../Animations/Hero/runRight)");
//
//	// OpenCV is a popular image processing and computer vision library.
//	// it is not part of the C++ language, but it's written in C++.
//	// in this project we integrated it for you.
//	// The class that represents an image in OpenCV is Mat (i.e. Matrix),
//	// and it's defined inside namespace cv. So the full name is Mat.
//
//	auto slime = CreateSlimeEnemy(R"(../Animations/SlimeOrange)");
//	slime->reset(Point(background.size().width * 2 / 3, background.size().height * 4 / 5));
//	//exampleOfPutText(background);
//
//	Frame heart("../Animations/heart.png");
//	//cv::resize(heart.image, heart.image, cv::Size(100,100));
//	LivesGraphics lives(heart, 4);
//	lives.draw(background,Point(850,20));
//	ScoresGraphics score;
//	score.draw(background,Point(50,50));
//	// Draw
//	bool isToExit = false;
//	while (false == isToExit)
//	{
//		for (int i = 0; i < animation.numFrames(); i++)
//		{
//			Frame const& frame = animation.getFrame(i);
//			// create a copy of the original backgrount - not to "dirty" it.
//			canvas = background.clone();
//			// the point on the canvas where we would like to place the top left corner 
//			// of our frame. NOTE: the y axis on the screen goes from top to bottom.
//			drawFrame(frame, canvas, topLeft);
//
//
//			// @2: slime is an "EntityState" and knows how to take care of itself:
//			slime->update();
//			slime->draw(canvas);
//
//
//			// show what we've got (use BOTH of the following two commands):
//			imshow("test", canvas);
//			// wait 100 milliseconds for a key to be pressed. if not pressed, returns -1:
//			// (you MUST call it after imshow. we'll discuss why in image processing).
//			int key = waitKey(100);
//
//			if (key == 27) // if you pressed ESC key
//			{
//				isToExit = true;
//			}
//		}
//	}
//
//	return 0;
//}
//
//// 1.
//// draws an animation of our hero that ducks in place, and stays down till you 
//// press the 'w' key. (if (key == 'w'))
//void duck();
//
//// 2.
//// draws an animation of our hero that walks to the right, and ACTUALLLY MOVES RIGHT
//// with a constant speed, of (1/10)*(image width) per 100 milliseconds (=0.1 second).
//void walkRight();
//
//// 3.
//// draw our hero jumps up and right, and stays standing after jump finishes.
//void jumpRight();
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
//// hint: you can get hero image Mat with frame.image
//
//// 6.
//// do the same with jumpRight