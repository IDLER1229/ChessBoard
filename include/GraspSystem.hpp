#include "RealsensePointsCloud.hpp"
#include "Realsense.hpp"
#include "FileOperation.hpp"
#include "Segmentation.hpp"
//#include "Classification.hpp"
#include <thread>
#include <mutex>
#include <condition_variable> 
using std::thread;
using std::mutex;
using std::condition_variable;

using _IDLER_::RealsensePointsCloud;
using _IDLER_::Segmentation;
//using _IDLER_::Classification;


class GraspSystem: public FileOperation
{
public:
	GraspSystem(int width, int height, float fps = 30);
	int configureRealsense();
	int releaseRealsense();
	int acquireRealsenseData(Mat &color, Mat &depth, vector<PXCPoint3DF32> &pointscloud);
	int captureFrame();
	int dobotCTRL();


	vector<Rect> classification(vector<Rect> &regions);
	int registration();
	int localization();
	int Grasp();
	
private:
	// Callback
	static void selectPoint(int event, int x, int y, int flags, void* paras);
	void drawCornerText(const Mat &color, const Mat &depth, const vector<Point2f> &corners);
	// 
	int commandParse(int key);
	void placeWindows(int topk);
	Mat PXCImage2Mat(PXCImage* pxc);
	// Dobot related 
	Mat calibrationR2D(Mat &color, Mat &depth, vector<PXCPoint3DF32> &pointscloud);
	vector<Point2f> findChessBoardCorners(Mat &color, Mat &depth, Size pattern = { 3, 3 });
	void calArmCoordinate(PXCPoint3DF32 origin, float side);
	// Create chessboard
	Mat makeChessBoard(int pixels, int count);
private:
	// Realsense
	PXCSession *pxcsession_ = 0;
	PXCSenseManager *pxcsm_ = 0;
	PXCCapture::Device *pxcdev_ = 0;
	PXCProjection *projection_ = 0;
	PXCCapture::Sample *sample_ = 0;
	PXCImage *pxcdepth_ = 0 , *pxccolor_ = 0;
	PXCSizeI32 camera_;
	pxcF32 fps_;
	RealsensePointsCloud dw_;
	//OpenCV
	Mat color_;
	Mat depth_;
	// ChessBoard
	bool calibrated_ = false;
	Size pattern_ = { 3, 3 };
	vector<Point2f> corners_;
	vector<PXCPoint3DF32> corresponding_;
	PXCPoint3DF32 origin_;
	float side_;
	// Grasp
	bool autoLocalization_ = false;
	// Mouse Click to select point
	Point grasppoint_;
	Point preClick_ = { -1, -1 };
	Point click_ = { 0, 0 };
	//thread
	mutex myLock_;
	condition_variable myWait_;


	// Classifier
	//Classification classifier_;
};