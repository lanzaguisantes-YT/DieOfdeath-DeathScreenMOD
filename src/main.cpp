#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/CCParticleSystemQuad.hpp>
#include <cocos2d.h>

using namespace geode::prelude;

static CCLabelBMFont* g_fpsLabel = nullptr;
static int g_frameCount = 0;
static float g_fpsTimer = 0.f;

class $modify(CCParticleSystemQuad) {
    bool initWithTotalParticles(unsigned int numberOfParticles, bool p1) {
        if (Mod::get()->getSettingValue<bool>("disable-particles")) {
            return false;
        }
        return CCParticleSystemQuad::initWithTotalParticles(numberOfParticles, p1);
    }
};

class $modify(MyPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (Mod::get()->getSettingValue<bool>("disable-backgrounds")) {
            if (auto bg = this->getChildByID("background")) {
                bg->setVisible(false);
            }
        }

        if (Mod::get()->getSettingValue<bool>("show-fps")) {
            g_fpsLabel = CCLabelBMFont::create("FPS: 0", "bigFont.fnt");
            g_fpsLabel->setScale(0.4f);
            g_fpsLabel->setOpacity(200);
            g_fpsLabel->setPosition({50.f, CCDirector::get()->getWinSize().height - 15.f});
            g_fpsLabel->setZOrder(999);
            this->addChild(g_fpsLabel);
            g_frameCount = 0;
            g_fpsTimer = 0.f;
        }

        return true;
    }

    void update(float dt) {
        PlayLayer::update(dt);

        if (Mod::get()->getSettingValue<bool>("show-fps") && g_fpsLabel) {
            g_frameCount++;
            g_fpsTimer += dt;
            if (g_fpsTimer >= 0.5f) {
                int fps = (int)(g_frameCount / g_fpsTimer);
                g_fpsLabel->setString(fmt::format("FPS: {}", fps).c_str());
                g_frameCount = 0;
                g_fpsTimer = 0.f;
            }
        }
    }

    void onQuit() {
        g_fpsLabel = nullptr;
        PlayLayer::onQuit();
    }
};

$on_mod(Loaded) {
    int fpsLimit = Mod::get()->getSettingValue<int>("fps-limit");
    CCDirector::get()->setAnimationInterval(1.0 / fpsLimit);
}
