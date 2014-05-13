// Author: Michael Terry

#include "XApplication.h"

#include <X11/Xutil.h>
#include <unistd.h> // For usleep

#include "XWindow.h"
#include "SchemeInterpreter.h"
#include "Logging.h"
#include "KeyEvent.h"
#include "StopWatch.h"

namespace cs349
{

  /**
   * This is a callback from X whenever it encounters a serious
   * error (such as when the user closes the window). You can
   * ignore it
   */
  int ErrorHandler(Display* display);

  XApplication* XApplication::s_Instance = NULL;

  int ErrorHandler(Display* display)
  {
    return XApplication::GetInstance()->HandleXError(display);
  }

  XApplication::XApplication()
  {
    // Initialize the X display and get the default screen
    display = XOpenDisplay("");
    if (!display) {
      LOG_FATAL << "Error: Couldn't open X display";
      return;
    }
    screen = DefaultScreen(display);
    quit   = false;
  }

  XApplication::~XApplication()
  {
    ; // No-op
  }

  XApplication* XApplication::GetInstance()
  {
    if (!XApplication::s_Instance) {
      s_Instance = new XApplication();
    }
    return s_Instance;
  }


  unsigned long XApplication::GetBlackColor() const
  {
    return BlackPixel(this->display, this->screen);
  }

  int XApplication::GetDefaultScreen() const
  {
    return this->screen;
  }

  EventQueue* XApplication::GetEventQueue()
  {
    return &eventQueue;
  }

  unsigned long XApplication::GetWhiteColor() const
  {
    return WhitePixel(this->display, this->screen);
  }

  Display* XApplication::GetXDisplay() const
  {
    return this->display;
  }

  int XApplication::HandleXError(Display* display)
  {
    /*
      This method is called whenever there is a serious
      X error. This method will be called when the user
      manually closes the window. You could clean up
      the event queue, release memory, and anything else
      needed before the application quits, because once
      this method returns, X will forcibly terminate
      the entire application.
    */

    // Shut down the Scheme interpreter so it cleanly exits readline.
    // Otherwise, it puts the terminal in a funky state
    scheme_interpreter_shutdown();
    return -1;
  }

  void XApplication::Quit()
  {
    quit = true;
  }


  void XApplication::CheckForXEvents() {

    /*
     * TODO for CS349 assignment:
     * Check if there is an X event pending. If so, package it up
     * using our own Event classes, and add it to the eventQueue.
     *
     * See the documentation at
     * http://tronche.com/gui/x/xlib/events/types.html to learn how to
     * identify the different types of X events.
     *
     * One thing you may be wondering is why we package up an event
     * and add it to the event queue, rather than dispatching it right
     * away. The reason is that events may be added to the queue by
     * other objects (such as TimerEvents). Thus, to preserve proper
     * temporal ordering of events, you need to add events to the
     * queue and dispath them one by one.
     */
      bool button_pressed = false;
    if (XPending(this->display)) {
      // Get event, package it up, add it to the queue
      XEvent event;
      Event* newEvent = NULL;
      XNextEvent(this->display, &event);
      XWindow* window = XWindow::GetXWindowForWindow(event.xany.window);
      KeySym key;
      char key2;
        int count;
        char buffer[500];
      LOG_TODO << "TODO CS349: Implement XApplication::CheckForXEvents (remove when implemented)";
      switch (event.type) {
// TODO CS349
          case Expose://handle expose event
              newEvent = new PaintEvent(window, window->GetBounds());
              break;
          case ConfigureNotify:
              window->GetGraphics()->SetClipRect(Rectangle(0, 0, event.xconfigure.width, event.xconfigure.height));
              newEvent = new PaintEvent(window, Rectangle(0, 0, event.xconfigure.width, event.xconfigure.height));
              break;
          case KeyPress://handle key press event
              count = XLookupString((XKeyEvent*)&event, buffer, 500, &key, NULL);
              key2 = (char)key;
              newEvent = new KeyEvent(window,KeyEvent::keyPress,key2);
              break;
          case KeyRelease:
              count = XLookupString((XKeyEvent*)&event, buffer, 500, &key, NULL);
              key2 = (char)key;
              newEvent = new KeyEvent(window,KeyEvent::keyRelease,key2);
              break;
          case ButtonPress:
              newEvent = new MouseEvent(window,MouseEvent::mouseDown,Point(event.xbutton.x, event.xbutton.y));
              button_pressed = true;
              break;
          case ButtonRelease:
              newEvent = new MouseEvent(window,MouseEvent::mouseUp,Point(event.xbutton.x, event.xbutton.y));
              button_pressed = false;
              break;
          case MotionNotify:
              if(button_pressed){
                  newEvent = new MouseEvent(window,MouseEvent::mouseDrag,Point(event.xmotion.x, event.xmotion.y));
              }
              else{
                  newEvent = new MouseEvent(window,MouseEvent::mouseMove,Point(event.xmotion.x, event.xmotion.y));
              }
              break;
          case DestroyNotify:
              LOG(INFO) << "Destroy notify event received";
              //this->Quit();
              break;
      }

      if (newEvent) {
        eventQueue.AddEventToQueue(newEvent);
      }
    }
  }

  void XApplication::Run()
  {
    XSetIOErrorHandler(ErrorHandler);
    /*
     * This is where you will implement the event loop. The basic
     * logic of the event loop is as follows:
     * - You should service the CS349 Scheme interpreter to ensure it
     *   progresses (executes any scripts, accepts any user keyboard
     *   input) (already done for you)
     * - Check the return code for scheme_interpreter_service. If it
     *   returns a non-zero value, the application should quit
     * - You need to determine whether there are any events pending in
     *   X lib (call CheckForXEvents()). (already done for you)
     * - You should call EventQueue's ProcessNextEvent() method to
     *   process events and service the timers. Note, you should call
     *   this *even if there are no events pending*, since it will
     *   service the timers.
     * - You'll need to flush X using XFlush() at some point so it
     *   actually communicates changes to the X server
     * - You should sleep if there is nothing left to do, so the app
     *   is not throttling the CPU. Before sleeping, make sure you
     *   check whether any X events are pending using XPending(). If
     *   there are, you shouldn't sleep.
     * - If you sleep, sleep for a very small amount of time, say
     *   1/1000 of a second, to enable interactive typing rates on the
     *   console. (See the usleep function for fine-grained sleep
     *   amounts).
     * - We *will* check to ensure your app doesn't throttle the CPU
     *   when idle, so make sure you sleep when there are no events.
     */
    while (!this->quit) {
      int interpreterReturnCode = 0;

      interpreterReturnCode = scheme_interpreter_service();
      if (interpreterReturnCode == 1) {
        this->Quit();
      }
        LOG_TODO << "TODO CS349: Implement event loop in XApplication::Run (remove when implemented)";
        CheckForXEvents();
        eventQueue.ProcessNextEvent();
        XFlush(display);
        usleep(10000);
        //window->Repaint(Rectangle(0,0,500,500));
// TODO CS349
    }
  }
}
