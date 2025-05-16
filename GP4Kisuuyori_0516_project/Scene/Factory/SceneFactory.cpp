#include "SceneFactory.h"
#include "../SceneBase.h"
#include "../TitleScene.h"
#include "../InGameScene.h"
#include "../RankingScene.h"
#include "../ResultScene.h"
#include "../EndScene.h"

SceneBase* SceneFactory::CreateScene(eSceneType scene)
{
	switch (scene)
	{
	case eTitle:
		return new TitleScene();
	case eInGame:
		return new InGameScene();
	case eRanking:
		return new RankingScene();
	case eResult:
		return new ResultScene();
	case eEnd:
		return new EndScene();
	default:
		break;
	}

	return nullptr;
}
