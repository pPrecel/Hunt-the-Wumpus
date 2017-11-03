#include <iostream>
#include <cstdlib>
#include <ctime>

namespace map {
	enum RoomState {
		nothing = 0,
		monster = 1,
		catapult = 2,
		player = 3,
		hole = 4
	};

	class Room {
	private:
		int number;
		Room* adjoing_1;
		Room* adjoing_2;
		Room* adjoing_3;
		RoomState state;
	public:
		Room(){}
		Room(const int& n, const RoomState& rs = nothing, Room* r1 = nullptr, Room* r2 = nullptr, Room* r3 = nullptr)
			: number{ n }, state{ rs }, adjoing_1{ r1 }, adjoing_2{ r2 }, adjoing_3{ r3 } {}

		Room* find(int i) {
			Room* tmp = this;
			while(tmp->number != i) tmp = closer_room(i, tmp->adjoing_1, tmp->adjoing_2, tmp->adjoing_3);
			return tmp;
		}

		Room* find_in_circle(int number) {
			Room* tmp = this;
			do {
				tmp = tmp->adjoing_1;
			} while (tmp->number != number && tmp->number != this->number);
			return (tmp->number == number) ? tmp : nullptr;
		}

		friend Room* map_generator();
		friend Room* create_circle(int start, int end);
		friend void merge_circles(Room* firCircle, Room* secCircle, Room* thiCircle);
		friend Room* closer_room(int target, Room* firstRoom, Room* secondRoom, Room* thirdRoom);
		friend void random_states(Room* room, int howMany, RoomState state);
	};

	Room* create_circle(int start, int end) {

		Room* firstRoom, *tmp;
		Room* newRoom = new Room(start);

		firstRoom = newRoom;

		for (int i = start + 1; i < end + 1; i++) {
			tmp = newRoom;
			newRoom = new Room(i, nothing, tmp);
			tmp->adjoing_2 = newRoom;
		}
		newRoom->adjoing_2 = firstRoom;
		firstRoom->adjoing_1 = newRoom;
		return firstRoom;
	}

	void merge_circles(Room* firCircle, Room* secCircle, Room* thiCircle) {
		for (int j = 0; j < 10; j++) {
			if (secCircle->number % 2 == 1) {
				thiCircle->adjoing_3 = secCircle;
				secCircle->adjoing_3 = thiCircle;
				thiCircle = thiCircle->adjoing_2;
			}
			else {
				firCircle->adjoing_3 = secCircle;
				secCircle->adjoing_3 = firCircle;
				firCircle = firCircle->adjoing_2;
			}
			secCircle = secCircle->adjoing_2;
		}
		return;
	}

	Room* map_generator() {
		Room* firCircle = create_circle(1, 5);
		Room* secCircle = create_circle(6, 15);
		Room* thiCircle = create_circle(16, 20);

		secCircle = secCircle->find_in_circle(8);
		thiCircle = thiCircle->find_in_circle(18);

		merge_circles(firCircle, secCircle, thiCircle);

		random_states(firCircle, 4, catapult);
		random_states(firCircle, 4, hole);
		random_states(firCircle, 1, monster);
		random_states(firCircle, 1, player);

		return firCircle;
	}

	void random_states(Room* room, int howMany, RoomState newState) {
		srand(time(NULL));

		for (int i = 0; i != howMany; i++) {
			int random;
			do{ 
				random = (rand() % 20) +1; 
				room = room->find(random);
			} while (room->state != 0);
			room->state = newState;
		}
		return;
	}

	int how_far(int first, int second) {
		return abs(first - second);
	}

	Room* closer_room(int target, Room* firstRoom, Room* secondRoom, Room* thirdRoom) {
		Room* tmp = (how_far(target, firstRoom->number) <= how_far(target, secondRoom->number)) ? firstRoom : secondRoom;
		return (how_far(target, tmp->number) <= how_far(target, thirdRoom->number)) ? tmp : thirdRoom;

	}
}

namespace game_engine {

}

int main() {
	map::Room* map = map::map_generator();

	return 0;
}

/*Room* find(int i) {
			Room* tmp = this;

			for (int j = 0; tmp->number == i; j--) {
				if (j > 8 && i > tmp->number && tmp->number % 2 == 1) tmp = tmp->adjoing_3;
				else if (j > 8 && i < tmp->number && tmp->number % 2 == 0) tmp = tmp->adjoing_3;
				else tmp = tmp->adjoing_2;
			}
			return tmp;
			}*/