//
// Created by Tony Horrobin on 18/05/2026.
//

#define Uses_TApplication
#define Uses_TKeys
#define Uses_TSubMenu
#define Uses_TMenuBar
#define Uses_TStatusLine
#define Uses_TStatusItem
#define Uses_TStatusDef
#define Uses_TWindow
#define Uses_TDeskTop
#define Uses_TText
#define Uses_TInputLine

#include <tvision/tv.h>

#include "tvision/menus.h"
#include "tvision/ttext.h"
#include "tvision/internal/ansiwrit.h"
#include "tvision/internal/ansiwrit.h"

class TInterior : public TView
{
public:
    TInterior(const TRect& bounds);
    void draw() override;
};

TInterior::TInterior(const TRect& bounds)
    :
TView(bounds)
{
    growMode = gfGrowHiX | gfGrowHiY;
    options |= ofFramed;
}

void TInterior::draw()
{
    const char* str = "Hello World!";
    ushort colour = getColor(0x0301);
    TView::draw();
    TDrawBuffer b;
    b.moveStr( 0, str, colour );
    writeLine( 4, 2, 12, 1, b);
}
class TDemoWindow : public TWindow
{
public:
    TDemoWindow( const TRect& r, const char* title, short number );
};

TDemoWindow::TDemoWindow(const TRect &bounds, const char *title, short number)
    :
TWindowInit( &TDemoWindow::initFrame),
TWindow(bounds, title, number)
{
    TRect r = getClipRect();
    r.grow(-1,-1);
    //insert( new TInterior( r ) );
    TRect inputBounds(r.a.x, r.a.y, r.b.x, r.a.y+1);
    insert( new TInputLine(inputBounds, 10, nullptr));
}

class TDemoTurboVision : public TApplication
{
public:
    TDemoTurboVision();

    void createWindow();
    void handleEvent( TEvent& e );

    static TMenuBar* initMenuBar(TRect r);
    static TStatusLine* initStatusLine(TRect r);
    static short winNumber;
};

short TDemoTurboVision::winNumber = 0;
const int cmNewWin = 199;

TDemoTurboVision::TDemoTurboVision()
    :
TProgInit(&TDemoTurboVision::initStatusLine, &TDemoTurboVision::initMenuBar, &TDemoTurboVision::initDeskTop)
{
    // Do nothing.
}

void TDemoTurboVision::createWindow()
{
    TRect r(0, 0, 26, 7);
    r.move( 20, 20 );
    auto* window = new TDemoWindow( r, "TDemoWindow", ++winNumber );
    deskTop->insert(window);
}

void TDemoTurboVision::handleEvent(TEvent &event)
{
    TApplication::handleEvent(event);
    if (event.what == evCommand)
    {
        switch (event.message.command)
        {
            case cmNewWin:
                createWindow();
                break;
            default:
                return;
        }
        clearEvent(event);
    }
}

TMenuBar * TDemoTurboVision::initMenuBar(TRect r)
{
    r.b.y = r.a.y + 1;
    return new TMenuBar( r,
        *new TSubMenu("~F~ile", kbAltF) +
            *new TMenuItem( "~O~pen", cmFileOpen, kbF3, hcNoContext, "F3") +
            *new TMenuItem( "~N~ew", cmNewWin, kbF4, hcNoContext, "F4")
            );
}

TStatusLine * TDemoTurboVision::initStatusLine(TRect r)
{
    r.a.y = r.b.y - 1;
    return new TStatusLine(r,
        *new TStatusDef( 0, 0xffff ) +
        *new TStatusItem( "~Alt-X~ Exit", kbAltX, cmQuit ) +
        *new TStatusItem( "~Alt-F3~ Close", kbAltF3, cmClose )
        );
}

int main()
{
    TDemoTurboVision app;

    app.run();

    return 0;
}
