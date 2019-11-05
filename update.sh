git pull
cd ..


FILE=Discord_Minecraft
if [ -f "$FILE" ]; then
    cd Discord_Minecraft
	git pull
	cd ..
else 
    git clone https://github.com/Xemuth/Discord_Minecraft
fi

FILE=Discord_Overwatch
if [ -f "$FILE" ]; then
	cd Discord_Overwatch
	git pull
	cd ..
else 
    git clone https://github.com/Xemuth/Discord_Overwatch
fi

FILE=Discord_RNG
if [ -f "$FILE" ]; then
	cd Discord_RNG
	git pull
	cd ..
else 
    git clone https://github.com/Xemuth/Discord_RNG
fi

FILE=Discord_WOW
if [ -f "$FILE" ]; then
	cd Discord_WOW
	git pull
	cd ..
else 
    git clone https://github.com/Xemuth/Discord_WOW
fi

FILE=./EasyConfiguration
if [ -f "$FILE" ]; then
	cd EasyConfiguration
	git pull
	cd ..
else 
    git clone https://github.com/Xemuth/EasyConfiguration
fi

FILE=GraphBuilder
if [ -f "$FILE" ]; then
	cd GraphBuilder
	git pull 
	cd ..
else 
    git clone https://github.com/Xemuth/GraphBuilder
fi


FILE=RconManager
if [ -f "$FILE" ]; then
	cd RconManager
	git pull
	cd ..
else 
    git clone https://github.com/Xemuth/RconManager
fi