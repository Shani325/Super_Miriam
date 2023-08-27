#include "Graphics.h"
#include <algorithm>

using namespace cv;
using namespace std;

//////////////////////////////////////////////////

SingleAnimationGraphics::SingleAnimationGraphics(AnimationPtr animPtr, bool isCyclic)//c'tor
    :_animation(animPtr), _currFrameIdx(0)
    , _isCyclic(isCyclic)
{    
}


cv::Mat SingleAnimationGraphics::getCollisionMask()
{
    return _animation->getFrame(_currFrameIdx).mask;
}

void SingleAnimationGraphics::draw(cv::Mat& canvas, cv::Point const& topLeft)
{
    drawFrame(_animation->getFrame(_currFrameIdx), canvas, topLeft);
}

void SingleAnimationGraphics::reset(int code)
{
    _currFrameIdx = 0;
}

bool SingleAnimationGraphics::update()
{
    bool hasFinished = (_currFrameIdx == _animation->numFrames() - 1);
    if (_isCyclic)
        _currFrameIdx = (_currFrameIdx + 1) % _animation->numFrames();
    else 
        _currFrameIdx = min<int>(_currFrameIdx + 1, (int)_animation->numFrames() - 1);

    return hasFinished;
}


//////////////////////////////////////////////////


//Lives Graphics

LivesGraphics::LivesGraphics(Frame singleLife, int livesCount):_singleLife(singleLife), _livesCount(livesCount)
{
}

cv::Mat LivesGraphics::getCollisionMask()
{
    return _singleLife.mask;
}

void LivesGraphics::draw(cv::Mat& canvas, cv::Point const& topLeft)//Draws hearts as the number of lives.
{ 
    cv::Point position(1100, 20);
     
    for (int i = 0; i < _livesCount; i++)
    {
        drawFrame(_singleLife, canvas, position);
        position.x -= 100;
    }
}

void LivesGraphics::reset(int code)
{
    cout << code << endl;
    _livesCount = code;
}

bool LivesGraphics::update()
{
    return false;
}

//Scores Graphics

ScoresGraphics::ScoresGraphics(int score, float fontScale, int fontFace)
    :_fontScale(fontScale), _score(score), _fontFace(fontFace)
{
}

cv::Mat ScoresGraphics::getCollisionMask()
{
    return cv::Mat();//It doesn't have animation or frame so alse mask.
}

void exampleOfPutText(cv::Mat background)
{
    //Mat image = imread(R"(../Animations/background.png)", IMREAD_UNCHANGED);
    string text = "GAME OVER!";
    int score = 465;
    string moreText = to_string(score);
    //text = text + moreText;
    Point topLeft(background.size().width/6,background.size().height/2);
    int fontFace = FONT_HERSHEY_SIMPLEX; // normal size sans - serif font
    // other options for fontFace:
    FONT_HERSHEY_PLAIN;// small size sans - serif font
    FONT_HERSHEY_DUPLEX;// normal size sans - serif font(more complex than FONT_HERSHEY_SIMPLEX)
    FONT_HERSHEY_COMPLEX; // normal size serif font
    FONT_HERSHEY_TRIPLEX; //normal size serif font(more complex than FONT_HERSHEY_COMPLEX)
    FONT_HERSHEY_COMPLEX_SMALL; //smaller version of FONT_HERSHEY_COMPLEX
    FONT_HERSHEY_SCRIPT_SIMPLEX; //hand - writing style font
    FONT_HERSHEY_SCRIPT_COMPLEX;// more complex variant of FONT_HERSHEY_SCRIPT_SIMPLEX
    double fontScale = 5; // Font scale factor that is multiplied by the font-specific base size.
    Scalar color(/*Blue between 0...255 =*/190,/*Green between 0...255 =*/28,/*Red between 0...255 =*/228);
    int thickness = 10;
    int lineType = FILLED;
    // other options for line type:
    LINE_4; LINE_8; LINE_AA;

    // finally:
    putText(background, text, topLeft, fontFace, fontScale, color, thickness, lineType);

    imshow("test", background);
    //waitKey();//waiting forever till some key is hit.
}

void ScoresGraphics::draw(cv::Mat& canvas, cv::Point const& topLeft)
{
    string text = "score: ";
    string moreText = to_string(_score);
    text = text + moreText;
    Scalar color(/*Blue between 0...255 =*/0,/*Green between 0...255 =*/0,/*Red between 0...255 =*/255);
    int thickness = 2;
    int lineType = FILLED;

    putText(canvas, text, topLeft, _fontFace, _fontScale, color, thickness, lineType);
    imshow("test", canvas);
}

void ScoresGraphics::reset(int code)
{
    _score = code;
}

bool ScoresGraphics::update()
{
    return false;
}
