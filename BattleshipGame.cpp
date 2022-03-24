/* @Author
Student Name: Berdan Çaðlar AYDIN
Student ID: 150170068
Date: 26.05.2020 */

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>

using namespace std;
class Point {
	int x, y;
public:
	Point() { x = 0; y = 0; };
	Point(int _x , int _y) {
		x = _x;
		y = _y;
	}
};

class Ship {
	Point start, end;
	bool exist;
public:
	Ship(Point _start, Point _end) {
		start = _start;
		end = _end;
		exist = true;
	}
};

class Player {
	string algo;
	Point init;
	int B, N;
	vector<Ship> ships;
public:
	Player(string _algo, Point _init, int _B, int _N, vector<Ship> _ships) {
		algo = _algo;
		init = _init;
		B = _B;
		N = _N;
		ships = _ships;
	}
	void print();
};

void Player::print() {
	cout << algo;
}

vector<int> split(const string& s, char delimiter) {
	vector<int> tokens;
	string token;
	istringstream tokenStream(s);
	while (getline(tokenStream, token, delimiter)) {
		tokens.push_back(stoi(token));
	}
	return tokens;
}

Player readInput(string fname) {
	fstream newfile;
	string a;
	int b, n;
	vector<int> i;
	vector<Ship> s;
	Point init;

	newfile.open(fname, ios::in);
	if (newfile.is_open()) {
		string tp;
		int line = 0;
		while (getline(newfile, tp)) {
			switch (line)
			{
			case 0:
				a = tp;
				break;
			case 1:
				i = split(tp, ' ');
				init = Point(i[0], i[1]);			
				break;
			case 2:
				i = split(tp, ' ');
				b = i[0];
				n = i[1];
				break;
			default:
				i = split(tp, ' ');
				Ship ship(Point(i[0], i[1]) , Point(i[2], i[3]));
				s.push_back(ship);
				break;
			}
			line++;
		}
		newfile.close();
	}
	Player player(a, init, b, n, s);
	return player;
}

int main(int argc, char* argv[])
{
	Player player1 = readInput(argv[1]);
	Player player2 = readInput(argv[2]);
	return 0;
}
