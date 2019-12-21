#ifndef DIC_H
#define DIC_H

#include <cassert>
#include "utils.h"

template <typename T1, typename T2>

class Dic {
private:
	struct Node {
		T1 k;
		int h;
		Node *i, *j;
		T2 v;
		int d() {
			return i->h - j->h;
		}
	};

	static Node nil;
	Node* r;
	int n;

public:
	Dic(): r(&nil), n(0) {}

	int size() const {
		return n;
	}

	void insert(const T1& k) {
		insert(r, k);
	}

	T2& operator[](const T1& k) {
		insert(r, k);
		return find(r, k)->v;
	}

	T2* find(const T1& k) {
		if (Node* o = find(r, k))
			return &o->v;
		return 0;
	}

	const T2* find(const T1& k) const {
		if (Node* o = find(r, k))
			return &o->v;
		return 0;
	}

	template <typename Function>
	Dic& each(const Function& f) {
		dfs<Node>(r, f);
	}

	template <typename Function>
	const Dic& each(const Function& f) const {
		dfs<const Node>(r, f);
	}

private:
	static Node* find(Node* o, const T1& k) {
		if (o == &nil)
			return 0;
		if (k < o->k)
			return find(o->i, k);
		if (o->k < k)
			return find(o->j, k);
		return o;
	}

	template <typename NodeType, typename Function>
	static void dfs(NodeType* o, const Function& f) {
		if (o != &nil) {
			f(o);
			dfs(o->i, f);
			dfs(o->j, f);
		}
	}

	static Node* upd(Node* o) {
		o->h = max(o->i->h, o->j->h) + 1;
		return o;
	}

	static void lrot(Node*& o) {
		Node* s = o->j;
		o->j = s->i;
		s->i = upd(o);
		o = upd(s);
	}

	static void rrot(Node*& o) {
		Node* s = o->i;
		o->i = s->j;
		s->j = upd(o);
		o = upd(s);
	}

	void insert(Node*& o, const T1& k) {
		if (o == &nil) {
			o = new Node{k, 1, &nil, &nil};
			++n;
		}else if (k != o->k) {
			if (k < o->k)
				insert(o->i, k);
			else
				insert(o->j, k);
			adjust(o);
		}
	}

	static void adjust(Node*& o) {
		switch (o->d()) {
		default:
			upd(o);
			break;
		case 2:
			switch (o->i->d()) {
			case -1:
				lrot(o->i);
			case 1:
				rrot(o);
				break;
			default:
				assert(0);
			}
			break;
		case -2:
			switch (o->j->d()) {
			case 1:
				rrot(o->j);
			case -1:
				lrot(o);
				break;
			default:
				assert(0);
			}
			break;
		}
	}
};

#endif
