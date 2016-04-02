#ifndef DEF_ACTIONS_H
#define DEF_ACTIONS_H

#include <string>
#include "prologin.hh"

using namespace std;

class state;

class action
{
	
	public:
		action () {}
		virtual void applyTo(state* s) = 0;
		virtual string name() = 0;
	
};

class action_endTurn : public action
{
	public:
		void applyTo(state* s);
		string name() { return "End Of Turn"; };
};

class action_build : public action
{
	public:
		action_build(position where, bool what) : where(where), what(what) {}
		void applyTo(state* s);

		position where;
		bool what;//0 car, 1 gal
		string name() { return string("Build ")+((what) ? string("gallion") : string("caravelle")); };
};

class action_colonize : public action
{
	public:
		action_colonize(position where) : where(where) {}
		void applyTo(state* s);

		position where;

		string name() { return "Colonize"; };
};

class action_move : public action
{
	public:
		action_move(int id, position whereTo) : id(id), whereTo(whereTo) {}
		void applyTo(state* s);

		int id;
		position whereTo;

		string name() { return "Move"; };
};

class action_transfer_island : public action
{
	public:
		action_transfer_island(int id, int money) : id(id), money(money) {}
		void applyTo(state* s);

		int id;
		int money;//algebraic transfer

		string name() { return "Transfer island"; };
};

class action_transfer_boat : public action
{
	public:
		action_transfer_boat(int idFrom, int idTo, int money) : idFrom(idFrom), idTo(idTo), money(money) {}
		void applyTo(state* s);

		int idFrom, idTo, money;
		string name() { return "Transfer boat"; };
};

#endif