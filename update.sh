git pull
cd ..


FILE=./Discord_Minecraft
if [ -f "$FILE" ]; then
	cd Discord_Minecraft
	git pull
	cd ..
else 
	git clone https://github.com/Xemuth/Discord_Minecraft
fi

FILE=./Discord_Overwatch
if [ -f "$FILE" ]; then
	 git clone https://github.com/Xemuth/Discord_Overwatch
else 
   cd Discord_Overwatch
	git pull
	cd ..
fi

FILE=./Discord_RNG
if [ -f "$FILE" ]; then
	git clone https://github.com/Xemuth/Discord_RNG
else 

	cd Discord_RNG
	git pull
	cd ..
    
fi

FILE=./Discord_WOW
if [ -f "$FILE" ]; then
	git clone https://github.com/Xemuth/Discord_WOW
else 
	cd Discord_WOW
	git pull
	cd ..
    
fi

FILE=./EasyConfiguration
if [ -f "$FILE" ]; then
	git clone https://github.com/Xemuth/EasyConfiguration
else 
	cd EasyConfiguration
	git pull
	cd ..
fi

FILE=./GraphBuilder
if [ -f "$FILE" ]; then
	git clone https://github.com/Xemuth/GraphBuilder
else 
    cd GraphBuilder
	git pull 
	cd ..
fi


FILE=./RconManager
if [ -f "$FILE" ]; then

	git clone https://github.com/Xemuth/RconManager
else 
    cd RconManager
	git pull
	cd ..
fi