#pragma once
#include "../Base/EffectBase.h"

class Ripples :
	public EffectBase
{
private:

public:
	Ripples();
	~Ripples();

	//‰Šú‰»ˆ—
	void Initialize(ObjectManager* _manager, Vector2D init_location, int _timer, int _anim_span)override;

	//I—¹ˆ—
	void Finalize()override;

	//XVˆ—
	void Update()override;

	//•`‰æˆ—
	void Draw()const override;
};