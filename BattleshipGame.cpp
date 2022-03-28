/* @Author
Student Name: Berdan Çaðlar AYDIN
Student ID: 150170068
Date: 26.05.2020 */

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<algorithm>
#include<stack>
#include<queue>
#include <chrono>

using namespace std;

class Point {
	int x, y;
public:
	Point() { x = 0; y = 0; };
	Point(int _x, int _y) {
		x = _x;
		y = _y;
	}
	void print() {
		cout << "(" << x << "," << y << ")" << endl;
	};
	friend class Ship;
	friend class Player;
	friend class BattleshipGame;
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
	vector<Point> find_ship_points();
	friend class Player;
};

vector<Point> Ship::find_ship_points() {
	vector<Point> ship_points;
	if (start.x == end.x) {
		for (int i = start.y; i <= end.y; i++) {
			ship_points.push_back(Point(start.x, i));
		}
	}
	else if (start.y == end.y) {
		for (int i = start.x; i <= end.x; i++) {
			ship_points.push_back(Point(i, start.y));
		}
	}
	return ship_points;
}

class Player {
	string algo;
	Point init;
	int B, N;
	vector<Ship> ships;
	vector<vector<bool>> board;
	vector<vector<bool>> visited;
	int n_of_ship_points;
	int n_visited;
	int n_memory;
	bool check(Point p);
	queue<Point> add_queue(queue<Point> nodes, Point p);
public:
	Player(string _algo, Point _init, int _B, int _N, vector<Ship> _ships) {
		algo = _algo;
		init = _init;
		B = _B;
		N = _N;
		ships = _ships;
		n_of_ship_points = 0;
		n_visited = 0;
		n_memory = 0;
	}
	void make_board();
	void print_board();
	Point playDFS(vector<vector<bool>> op_board, int op_ship_points);
	Point playBFS(vector<vector<bool>> op_board, int op_ship_points);
	friend class BattleshipGame;
};


void Player::make_board() {
	board = vector<vector<bool>>(B, vector<bool>(B, false));
	visited = vector<vector<bool>>(B, vector<bool>(B, false));
	vector<vector<Point>> ship_points;

	for (auto it = begin(ships); it != end(ships); ++it) {
		ship_points.push_back((*it).find_ship_points());
	}

	for (vector<Point> vect1D : ship_points)
	{
		for (Point p : vect1D)
		{
			board[p.x][p.y] = true;
			n_of_ship_points++;
		}
	}
}

void Player::print_board() {
	for (int i = 0; i < B; i++)
	{
		for (int j = 0; j < B; j++)
		{
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
}

Point Player::playDFS(vector<vector<bool>> op_board, int op_ship_points) {
	stack<Point> nodes;
	nodes.push(init);
	n_memory++;
	while (!nodes.empty()) {
		Point curr = nodes.top();
		nodes.pop();
		if (!check(curr)) {
			continue;
		}
		int x = curr.x;
		int y = curr.y;

		visited[x][y] = true;
		n_visited++;
		if (op_board[x][y]) {
			op_ship_points--;
			if(!op_ship_points){
				return Point(n_visited, n_memory);
			}
		}
		nodes.push(Point(x, y + 1));
		nodes.push(Point(x + 1, y));
		nodes.push(Point(x, y - 1));
		nodes.push(Point(x - 1, y));
		n_memory += 4;
	}
	return Point(n_visited, n_memory);
}

queue<Point> Player::add_queue(queue<Point> nodes, Point p) {
	if (check(p)) {
		nodes.push(p);
		visited[p.x][p.y] = true;
		n_visited++;
		n_memory++;
	}
	return nodes;
}

Point Player::playBFS(vector<vector<bool>> op_board, int op_ship_points) {
	queue<Point> nodes;
	nodes.push(init);
	visited[init.x][init.y] = true;
	n_visited++;
	n_memory++;
	while (!nodes.empty()) {
		Point curr = nodes.front();
	
		int x = curr.x;
		int y = curr.y;
	
		if (op_board[x][y]) {
			op_ship_points--;
			if (!op_ship_points) {
				return Point(n_visited, n_memory);
			}
		}

		nodes.pop();

		Point p = Point(x, y + 1);
		nodes = add_queue(nodes, p);
		p = Point(x + 1, y);
		nodes = add_queue(nodes, p);
		p = Point(x, y - 1);
		nodes = add_queue(nodes, p);
		p = Point(x - 1, y);
		nodes = add_queue(nodes, p);
		
	}
	return Point(n_visited, n_memory);
}

bool Player::check(Point p) {
	if (p.x < 0 || p.y < 0 || p.x >= B || p.y >= B) {
		return false;
	}

	if (visited[p.x][p.y])
		return false;

	return true;
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
				Ship ship(Point(i[0], i[1]), Point(i[2], i[3]));
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
class BattleshipGame {
public:
	void start_game(Player player1, Player player2);
};

void BattleshipGame::start_game(Player player1, Player player2) {

	Point p1_result, p2_result;

	if (player1.algo == "DFS") {
		p1_result = player1.playDFS(player2.board, player2.n_of_ship_points);
	}
	else if (player1.algo == "BFS") {
		p1_result = player1.playBFS(player2.board, player2.n_of_ship_points);
	}
	else {
		cout << "Can't recognize the algorithm" << endl;
	}


	if (player2.algo == "DFS") {
		p2_result = player2.playDFS(player1.board, player1.n_of_ship_points);
	}
	else if (player2.algo == "BFS") {
		p2_result = player2.playBFS(player1.board, player1.n_of_ship_points);
	}
	else {
		cout << "Can't recognize the algorithm" << endl;
	}

	cout << "The algorithm: Player1: " << player1.algo << ", Player2: " << player2.algo << endl;
	cout << "The number of visited nodes: Player1: " << p1_result.x << ", Player2: " << p2_result.x << endl;
	cout << "The number of nodes kept in the memory: Player1: " << p1_result.y << ", Player2: " << p2_result.y << endl;
	cout << "The result: ";
	if (p1_result.x <= p2_result.x) {
		cout << "Player1";
	}
	else if (p1_result.x > p2_result.x) {
		cout << "Player2";
	}
	cout << " Won!" << endl;
	 
}

int main(int argc, char* argv[])
{
	auto start = chrono::high_resolution_clock::now();
	Player player1 = readInput(argv[1]);
	Player player2 = readInput(argv[2]);

	player1.make_board();
	cout << "Player1" << endl;
	player1.print_board();
	cout << endl;

	player2.make_board();
	cout << "Player2" << endl;
	player2.print_board();
	cout << endl;

	BattleshipGame game;
	game.start_game(player1, player2);
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
	cout << "The running time: " << duration.count() << "ms" << endl;
	return 0;
}
