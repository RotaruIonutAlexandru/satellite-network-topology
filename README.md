read me - Rotaru Alex 

Cerința 1: Construirea Arborelui Binar
La cerința asta am implementat un arbore binar care să modeleze rețeaua de sateliți. Am început prin a citi lista de sateliți și am folosit un Min-Heap pentru a-i sorta după frecvență. La fiecare pas, am extras cei doi sateliți cu cele mai mici frecvențe și i-am combinat într-un nod de legătură. Nodul nou creat are frecvența egală cu suma celor doi copii, iar numele e format din concatenarea numelor lor. Am repetat procesul până a rămas un singur nod (rădăcina arborelui). La final, am afișat arborele nivel cu nivel, de sus în jos, ca să se vadă structura ierarhiei.

Cerința 2: Decodificarea Mesajului Binar
Aici am avut de transformat un șir de 0-uri și 1-uri într-o listă de sateliți. Am pornit de la arborele construit la cerința 1. Pentru fiecare bit din șir, am mers la stânga (pentru 0) sau la dreapta (pentru 1) în arbore. Când am ajuns la un nod frunză (adică un satelit real), am salvat numele lui și am resetat parcurgerea la rădăcină pentru următorii biți. Am ținut cont și de cazurile în care biții rămași nu mai duc la niciun satelit valid (ignorându-i). Rezultatul a fost afișat ca o listă de nume separate prin spații.

Cerința 3: Codificarea Sateliților
La cerința asta, am făcut exact opusul decodificării. Pentru fiecare satelit din listă, am căutat calea de la rădăcina arborelui până la el. Am folosit o funcție recursivă care adaugă 0 la stânga și 1 la dreapta. De exemplu, dacă un satelit e accesibil mergând stânga -> dreapta -> dreapta, codificarea lui e "011". Am concatenat toate codificările într-un singur șir, care reprezintă mesajul final.

Cerința 4: Cel Mai Apropiat Strămoș Comun (LCA)
Aici a trebuit să găsesc nodul comun cel mai apropiat pentru o listă de sateliți cu probleme. Am parcurs arborele recursiv și, pentru fiecare nod, am verificat dacă el sau subarborii săi conțin toți sateliții din listă. Dacă un nod are sateliți în ambele subarbori (stâng și drept), el este strămoșul comun. Dacă nu, am continuat căutarea în subarborele care conține toți sateliții. La final, am afișat numele nodului găsit, care e punctul cheie pentru reparații.