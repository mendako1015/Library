template <class T = int>
class SegmentTree {
	int leaf_num;
	bool is_lazy;
	vector<T> data;
	vector<T> lazy;
	T identity_element;
	function<T(T, T)> operation;
	function<T(T, T)> update_type;
	// ex.) point add RSQ
	// SegmentTree<ll> segtree(n, 0, [](ll a, ll b) { return a + b; }, [](ll a, ll b) { return b; });

	void evaluation(int pos) {
        if(lazy[pos] == identity_element) return ;
        if(pos < leaf_num - 1) {
            lazy[pos * 2 + 1] = lazy[pos];
            lazy[pos * 2 + 2] = lazy[pos];
        }
        data[pos] = lazy[pos];
        lazy[pos] = identity_element;
    }

	void range_update(int l, int r, T x, int pos, int btm, int tp) {
        evaluation(pos);
        if(tp <= l || r <= btm) return ;
        if(l <= btm && tp <= r) {
            lazy[pos] = x;
            evaluation(pos);
        } else {
            int mid = (btm + tp) / 2;
            range_update(l, r, x, pos * 2 + 1, btm, mid);
            range_update(l, r, x, pos * 2 + 2, mid, tp);
            data[pos] = operation(data[pos * 2 + 1], data[pos * 2 + 2]);
        }
    }

	T get_interval(int l, int r, int pos, int btm, int tp) {
		if(is_lazy) evaluation(pos);
		if(tp <= l || r <= btm) return identity_element;
		if(l <= btm && tp <= r) return data[pos];
		int mid = (btm + tp) / 2;
		T l_child = get_interval(l, r, 2 * pos + 1, btm, mid);
		T r_child = get_interval(l, r, 2 * pos + 2, mid, tp);
		return operation(l_child, r_child);
	}

	public:
	SegmentTree(size_t n, bool lzy, T id_el, function<T(T, T)> ope, function<T(T, T)> upd)
	: is_lazy(lzy), identity_element(id_el), operation(ope), update_type(upd) {
		leaf_num = 1;
		while(leaf_num < n) leaf_num *= 2;
		data = vector<T>(2 * leaf_num - 1, identity_element);
		lazy = vector<T>(2 * leaf_num - 1, identity_element);
	}

	// point update query(0-indexed)
	void point_update(int pos, T x) {
		pos += leaf_num - 1;
		data[pos] = update_type(data[pos], x);
		while(pos > 0) {
			pos = (pos - 1) / 2;
			data[pos] = operation(data[pos * 2 + 1], data[pos * 2 + 2]);
		}
	}

	void range_update(int l, int r, T x) {
        range_update(l, r, x, 0, 0, leaf_num);
    }

	// get [l, r) (0-indexed)
	T get_interval(int l, int r) {
		return get_interval(l, r, 0, 0, leaf_num);
	}

	T operator[](int pos) {
		return data[pos + leaf_num - 1];
	}

	void print(int n) {
		for(int i = 0; i < n; i++) cout << data[i + leaf_num - 1] << " ";
		cout << endl;
	}
};