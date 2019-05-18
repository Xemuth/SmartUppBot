#include <Core/Core.h>
#include "SmartBotUpp.h"

using namespace Upp;


#include <Discord_Overwatch/Discord_Overwatch.h>
#include <Discord_Minecraft/Discord_Minecraft.h>
#include <Discord_DrawPackage/Discord_DrawPackage.h>
//Module OverWatch : https://github.com/Xemuth/Discord_Overwatch
//Module Minecraft : https://github.com/Xemuth/Discord_Minecraft
//Module DrawPackage : https://github.com/Xemuth/Discord_DrawPackage
CONSOLE_APP_MAIN {
	StdLogSetup(LOG_COUT|LOG_FILE);
	SmartBotUpp mybot("314391413200650250","MzE0MzkxNDEzMjAwNjUwMjUw.XLbeew.4-EvNJLFiPGMVoZ6s7pTnoqEObc");
	
	Discord_Overwatch ow("OverWatch","ow");
	mybot.AddModule(&ow);
	
	Discord_Minecraft mc("Minecraft","mc");
	mybot.AddModule(&mc);
	
	Discord_DrawPackage dr("Draw","d");
	mybot.AddModule(&dr);
	
	mybot.Launch();
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