Vorgangsweise zum Kompilieren:

(1) In ./src/CMakeLists.txt am Ende der Datei die gewuenschte C-Eingabedatei
    durch (Aus-)Kommentieren auswaehlen.
(2) Befehle: 
     (2a)   cmake .
     (2b)   make
Achten Sie darauf, dass alle Header-Dateien und Libs auch tatsaechlich
gefunden werden! Bei mir kompiliert das Zeugs sowohl zuhause wie auch im Buero
ohne Warnungen.

Man findet nach dem Kompilieren in ./src die ausfuehrbare Datei "openGL_demo".

Loeschen der CMake-Einstellungen mittels Skript "cleancmake.sh".



Viel Spass und Erfolg beim Experimentieren mit OpenGL!
