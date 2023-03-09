#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct Starships
{
	struct Weapon
	{
		string weaponName;
		int weaponRating;
		float powerConsumption;

		Weapon(string weaponName, int weaponRating, float powerConsumption);
	};
	string shipName;
	string shipClass;
	short shipLength;
	int shipShield;
	float shipWarp;
	int firePower;
	vector<Weapon> shipInv;

	Starships()
	{
		shipName = "";
		shipClass = "";
		shipLength = 0;
		shipShield = 0;
		shipWarp = 0.00;
		firePower = 0;
		shipInv = {};

	}
	Starships(string shipName, string shipClass, short shipLength, int shipShield, float shipWarp, int firePower, vector<Weapon> ships);
};
void ReadFile(vector<Starships>& ships, string file);
void PrintShips(vector<Starships>& ships);
void strongestWeapon(vector<Starships> ships);
void PrintOneShip(Starships& ship);


int main()
{
	cout << "Which file(s) to open?\n";
	cout << "1. friendlyships.shp" << endl;
	cout << "2. enemyships.shp" << endl;
	cout << "3. Both files" << endl;
	int option;
	cin >> option;

	/* Load files here */
	vector<Starships> ships;
	if (option == 1)
		ReadFile(ships, "friendlyships.shp");
	else if (option == 2)
		ReadFile(ships, "enemyships.shp");
	else if (option == 3)
	{
		ReadFile(ships, "friendlyships.shp");
		ReadFile(ships, "enemyships.shp");
	}


	cout << "1. Print all ships" << endl;
	cout << "2. Starship with the strongest weapon" << endl;
	cout << "3. Strongest starship overall" << endl;
	cout << "4. Weakest ship (ignoring unarmed)" << endl;
	cout << "5. Unarmed ships" << endl;

	cin >> option;

	if (option == 1) //print all ships
	{
		PrintShips(ships);
	}
	if (option == 2) //print strongest ships
	{
		strongestWeapon(ships);
	}
	return 0;
}

void ReadFile(vector<Starships>& ships, string file)
{
	ifstream shipFile;
	shipFile.open(file, ios_base::binary);
	if (shipFile.is_open())
	{
		cout << "open" << endl;
		unsigned int shipCount;
		shipFile.read((char*)&shipCount, sizeof(shipCount));
		for (unsigned int i = 0; i < shipCount; i++)
		{
			// get ship name (4 bytes)
			unsigned int fourbytes;
			shipFile.read(reinterpret_cast<char*>(&fourbytes), sizeof(fourbytes));
			char* tempname = new char[fourbytes];
			shipFile.read(tempname, fourbytes); //reads string
			string shipName = tempname;
			delete[] tempname;

			// get ship class (4 bytes)
			shipFile.read(reinterpret_cast<char*>(&fourbytes), sizeof(fourbytes));
			char* tempclass = new char[fourbytes];
			shipFile.read(tempclass, fourbytes); //reads string
			string shipClass = tempclass;
			delete[] tempclass;

			// get ship length
			short shipLength; //2 bytes
			shipFile.read(reinterpret_cast<char*>(&shipLength), sizeof(shipLength));

			//get ship shield
			unsigned int shipShield; //4 bytes
			shipFile.read(reinterpret_cast<char*>(&shipShield), sizeof(shipShield));

			// get shield warp
			unsigned int shipWarp; //4 bytes
			shipFile.read(reinterpret_cast<char*>(&shipWarp), sizeof(shipWarp));

			unsigned int shipWeapons; //4 bytes
			shipFile.read(reinterpret_cast<char*>(&shipWeapons), sizeof(shipWeapons));

			int firePower = 0;

			vector<Starships::Weapon> weapons;
			for (unsigned int j = 0; j < shipWeapons; j++)
			{
				unsigned int weaponLength;
				shipFile.read(reinterpret_cast<char*>(&weaponLength), sizeof(weaponLength));
				char* tempname = new char[weaponLength];
				shipFile.read(tempname, weaponLength); //reads string
				string weaponName = tempname;
				delete[] tempname;

				unsigned int weaponPower;
				shipFile.read(reinterpret_cast<char*>(&weaponPower), sizeof(weaponPower));
				firePower += weaponPower;

				unsigned int weaponConsumption;
				shipFile.read(reinterpret_cast<char*>(&weaponConsumption), sizeof(weaponConsumption));

				Starships::Weapon tempweapon(weaponName, weaponPower, weaponConsumption);
				weapons.push_back(tempweapon); //adds weapon to weapon vector
			}
			Starships tempShip(shipName, shipClass, shipLength,shipShield, shipWarp, firePower, weapons);
			ships.push_back(tempShip);
		}
	}
	else
	{
		cout << "file not open" << endl;
	}
}
void PrintShips(vector<Starships>& ships)
{
	for (unsigned int i = 0; i < ships.size(); i++) 
	{
		cout << "Name: " << ships[i].shipName << endl;
		cout << "Class: " << ships[i].shipClass << endl;
		cout << "Shield Capacity: " << ships[i].shipShield << endl;
		cout << "Maximum Warp: " << ships[i].shipWarp << endl;
		cout << "Armaments: "<< endl;
		if (ships[i].shipInv.size() == 0)
			cout << "Ship is unarmed" << endl;
		else
		{
			for (unsigned int j = 0; j < ships[i].shipInv.size(); j++)
			{
				cout << ships[i].shipInv[j].weaponName << ", " << ships[i].shipInv[j].weaponRating << ", " << ships[i].shipInv[j].powerConsumption << endl;
			}
			cout << "Total firepower: " << ships[i].firePower << endl;
		}

		cout << endl;
	}
}
void strongestWeapon(vector<Starships> ships)
{
	Starships strongest = ships[0];
	for (unsigned int i = 1; i < ships.size(); i++)
	{
		if (ships[i].firePower > strongest.firePower)
			strongest = ships[i];
	}
	PrintOneShip(strongest);

}
void PrintOneShip(Starships& ship)
{
	cout << "Name: " << ship.shipName << endl;
	cout << "Class: " << ship.shipClass << endl;
	cout << "Shield Capacity: " << ship.shipShield << endl;
	cout << "Maximum Warp: " << ship.shipWarp << endl;
	cout << "Armaments: " << endl;

	if (ship.shipInv.size() == 0)
		cout << "Ship is unarmed" << endl;
	else
	{
		for (unsigned int j = 0; j < ship.shipInv.size(); j++)
		{
			cout << ship.shipInv[j].weaponName << ", " << ship.shipInv[j].weaponRating << ", " << ship.shipInv[j].powerConsumption << endl;
		}
		cout << "Total firepower: " << ship.firePower << endl;
	}
}
//Starships constructor
Starships::Starships(string shipName, string shipClass, short shipLength, int shipShield, float shipWarp, int firePower, vector<Weapon> weapons)
{
	this->shipName = shipName;
	this->shipClass = shipClass;
	this->shipLength = shipLength;
	this->shipShield = shipShield;
	this->shipWarp = shipWarp;
	this->firePower = firePower;
	this->shipInv = weapons;
}

Starships::Weapon::Weapon(string weaponName, int weaponRating, float powerConsumption)
{
	this->weaponName = weaponName;
	this->weaponRating = weaponRating;
	this->powerConsumption = powerConsumption;
}

