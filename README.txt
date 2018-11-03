Gabriel Boroghina, 333CB

                                  BRICK BREAKER


	Implementarea temei este facuta in folderul Source/BrickBreaker, unde exista 
clasele pentru obiectele din joc, precum si fisierul BrickBreaker.cpp, care contine
logica generala a jocului.

	* Clasele Ball, Bricks, Walls si Paddle contin obiectele 2D principale din joc.

	* Clasa PowerupTicket reprezinta tichetele (patratele care se rotesc) prin care
se pot activa anumite powerup-uri.

	* Clasa Powerup exte o clasa abstracta ce este mostenita de toate powerup-urile
implementate si contine logica referitoare la timpul de viata al powerup-ului
(powerup-urile se dezactiveaza automat dupa expirarea acestui timp).

	* Clasa PowerupManager este un singleton folosit pentru managementul powerup-urilor
si al tichetelor. Aceasta decide daca se genereaza tichete atunci cand se produc
coliziuni ball-brick si mentine o lista a powerup-urilor active, pentru a putea fi
accesate din clasa BrickBreaker.

-----------------------------------------------------------------------------------

Detalii de implementare

	Miscarea bilei este definita prin 2 valori vx si vy, reprezentand vitezele pe
cele 2 axe Ox si Oy. In cazul coliziunilor orizontale, vx va deveni -vx, iar in
cazul celor verticale, vy va deveni -vy. In cazul coliziunilor cu platforma, 
se determina pozitia de pe platforma in care bila loveste, normalizata la intervalul
[-1, 1], iar aceasta valoare va reprezenta cosinusul unghiului vitezei finale:
vx = cos * velocity, vy = sin * velocity, unde velocity este viteza (constanta) a 
bilei.

	La verificarea coliziunilor dintre bila si un alt obiect se verifica si daca 
sensul vitezei bilei pe directia de coliziune (orizontala sau verticala) este 
spre obiectul respectiv (in acest fel, bila nu va incepe sa "tremure" niciodata 
langa un obiect cu care se ciocneste).

	Transformarile geometrice sunt realizate folosind matricele din fisierul 
Trandform2D.h, implementate in cadrul laboratorului 3.
	Randarea obiectelor se face in functia Update, folosind pentru anumite obiecte si
matrice de transformare (translatie, si eventual scalare - de exemplu, la bila).
	Tot din functia Update se apeleaza si metodele de verificare a coliziunilor.

	Daca jucatorul ramane fara vieti (GAME OVER), zona de joc va ramane goala 
(neagra); jucatorul poate incepe un nou joc facand click stanga.

	Pentru ca jocul sa poata fi jucat si dupa o eventuala redimensionare a ferestrei,
pozitia cursorului a fost scalata in functie de scalarea ferestrei fata de
dimensiunile initiale (cu care a pornit jocul). Singurul dezavantaj este faptul ca
transformarea fereastra-poarta existenta este neuniforma, ceea ce face ca obiectele
sa se "turteasca" atunci cand redimensionarea ferestrei modifica aspect ratio-ul.

-----------------------------------------------------------------------------------

Powerup-uri implementate:
	* Bottom Wall (tichet galben) - apare o bara in partea de jos a scenei, sub 
platforma, care nu lasa bila sa iasa din scena.
	* Shooter (tichet rosu) - bila dispare din scena cat timp acest powerup este 
activ, iar platforma trage cu gloante pe verticala, in sus, la apasarea click 
stanga.
	* Fat Ball (tichet mov) - bila isi mareste dimensiunea.

	Powerup-urile dispar dupa o durata fixa (4s).