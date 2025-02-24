#include "Scene.hpp"

#include "net.hpp"

class GuestLoginScene : public Scene {
public:
    GuestLoginScene();
    ~GuestLoginScene();
    virtual void drawFrame();
    virtual void updateFrame(float dt);

private:
    void InitiateGuestLogin();

private:
    bool m_LoggedIn;

    bool m_RequestUnderProgress;
    net::FetchedData m_FetchedData;

    char m_UserName[20];
};
