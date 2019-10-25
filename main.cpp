#include <Core/Core.h>
#include "SmartBotUpp.h"
#include <EasyConfiguration/EasyConfiguration.h>

#ifdef flagOVERWATCH
	#include <Discord_Overwatch/Discord_Overwatch.h>
#endif
#ifdef flagMINECRAFT
	#include <Discord_Minecraft/Discord_Minecraft.h>
#endif
#ifdef flagRNG
	#include <Discord_RNG/Discord_RNG.h>
#endif


using namespace Upp;
//Module OverWatch : https://github.com/Xemuth/Discord_Overwatch
//Module Minecraft : https://github.com/Xemuth/Discord_Minecraft
//Module RNG : https://github.com/Xemuth/Discord_RNG
//Module GraphBuilder : https://github.com/Xemuth/GraphBuilder
//EasyConfiguration : https://github.com/Xemuth/EasyConfiguration

CONSOLE_APP_MAIN {
	StdLogSetup(LOG_COUT|LOG_FILE);
	#ifdef flagLINUX
		String CfgFile = R"(/home/Xemuth/token.cfg)";
	#else
		String CfgFile = R"(C:/discordTokens.txt)";
	#endif
	
	EasyConfiguration ez(R"(C:/discordTokens.txt)");
	if(ez.GetCount() >= 2){
		SmartBotUpp mybot(ez.GetValue<String>("BotId"),ez.GetValue<String>("BotToken"));
		
		#ifdef flagOVERWATCH
			mybot.CreateModule<Discord_Overwatch>("OverWatch","ow");
		#endif
		
		#ifdef flagMINECRAFT
			#ifdef flagLINUX
				String RconFile = R"(/home/Xemuth/rconLogs.cfg)";
			#else
				String RconFile = R"(C:/rconLogs.txt)";
			#endif
			mybot.CreateModule<Discord_Minecraft>("Minecraft","mc",RconFile);
		#endif
		
		#ifdef flagRNG
			mybot.CreateModule<Discord_RNG>("RNG", "rng");
		#endif
		
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
