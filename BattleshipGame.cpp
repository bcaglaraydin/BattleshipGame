/* @Author
Student Name: Berdan Çağlar AYDIN
Student ID: 150170068
Date: 26.05.2020 */

#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<stack>
#include<queue>
#include <chrono>

using namespace std;

//class to represent points on board
//also used to return # of visited nodes and # of nodes  in memory
class Point {
	int x, y;
public:
	Point() { x = 0; y = 0; };
	Point(int _x, int _y) {
		x = _x;
		y = _y;
	}
	friend class Ship;
	friend class Player;
	friend class BattleshipGame;
};

//class to represent a ship
class Ship {
	Point start, end;
	vector<Point> find_ship_points();
public:
	Ship(Point _start, Point _end) {
		start = _start;
		end = _end;
	}
	friend class Player;
};

//this method finds the all points a ship covers
// start (0,3), end (0,5) --> ship points: (0,3) - (0,4) - (0,5)
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

//class to represent a player
class Player {
	string algo; //the search algorithm player uses
	Point init; //initial starting point
	int B, N;
	vector<Ship> ships; //the ships of this player
	vector<vector<bool>> board; //BXB matrix, storing ship point locations
	vector<vector<bool>> visited; //BXB matrix, storing visited nodes
	int n_of_ship_points; //# of ship points
	int n_visited; //# of visited nodes
	int n_memory; //# of nodes stored in the memory
	bool check(Point p); //check if the given node is avaiblable for traverse
	queue<Point> add_queue(queue<Point> nodes, Point p); //doing BFS, check the availability and add node to the queue
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
	void make_board();  //fill the board with ships
	void print_board();
	Point playDFS(vector<vector<bool>> op_board, int op_ship_points);
	Point playBFS(vector<vector<bool>> op_board, int op_ship_points);
	friend class BattleshipGame;
};


void Player::make_board() {
	board = vector<vector<bool>>(B, vector<bool>(B, false));
	visited = vector<vector<bool>>(B, vector<bool>(B, false));
	vector<vector<Point>> ship_points;

	//find all ship points
	for (auto it = begin(ships); it != end(ships); ++it) {
		ship_points.push_back((*it).find_ship_points());
	}

	for (vector<Point> vect1D : ship_points)
	{
		for (Point p : vect1D)
		{
			board[p.x][p.y] = true; //fill the board
			n_of_ship_points++; //increment # of ship points
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

bool Player::check(Point p) {
	//check if the Point is out of the board boundry
	if (p.x < 0 || p.y < 0 || p.x >= B || p.y >= B) { 
		return false;
	}
	 //check if the point is already visited
	if (visited[p.x][p.y]) {
		return false;
	}
		
	return true;
}

Point Player::playDFS(vector<vector<bool>> op_board, int op_ship_points) {
	stack<Point> nodes;
	nodes.push(init); //push the initial node to the stack
	n_memory++; //increment nodes in memory
	while (!nodes.empty()) { //iterate until stack is empty
		Point curr = nodes.top(); //get the top Point
		nodes.pop();
		if (!check(curr)) { //check if it is traverseable
			continue; //skip if no
		}
		int x = curr.x;
		int y = curr.y;

		visited[x][y] = true; //add the node to the visited matrix
		n_visited++; //increment visited nodes
		if (op_board[x][y]) { //check if player hits a ship on opponent's board
			op_ship_points--; //decrement opponent's ship points
			if(!op_ship_points){
				return Point(n_visited, n_memory); //if the opponent has 0 ships, return the # of visited and stored nodes
			}
		}
		//add adjacent nods to the stack (highest priority node pushed last)
		nodes.push(Point(x, y + 1));
		nodes.push(Point(x + 1, y));
		nodes.push(Point(x, y - 1));
		nodes.push(Point(x - 1, y));
		//increment nodes in memory by 4 (this approach checks the nodes after pushing to the stack, they could be checked before pushing too)
		n_memory += 4;
	}
	return Point(n_visited, n_memory);
}

//check the traversability of the node, push to the queue if its okay
queue<Point> Player::add_queue(queue<Point> nodes, Point p) {
	if (check(p)) { //check if its traverseable
		nodes.push(p); //push to the queue
		visited[p.x][p.y] = true; //make it visited
		n_visited++; //increment # of visited nodes and nodes in memory
		n_memory++;
	}
	return nodes;
}

Point Player::playBFS(vector<vector<bool>> op_board, int op_ship_points) {
	queue<Point> nodes;
	//push the initial node to the queue
	nodes.push(init);
	visited[init.x][init.y] = true;
	//increment # of nodes  visited and in memory by 1
	n_visited++;
	n_memory++;
	while (!nodes.empty()) { //iterate until queue is empty
		Point curr = nodes.front(); //get the next node fom the queue
	
		int x = curr.x;
		int y = curr.y;
		
		
		if (op_board[x][y]) { //check if player hits a ship on opponent's board
			op_ship_points--; //decrement opponent's ship points
			if (!op_ship_points) {
				return Point(n_visited, n_memory); //if the opponent has 0 ships, return the # of visited and stored nodes
			}
		}
		 
		nodes.pop(); //remove the node from queue

		//pushing all the adjacent nodes to the queue (in this approach, node is not pushed to the memory if it is not traverseable)
		//highest priority node pushed first
		Point p = Point(x - 1, y);
		nodes = add_queue(nodes, p);
		p = Point(x, y - 1);
		nodes = add_queue(nodes, p);
		p = Point(x + 1, y);
		nodes = add_queue(nodes, p);
		p = Point(x, y + 1); 
		nodes = add_queue(nodes, p);		
		
	}
	return Point(n_visited, n_memory);
}

//split string into integer vector
vector<int> split(const string& s, char delimiter) {
	vector<int> tokens;
	string token;
	istringstream tokenStream(s);
	while (getline(tokenStream, token, delimiter)) {
		tokens.push_back(stoi(token));
	}
	return tokens;
}

//read files and return player objects
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

//class to represent the BattleshipGame
class BattleshipGame {
public:
	void start_game(Player player1, Player player2);
};

void BattleshipGame::start_game(Player player1, Player player2) {

	//initialize player boards
	player1.make_board();
	cout << "Player1" << endl;
	player1.print_board();
	cout << endl;

	player2.make_board();
	cout << "Player2" << endl;
	player2.print_board();
	cout << endl;

	Point p1_result, p2_result;

	//the corresponding function is called according to the players algorithm
	if (player1.algo == "DFS") {
		p1_result = player1.playDFS(player2.board, player2.n_of_ship_points); //passing the opponent board and # of ship points as argument
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

	//printing the results
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

	//read files
	Player player1 = readInput(argv[1]);
	Player player2 = readInput(argv[2]);

	//start the game
	BattleshipGame game;
	game.start_game(player1, player2);

	//calculate time
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
	cout << "The running time: " << duration.count() << "ms" << endl;
	return 0;
}
