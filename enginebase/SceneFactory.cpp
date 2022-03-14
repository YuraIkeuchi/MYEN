#include "SceneFactory.h"
#include "GamePlayScene.h"
#include "TitleScene.h"
#include "ClearScene.h"
#include "GameoverScene.h"
BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//éüÇÃÉVÅ[ÉìÇÃçÏê¨
	BaseScene* newScene = nullptr;
	if (sceneName=="TITLE") {
		newScene=new TitleScene();
	}
	else if(sceneName == "GAMEPLAY"){
		newScene = new GamePlayScene();
	}
	else if (sceneName == "GAMEOVER") {
		newScene = new GameoverScene();
	}
	else if (sceneName == "GAMECLEAR") {
		newScene = new ClearScene();
	}
	return newScene;
}
