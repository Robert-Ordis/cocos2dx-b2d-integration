#ifndef _TEST_LAYER_H
#define	_TEST_LAYER_H

#include <cocos2d.h>
#include <Box2D/Box2D.h>
#include "ro-library/b2physics/world/B2WorldLayer.h"

USING_NS_CC;

class TestLayer :public B2WorldLayer{
public:
	virtual bool	init();

	virtual void	update(float dt);

	CREATE_FUNC(TestLayer);
	
	static Scene*	createWithScene();

	virtual	~TestLayer();

public:		//B2WorldLayerからの継承
	virtual void	BeginContact(b2Contact *contact);
	virtual void	EndContact(b2Contact *contact);
	virtual void	PreSolve(b2Contact *contact, const b2Manifold* oldManifold);
	virtual void	PostSolve(b2Contact* contact, const b2ContactImpulse *impulse);
	virtual void	SayGoodbye(b2Joint *joint);
	virtual void	SayGoodbye(b2Fixture *fixture);


private:
	std::tuple<Sprite*, B2NodeComponent*> _makePhysicsNode(const std::string& path, float scale);
};


#endif // !_TEST_LAYER_H

