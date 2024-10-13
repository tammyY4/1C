#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <queue>
#include <set>
#include <unordered_map>
using namespace std;
//Пасьянс

//Функция возращающая ответ на то, можно ли положить на данную набор карт карту новую
bool possibility_move(vector<int64_t>& pack_cards, int64_t& new_card) {
  if (pack_cards.empty()) {
    return true;
  } else {
    if (pack_cards.back() > new_card) {
      return true;
    }
    return false;
  }
}

//Проверяет нашлись ли 9 подрядидущих в порядке убывания карт
bool delete_set(vector<int64_t>& cards) {
  int size = cards.size();
  if (size == 9) {
    int temp = 1;
    for (int i = 1; i < 10; ++i) {
      if (cards[size - i] == temp) {
        ++temp;
      } else {
        break;
      }
    }
    if (temp == 10) {
      return true;
    }
  }
  return false;
}

// // Получаем всевозможные исходы при выборе карты new_card
// vector<vector<vector<int>>> find_outcomes(vector<vector<int64_t>>& packs, int64_t& new_card) {
//   vector<vector<vector<int>>> outcomes;
//   for (int i = 0; i < 8; ++i) {
//     if (possibility_move(packs[i], new_card)) {
//       vector<vector<int64_t>> copy_pack = packs;
//       copy_pack[i].emplace_back(new_card);
//       outcomes.emplace_back(copy_pack);
//       if (delete_set(copy_pack[i])) {
//         copy_pack[i].erase(copy_pack[i].end() - 9, copy_pack[i].end());
//       }
//       outcomes.emplace_back(copy_pack);
//     }
//   }
// } 

int64_t sum_of_pack(vector<int64_t>& p) {
  int64_t temp = 0;
  int size = p.size();
  for (int i = 0; i < size; ++i) {
    temp += p[size - 1 - i] * pow(10,(2*i));
  }
  return temp;
}

int64_t state(vector<vector<int64_t>>& packs) {
  int64_t temp = 0;
  for (int i = 0; i < 8;++i) {
    temp += sum_of_pack(packs[i]) * pow(10, 18*i);
  }
  return temp;
}

// struct Graph {
//   int64_t v = pow(10, 160);
//   vector<vector<int64_t>> first_state;
//   std::vector<std::vector<std::pair<int, int>>> edge;
//   Graph(const vector<vector<int64_t>>& first_state);
// };

// Graph::Graph(const vector<vector<int64_t>>& first_state) : first_state(first_state) {
//   edge.resize(v);
//   vector<int> visit(0, v);
//   queue<vector<vector<int64_t>>> q;
//   set<int64_t> visited;
//   q.push(first_state);
//   while (!q.empty()) {
//     vector<vector<int64_t>> temp = q.front();
//     q.pop();
//     int64_t state_ = state(temp);
//     if (visit[state_]) {
//       continue;
//     }
//     visit[state_] = 1;
//     for (int i = 0; i < 8; ++i) {
//       for (int j = 0; j < 8; ++j) {
//         if (i != j and possibility_move(temp[j], temp[i].back())) {
//           vector<vector<int64_t>> copy_pack = temp;
//           copy_pack[j].emplace_back(temp[i].back());
//           copy_pack[i].erase(copy_pack[i].end() - 1, copy_pack[i].end());
//           q.push(copy_pack);
//           edge[state_].emplace_back(state(copy_pack));
//           if (delete_set(copy_pack[j])) {
//             copy_pack[j].erase(copy_pack[j].end() - 9, copy_pack[j].end());
//           }
//           q.push(copy_pack);
//           edge[state_].emplace_back(state(copy_pack));
//         }
//       }
//     }
//   }
// }

//присваивается уникальный номер каждой расстоновке
bool theend(vector<vector<int64_t>>& cards) {
  return (state(cards) == 0);
}

// поиск в ширину
void bfs(const vector<vector<int64_t>>& first_state) {
  int coount = 0;
  queue <pair<vector<vector<int64_t>>, int64_t>> q;
  set<int64_t> visited;
  q.push(make_pair(first_state, 0));
  while (!q.empty()) {
    pair<vector<vector<int64_t>>, int64_t> temp = q.front();
    q.pop();
    if (theend(temp.first)) {
      cout << "Можно убрать все карты за минимальное количество действий =  " << temp.second << endl;
      ++coount;
      return;
    }
    if (visited.count(state(temp.first))) {
      continue;
    }
    visited.emplace(state(temp.first));
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (i != j and possibility_move(temp.first[j], temp.first[i].back())) {
          vector<vector<int64_t>> copy_pack = temp.first;
          copy_pack[j].emplace_back(temp.first[i].back());
          copy_pack[i].erase(copy_pack[i].end() - 1, copy_pack[i].end());
          q.push(make_pair(copy_pack, temp.second + 1));
          if (delete_set(copy_pack[j])) {
            copy_pack[j].erase(copy_pack [j].end() - 9, copy_pack[j].end());
          }
          q.push(make_pair(copy_pack, temp.second + 1));
        }
      }
    }
  }
  if (coount = 0) {
    cout << "Невозможно убрать все карты." << endl;
  }
}

unordered_map<string, int> card_values = {
  {"туз", 9}, {"король", 8}, {"дама", 7}, {"валет", 6},
  {"десятка", 5}, {"девятка", 4}, {"восьмёрка", 3}, {"семёрка", 2}, {"шестёрка", 1}
};

unordered_map<string, string> card_suits = {
  {"пик", "пик"}, {"бубен", "бубен"}, {"черв", "черв"}, {"крести", "крести"}
};

int main() {
  vector<vector<int64_t>> first_state(8);
  for (int i = 0; i < 8; i++) {
    string card_name, card_suit;
    cout << "кучка № " << i + 1 << ": ";
    for (int j = 0; j < 9; j++) {
      cin >> card_name >> card_suit;
      int64_t card_value = card_values[card_name];
      first_state[i].emplace_back(card_value);
    }
  }
  
  bfs(first_state);

  return 0;
}