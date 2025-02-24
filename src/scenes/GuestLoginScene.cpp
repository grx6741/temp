#include "GuestLoginScene.hpp"

#include <pch.h>

#define GAME_KEY "dev_93c481864de44fdcb2772e4a4c2ff425"

void GuestLoginScene::drawFrame()
{
    float centerx = GetScreenWidth() / 2.0f;
    float centery = GetScreenHeight() / 2.0f;
    // float gap = 20;

    float buttonWidth = 150;
    float buttonHeight = 30;

    float boxWidth = 200;
    float boxHeight = 50;

    float topleftx = centerx - buttonWidth / 2;
    float toplefty = centery - buttonHeight / 2 - 0.1 * GetScreenHeight();


    if (
	    GuiTextBox({ topleftx, toplefty, boxWidth, boxHeight }, m_UserName, sizeof(m_UserName), true) || 
	    GuiButton({ topleftx, toplefty + 20 + boxHeight, buttonWidth, buttonHeight }, "Login")
	)
    {
	if (!m_RequestUnderProgress) {
	    InitiateGuestLogin();
	    m_RequestUnderProgress = true;
	}
    }
}

void GuestLoginScene::updateFrame(float dt)
{

}

GuestLoginScene::GuestLoginScene() :
    m_LoggedIn(false),
    m_RequestUnderProgress(false),
    m_FetchedData{0},
    m_UserName{0}
{

}

GuestLoginScene::~GuestLoginScene()
{
    
}

// HTTP Requests

void GuestLoginScene::InitiateGuestLogin() {
    net::RequestBody body;
    body.url = "https://api.lootlocker.io/game/v2/session/guest";
    body.method = net::HTTPMethod::POST;
    body.data = {
	{ "game_key", GAME_KEY },
	{ "game_version", "0.0.0.1" }
    };
    body.headers = {
	"Content-Type", "application/json"
    };
    body.userData = this;

    auto onSuccess = [](emscripten_fetch_t* fetch_data) {
	auto self = static_cast<GuestLoginScene*>(fetch_data->userData);
	self->m_RequestUnderProgress = false;

	const auto data_str = std::string(fetch_data->data, fetch_data->numBytes);

	// const json data = data_str;

	TraceLog(LOG_INFO, "data: %s", data_str.c_str());
	emscripten_fetch_close(fetch_data);
    };

    auto onError = [](emscripten_fetch_t* fetch_data) {
	auto self = static_cast<GuestLoginScene*>(fetch_data->userData);
	self->m_RequestUnderProgress = false;

	TraceLog(LOG_ERROR, "Request failed from %s : [%d] %s", fetch_data->url, fetch_data->status, fetch_data->statusText);
	emscripten_fetch_close(fetch_data);
    };

    auto onProgress= [](emscripten_fetch_t* fetch_data) {
	auto self = static_cast<GuestLoginScene*>(fetch_data->userData);

	if (fetch_data->totalBytes > 0) {
	    self->m_FetchedData.progress = fetch_data->dataOffset / (float)fetch_data->totalBytes;
	}
	TraceLog(LOG_INFO, "Status %f", self->m_FetchedData.progress);
    };

    net::request(body, onSuccess, onError, onProgress);
}
