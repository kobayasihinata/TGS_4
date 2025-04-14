#include "SceneFactory.h"
#include "../SceneBase.h"
#include "../TitleScene.h"
#include "../InGameScene.h"
#include "../ResultScene.h"
#include "../Editor/EditScene.h"

SceneBase* SceneFactory::CreateScene(eSceneType scene)
{
	switch (scene)
	{
	case eTitle:
		return new TitleScene();
	case eInGame:
		return new InGameScene();
	case eResult:
		return new ResultScene();
	case eEdit:
		return new EditScene();
	default:
		break;
	}

	return nullptr;
}
