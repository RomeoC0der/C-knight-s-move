#include <iostream>
#include <string>
#include <Windows.h>
template<typename Key, typename Args>
class deque_table {
public:
	deque_table() {
		this->capacity = NULL;
		this->_size_ = NULL;
		this->keys = nullptr;
		this->args = nullptr;
	};
	deque_table(int capacity) {
		this->capacity = capacity;
		this->_size_ = NULL;
		this->keys = nullptr;
		this->args = nullptr;
		reserve(capacity);
	};
	~deque_table() {
		if (args)delete[] args;
		if (keys)delete[] keys;
		args = nullptr;
		keys = nullptr;
		_size_ = 0;
		capacity = 0;
	};
	inline int size() { return _size_; };
	void reserve(int size);
	void push_back(std::pair<Key, Args>);
	inline void force_clear();
	Args& find(Key k);
private:
	Key* keys;//указатель на массив
	Args* args;//указатель на массив
	int capacity = 0;
	int _size_ = 0;
};
class game_sheet {
public:
	game_sheet() {
		this->_column_ = -1;
		this->_row_ = -1;
		this->_filled_ = false;
		this->name = "0";
		figure_on_sheet = false;
	};
	~game_sheet() {};
	game_sheet(int col, int row, std::string name) {
		this->_column_ = col;
		this->_row_ = row;
		this->_filled_ = false;
		this->name = name;
		figure_on_sheet = false;
	};
	game_sheet(int col, int row) {
		this->_column_ = col;
		this->_row_ = row;
		this->_filled_ = false;
		this->name = "0";
		figure_on_sheet = false;
	};
	static bool is_error_class(const game_sheet& g_other) {
		if (g_other._column_ == -1 || g_other._row_ == -1)return true;
		return false;
	}
	bool is_error()
	{
		if (_column_ == -1 || _row_ == -1)return true;
		return false;
	}
	bool filled() { return _filled_; };
	bool figure() { return figure_on_sheet; };
	int get_column() { return _column_; };
	int get_row() { return _row_; };
	std::string get_name() { return name; };
	void fill() { _filled_ = true; };
	void set_figure() { figure_on_sheet = true; };
	static game_sheet throw_error_class() {
		return game_sheet(-1, -1);
	}
private:
	int _column_;
	int _row_;
	bool _filled_;
	bool figure_on_sheet;
	std::string name;
	const int error_row = -1;
	const int error_col = -1;
};
void print_table(deque_table<std::string, game_sheet*>& table) {
	std::string buff = "";
	std::cout << "\n";
	for (int i = 0; i < 8; i++)
	{
		char current_column = static_cast<char>('A' + i);
		std::cout << "\t" << current_column << "\t";
		for (int j = 0; j < 8; j++)
		{
			char current_row = static_cast<char>('1' + j);
			buff += current_column;
			buff += current_row;
			game_sheet* z = table.find(buff);
			if (z->filled() && !z->figure())
			{
				std::cout << "X ";
			}
			else if (z->figure()) {
				std::cout << "F ";
			}
			else std::cout << "# ";
			buff.clear();
		}
		std::cout << "\n";
	}
	std::cout << "\n\t\t1 2 3 4 5 6 7 8 ";
}
int horse_step(deque_table<std::string, game_sheet*>& table, std::string start_pos)
{
	game_sheet* sheet = table.find(start_pos);
	if (sheet->is_error())return 0;
	char col = sheet->get_name()[0];
	char row = sheet->get_name()[1];
	std::pair<char, char> all_might_steps[8] = {
		{col + 2, row + 1},/*вправо вверх*/
		{col + 1, row + 2},/*вправо вверх*/
		{col + 1, row - 2},/*вправо вниз*/
		{col + 2, row - 1},/*вправо вниз*/
		{col - 2, row - 1},/*вlevo вниз*/
		{col - 1, row - 2},/*вlevo вниз*/
		{col - 2, row + 1},/*вlevo vverh*/
		{col - 1, row + 2},/*вlevo vverh*/
	};
	std::string buffer = "";
	int counter = 0;
	for (int i = 0; i < 8; i++)
	{
		if (all_might_steps[i].first < 'A')all_might_steps[i].first = 'A';
		if (all_might_steps[i].first > 'H')all_might_steps[i].first = 'H';
		if (all_might_steps[i].second < '1')all_might_steps[i].second = '1';
		if (all_might_steps[i].second > '8')all_might_steps[i].second = '8';
		buffer += all_might_steps[i].first;
		buffer += all_might_steps[i].second;
		game_sheet* might_sheet = table.find(buffer);
		buffer.clear();
		if (might_sheet->is_error() || might_sheet->filled())continue;
		int target_col = might_sheet->get_column();
		int target_row = might_sheet->get_row();
		int local_col = sheet->get_column();
		int local_row = sheet->get_row();
		if ((abs(target_col - local_col) == 2 && abs(local_row - target_row) == 1) || (abs(target_col - local_col) == 1 && abs(local_row - target_row) == 2))
		{
			might_sheet->fill();
			counter++;
		}
	}
	return counter;
}
int ladya_step(deque_table<std::string, game_sheet*>& table, std::string start_pos)
{
	game_sheet* sheet = table.find(start_pos);
	if (sheet->is_error())return 0;
	char col = sheet->get_name()[0];
	char row = sheet->get_name()[1];
	std::string buffer = "";
	int counter = 0;
	for (int i = 1; i < 9; i++)
	{
		buffer += col;
		buffer += (char)('0' + i);
		game_sheet* might_sheet = table.find(buffer);
		buffer.clear();
		if (might_sheet->is_error() || might_sheet->filled())continue;
		might_sheet->fill();
		counter++;
	}
	for (int i = 0; i < 8; i++)
	{
		buffer += (char)('A' + i);
		buffer += row;
		game_sheet* might_sheet = table.find(buffer);
		buffer.clear();
		if (might_sheet->is_error() || might_sheet->filled())continue;
		might_sheet->fill();
		counter++;
	}
	return counter++;
}
int ferz_step(deque_table<std::string, game_sheet*>& table, std::string start_pos)
{
	game_sheet* sheet = table.find(start_pos);
	if (sheet->is_error())return 0;
	char col = sheet->get_name()[0];
	char row = sheet->get_name()[1];
	std::string buffer = "";
	int counter = 0;
	for (int i = 1; i < 9; i++)
	{
		buffer += col;
		buffer += (char)('0' + i);
		game_sheet* might_sheet = table.find(buffer);
		buffer.clear();
		if (might_sheet->is_error() || might_sheet->filled())continue;
		might_sheet->fill();
		counter++;
	}
	for (int i = 0; i < 8; i++)
	{
		buffer += (char)('A' + i);
		buffer += row;
		game_sheet* might_sheet = table.find(buffer);
		buffer.clear();
		if (might_sheet->is_error() || might_sheet->filled()) {
			continue;
		}
		might_sheet->fill();
		counter++;
	}
	for (int i = 1; i < 9; i++)
	{
		buffer += static_cast<char>(col + i);
		buffer += static_cast<char>(row + i);
		if (buffer[0] > 'H' || buffer[1] > '8') {
			buffer.clear(); continue;
		}
		game_sheet* might_sheet = table.find(buffer);
		buffer.clear();
		if (might_sheet != nullptr && !might_sheet->is_error() && !might_sheet->filled())
		{
			might_sheet->fill();
			counter++;
		}
	}
	for (int i = 1; i < 9; i++)
	{
		buffer += static_cast<char>(col - i);
		buffer += static_cast<char>(row - i);
		if (buffer[0] < 'A' || buffer[1] < '1') {
			buffer.clear();
			continue;
		}
		game_sheet* might_sheet = table.find(buffer);
		buffer.clear();
		if (might_sheet != nullptr && !might_sheet->is_error() && !might_sheet->filled())
		{
			might_sheet->fill();
			counter++;
		}
	}
	for (int i = 1; i < 9; i++)
	{
		buffer += static_cast<char>(col - i);
		buffer += static_cast<char>(row + i);
		if (buffer[0] < 'A' || buffer[1] > '8') {
			buffer.clear();
			continue;
		}
		game_sheet* might_sheet = table.find(buffer);
		buffer.clear();
		if (might_sheet != nullptr && !might_sheet->is_error() && !might_sheet->filled())
		{
			might_sheet->fill();
			counter++;
		}
	}
	for (int i = 1; i < 9; i++)
	{
		buffer += static_cast<char>(col + i);
		buffer += static_cast<char>(row - i);
		if (buffer[0] > 'H' || buffer[1] < '1') {
			buffer.clear();
			continue;
		}
		game_sheet* might_sheet = table.find(buffer);
		buffer.clear();
		if (might_sheet != nullptr && !might_sheet->is_error() && !might_sheet->filled())
		{
			might_sheet->fill();
			counter++;
		}
	}
	return counter++;
}
int main() {
	deque_table<std::string, game_sheet*> map;
	std::string buffer = "";
	for (int i = 0; i < 8; i++)
	{
		char current_column = static_cast<char>('A' + i);
		for (int j = 0; j < 8; j++)
		{
			char current_row = static_cast<char>('1' + j);
			buffer += current_column;
			buffer += current_row;
			map.push_back({ buffer, new game_sheet(i + 1, j + 1, buffer) });
			buffer.clear();
		}
	}
	std::string a, b, c;
	std::cin >> a >> b >> c;
	if (a.length() != 2 || b.length() != 2)return 0;
	game_sheet* a_sheet = map.find(a);
	game_sheet* b_sheet = map.find(b);
	game_sheet* c_sheet = map.find(c);
	if (!a_sheet || !b_sheet || !c_sheet || a_sheet->is_error() || b_sheet->is_error() || c_sheet->is_error())return 0;
	a_sheet->set_figure();
	a_sheet->fill();
	b_sheet->set_figure();
	b_sheet->fill();
	c_sheet->set_figure();
	c_sheet->fill();
	int h_steps = horse_step(map, c);
	int l_steps = ladya_step(map, b);
	int f_steps = ferz_step(map, a);
	//print_table(map);
	//map.force_clear();
	std::cout << h_steps + l_steps + f_steps;
	//system("pause");
	return 0;
}
template<typename Key, typename Args>
void deque_table<Key, Args>::reserve(int size)
{
	if (size <= capacity)return;
	if (this->_size_ == 0)
	{
		this->args = new Args[size];
		this->keys = new Key[size];
		this->capacity = size;
	}
	else {
		Args* copy_arg = new Args[this->_size_];
		Key* copy_key = new Key[this->_size_];

		/*memcpy(copy_arg, this->args, sizeof(Args) * this->_size_);
		memcpy(copy_key, this->keys, sizeof(Key) * this->_size_);*/
		for (int i = 0; i < _size_; i++)
		{
			copy_arg[i] = this->args[i];
			copy_key[i] = this->keys[i];
		}
		//delete[] this->args;
		//delete[] this->keys;
		this->args = new Args[size];
		this->keys = new Key[size];
		/*memcpy(this->args, copy_arg, sizeof(Args) * this->_size_);
		memcpy(this->keys, copy_key, sizeof(Key) * this->_size_);*/
		for (int i = 0; i < _size_; i++)
		{
			args[i] = copy_arg[i];
			keys[i] = copy_key[i];
		}
		this->capacity = size;
		delete[] copy_arg;
		memset(copy_key, 0, sizeof(Key) * _size_);
		//copy_key = nullptr;//memory leak, but fucking site doesnt crash it
	}
}
template<typename Key, typename Args>
void deque_table<Key, Args>::push_back(std::pair<Key, Args>elem)
{
	if (this->_size_ + 1 <= this->capacity)
	{
		this->args[_size_] = elem.second;
		//memcpy(&this->args[_size_], &elem.second, sizeof(Args));
		this->keys[_size_] = elem.first;
		this->_size_++;
	}
	else {
		reserve(this->_size_ + 1);
		push_back(elem);
	}
}
template<typename Key, typename Args>
inline void deque_table<Key, Args>::force_clear()
{
	delete[] args;
	delete[] keys;
	//memset(args, 0, sizeof(Args));
	//memset(keys, 0, sizeof(Key));
	args = nullptr;
	keys = nullptr;
	_size_ = 0;
	capacity = 0;
}
template<typename Key, typename Args>
Args& deque_table<Key, Args>::find(Key k)
{
	for (int i = 0; i < this->_size_; ++i)
	{
		if (k == this->keys[i])return args[i];
	}
	Args stack_arg = Args();
	return stack_arg;
}
