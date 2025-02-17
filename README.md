# SHA-1_on_328p
Prüfung für Embedded Systems

Aufgabe: 
Praktisch:
-SHA-1 Algorithmus!

Schriftlich:
-"Diskussion der zugrundeliegenden Theorie"
-"Analyse/Disskussion der Firmware Implementierung
	*Umsetzung des SHA-1 Verfahrens
	*Umsetzung des USART-Kommandointerpreters - Ablaufsteuerung"
"

Programmieren in ANSI C
Keine existierenden Bibs/Lösungen nutzen.
-------------------------------------------------------------------------

Minimale Anforderungen:

Datenvorverarbeitung kann auf PC passieren (lame)
SHA-1 Verarbeitung mit einem 512-bit breiten Datenblock (mehr = besser)
Per USART an uC übertragen und zwischenspeichern
!!!!Verarbeitung erfolgt erst durch nachfolgendes Senden des Steuerkommandos #!  !!!!
Ergebniss des SHA-1 Algorithmuses auf uC zwischenspeichern.
Steuerkommando #$ senden um Ergebniss von uC an PC zu senden (Putty?)
System testen mittels Teststring "ESY - Matrikelnummer"; Ergebniss davon sollte sein: 
b65e9709ff74d75174dbcfe3ba163b55e7217a2b
Erstellt durch Notepad++

-------------------------------------------------------------------------

Bewertungsparameter (Projekt + Dokumentation)

-Qualität und Tiefgang der geleisteten Arbeit
-Kreativität sowie dem einbringen eigener Ideen
-Selbstständigkeit?, Motivation???, Systematik
-Fähigkeit zur Realisierung bzw zur praktischen Umsetzung
-Aufbau/Struktur/Stil der schriftlichen Projektarbeit
-Eigene Ergebnisanalyse, Schlussfolgerung und Empfehlungen

-------------------------------------------------------------------------

Formale Gestaltung:

------> Template von Moodle (Latex vll??)
Gliederung:
	-Titelblatt (suprise)
	-Eigenständigkeitserklärung (Trust me Bro)
	-ca 5 Seiten im Kern, Doppelspaltig (IEEEEEEE Stil)
	-> Kurzfassung, Allgemeine Einführung, Hauptteile(ggf Untergliederungen), Zusammenfassung, Schlussfolgerung,  Quellenverzeichnis

"Aus fremden Arbeiten übernommene Textstellen, Tabellen, Grafiken etc. müssen als wörtliche oder
sinngemäße Zitate kenntlich gemacht werden (Angabe im Quellenverzeichnis). Andernfalls liegt ein Plagiat
vor, was zur Nichtanerkennung der Projektarbeit und zu einem Nichtbestehen der Veranstaltung führt."

Abgabe als PDF oder MS Word (buh), Software als gezippter Ordner (winrar oder 7zip?????!?!?) des Projektes


To Do:
-Git Projekt einrichten 
-Wie zum Fick zitiert man nochmal richtig...
-Sha-1 what is it and what does it do (Spoiler: Math + Magic = thgic) ---> RTFM (nist.fips.180-4)
-USART (Universal Synchronous and Asynchronous serial Receiver and Transmitter) Schnittstelle überlegen. 
	Randbedingungen, 
	abläufe, 
	interrupt (welches register??), 
	Zwischenspeichern (wo? Flash, RAM, SAM, EEPROM, CLOUD...?). 
	Wenn Datenkamen, speichern bis zur Unendlichkeit oder verwerfen nach Zeit x?
	Eichhörnchen?
-Datenlimit rausfinden für USART
	Was machen wenn Speicher überläuft? Übertragung abbrechen? Fehlermeldung zurück geben? LED panisch blinken lassen wäre gut ....(definiere panisch)
	












Quellen:
PDF: FIPS PUB 180-4; FEDERAL INFORMATION PROCESSING STANDARDS PUBLICATION Secure Hash Standard (SHS)
https://www.arnabkumardas.com/arduino-tutorial/usart-programming/ ??

