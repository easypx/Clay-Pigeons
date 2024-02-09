// Clay Pigeon Shooting in wxWidgets 

// TODO:
// - perspective, shadows, sprite scaling
// - multiple bullets & pigeons (shape objects)
// - draw bulletmagazine
// - speed changes

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/event.h>
#include <wx/dc.h>
#include <wx/sound.h>

class BasicDrawPane : public wxPanel
{

public:
    BasicDrawPane(wxFrame *parent);

    void paintEvent(wxPaintEvent &evt);
    void paintNow();
    void render(wxDC &dc);

    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void onMouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */

protected:
    void onTimer(wxCommandEvent &event);
    void onKeyDown(wxKeyEvent &event);
    void onMouseMove(wxMouseEvent &event);
    void onLeftMouseDown(wxMouseEvent &event);
    void onRightMouseDown(wxMouseEvent &event);
    void init();

private:
    wxTimer *timer;
    int i;
    int bulletX, bulletY, targetX, targetY, targetR;
    int bullet, points, speed, direction, bulletsNumber;
    bool isHit, isGameOver, isBulletFlying, isPause;
    int skyR, timeBonus, frameCount, timePerFrame;
    double targetAngle, time;
    wxSound *shoot;
    wxSound *hit;
    wxSound *laugh;
    wxSound *music;
    wxString status;

    DECLARE_EVENT_TABLE()
};

class MyApp : public wxApp
{
    bool OnInit();
    wxFrame *frame;
    BasicDrawPane *drawPane;

public:
};

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    frame = new wxFrame((wxFrame *)NULL, -1, wxT("Clay Pigeon Shooting in wxWidgets!"), wxPoint(50, 50), wxSize(800, 600));

    drawPane = new BasicDrawPane((wxFrame *)frame);
    sizer->Add(drawPane, 1, wxEXPAND);

    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);

    frame->Show();

    return true;
}

BEGIN_EVENT_TABLE(BasicDrawPane, wxPanel)
// some useful events
/*
 EVT_MOTION(BasicDrawPane::mouseMoved)
 EVT_LEFT_DOWN(BasicDrawPane::onMouseDown)
 EVT_LEFT_UP(BasicDrawPane::mouseReleased)
 EVT_RIGHT_DOWN(BasicDrawPane::rightClick)
 EVT_LEAVE_WINDOW(BasicDrawPane::mouseLeftWindow)
 EVT_KEY_DOWN(BasicDrawPane::keyPressed)
 EVT_KEY_UP(BasicDrawPane::keyReleased)
 EVT_MOUSEWHEEL(BasicDrawPane::mouseWheelMoved)
 */

// catch paint events
EVT_PAINT(BasicDrawPane::paintEvent)

END_EVENT_TABLE()

// some useful events
/*
 void BasicDrawPane::mouseMoved(wxMouseEvent& event) {}
 void BasicDrawPane::onMouseDown(wxMouseEvent& event) {}
 void BasicDrawPane::mouseWheelMoved(wxMouseEvent& event) {}
 void BasicDrawPane::mouseReleased(wxMouseEvent& event) {}
 void BasicDrawPane::rightClick(wxMouseEvent& event) {}
 void BasicDrawPane::mouseLeftWindow(wxMouseEvent& event) {}
 void BasicDrawPane::keyPressed(wxKeyEvent& event) {}
 void BasicDrawPane::keyReleased(wxKeyEvent& event) {}
 */

BasicDrawPane::BasicDrawPane(wxFrame *parent) : wxPanel(parent)
{
    timer = new wxTimer(this, 1); // The screen update timer.

    // Connect some events to functions. (This is the long method to event table.)
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(BasicDrawPane::onKeyDown));
    Connect(wxEVT_TIMER, wxCommandEventHandler(BasicDrawPane::onTimer));
    Connect(wxEVT_MOTION, wxMouseEventHandler(BasicDrawPane::onMouseMove));
    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(BasicDrawPane::onLeftMouseDown));
    Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(BasicDrawPane::onRightMouseDown));

    SetDoubleBuffered(true); // Could improve flickering. (Don't know if it is default doublebuffered.)

    // Redirects a log-messagebox to console
    wxLog *logger = new wxLogStream(&std::cout);
    wxLog::SetActiveTarget(logger);

    shoot = new wxSound(_T("shot.wav"), false);
    hit = new wxSound(_T("hit.wav"), false);
    laugh = new wxSound(_T("laugh.wav"), false);
    music = new wxSound(_T("music.wav"), false);

    init(); // Initialize some game variables.
}

void BasicDrawPane::init()
{
    timePerFrame = 20; // in ms
    time = 0.0;
    timeBonus = 0;
    frameCount = 0;
    targetX = 800;
    targetY = 400;
    bullet = 0;
    bulletX = 400;
    bulletY = 500;
    targetR = 30;
    isHit = false;
    points = 0;
    isGameOver = false;
    speed = 1;
    direction = 1;
    isBulletFlying = false;
    status = "New game! Press RMB to start and LMB to fire!";
    skyR = 400;
    targetAngle = 0.0;
    bulletsNumber = 30;
    isPause = false;
    music->Play(1);
    isPause = true;
}

void BasicDrawPane::onLeftMouseDown(wxMouseEvent &event)
{
    bullet += 1;
    bulletsNumber--;
    isBulletFlying = true;
    shoot->Play(1); // Play async (1)
    bulletY = 500;  // Reset to start value
    // Get mouse position.
    wxPoint pt = wxGetMousePosition();
    pt.x = bulletX; // Reset to mouse x-position

    event.Skip();
}

void BasicDrawPane::onRightMouseDown(wxMouseEvent &event)
{
    isPause = false;
    timer->Start(timePerFrame);
    status = "Let's go!";

    if (isGameOver)
    {
        init();
    }
    event.Skip();
}

void BasicDrawPane::onMouseMove(wxMouseEvent &event)
{
    if (!isBulletFlying)
    {
        wxPoint pt = wxGetMousePosition();
        bulletX = pt.x;
    }
}

void BasicDrawPane::onKeyDown(wxKeyEvent &event)
{
    int keycode = event.GetKeyCode();
    // wxChar uc = event.GetUnicodeKey();
    // wxLogMessage("You pressed '%c'", uc);

    switch (keycode)
    {
    // TODO: You have to press Alt + Arrow! (Don't know why?!)
    case WXK_LEFT:
        bulletX -= 10;
        break;
    case WXK_RIGHT:
        bulletX += 10;
        break;
    case WXK_DOWN:
        break;
    case WXK_UP:
        break;
    case WXK_SPACE:
        bullet += 1;
        break;
    case 'R':
        //init();
        break;
    case 'P':
        isPause = true;
        status = "Game paused. Press RMB to continue!";
        break;
    default:
        event.Skip();
    }
}

void BasicDrawPane::onTimer(wxCommandEvent &event)
{
    frameCount++;
    time = ((double)frameCount / 1000.0 * timePerFrame);
    wxLogMessage("frameCount: %i time: %f", frameCount, time);

    // Draw a point on a circle:
    // x = r * cos(beta)
    // y = r * sin(beta)

    if (direction == 1)
        //targetX = targetX + 1 * speed; // Simple linear move
        targetAngle = (targetAngle + 3.141 / 180.0) /** speed*/;
    if (targetAngle >= 3.141)
    {
        targetAngle = 3.141;
        direction = -1;
    }
    targetX = skyR * cos(targetAngle);
    targetX += skyR;
    targetY = skyR * sin(targetAngle);
    targetY = (targetY * -1.0) + skyR;

    if (direction == -1)
        targetAngle = (targetAngle - 3.141 / 180.0) /**speed*/;
    if (targetAngle <= 0)
    {
        targetAngle = 0.0;
        direction = 1;
    }
    //targetX = targetX - 1 * speed;
    targetX = skyR * cos(targetAngle);
    targetX += skyR;
    targetY = skyR * sin(targetAngle);
    targetY = (targetY * -1.0) + skyR;

    if (bulletY >= 0 && isBulletFlying)
    {
        for (size_t i = 0; i < bullet; i++)
        {
            bulletY -= 20;
        }
    }
    else
    {
        bulletY = 500; // Reset to start value
        bullet = 0;
        isBulletFlying = false;
    }

    // Collision detection with target
    if (bulletY <= (targetY + targetR) && (bulletX <= targetX + (targetR - 10) && bulletX >= targetX - (targetR - 10) /*- targetR*/))
    {
        isHit = true;
    }

    if (bulletY < 0)
    {
        status = "Try harder!";
        laugh->Play(1); // Play async (1)
        if (bulletsNumber <= 0)
        {
            isGameOver = true;
        }
    }

    if (isHit)
    {
        status = "Hit!";
        points += 10;
        //speed++; // We implement this later.
        isHit = false;
        isBulletFlying = false;

        // Change direction
        if (direction == 1)
        {
            direction = -1;
            targetAngle = 3.141;
        }
        else
        {
            direction = 1;
            targetAngle = 0;
        }

        hit->Play(1); // Play async (1)
    }

    if (isGameOver)
    {
        timeBonus = (int)(30.0 / time * 30); // We assume a normal playtime of 30s
        points += timeBonus;                 // Add this to points (100s = +100, 200s = +50 ...)
        status = "Game Over! Press RMB to restart!";
        timer->Stop();
        music->Play(1);
        isPause = true;
    }

    if (bulletsNumber <= 0) isGameOver = true;

    Refresh(); // Repaint
}
/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void BasicDrawPane::paintEvent(wxPaintEvent &evt)
{
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 *
 * In most cases, this will not be needed at all; simply handling
 * paint events and calling Refresh() when a refresh is needed
 * will do the job.
 */
void BasicDrawPane::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void BasicDrawPane::render(wxDC &dc)
{
    // draw environment
    dc.SetBrush(*wxCYAN_BRUSH); // sky
    dc.SetPen(wxPen(wxColor(0, 255, 255), 1));
    dc.DrawRectangle(0, 0, 800, 300);
    dc.SetBrush(*wxGREEN_BRUSH); // gras
    dc.SetPen(wxPen(wxColor(0, 255, 0), 1));
    dc.DrawRectangle(0, 300, 800, 300);
    dc.SetBrush(*wxYELLOW_BRUSH); // sun
    dc.SetPen(wxPen(wxColor(255, 255, 0), 1));
    dc.DrawCircle(wxPoint(400, 100), 40 /* radius */);

    // draw a pigeon
    dc.SetBrush(*wxWHITE_BRUSH);
    dc.SetPen(wxPen(wxColor(255, 255, 255), 1));
    dc.DrawCircle(wxPoint(targetX, targetY), 30 /* radius */);

    // draw a bullet
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.SetPen(wxPen(wxColor(0, 0, 0), 1));
    dc.DrawCircle(wxPoint(bulletX, bulletY), 10 /* radius */);

    if (isGameOver)
    {
       dc.DrawText(wxString::Format(wxT("%s%i"), "Timebonus: ", timeBonus), 40, 140); 
    }

    // Print some text
    dc.DrawText(wxString::Format(wxT("%s%s"), "Status: ", status), 40, 60);
    dc.DrawText(wxString::Format(wxT("%s%i"), "Points: ", points), 40, 80);
    dc.DrawText(wxString::Format(wxT("%s%i"), "Bullets: ", bulletsNumber), 40, 100);
    dc.DrawText(wxString::Format(wxT("%s%.2f s"), "Time: ", time), 40, 120);
}