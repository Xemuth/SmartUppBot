@echo off
git pull
cd ..
if EXIST ./Discord_Minecraft (
	echo "Pulling Discord_Minecraft"
    cd Discord_Minecraft
	git pull
	cd ..
) else ( 
	echo "Creating Discord_Minecraft"
    git clone https://github.com/Xemuth/Discord_Minecraft
)
if EXIST ./Discord_Overwatch (
	echo "Pulling Discord_Overwatch"
    cd Discord_Overwatch
	git pull
	cd ..
) else ( 
	echo "Creating Discord_Overwatch"
    git clone https://github.com/Xemuth/Discord_Overwatch
)
if EXIST  ./Discord_RNG (
	echo "Pulling Discord_RNG"
    cd Discord_RNG
	git pull
	cd ..
) else ( 
	echo "Creating Discord_RNG"
    git clone https://github.com/Xemuth/Discord_RNG
)
if EXIST  ./Discord_WOW (
	echo "Pulling Discord_WOW"
    cd Discord_WOW
	git pull
	cd ..
) else ( 
	echo "Creating Discord_WOW"
    git clone https://github.com/Xemuth/Discord_WOW
)
if EXIST  ./EasyConfiguration (
	echo "Pulling EasyConfiguration"
    cd EasyConfiguration
	git pull
	cd ..
) else ( 
	echo "Creating EasyConfiguration"
    git clone https://github.com/Xemuth/EasyConfiguration
)
if EXIST  ./GraphBuilder (
	echo "Pulling GraphBuilder"
    cd GraphBuilder
	git pull
	cd ..
) else ( 
	echo "Creating GraphBuilder"
    git clone https://github.com/Xemuth/GraphBuilder
)


