/**
*/
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/cursorfont.h>
/* include some silly stuff */
#include <stdio.h>
#include <stdlib.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <raspicam/raspicam.h>
using namespace std;
/* here are our X variables */
Display *dis;
int screen;
Window win;
Pixmap pixmap;
GC gc;

/* here are our X routines declared! */
void init_x();
void close_x();
void redraw();

int main(int argc, char **argv)
{
    XEvent event;   /* the XEvent declaration !!! */
    KeySym key;     /* a dealie-bob to handle KeyPress Events */
    char text[255]; /* a char buffer for KeyPress Events */
    init_x();
    raspicam::RaspiCam Camera; //Cmaera object
    //Open camera
    cout << "Opening Camera..." << endl;
    if (!Camera.open())
    {
        cerr << "Error opening camera" << endl;
        return -1;
    }
    //wait a while until camera stabilizes

    while (1)
    {
        /* get the next event and stuff it into our event variable.
		   Note:  only events we set the mask for are detected!
		*/
        XNextEvent(dis, &event);

        if (event.type == Expose && event.xexpose.count == 0)
        {
            /* the window was exposed redraw it! */
            redraw();
        }
        if (event.type == KeyPress &&
            XLookupString(&event.xkey, text, 255, &key, 0) == 1)
        {
            /* use the XLookupString routine to convert the invent
		   KeyPress data into regular text.  Weird but necessary...
		*/
            if (text[0] == 'q')
            {
                close_x();
            }
            printf("You pressed the %c key!\n", text[0]);
        }
        if (event.type == ButtonPress)
        {
            /* tell where the mouse Button was Pressed */
            int x = event.xbutton.x,
                y = event.xbutton.y;

            strcpy(text, "X is FUN!");

            //camera
            cout << "Sleeping for 0.5 secs" << endl;
            usleep(0.5 * 1000000);
            //capture
            Camera.grab();
            //allocate memory
            unsigned char *data = new unsigned char[Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];
            //extract the image in rgb format
            Camera.retrieve(data, raspicam::RASPICAM_FORMAT_RGB); //get camera image
            //save
            std::ofstream outFile("raspicam_image.ppm", std::ios::binary);
            outFile << "P6\n"
                    << Camera.getWidth() << " " << Camera.getHeight() << " 255\n";
            outFile.write((char *)data, Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));

            int WIDTH = Camera.getWidth();
            int HEIGHT = Camera.getHeight();
            cout << "Camera Size: " << WIDTH << ":" << HEIGHT << endl;
            cout << "image buffer size :" << sizeof(data) << ":" << Camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB) << endl;
            // for (int i = 0; i < WIDTH; i++)
            // {
            //     for (int j = 0; j < HEIGHT; j++)
            //     {
            //         int indexR = 3 * i + 3 * j * WIDTH + 0;
            //         int indexG = 3 * i + 3 * j * WIDTH + 1;
            //         int indexB = 3 * i + 3 * j * WIDTH + 2;//index

            //         XSetForeground(dis, gc, (data[indexR] << 16) + (data[indexG] << 8) + (data[indexB] << 0));
            //         XDrawPoint(dis, win, gc, i, j);
            //     }
            // }
            // Pixmap bitmap = XCreateBitmapFromData(dis,
            //                                       win, (char *)data,
            //                                       WIDTH, HEIGHT);

            // XCopyPlane(dis, bitmap, win, gc,
            //            0, 0,
            //            WIDTH, HEIGHT,
            //            WIDTH, HEIGHT,
            //            1);
            // XSync(dis, False);
            int xlogo16_width = WIDTH;
            int xlogo16_height = HEIGHT;
            static char xlogo16_bits[] = {
                0x0f, 0x80, 0x1e, 0x80, 0x3c, 0x40, 0x78, 0x20, 0x78, 0x10, 0xf0, 0x08,
                0xe0, 0x09, 0xc0, 0x05, 0xc0, 0x02, 0x40, 0x07, 0x20, 0x0f, 0x20, 0x1e,
                0x10, 0x1e, 0x08, 0x3c, 0x04, 0x78, 0x02, 0xf0};
            // Pixmap bitmap = XCreateBitmapFromData(dis,
            //                                       win, xlogo16_bits,
            //                                       xlogo16_width, xlogo16_height);
            Pixmap bitmap = XCreatePixmapFromBitmapData(dis, win, (char *)data, WIDTH, HEIGHT, 0, 0xFFFFFF, 24);

            /* start drawing the given pixmap on to our window. */
            {
                int i, j;

                for (i = 0; i < 1; i++)
                {
                    for (j = 0; j < 1; j++)
                    {
                        XCopyPlane(dis, bitmap, win, gc,
                                   0, 0,
                                   xlogo16_width, xlogo16_height,
                                   j * xlogo16_width, i * xlogo16_height,
                                   1);
                        XSync(dis, False);
                        usleep(100000);
                    }
                }
            }
            cout << "Image saved at raspicam_image.ppm" << endl;
            //free resrources
            delete data;
        }
    }

    return 0;
}

void init_x()
{
    /* get the colors black and white (see section for details) */
    unsigned long black, white;

    dis = XOpenDisplay((char *)0);
    screen = DefaultScreen(dis);
    black = BlackPixel(dis, screen),
    white = WhitePixel(dis, screen);
    win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0,
                              800, 600, 5, black, white);
    XSetStandardProperties(dis, win, "Howdy", "Hi", None, NULL, 0, NULL);
    XSelectInput(dis, win, ExposureMask | ButtonPressMask | KeyPressMask);
    gc = XCreateGC(dis, win, 0, 0);
    XSetBackground(dis, gc, white);
    XSetForeground(dis, gc, black);
    XClearWindow(dis, win);
    XMapRaised(dis, win);
};

void close_x()
{
    XFreeGC(dis, gc);
    XDestroyWindow(dis, win);
    XCloseDisplay(dis);
    exit(1);
};

void redraw()
{
    XClearWindow(dis, win);
};