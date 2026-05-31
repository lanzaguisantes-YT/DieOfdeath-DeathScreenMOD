#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <cocos2d.h>

using namespace geode::prelude;

static const std::vector<std::string> DEATH_MESSAGES = {
    "Was Pursued to their death.",
    "Died because of a sneaky spike.",
    "Died thinking too hard.",
    "Died because they forgot to breathe.",
    "Died of excess potassium.",
    "Died because grandma called them for dinner.",
    "Died because they blinked at the wrong time.",
    "Died because it was Monday.",
    "Died because they trusted the process.",
    "Died because a pigeon distracted them.",
    "Died because existing is hard.",
    "Died because the level hates them personally.",
    "Died because their finger got stuck.",
    "Died because they ran out of luck.",
    "Died because they looked away for 0.1 seconds.",
    "Died because the spike looked friendly.",
    "Died because gravity is a cruel mistress.",
    "Died because they sneezed.",
    "Died because they forgot to pay the wifi bill.",
    "Died because fate had other plans."
};

class DeathOverlay : public CCLayer {
protected:
    std::string m_playerName;
    CCSprite* m_animSprite = nullptr;
    CCLabelSystemFont* m_title = nullptr;
    CCLabelSystemFont* m_msg = nullptr;
    int m_currentFrame = 0;
    int m_totalFrames = 27;

public:
    static DeathOverlay* create(const std::string& playerName) {
        auto ret = new DeathOverlay();
        ret->m_playerName = playerName;
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init() {
        if (!CCLayer::init()) return false;

        auto winSize = CCDirector::get()->getWinSize();

        // Fondo negro total
        auto bg = CCLayerColor::create({0, 0, 0, 255});
        bg->setContentSize(winSize);
        this->addChild(bg, 0);

        // Sprite animado - empieza con frame_000
        m_animSprite = CCSprite::create("frame_000.png"_spr);
        if (!m_animSprite) {
            m_animSprite = CCSprite::create("player_01_001.png");
        }
        if (m_animSprite) {
            m_animSprite->setPosition({winSize.width * 0.32f, winSize.height * 0.5f});
            float scale = (winSize.height * 0.75f) / m_animSprite->getContentSize().height;
            m_animSprite->setScale(scale);
            m_animSprite->setOpacity(0);
            this->addChild(m_animSprite, 1);
        }

        // Titulo "IN MEMORY OF [nombre]"
        std::string titleText = "IN MEMORY OF " + m_playerName;
        m_title = CCLabelSystemFont::create(titleText.c_str(), "Arial Bold", 26);
        m_title->setPosition({winSize.width * 0.68f, winSize.height * 0.58f});
        m_title->setColor({255, 255, 255});
        m_title->setAnchorPoint({0.5f, 0.5f});
        m_title->setOpacity(0);
        this->addChild(m_title, 1);

        // Causa de muerte aleatoria
        int idx = rand() % DEATH_MESSAGES.size();
        m_msg = CCLabelSystemFont::create(DEATH_MESSAGES[idx].c_str(), "Arial", 16);
        m_msg->setPosition({winSize.width * 0.68f, winSize.height * 0.46f});
        m_msg->setColor({160, 160, 160});
        m_msg->setAnchorPoint({0.5f, 0.5f});
        m_msg->setOpacity(0);
        this->addChild(m_msg, 1);

        // Empezar animación después de 1 segundo (tiempo respawn)
        this->scheduleOnce(schedule_selector(DeathOverlay::startAnimation), 1.0f);

        // Auto cerrar en 6 segundos
        this->scheduleOnce(schedule_selector(DeathOverlay::removeSelf), 6.0f);

        // Touch para cerrar
        auto listener = CCEventListenerTouchOneByOne::create();
        listener->onTouchBegan = [this](CCTouch*, CCEvent*) {
            this->unscheduleAllSelectors();
            this->removeFromParentAndCleanup(true);
            return true;
        };
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

        return true;
    }

    void startAnimation(float) {
        // Fade in rápido del sprite y texto simultáneamente
        if (m_animSprite) m_animSprite->runAction(CCFadeIn::create(0.4f));
        if (m_title) m_title->runAction(CCFadeIn::create(0.4f));
        if (m_msg) m_msg->runAction(CCSequence::create(
            CCDelayTime::create(0.1f),
            CCFadeIn::create(0.4f),
            nullptr
        ));

        // Animar frames
        this->schedule(schedule_selector(DeathOverlay::nextFrame), 0.05f);
    }

    void nextFrame(float) {
        m_currentFrame = (m_currentFrame + 1) % m_totalFrames;
        std::string frameName = fmt::format("frame_{:03d}.png", m_currentFrame);
        
        if (m_animSprite) {
            auto tex = CCTextureCache::get()->addImage(
                Mod::get()->getResourcesDir().append(frameName).string().c_str(), false
            );
            if (tex) m_animSprite->setTexture(tex);
        }
    }

    void removeSelf(float) {
        this->unscheduleAllSelectors();
        this->runAction(CCSequence::create(
            CCFadeOut::create(0.4f),
            CCCallFunc::create(this, callfunc_selector(DeathOverlay::doRemove)),
            nullptr
        ));
    }

    void doRemove() {
        this->removeFromParentAndCleanup(true);
    }
};

class $modify(MyPlayLayer, PlayLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* obj) {
        PlayLayer::destroyPlayer(player, obj);

        std::string playerName = "Unknown";
        if (auto gm = GameManager::get()) {
            playerName = gm->m_playerName;
        }

        auto overlay = DeathOverlay::create(playerName);
        if (overlay) {
            overlay->setZOrder(1000);
            this->addChild(overlay);
        }
    }
};
