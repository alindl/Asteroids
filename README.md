<!---
     _        _                 _     _       _____ ____    
    / \   ___| |_ ___ _ __ ___ (_) __| |___  |___ /|  _ \ _ 
   / _ \ / __| __/ _ \ '__/ _ \| |/ _` / __|   |_ \| | | (_)
  / ___ \\__ \ ||  __/ | | (_) | | (_| \__ \  ___) | |_| |_ 
 /_/   \_\___/\__\___|_|  \___/|_|\__,_|___/ |____/|____/(_)
                                                            
-->

# Asteroids 3D:

 Unter anderem ASSIMP installieren (wahrscheinlich noch paar Sachen, füg ich dann hier dazu)  


  cmake .;  
  make;  
  cd src;  
  g++ -c -std=c++0x resource_manager.cpp game_object.cpp ball_object.cpp aster_object.cpp game_level.cpp sprite_renderer.cpp game.cpp;  
  ./openGL_demo  


## Grobe Idee was zu machen ist(ungeordnet)
- Raumschiff
    - Modell/Textur (First Person, also nicht notwendig)
    - Kollisionen (fertig, auch wenn Bounding Boxes nicht perfekt sind)
    - Waffen und Schüsse (fertig)
    - Kamera (fertig)
- Welt
    - Skybox (fertig)
    - (gefühlt) unendliche 3D (fertig)
- Steuerung im 3D
    - Beschleunigung/Entschleunigung (bähh, aber möglich)
- Asteroiden 
    - deren Ausbau/Zerstörungsverhalten (verschwinden einfach lol)
    - Generator (No, Level wird zuerst generiert)
    - Größe, Geschwindigkeit und Richtung (fertig, nein, nein)
    - Textur (fertig)
- "Leben" (fertig)



## Zusätzliche Features?
- Menü
- Minimap
- Nyancat
- Highscore
