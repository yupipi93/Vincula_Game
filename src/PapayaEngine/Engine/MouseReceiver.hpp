struct MouseReceiver {
    float mouseX;
    float mouseY;
    bool LeftButtonDown;
    bool RightButtonDown;
    bool MiddleButtonDown;
    bool ScrollUp;
    bool ScrollDown;
    
    MouseReceiver() : ScrollUp(false), ScrollDown(false), MiddleButtonDown(false), RightButtonDown(false), LeftButtonDown(false), mouseX(0.f), mouseY(0.f) { }
};