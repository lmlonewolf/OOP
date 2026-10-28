#include "Tests.h"
#include "TextIndex.h"
#include "Intersection.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>


void require(bool condition, const std::string& name) {
    if (!condition) {
        throw std::runtime_error("Тест не пройден: " + name);
    }
    std::cout << "[OK] " << name << "\n";
}

void testFiles() {
    TextIndex index;
    require(index.addDocument(Document(1, "test_data/spaces.txt")) && index.getIndex().empty(),
        "файл только с пробелами не создаёт слов");
    require(index.addDocument(Document(2, "test_data/with spaces.txt")) &&
        index.searchWord("код") == std::vector<int>({2}),
        "файл с пробелами в имени читается и индексируется");
}

void testDirectory() {
    TextIndex index;
    index.loadDirectory("test_data/directory");
    require(index.getDocuments().size() == 2, "загрузка txt-файлов из папки");
    require(index.searchWord("код") == std::vector<int>({1, 2}),
        "поиск по документам из папки");
    index.loadDirectory("test_data/directory");
    require(index.getDocuments().size() == 2 &&
        index.searchWord("код") == std::vector<int>({1, 2}),
        "перечитывание папки без дублирования документов");
}

void runTests() {
    testFiles();
    testDirectory();
    BinaryTree<int, std::string> tree;
    require(!tree.contains(1) && !tree.erase(1) && tree.inorder().empty(), "пустое дерево");
    for (int key : {5, 3, 7, 2, 4, 6, 8}) {
        require(tree.insert(key, std::to_string(key)), "вставка ключа " + std::to_string(key));
    }
    require(!tree.insert(5, "дубликат") && *tree.find(5) == "5", "повтор ключа отклонён");
    const BinaryTree<int, std::string>& view = tree;
    require(*view.find(6) == "6" && view.find(99) == nullptr, "константный поиск");
    require(tree.erase(2), "удаление листа");
    require(tree.erase(3) && tree.contains(4), "удаление узла с одним потомком");
    require(tree.erase(5) && tree.contains(6), "удаление корня с двумя потомками");
    require(tree.inorder() == std::vector<std::pair<int, std::string>>({
        {4, "4"}, {6, "6"}, {7, "7"}, {8, "8"}}),
        "симметричный обход после удаления");
    for (int key : {4, 6, 7, 8}) {
        require(tree.erase(key), "удаление оставшегося ключа");
    }
    require(tree.inorder().empty() && tree.insert(1, "снова"), "повторное использование дерева");
    BinaryTree<int, int> leftChild;
    leftChild.insert(3, 30);
    leftChild.insert(2, 20);
    require(leftChild.erase(3) && *leftChild.find(2) == 20,
        "удаление корня с единственным левым ребёнком");
    BinaryTree<int, int, std::greater<int>> reverse;
    reverse.insert(1, 10);
    reverse.insert(3, 30);
    reverse.insert(2, 20);
    require(reverse.inorder()[0].first == 3 && reverse.erase(3) && reverse.contains(2),
        "другой тип значений и обратное сравнение");

    BinaryTree<int, int> successor;
    for (int key : {10, 5, 20, 15, 17, 25}) {
        successor.insert(key, key * 10);
    }
    require(successor.erase(10) && successor.inorder() == std::vector<std::pair<int, int>>({
        {5, 50}, {15, 150}, {17, 170}, {20, 200}, {25, 250}}),
        "удаление корня: преемник имеет правого потомка");
    require(!successor.erase(99) && successor.find(99) == nullptr,
        "удаление отсутствующего ключа в непустом дереве");


    BinaryTree<int, int> labTree;
    for (int key : {70, 50, 30, 60, 40, 20, 80, 150}) {
        labTree.insert(key, key);
    }
    require(labTree.inorder() == std::vector<std::pair<int, int>>({
        {20, 20}, {30, 30}, {40, 40}, {50, 50},
        {60, 60}, {70, 70}, {80, 80}, {150, 150}}),
        "числа из работы по алгоритмам: симметричный обход дерева поиска");
    require(!labTree.insert(50, 500) && !labTree.insert(70, 700) &&
        *labTree.find(50) == 50 && *labTree.find(70) == 70,
        "повторы из работы по алгоритмам отклонены по условию ООП");
    require(intersection<int>({1, 2, 2, 3}, {2, 3, 4}) == std::vector<int>({2, 3}),
        "пересечение без повторов");
    require(intersection<int>({1}, {2}).empty() && intersection<int>({}, {1}).empty(),
        "пустое пересечение");
    require(intersection<std::string>({"a", "b"}, {"b"}) == std::vector<std::string>({"b"}),
        "пересечение строк");
    auto sameRemainder = [](int first, int second) {
        return first % 10 == second % 10;
    };
    require(intersection<int>({11, 21, 12}, {31}, sameRemainder) == std::vector<int>({11}),
        "пересечение с переданным правилом равенства");
    TextIndex index;
    require(index.searchAll(" ").empty() && index.searchWord("нет").empty(), "пустой индекс");
    require(index.addDocument(Document(1, "test_data/first.txt")), "русский документ из файла");
    require(index.addDocument(Document(2, "test_data/second.txt")), "второй документ из файла");
    require(index.addDocument(Document(3, "test_data/third.txt")), "третий документ из файла");
    auto before = index.getIndex();
    require(!index.addDocument(Document(1, "test_data/duplicate.txt")) && index.getIndex() == before,
        "повтор идентификатора отклонён без изменения индекса");
    require(index.searchWord("кот") == std::vector<int>({1, 2}), "регистр и повторы слова");
    require(index.searchWord("ёж") == std::vector<int>({1}), "буква ё");
    require(index.searchWord("17") == std::vector<int>({1}), "цифры и разделители");
    require(index.searchWord("C") == std::vector<int>({1}), "латинская буква приводится к нижнему регистру");
    require(index.searchWord("два слова").empty(), "одиночный поиск не принимает два слова");
    require(index.searchAll("КОТ код кот") == std::vector<int>({1, 2}), "все слова запроса");
    require(index.searchAll("кот пёс") == std::vector<int>({2}), "пересечение документов");
    require(index.searchAll("кот дом").empty() && index.searchAll("кот нет").empty(),
        "запросы без результатов");
    require(index.searchAll("нет кот").empty() && index.searchAll("., !").empty(),
        "нет первого слова и запрос только из разделителей");
    require(index.removeDocument(1) && !index.removeDocument(99), "удаление документа");
    require(index.searchWord("кот") == std::vector<int>({2}) && index.searchWord("ёж").empty(),
        "удаление идентификатора и последнего слова");
    for (const auto& entry : index.getIndex()) {
        require(!entry.second.empty(), "в дереве нет пустых записей");
    }
    require(index.removeDocument(2) && index.removeDocument(3) && index.getIndex().empty(),
        "полное очищение индекса");
    require(index.addDocument(Document(1, "test_data/empty.txt")) && index.getIndex().empty(),
        "пустой файл не создаёт слов");
    require(index.removeDocument(1) && index.addDocument(Document(1, "test_data/second.txt")),
        "повтор ID после удаления");
    std::cout << "Тесты задания 2 пройдены.\n";
}
