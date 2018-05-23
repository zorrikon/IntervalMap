#include <map>
#include <limits>
#include <iostream>
#include <iterator>

template <typename K,typename V>
class IntervalMap {
public:
	IntervalMap(V const & val) {
		m_map.insert(m_map.begin(),
					std::make_pair(std::numeric_limits<K>::min(), val));
	}
	V & operator [] (K const & key) {
		return (--m_map.upper_bound(key))->second;
	}

	void assign(K const& keyBegin, K const& keyEnd, V const& val) {
	    using iterator_t = typename std::map<K, V>::iterator;
        // We will erase every key where keyBegin <= key <= keyEnd
        iterator_t firstErase = m_map.lower_bound(keyBegin);
        iterator_t lastErase = m_map.upper_bound(keyEnd);
        
        // We need to write m_map[keyBegin] = val as long as the element before
        // our range doesn't have value = val.
        bool needBegin = firstErase == m_map.begin() || 
                         std::prev(firstErase)->second != val;

        // endVal is the value after keyEnd.
        V endVal = std::prev(lastErase)->second;

        m_map.erase(firstErase, lastErase);
        if (needBegin) {
            m_map.insert(lastErase, std::make_pair(keyBegin, val));
        }
        // Only write endVal if it's not redundant (!= val).
        if (endVal != val) {
            m_map.insert(lastErase, std::make_pair(keyEnd, endVal));
        }
        // If the element after keyEnd is redundant (== endVal), erase it.
        if (lastErase != m_map.end() && lastErase->second == endVal) {
            m_map.erase(lastErase);
        }
	}

	void test_interval_map() {
		m_map.insert(std::make_pair(1, 'b'));
		m_map.insert(std::make_pair(4, 'c'));
		m_map.insert(std::make_pair(7, 'd'));
		m_map.insert(std::make_pair(10, 'e'));
		m_map.insert(std::make_pair(13, 'a'));
	}

	// a print function for debugging
	void show() {
		std::cout << "show" << std::endl;
		for(auto entry : m_map) {
			std::cout << entry.first << entry.second << std::endl;
		}
	}

private:
	std::map<K,V> m_map;
};

int main() {
	IntervalMap<unsigned int, char> imap {'a'};
	imap.show();
	imap.assign(8, 10, 'k');
	imap.show();
	imap.assign(8, 12, 'k');
	imap.show();
	imap.assign(2, 12, 'k');
	imap.show();
	imap.assign(2, 12, 'b');
	imap.show();
	imap.assign(5, 12, 'b');
	imap.show();
	imap.assign(4, 10, 'b');
	imap.show();
	imap.assign(4, 12, 'b');
	imap.show();
	imap.assign(7, 13, 'a');
	imap.show();
	imap.assign(0, 10, 'e');
	imap.show();
}
