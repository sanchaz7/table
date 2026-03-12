#include <iostream>
#include <vector>

using namespace std;

template <typename TK, typename TV>

class Table
{
public:
	virtual const TV& Find(const TK& Key) const = 0; //только чтение
	virtual void Delete(const TK& Key) = 0;
	virtual void Insert(const TK& Key, const TV& Val) = 0;
	virtual void PrintAll() const = 0;

};

template <typename TK, typename TV>
class NotOrderedMas : public Table<TK,TV>  //неупорядоченный динакмический массив
{
private:
	vector <pair<TK, TV>> data;
public:
	NotOrderedMas() : data() {}
	void Insert(const TK& Key, const TV& Val) override {
		data.push_back(make_pair(Key, Val)); //make_pair создает пару из ключа и значения
	}

	void Delete(const TK& Key) override {
		for (int i = 0; i < data.size(); i++) {
			if (data[i].first == Key)
			{
				data.erase(data.begin() + i); //удаляю элемент с индексом i
				return;
			}
		}
		cout << "Элемента с ключом " << Key << " нет" << endl;
	}

	const TV& Find(const TK& Key) const override {
		for (int i = 0; i < data.size(); i++) {
			if (data[i].first == Key) return data[i].second;
		}
		throw runtime_error("Такого элемента нет"); //??
	}

	void PrintAll() const override {
		for (int i = 0; i < data.size(); i++) {
			cout << data[i].first << " - " << data[i].second << " ";
		}
		cout << endl;
	}

};


template <typename TK, typename TV>
class OrderedMas : public Table<TK,TV>  //упорядоченный динакмический массив
{
private:
	vector <pair<TK, TV>> data;
public:
	OrderedMas() : data() {}

	int SearchPlaceBin(const TK&Key) const
	{
		int left = 0;
		int right = data.size()-1;
		while (left<=right)
		{
			int mid = left + (right-left)/2;
			if (data[mid].first==Key) return mid;
			else if (data[mid].first<Key) left = mid + 1;
			else right = mid - 1;
		}
		return left;
	}

	void Insert(const TK& Key, const TV& Val) override 
	{
		int pos = SearchPlaceBin(Key);
		data.resize(data.size()+1);
		for (int i = data.size()-1;i>pos;i--)
		{
			data[i]=data[i-1];
		}
		data[pos]= make_pair(Key,Val);
	}

	void Delete(const TK& Key) override 
	{
		int pos = SearchPlaceBin(Key);
		if (pos<data.size() && data[pos].first == Key)
		{
			data.erase(data.begin()+pos);
			return;
		}
		cout << "Элемента с ключом " << Key << " нет" << endl;
	}

	const TV& Find(const TK& Key) const override {
		int pos = SearchPlaceBin(Key);
		if (pos<data.size() && data[pos].first == Key)
		{
			return data[pos].second;
		}
		throw runtime_error("Такого элемента нет"); 
	}

	void PrintAll() const override {
		for (int i = 0; i < data.size(); i++) {
			cout << data[i].first << " - " << data[i].second << " ";
		}
		cout << endl;
	}

};

template <typename TK, typename TV>
class StandartTree : public Table<TK, TV> {
struct Node {
pair <TK, TV> KV; //ключ-значение
Node* left;
Node* right;
Node* parent;
Node() : left(nullptr), right(nullptr), parent(nullptr) {}
Node(TK k, TV v, Node* l, Node* r, Node* p) : KV(k, v), left(l), right(r), parent(p) {} //добавил конструктор с параметрами в класс BST
};
protected:
Node* root; //корень

public:
StandartTree() : root(nullptr) {}
~StandartTree() { clear_all(root); } //дописать
protected:
auto FindMin(Node* subtree) const-> Node*;
auto FindMax(Node* subtree) const -> Node*;

auto FindNext(Node* x) const -> Node*;
auto FindBefor(Node* x) const -> Node*;
void clear_all(Node* node) {
if (node) {
clear_all(node->left);
clear_all(node->right);
delete node;
}
}

auto FindNodeInTree(const TK& key) const->Node*;

void BeautifullPrintTree(Node* node = root, string prefix = "", bool isLeft = true) const  {
if (node == nullptr) return;

cout << prefix;
cout << (isLeft ? "+-- " : "L-- ");
cout << node->KV.first << "(" << node->KV.second << ")" << endl;

// Формируем префикс для детей
string newPrefix = prefix + (isLeft ? "|   " : "    ");

BeautifullPrintTree(node->left, newPrefix, true);
BeautifullPrintTree(node->right, newPrefix, false);
}

public:
const TV& Find(const TK& Key) const override {
if (root == nullptr) throw out_of_range("Дерево пусто");

Node* t = root;
while ((t->KV).first != Key) {
if (Key < (t->KV).first) { //КЛЮЧ < Текущего
if (t->left == nullptr) throw out_of_range("Нет такого узла по заданному ключу");//нет того, что ищем
else t = t->left;
}
else {
if (t->right == nullptr) throw out_of_range("Нет такого узла по заданному ключу"); //нет того, что ищем
else t = t->right;
}
}
return (t->KV).second;
}

void PrintAll() const override {
Node* t = this->FindMin(root);

if (t == nullptr) { cout << "Дерево пусто" << endl; return; }

while (t != nullptr) {
cout << t->KV.first << " - " << t->KV.second << "   ";
t = this->FindNext(t);
}
cout << endl;
} // до сюда вроде норм проверено

void BeautifullPrint() const {
if (root == nullptr) { cout << "Дерево пустое" << endl; return; }
else BeautifullPrintTree(root, "", true);
}

void Insert(const TK& key, const TV& val) override {
Node* node = new Node;
node->KV.first = key;
node->KV.second = val;

if (root == nullptr) {
node->parent = nullptr; //родитель корня нулевой или ссылается на себя?
root = node;
}
else {
Node* t = root;
while (true) {
if (node->KV.first < t->KV.first) {
if (t->left == nullptr) {
t->left = node;
node->parent = t;
break;
}
else t = t->left;
}
else {
if (t->right == nullptr) {
t->right = node;
                   node->parent = t;
break;
    }
else t = t->right;
}
}
}
}

void Delete(const TK& Key) override {
Node* x = FindNodeInTree(Key);

if (x->left == nullptr && x->right == nullptr) { // это лист
if (x == root) { root = nullptr; delete x; return; }
Node* par = x->parent;
if (par->left == x) par->left = nullptr; // удаляю левый
else par->right = nullptr; //удаляю правый лист
delete x;
}
else if(x->left == nullptr && x->right != nullptr) { // удаляю узел с правым потомком
if (x == root) {
Node* child = x->right;
child->parent = nullptr;
root = child;
delete x;
return;
}

Node* par = x->parent;
Node* child = x->right;
child->parent = par;
if (par->left == x) par->left = child;
else par->right = child;
delete x;
}
else if(x->left != nullptr && x->right == nullptr) { //удаляю только с левым потомком
if (x == root) {
Node* child = x->left;
child->parent = nullptr;
root = child;
delete x;
return;
}

Node* par = x->parent;
Node* child = x->left;
child->parent = par;
if (par->left == x) par->left = child;
else par->right = child;
delete x;
}
else { // удаляю узел с двумя потомками
Node* y = FindMin(x->right); //новый элемент на месте удаляемого

x->KV.first = y->KV.first;   //
x->KV.second = y->KV.second; //в х сохранил значения y

Node* par_y = y->parent;
Node* child_y = y->right;

if (par_y->left == y) { // y слева
par_y->left = child_y;
}
else { // y справа
par_y->right = child_y;
}

if (child_y != nullptr) child_y->parent = par_y;

delete y;
}
}
};

template<typename TK, typename TV>
auto StandartTree<TK,TV>::FindMin(Node* subtree) const -> Node* { //поиск минимума в поддеревее subtree
Node* t = subtree;
if (t == nullptr) return nullptr;

while (t->left != nullptr) { t = t->left; }
return t;
}

template<typename TK, typename TV>
auto StandartTree<TK, TV>::FindMax(Node* subtree) const -> Node* {
Node* t = subtree;
if (t == nullptr) return nullptr;

while (t->right != nullptr) t = t->right;
return t;
}

template<typename TK, typename TV>
auto StandartTree<TK,TV>::FindNext(Node* x) const -> Node* { //ищем следующий по узлу
if (x == nullptr) return nullptr;

if (this->FindMax(root) == x) return nullptr; //нет больше максимума

if (x->right != nullptr) return this->FindMin(x->right); //если есть правый потомок, ищу в после него минимум

else { // если нет правого потомка
Node* cur = x; Node* par = x->parent;
while (par != nullptr && par->left != cur) { //пока не пришли к ролителю слева
cur = cur->parent; par = par->parent;
}
return par;
}
}

template<typename TK, typename TV>
auto StandartTree<TK,TV>::FindBefor(Node* x) const -> Node* { //ищем предыдущий по узлу
if (x == nullptr) return nullptr;

if (x == this->FindMin(root)) return nullptr;

if (x->left != nullptr) return this->FindMax(x->left);

else {
Node* cur = x; Node* par = x->parent;
while (par->right != cur) {
cur = cur->parent; par = par->parent;
}
return par;
}
}

template<typename TK, typename TV>
auto StandartTree<TK, TV>::FindNodeInTree(const TK& key) const -> Node* { //поиск в дереве, только возращает узел
if (root == nullptr) return nullptr;
Node* t = root;
while (t->KV.first != key) {
if (key < t->KV.first) {
if (t->left == nullptr) throw out_of_range("Нет такого узла по заданному ключу");
else t = t->left;
}
else {
if (t->right == nullptr) throw out_of_range("Нет такого узла по заданному ключу"); //нет того, что ищем
else t = t->right;
}
}
return t;
}




template<typename TK, typename TV>
class AVLTree : public Table {
struct AVLNode {
pair <TK, TV> KV; //ключ-значение
AVLNode* left;
AVLNode* right;
AVLNode* parent;
int balance;

AVLNode() : left(nullptr), right(nullptr), parent(nullptr), balance(0) {}
AVLNode(TK k, TV v, AVLNode* r = nullptr, AVLNode* l = nullptr, AVLNode* p = nullptr) : KV(k, v), left(l), right(r), parent(p), balance(0) {}
};
AVLNode* root;

public:
AVLTree() : root(nullptr) {}
~AVLTree() { clear_all(root); }

protected:
void clear_all(AVLNode* node) {
if (node) {
clear_all(node->left);
clear_all(node->right);
delete node;
}
}

auto FindMin(AVLNode* subtree) const->AVLNode*;
auto FindMax(AVLNode* subtree) const->AVLNode*;

auto FindNext(AVLNode* x) const->AVLNode*;
auto FindBefor(AVLNode* x) const->AVLNode*;

auto FindNodeInTree(const TK& key) const->AVLNode*;

void BeautifullPrintTree(AVLNode* node = root, string prefix = "", bool isLeft = true) const {
if (node == nullptr) return;

cout << prefix;
cout << (isLeft ? "+-- " : "L-- ");
cout << node->KV.first << "(" << node->KV.second << ")" << endl;

// Формируем префикс для детей
string newPrefix = prefix + (isLeft ? "| " : " ");

BeautifullPrintTree(node->left, newPrefix, true);
BeautifullPrintTree(node->right, newPrefix, false);
}



AVLNode* RR(AVLNode* p) { // p - узел, в котором нарушается условие АВЛ дерева
AVLNode* x = p->right; // x - потомок справа у p
AVLNode* y = x->left;

if (p == root) root = x;

if (p->parent) {
x->parent = p->parent;
if (p->parent->right == p) p->parent->right = x; // если p правый сын
else p->parent->left = x; // если p левый сын
} //связал родителя p и x

if (y) { p->right = y; y->parent = p; }
else { p->right = nullptr; }
//поменял детей p

x->left = p;
p->parent = x;

p->balance = 0;
x->balance = 0;

return x;
}
AVLNode* LL(AVLNode* p) {
AVLNode* x = p->left;
AVLNode* y = x->right;

if (p == root) root = x;

if (p->parent) { //если есть родители p => меняю связи
x->parent = p->parent;
if ((p->parent)->left == p) p->parent->left = x;
else p->parent->right = x;
}

if (y) { p->left = y; y->parent = p; }
else p->left = nullptr;

x->right = p;
p->parent = x;

x->balance = 0;
p->balance = 0;

return x;
}


void updateBalancesAfterRL(AVLNode* p, AVLNode* x, AVLNode* y, int OriginalBalY) {
switch (OriginalBalY) {
case 1:  // y имел перевес вправо
p->balance = -1;   // левое поддерево p выше
x->balance = 0;     // x уравновешен
break;

case -1: // y имел перевес влево
p->balance = 0;
x->balance = 1;     // правое поддерево x выше
break;

case 0:  // y был уравновешен
p->balance = 0;
x->balance = 0;
break;
}
y->balance = 0;
}
AVLNode* RL(AVLNode* p) {
AVLNode* x = p->right;
AVLNode* y = x->left;
AVLNode* pParent = p->parent;

bool pIsLeftChild = (pParent && pParent->left == p); // какой p ребенок: правый или левый

int OriginalBalY = y->balance; // для изменения баланса после

bool pIsRoot = (p == root); // корень ли p

p->right = nullptr;
x->parent = nullptr; // разрываю связи для корректной работы для RR поворота

RR(x);

p->right = y;
y->parent = p; // восстановил связи после RR поворота

LL(p);

updateBalancesAfterRL(p, x, y, OriginalBalY); // обновляю балансы

y->parent = pParent;
if (pParent) {
if (pIsLeftChild) pParent->left = y;
else pParent->right = y;
}

if (pIsRoot) root = y;

return y;
}


void updateBalancesAfterLR(AVLNode* p, AVLNode* x, AVLNode* y, int OriginalBalY) {
switch (yOriginalBalance) {
case 1:  // y имел перевес вправо
p->balance = 0;
x->balance = -1;    // у x левое поддерево выше
break;

case -1: // y имел перевес влево
p->balance = 1;     // у p правое поддерево выше
x->balance = 0;
break;

case 0:  // y был уравновешен
p->balance = 0;
x->balance = 0;
break;
}
// y всегда становится сбалансированным
y->balance = 0;
}
AVLNode* LR(AVLNode* p) {
AVLNode* x = p->left;
AVLNode* y = x->right;
AVLNode* pParent = p->parent;

bool pIsRoot = (p == root);

bool pIsLeftChild = (pParent && p->parent->left == p);

int OriginalBalY = y->balance;

x->parent = nullptr;
p->left = nullptr; //разрываю связь для нормального ЛЛ поворота

LL(x);

p->left = y;
y->parent = p; //восставновил свзязь

RR(p);

updateBalancesAfterLR(p, x, y, OriginalBalY);

if (pParent) {
if (pIsLeftChild) pParent->left = y;
else pParent->right = y;
}
y->parent = pParent;

if (pIsRoot) root = y;

return y;
}


public:
const TV& Find(const TK& Key) const override {
if (root == nullptr) throw out_of_range("Дерево пусто");

AVLNode* t = root;
while ((t->KV).first != Key) {
if (Key < (t->KV).first) { //КЛЮЧ < Текущего
if (t->left == nullptr) throw out_of_range("Нет такого узла по заданному ключу");//нет того, что ищем
else t = t->left;
}
else {
if (t->right == nullptr) throw out_of_range("Нет такого узла по заданному ключу"); //нет того, что ищем
else t = t->right;
}
}
return (t->KV).second;
}

void PrintAll() const override {
AVLNode* t = this->FindMin(root);

if (t == nullptr) { cout << "Дерево пусто" << endl; return; }

while (t != nullptr) {
cout << t->KV.first << " - " << t->KV.second << " ";
t = this->FindNext(t);
}
cout << endl;
} // до сюда вроде норм проверено

void BeautifullPrint() const {
if (root == nullptr) { cout << "Дерево пустое" << endl; return; }
else BeautifullPrintTree(root, "", true);
}

void Insert(const TK& key, const TV& val) override {
//1.вставка, как в обычном BST
AVLNode* node = new AVLNode;
node->KV.first = key;
node->KV.second = val;

if (root == nullptr) {
node->parent = nullptr; //родитель корня нулевой или ссылается на себя?
root = node;
}
else {
AVLNode* t = root;
while (true) {
if (node->KV.first < t->KV.first) {
if (t->left == nullptr) {
t->left = node;
node->parent = t;
break;
}
else t = t->left;
}
else {
if (t->right == nullptr) {
t->right = node;
node->parent = t;
break;
}
else t = t->right;
}
}
}
if(node->parent) BalanceAfterInsert(node->parent, key < node->parent->KV.first); //передаю родителя вствленного элемента и понимание куда вставили: влево или вправо
}
void BalanceAfterInsert(AVLNode* node, bool GoLeft) {
//от нового коря вверх, регулируя баланс
AVLNode* cur = node;

while (cur != nullptr) {
if (GoLeft) cur->balance--;
else cur->balance++;

if (cur->balance == 0) return; // критрий выхода из балансировки

if (cur->balance == 2 || cur->balance == -2) { //нарушение баланса!
AVLNode* newSubstreeRoot = nullptr;
if (cur->balance == 2) {
if (cur->right->balance > 0) newSubstreeRoot = RR(cur); //перевес в правом поддереве справа
else newSubstreeRoot = RL(cur); //перевес в правом поддереве слева
}
else { //cur->bal == -2
if (cur->left->balance < 0) newSubstreeRoot = LL(cur); //перевес в левом поддере слева
else newSubstreeRoot = LR(cur);
}

if (newSubstreeRoot->parent == nullptr) root = newSubstreeRoot; // если корнем стал

return; //тк в АВЛ нужен только один поворот при поочереднои добавлении в дерево
}

if (cur->parent != nullptr) {
GoLeft = (cur->parent->left == cur);
}
cur = cur->parent;
}
}

void Delete(const TK& Key) override  {
Node* x = FindNodeInTree(Key);
if (!x) return; // ничего не происходит при остуствии того, кого искали

AVLNode* balanceStart = nullptr; // откуда начинаем балансировку
bool GoLeft;


if (x->left == nullptr && x->right == nullptr) { // это лист
balanceStart = x->parent; // балансируем, начиная от родителя
if (x->parent) GoLeft = (x->parent->left == x); // откуда пришли анализирую

if (x == root) { root = nullptr; delete x; return; }
Node* par = x->parent;
if (par->left == x) par->left = nullptr; // удаляю левый
else par->right = nullptr; //удаляю правый лист
delete x;
}


else if (x->left == nullptr && x->right != nullptr) { // удаляю узел с правым потомком
balanceStart = x->parent;
if(balanceStart) GoLeft = (x->parent->left == x);

if (x == root) {
Node* child = x->right;
child->parent = nullptr;
root = child;
delete x;
return;
}

Node* par = x->parent;
Node* child = x->right;
child->parent = par;
if (par->left == x) par->left = child;
else par->right = child;
delete x;
}
else if (x->left != nullptr && x->right == nullptr) { //удаляю только с левым потомком
balanceStart = x->parent;
if (balanceStart) GoLeft = (x->parent->left == x);

if (x == root) {
Node* child = x->left;
child->parent = nullptr;
root = child;
delete x;
return;
}

Node* par = x->parent;
Node* child = x->left;
child->parent = par;
if (par->left == x) par->left = child;
else par->right = child;
delete x;
}
else { // удаляю узел с двумя потомками !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! тяжелый случай
Node* y = FindMin(x->right); //новый элемент на месте удаляемого

x->KV.first = y->KV.first;   //
x->KV.second = y->KV.second; //в х сохранил значения y

Node* par_y = y->parent;
Node* child_y = y->right;

if (par_y->left == y) { // y слева
par_y->left = child_y;
}
else { // y справа
par_y->right = child_y;
}

if (child_y != nullptr) child_y->parent = par_y;

//точка старта балансировки
if (child_y) {
balanceStart = child_y;        // балансируем от поднявшегося потомка
GoLeft = false;              // child_y всегда справа
}
else {
balanceStart = y_parent;       // балансируем от родителя y
GoLeft = (y_parent && y_parent->left == y); // направление удаления
}

delete y;
}
if (balanceStart) BalanceAfterDelete(balanceStart, GoLeft);
}
void BalanceAfterDelete(AVLNode* balanceStart, bool GoLeft) { //!!! проверить
AVLNode* cur = balanceStart;

while (cur) {
if (GoLeft) cur->balance--;
else cur->balance++;

if (cur->balance == 2) {
if (cur->right->balance >= 0)
cur = RR(cur);
else
cur = RL(cur);
}
else if (cur->balance == -2) {
if (cur->left->balance <= 0)
cur = LL(cur);
else
cur = LR(cur);
}
// Если после поворота current стал корнем
if (cur->parent == nullptr)
root = cur;

// Критерий остановки: баланс стал ±1 (высота не изменилась)
// или мы дошли до корня
if (cur->balance == 1 || cur->balance == -1)
break;

// Поднимаемся выше
if (cur->parent) {
GoLeft = (cur->parent->left == cur);
}
cur = cur->parent;
}
}


};

template<typename TK, typename TV>
auto AVLTree<TK, TV>::FindMin(AVLNode* subtree) const -> AVLNode* {//поиск минимума в поддеревее subtree
AVLNode* t = subtree;
if (t == nullptr) return nullptr;

while (t->left != nullptr) { t = t->left; }
return t;
}

template<typename TK, typename TV>
auto AVLTree<TK, TV>::FindMax(AVLNode* subtree) const -> AVLNode* {
AVLNode* t = subtree;
if (t == nullptr) return nullptr;

while (t->right != nullptr) t = t->right;
return t;
}

template<typename TK, typename TV>
auto AVLTree<TK, TV>::FindNext(AVLNode* x) const -> AVLNode* { //ищем следующий по узлу
if (x == nullptr) return nullptr;

if (this->FindMax(root) == x) return nullptr; //нет больше максимума

if (x->right != nullptr) return this->FindMin(x->right); //если есть правый потомок, ищу в после него минимум

else { // если нет правого потомка
AVLNode* cur = x; AVLNode* par = x->parent;
while (par != nullptr && par->left != cur) { //пока не пришли к ролителю слева
cur = cur->parent; par = par->parent;
}
return par;
}
}

template<typename TK, typename TV>
auto AVLTree<TK, TV>::FindBefor(AVLNode* x) const -> AVLNode* { //ищем предыдущий по узлу
if (x == nullptr) return nullptr;

if (x == this->FindMin(root)) return nullptr;

if (x->left != nullptr) return this->FindMax(x->left);

else {
AVLNode* cur = x; AVLNode* par = x->parent;
while (par->right != cur) {
cur = cur->parent; par = par->parent;
}
return par;
}
}

template<typename TK, typename TV>
auto AVLTree<TK, TV>::FindNodeInTree(const TK& key) const -> AVLNode* { //поиск в дереве, только возращает узел
if (root == nullptr) return nullptr;
AVLNode* t = root;
while (t->KV.first != key) {
if (key < t->KV.first) {
if (t->left == nullptr) throw out_of_range("Нет такого узла по заданному ключу");
else t = t->left;
}
else {
if (t->right == nullptr) throw out_of_range("Нет такого узла по заданному ключу"); //нет того, что ищем
else t = t->right;
}
}
return t;
}




int main()
{
}

