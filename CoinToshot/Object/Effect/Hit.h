#pragma once
#include "../Base/EffectBase.h"

class Hit :
	public EffectBase
{
private:
	
public:
	Hit();
	~Hit();

	//‰Šú‰»ˆ—
	void Initialize(ObjectManager* _manager, Vector2D init_location, bool _front_flg, int _timer, int _anim_span, Vector2D init_velocity)override;

	//I—¹ˆ—
	void Finalize()override;

	//XVˆ—
	void Update()override;

	//•`‰æˆ—
	void Draw()const override;
};