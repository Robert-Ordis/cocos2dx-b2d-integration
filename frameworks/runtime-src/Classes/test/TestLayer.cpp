#include <cocos2d.h>
#include "TestLayer.h"
#include "ro-library/b2physics/debug/B2DebugDrawLayer.h"
#include "ro-library/b2physics/utils/B2RangeConverter.h"
USING_NS_CC;

//------static functions------

Scene*	TestLayer::createWithScene() {
	auto	layer = TestLayer::create();
	auto	scene = Scene::create();

	scene->addChild(layer);
	for (int i = 0; i < 0; i++) {
		layer = TestLayer::create();
		layer->setPosition(Vec2(0, 50*i));
		scene->addChild(layer);
	}
	
	auto dlayer = B2DebugDrawLayer::create(layer->getB2World(), layer->getPtmRatio());

	// シーンに子としてレイヤーを追加
	scene->addChild(dlayer);

	return scene;

}

Node *watchnode;
B2NodeComponent *watchcmp;
//------member functions------
bool	TestLayer::init() {
	bool res = B2WorldLayer::init(32.0);
	CCLOG("[%x]%s::%s, init result->%d", this, typeid(this).name(), __func__, res);
	if (!res) {
		return false;
	}

	this->getB2World()->SetContactListener(this);


	Sprite *spr;
	B2NodeComponent *cmp;

	spr = nullptr;
	cmp = nullptr;
	std::tie(spr, cmp) = _makePhysicsNode("HelloWorld.png", 0.5);
	spr->setPosition(Vec2(240, 240));
	spr->setPositionZ(100);
	spr->setRotation(90);
	
	this->addChild(spr);
	//watchnode->addChild(spr);

	CCLOG("spr:%x, cmp:%x", spr, cmp);
	CCLOG("->body:%x, fixtures:%x", cmp->getBody(), cmp->getBody()->GetFixtureList());
	spr = nullptr;
	cmp = nullptr;
	std::tie(spr, cmp) = _makePhysicsNode("HelloWorld.png", 1.2);
	spr->setPosition(Vec2(350, 350));
	spr->setPositionZ(-100);
	spr->setRotation(90);
	cmp->getBody()->SetAngularVelocity(-1);
	cmp->getBody()->SetLinearDamping(1);
	
	
	this->addChild(spr);


	CCLOG("spr:%x, cmp:%x", spr, cmp);
	CCLOG("->body:%x, fixtures:%x", cmp->getBody(), cmp->getBody()->GetFixtureList());
	
	this->scheduleUpdate();


	return true;
}

void	TestLayer::update(float dt) {
	//dt *= 0.5;
	//CCLOG("[%x]%s,%s: rotation->%f", this, typeid(this).name(), __func__, watchnode->getRotation());
	static float elapsed = 0.0;
	static float next = 10;
	this->getB2World()->Step(dt, 1, 1);
	this->_execB2Task();
	elapsed += dt/6;
	
}

std::tuple<Sprite*, B2NodeComponent*> TestLayer::_makePhysicsNode(const std::string& path, float scale) {
	auto	*spr = Sprite::create(path);
	spr->setScale(scale);
	auto	sprSize = spr->getContentSize() * scale;
	b2BodyDef blockBodyRef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

	blockBodyRef.type = b2_dynamicBody;

	shape.SetAsBox(sprSize.width / getPtmRatio() / 2, sprSize.height / getPtmRatio() / 2);
	fixtureDef.shape = &shape;
	fixtureDef.density = 10 * scale;
	fixtureDef.friction = 0.9;
	fixtureDef.restitution = 0;
	
	auto body = getB2World()->CreateBody(&blockBodyRef);
	body->CreateFixture(&fixtureDef);
	auto	component = B2NodeComponent::create(this, body);
	component->setName("B2Component");
	component->setEffectToNode(true);
	spr->addComponent(component);

	return std::tuple<Sprite*, B2NodeComponent*>(spr, component);

}

TestLayer::~TestLayer() {
}

void	TestLayer::BeginContact(b2Contact *contact) {
	//CCLOG("[%x]%s::%s", this, typeid(this).name(), __func__);
	auto bodA = contact->GetFixtureA()->GetBody();
	auto bodB = contact->GetFixtureB()->GetBody();
}
void	TestLayer::EndContact(b2Contact *contact) {
	//CCLOG("[%x]%s::%s", this, typeid(this).name(), __func__);
	auto bodA = contact->GetFixtureA()->GetBody();
	auto bodB = contact->GetFixtureB()->GetBody();

}



void	TestLayer::PreSolve(b2Contact *contact, const b2Manifold* oldManifold) {
	//何とかしてコンポーネントにまでたどり着く。
	auto cmpA = static_cast<Component*>(contact->GetFixtureA()->GetBody()->GetUserData());
	auto cmpB = static_cast<Component*>(contact->GetFixtureB()->GetBody()->GetUserData());
	if (cmpA == nullptr || cmpB == nullptr) {
		CCASSERT(false, "STOOOP!!!");
		return;
	}
	auto nodeA = cmpA->getOwner();
	auto nodeB = cmpB->getOwner();
	CCLOG("[%x]%s::%s, %x vs %x", this, typeid(this).name(), __func__, nodeA, nodeB);


}
void	TestLayer::PostSolve(b2Contact* contact, const b2ContactImpulse *impulse) {
	//CCLOG("[%x]%s::%s", this, typeid(this).name(), __func__);
}




void	TestLayer::SayGoodbye(b2Joint *joint) {
	//CCLOG("[%x]%s::%s good bye (joint) %x", this, typeid(this).name(), __func__, joint);
}

void	TestLayer::SayGoodbye(b2Fixture *fixture) {
	//CCLOG("[%x]%s::%s good bye (fixture) %x", this, typeid(this).name(), __func__, fixture);
}