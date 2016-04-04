#ifndef DEF_ACTIONS_H
#define DEF_ACTIONS_H

#include <string>
#include "prologin.hh"

using namespace std;

class state;

enum action_t
{
	ACTION_END_TURN,
	ACTION_BUILD,
	ACTION_COLONIZE,
	ACTION_MOVE,
	ACTION_TRANSFER_ISLAND,
	ACTION_TRANSFER_BOAT
};

class action
{
	
	public:
		action () {}
		virtual void applyTo(state* s) = 0;
		virtual string name() { return ""; };

		action_t actionType;
};

class action_endTurn : public action
{
	public:
		action_endTurn() { actionType = ACTION_END_TURN; }
		void applyTo(state* s);
		string name() { return "End Of Turn"; };
};

class action_build : public action
{
	public:
		action_build(position where, bool what) : where(where), what(what) { actionType = ACTION_BUILD; } //Funny fact, heritated attributes cannot be (compil err) init with " foo(bar) "
		void applyTo(state* s);

		void a() {}

		position where;
		bool what;//0 car, 1 gal
		string name() { return string("Build ")+((what) ? string("gallion") : string("caravelle")) + " on ("+to_string(where.x)+","+to_string(where.y)+")"; };
};

class action_colonize : public action
{
	public:
		action_colonize(position where) : where(where) { actionType = ACTION_COLONIZE; }
		void applyTo(state* s);

		position where;

		string name() { return "Colonize ("+to_string(where.x)+","+to_string(where.y)+")"; };
};

class action_move : public action
{
	public:
		action_move(int id, position whereTo) : id(id), whereTo(whereTo) { actionType = ACTION_MOVE; }
		void applyTo(state* s);

		int id;
		position whereTo;

		string name() { return "Move "+to_string(id)+" to ("+to_string(whereTo.x)+","+to_string(whereTo.y)+")"; };
};

class action_transfer_island : public action
{
	public:
		action_transfer_island(int id, int money) : id(id), money(money) { actionType = ACTION_TRANSFER_ISLAND; }
		void applyTo(state* s);

		int id;
		int money;//algebraic transfer

		string name() { return "Transfer " + to_string(money) + " to island"; };
};

class action_transfer_boat : public action
{
	public:
		action_transfer_boat(int idFrom, int idTo, int money) : idFrom(idFrom), idTo(idTo), money(money) { actionType = ACTION_TRANSFER_BOAT; }
		void applyTo(state* s);

		int idFrom, idTo, money;
		string name() { return "Transfer "+ to_string(money) +" from boat "+ to_string(idFrom)+" to boat "+to_string(idTo); };
};

#endif