#include "he.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>
#include <cstring>
#include <vector>
#include <unistd.h>
using namespace cv;
using namespace std;

void mouseClick(int button)
{
    Display *display = XOpenDisplay(NULL);

    XEvent event;

    if(display == NULL)
    {
        fprintf(stderr, "Cannot initialize the display\n");
        exit(EXIT_FAILURE);
    }

    memset(&event, 0x00, sizeof(event));

    event.type = ButtonPress;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;

    XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

    event.xbutton.subwindow = event.xbutton.window;

    while(event.xbutton.subwindow)
    {
        event.xbutton.window = event.xbutton.subwindow;

        XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    }

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");

    XFlush(display);

    usleep(100000);

    event.type = ButtonRelease;
    event.xbutton.state = 0x100;

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");

    XFlush(display);

    XCloseDisplay(display);
}

void ImageFromDisplay(std::vector<uint8_t>& Pixels, int& Width, int& Height, int& BitsPerPixel)
{
    Display* display = XOpenDisplay(nullptr);
    Window root = DefaultRootWindow(display);

    XWindowAttributes attributes = {0};
    XGetWindowAttributes(display, root, &attributes);

    Width = attributes.width;
    Height = attributes.height;

    XImage* img = XGetImage(display, root, 0, 0 , Width, Height, AllPlanes, ZPixmap);
    BitsPerPixel = img->bits_per_pixel;
    Pixels.resize(Width * Height * 4);

    memcpy(&Pixels[0], img->data, Pixels.size());

    XFree(img);
    XCloseDisplay(display);
}
int main()
{
    //sleep(1);
 srand(time(NULL));
    int Width = 0;
    int Height = 0;
    int Bpp = 0;
    std::vector<std::uint8_t> Pixels;
    ImageFromDisplay(Pixels, Width, Height, Bpp);
    if (Width && Height)
    {
        Mat img = Mat(Height, Width, Bpp > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]); //Mat(Size(Height, Width), Bpp > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]);

    string outputName = "dtc.bmp";
    Mat templ = imread( "tst.bmp", 1 );
    imwrite(outputName, img);
    img = imread( "dtc.bmp", 1 );

    /// Create the result matrix
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    Mat result( result_cols, result_rows, CV_8UC4 );

    /// Do the Matching and Normalize
    matchTemplate(img, templ, result, CV_TM_CCOEFF);
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
    int keypressed = -1;
  for (int i = 0; i < 16; i++)
    {
    double minval, maxval, threshold = 0.95;
    Point minLoc, maxLoc;

   minMaxLoc(result, &minval, &maxval, &minLoc, &maxLoc);
if (maxval >= threshold) {
    rectangle(img, maxLoc, Point( maxLoc.x + templ.cols , maxLoc.y + templ.rows ), Scalar(0, 255, 0), 2);
    rectangle(result, maxLoc, Point( maxLoc.x + templ.cols , maxLoc.y + templ.rows ), Scalar(0, 255, 0), 2);
     Display *display = XOpenDisplay(0);
      int x , y;
    x = maxLoc.x + templ.cols / 2;
    y = maxLoc.y + templ.rows / 2;
    Window root = DefaultRootWindow(display);
    XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
    keypressed = waitKey(100);
    cout << keypressed << endl;
    mouseClick(Button1);
    XFlush(display);
    XCloseDisplay(display);
if (i == 0)
waitKey(300);

    if (keypressed == -1)
    keypressed = waitKey(rand() % 100 + 150);
    cout << keypressed << endl;
}

        else
            break;
             if (keypressed != -1)
        break;
    }
    mouseClick(Button1);
    imshow("img", img);

    imwrite(outputName, img);
     //   waitKey(0);
    }

    return 0;
}

