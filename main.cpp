#include <Core/Core.h>
#include "SmartBotUpp.h"

#include <Discord_Overwatch/Discord_Overwatch.h>
#include <EasyConfiguration/EasyConfiguration.h>
#include <Discord_Minecraft/Discord_Minecraft.h>
#include <Discord_RNG/Discord_RNG.h>

using namespace Upp;
//Module OverWatch : https://github.com/Xemuth/Discord_Overwatch
//Module Minecraft : https://github.com/Xemuth/Discord_Minecraft
//Module RNG : https://github.com/Xemuth/Discord_RNG
//Module GraphBuilder : https://github.com/Xemuth/GraphBuilder
//EasyConfiguration : https://github.com/Xemuth/EasyConfiguration

CONSOLE_APP_MAIN {
	StdLogSetup(LOG_COUT|LOG_FILE);
	EasyConfiguration ez(R"(C:/discordTokens.txt)");
	if(ez.GetCount() >= 2){
		SmartBotUpp mybot(ez.GetValue<String>("BotId"),ez.GetValue<String>("BotToken"));
		
		Discord_Overwatch ow("OverWatch","ow");
		mybot.AddModule(&ow);
	
		Discord_Minecraft mc("Minecraft","mc");
		mybot.AddModule(&mc);
	
	//	Discord_RNG rng("RNG", "rng");
	//	mybot.AddModule(&rng);
		
		mybot.Launch();
	}else{
		LOG( "Le fichier de configuration est invalide !\n"); 	
	}
}

/* 
Project created 14/05/2019
By Clément Hamon 
Lib used to give life to the Smartest bot ever ! (not even joking)
This project have to be used with Ultimate++ FrameWork and required the Core Librairy from it
http://www.ultimatepp.org
Copyright © 1998, 2019 Ultimate++ team
All those sources are contained in "plugin" directory. Refer there for licenses, however all libraries have BSD-compatible license.
Ultimate++ has BSD license:
License : https://www.ultimatepp.org/app$ide$About$en-us.html
Thanks to UPP team !
*/
