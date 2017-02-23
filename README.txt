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






Asteroids 3D:

Zusaetzlich benoetigte Librarys: ASSIMP (http://assimp.org/) fuer das laden der 3D Modelldateien

Kompilierung ist unveraendert wie oben von Ihnen beschrieben.

Steuerung: 

  ESC - Beenden des Spiels
  W - Bewegung nach Vorne
  A - Bewegung nach Links
  S - Bewegung nach Hinten
  D - Bewegung nach Rechts
  Pfeiltaste Oben   - Kamera nach Oben 
  Pfeiltaste Links  - Kamera nach Links
  Pfeiltaste Unten  - Kamera nach Unten
  Pfeiltaste Rechts - Kamera nach Rechts
  Umschalttaste/Shift Links - Boost (Erhoeht die Geschwindigkeit waehrend des Haltens auf das Dreifache)
  Leertaste - Schuss des Lazers (Schuss wird erst beim Loslassen der Taste abgefeuert)
  H - Schummeltaste, zerstoert einen Asteroiden
  

Der Spieler wird an den Anfangspunkt gesetzt wenn er in einen Asteroiden fliegt, der Asteroid selbst wird dabei zerstoert.

Wenn der Lazer abgefeuert wurde, aber keinen Asteroiden trifft, dann fliegt der Schuss absichtlich unangenehm lange weiter,
denn es kann waehrenddessen nicht erneut geschossen werden.

Der Spieler hat insgesamt 4 Leben, in der rechten oberen Ecke wird die Anzahl der restlichen Leben angezeigt.

Es sind 10 Asteroiden um den Spieler verteilt. Die Anzahl der restlichen Asteroiden wird in der linken oberen Ecke angezeigt.

Das Spiel beendet sich, wenn alle Asteroiden zerstoert oder alle Leben verbraucht wurden.
