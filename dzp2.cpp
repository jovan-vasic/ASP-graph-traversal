#include <iostream>
#include <set>
#include <map>

using namespace std;

struct distanca
{
    int duzina;
    int neizgradjen; // kumalativan broj neizgradjenih puteva
};

void napraviGraf(map<int, map<int, distanca>> &al, int n)
{
    for (int i = 1; i <= n; i++)
    {
        al.insert(pair<int, map<int, distanca>>(i, map<int, distanca>()));
    }
}

// NOTE: Dodavanje vec postojeceg ulaza u mapu se ignorise
void dodajCvor(map<int, map<int, distanca>> &al, int cvor)
{
    al.insert(pair<int, map<int, distanca>>(cvor, map<int, distanca>()));
}

void obrisiCvor(map<int, map<int, distanca>> &al, int cvor)
{
    // Obrisi grane ka datom cvoru
    for (auto itr = al.begin(); itr != al.end(); ++itr)
    {
        itr->second.erase(cvor);
    }
    // Obrisi cvor i grane koje idu od njega
    al.erase(cvor);
}

void dodajGranu(map<int, map<int, distanca>> &al, int cvor1, int cvor2, int duzina, int neizgradjen)
{
    distanca d;
    d.duzina = duzina;
    d.neizgradjen = neizgradjen;
    al[cvor1][cvor2] = d;
    al[cvor2][cvor1] = d;
}

void obrisiGranu(map<int, map<int, distanca>> &al, int cvor1, int cvor2)
{
    al[cvor1].erase(cvor2);
    al[cvor2].erase(cvor1);
}

void ispisiGraf(map<int, map<int, distanca>> &al)
{
    for (auto itr = al.begin(); itr != al.end(); ++itr)
    {
        cout << itr->first << ": ";
        for (auto itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
        {
            cout << itr2->first << "("
                 << "duzina: " << itr2->second.duzina << " broj neizgradjenih: " << itr2->second.neizgradjen << ")";
            if (next(itr2) != itr->second.end())
            {
                cout << "->";
            }
        }
        cout << endl;
    }
    cout << "\n";
    cin.ignore(100, '\n');
    do
    {
        cout << '\n'
             << "Pritisnite ENTER za nastavak ";
    } while (cin.get() != '\n');
}

int postojiGrana(map<int, map<int, distanca>> &al, int cvor1, int cvor2)
{
    return al[cvor1].find(cvor2) != al[cvor1].end();
}

void DajkstrinAlgoritam(int vatrogasci, map<int, map<int, distanca>> &al)
{
    int najbliziCvor = 0;

    // dok ne posetimo sve cvorove
    while (al.size() > 1)
    {
        // pronalazak trenutno najblizeg neposecenog cvora u grafu
        distanca najkracaDistanca;
        najkracaDistanca.duzina = -1;
        for (auto itr = al[vatrogasci].begin(); itr != al[vatrogasci].end(); ++itr)
        {
            if (najkracaDistanca.duzina == -1 || itr->second.duzina < najkracaDistanca.duzina || (itr->second.duzina == najkracaDistanca.duzina && itr->second.neizgradjen < najkracaDistanca.neizgradjen))
            {
                najkracaDistanca = itr->second;
                najbliziCvor = itr->first;
            }
        }
        // priprema za brisanje najblizeg cvora i njegova prebacivanje u graf posecenih sa odgovarajucom distancom
        // distanca za sve druge cvorove postoju min trenutne distance (ako postoji) i sume distanci od tog cvora i najblizeg cvora

        // prvo pronalazimo minimum duzine za one cvorove do kojih je vec postoji neki put

        for (auto itr = al[vatrogasci].begin(); itr != al[vatrogasci].end(); ++itr)
            if (postojiGrana(al, itr->first, najbliziCvor))
                if (najkracaDistanca.duzina + al[najbliziCvor][itr->first].duzina < al[vatrogasci][itr->first].duzina || (najkracaDistanca.duzina + al[najbliziCvor][itr->first].duzina == al[vatrogasci][itr->first].duzina && najkracaDistanca.neizgradjen + al[najbliziCvor][itr->first].neizgradjen < al[vatrogasci][itr->first].neizgradjen))
                    dodajGranu(al, vatrogasci, itr->first, najkracaDistanca.duzina + al[najbliziCvor][itr->first].duzina, najkracaDistanca.neizgradjen + al[najbliziCvor][itr->first].neizgradjen);

        // zatim dodajemo one cvorove do kojih trenutno ne postoji distanca

        for (auto itr = al[najbliziCvor].begin(); itr != al[najbliziCvor].end(); ++itr)
            if (itr->first != vatrogasci && !postojiGrana(al, vatrogasci, itr->first))
                dodajGranu(al, vatrogasci, itr->first, najkracaDistanca.duzina + itr->second.duzina, najkracaDistanca.neizgradjen + itr->second.neizgradjen);

        // izbacujemo najblizi cvor iz grafa i dodajemo ga u obradjenje
        cout << "Najblize rastojanje cvora " << najbliziCvor << " od vatrogasaca je " << al[vatrogasci][najbliziCvor].duzina;
        cout << " uz " << al[vatrogasci][najbliziCvor].neizgradjen << " neizgradjenienih puteva \n";
        obrisiCvor(al, najbliziCvor);

        //        rezultat.insert(pair<int, pair<int,vector<int>>(najbliziCvor, pair<int,distanca>()));

        //        dodajCvor(rezultat,najbliziCvor);
        //        dodajGranu(rezultat,vatrogasci,najbliziCvor,najkracaDistanca.duzina,najkracaDistanca.neizgradjen);
        // ispisiGraf(al);
    }
}

int main()
{
    map<int, map<int, distanca>> al;
    map<int, map<int, distanca>> rezultat;

    bool izadji = false;

    while (!izadji)
    {
        cout << "1. Napravi graf" << endl;
        cout << "2. Dodaj cvor" << endl;
        cout << "3. Obrisi cvor" << endl;
        cout << "4. Dodaj granu" << endl;
        cout << "5. Obrisi granu" << endl;
        cout << "6. Ispisi graf" << endl;
        cout << "7. Obrisi graf" << endl;
        cout << "8. Pronadji udaljenost od vatrogasaca" << endl;
        cout << "9. Izadji" << endl;

        int i;
        cout << "Izbor: ";
        cin >> i;
        switch (i)
        {
        case 1:
        {
            int velicina;
            cout << "Unesite velicinu: ";
            cin >> velicina;
            napraviGraf(al, velicina);
            break;
        }
        case 2:
        {
            int cvor;
            cout << "Unesite redni broj cvora: ";
            cin >> cvor;
            dodajCvor(al, cvor);
            break;
        }
        case 3:
        {
            int cvor;
            cout << "Unesite redni broj cvora: ";
            cin >> cvor;
            obrisiCvor(al, cvor);
            break;
        }
        case 4:
        {
            int cvor1, cvor2, duzina, neizgradjen;
            cout << "Unesite redne brojeve cvorova i duzinu i status izgradnje (0 za izgradjen, 1 za neizgradjen): ";
            cin >> cvor1 >> cvor2 >> duzina >> neizgradjen;
            dodajGranu(al, cvor1, cvor2, duzina, neizgradjen);
            break;
        }
        case 5:
        {
            int cvor1, cvor2;
            cout << "Unesite redne brojeve cvorova: ";
            cin >> cvor1 >> cvor2;
            obrisiGranu(al, cvor1, cvor2);
            break;
        }
        case 6:
            ispisiGraf(al);
            break;
        case 7:
            al.clear();
            break;
        case 8:
            int vatrogasci;
            cout << "Unesite indeks vatrogasaca: ";
            cin >> vatrogasci;
            DajkstrinAlgoritam(vatrogasci, al);
            break;
        default:
            izadji = true;
        }
    }
    return 0;
}
